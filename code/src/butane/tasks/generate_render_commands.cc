// This file is part of Butane. See README.md and LICENSE.md for details.
// Copyright (c) 2012 Michael Williams <devbug@bitbyte.ca>

#include <butane/tasks/generate_render_commands.h>

#include <butane/world.h>
#include <butane/application.h>
#include <butane/tied_resources.h>
#include <butane/graphics/swap_chain.h>
#include <butane/graphics/render_device.h>
#include <butane/graphics/render_context.h>

namespace butane {
namespace Tasks {
  static RenderContext::Command::Key key(
    const RenderConfig::Layer::Id layer,
    const uint8_t pass,
    const uint32_t depth = 0xFFFFFFFFu )
  {
    return (((RenderContext::Command::Key)(layer & 0xFFull) << 56ull) |
            ((RenderContext::Command::Key)(pass) << 48ull) |
            ((RenderContext::Command::Key)(depth) << 0ull));
  }

  void generate_render_commands(
    Task* task,
    uintptr_t data )
  {
    GenerateRenderCommandsData* grcd = (GenerateRenderCommandsData*)data;
    RenderDevice* rd = Application::render_device();
    const Resource::Handle<RenderConfig>& render_config = Application::render_config();
    const SceneGraph::Node::Camera::VisualRepresentation* camera =
      (const SceneGraph::Node::Camera::VisualRepresentation*)grcd->camera;
    if (!render_config || !camera) {
      Allocators::scratch().free((void*)data);
      return; }

    // Prevent modifiations to visual representations (this should be handled by
    // seperate tasks).
    grcd->world->_visual_representation._mutex.lock();

    const Mat4& view = camera->transform;
    const Mat4 inv_view = view.inverse();
    const Mat4& proj = camera->projection;
    const Mat4 inv_proj = proj.inverse();
    const Mat4 view_proj = proj * view;
    const Frustum frustum = Frustum(view);

    // 1: Clear the back-buffer
    grcd->render_context->clear(
      (RenderContext::Command::Key)0x0000000000000000ull /* first */,
      grcd->swap_chain_and_resources->swap_chain()->render_target(),
      Vec4f(0.0f, 0.0f, 0.0f, 0.0f));

    for (size_t idx = 0; idx < render_config->globals().size(); ++idx) {
      const RenderConfig::Resource& resource =
        render_config->globals()[idx];
      switch (resource.type) {
        case RenderConfig::Resource::RENDER_TARGET:
          if (!resource.render_or_depth_stencil_target.clear) {
            grcd->render_context->clear(
              (RenderContext::Command::Key)0x0000000000000000ull /* first */,
              (RenderTarget*)grcd->swap_chain_and_resources->globals()[idx],
              Vec4f(0.0f, 0.0f, 0.0f, 0.0f)); }
          break;
        case RenderConfig::Resource::DEPTH_STENCIL_TARGET:
          if (!resource.render_or_depth_stencil_target.clear) {
            grcd->render_context->clear(
              (RenderContext::Command::Key)0x0000000000000000ull /* first */,
              (DepthStencilTarget*)grcd->swap_chain_and_resources->globals()[idx],
              1.0f, 0xFFFFFFFFu); }
          break;
      }
    }

    for (size_t idx = 0; idx < render_config->layers().size(); ++idx) {
      const RenderConfig::Layer& layer = render_config->layers()[idx];

      // 2: Set Render and Depth-Stencil Targets (and clear if requested)
      RenderTarget* render_targets[8];

      for (size_t rt = 0; rt < layer.num_of_render_targets; ++rt) {
        if (layer.render_targets[rt] == RenderConfig::Resource::Special::back_buffer) {
          render_targets[rt] = grcd->swap_chain_and_resources->swap_chain()->render_target();
          continue; }
        const RenderConfig::Resource& resource =
          render_config->globals()[layer.render_targets[rt]];
        render_targets[rt] = (RenderTarget*)grcd->swap_chain_and_resources->globals()[layer.render_targets[rt]];
        if (resource.render_or_depth_stencil_target.clear) {
          grcd->render_context->clear(
            key(layer.id, 0),
            render_targets[rt],
            Vec4f(0.0f, 0.0f, 0.0f, 0.0f)); }
      }

      DepthStencilTarget* depth_stencil_target =
        (layer.depth_stencil_target != RenderConfig::Resource::invalid) ?
        (DepthStencilTarget*)grcd->swap_chain_and_resources->globals()[layer.depth_stencil_target] : nullptr;

      if (depth_stencil_target) {
        const RenderConfig::Resource& resource =
          render_config->globals()[layer.depth_stencil_target];
        if (resource.render_or_depth_stencil_target.clear) {
          grcd->render_context->clear(
            key(layer.id, 0),
            depth_stencil_target,
            1.0f, 0xFFFFFFFFu); }
      }

      grcd->render_context->set_render_and_depth_stencil_targets(
        key(layer.id, 1),
        layer.num_of_render_targets,
        &render_targets[0],
        depth_stencil_target);

      // 3: Set Viewport(s)
      grcd->render_context->set_viewports(
        key(layer.id, 2),
        1, &grcd->viewport);
    }

    // 4: Generate commands for visible objects
    // TODO: Go wide.
    for (size_t idx = 0; idx < grcd->culled->size(); ++idx) {
      const VisualRepresentation::Culled& culled = (*grcd->culled)[idx];
      if (!(culled.visible & 1u))
        continue;
      switch (VisualRepresentation::type(culled.id)) {
        case VisualRepresentation::MESH: {
          const SceneGraph::Node::Mesh::VisualRepresentation* mesh =
            (const SceneGraph::Node::Mesh::VisualRepresentation*)(&grcd->world->_visual_representation._meshes[grcd->world->_visual_representation._mesh_ids[culled.id & 0xFFFFFFFFu]]);

          const Mat4& model = mesh->transform;
          const Mat4 model_view_proj = view_proj * model;

          const MeshResource::Material& material =
            mesh->resource->materials()[mesh->material];

          const float distance = max(frustum.near.distance(model.translation()), 0.0f);
          const uint32_t depth = (uint32_t)(distance * (float)0xFFFFFFFFu);
          const uint32_t inv_depth = (uint32_t)(0xFFFFFFFFu - depth);

          Sampler* samplers[8];
          Texture* textures[8];

          for (size_t i = 0; i < material.num_of_textures; ++i) {
            samplers[i] = material.textures[i]->sampler();
            textures[i] = material.textures[i]->texture(); }

          for (size_t i = 0; i < material.shader->layers().size(); ++i) {
            const RenderConfig::Layer* layer =
              render_config->find_layer_by_name(material.shader->layers()[i]);
            if (!layer)
              continue;
            // const bool front_to_back =
            //   (layer->sort == RenderConfig::Layer::FRONT_TO_BACK);
            grcd->render_context->draw(
              key(layer->id, 3, depth /* front_to_back ? depth : inv_depth */),
              material.shader->state()->rasterizer_state(),
              material.shader->state()->depth_stencil_state(),
              material.shader->state()->blend_state(),
              material.shader->vertex_shader(),
              material.shader->pixel_shader(),
              material.num_of_textures,
              &samplers[0], &textures[0],
              mesh->resource->vertex_declaration(),
              mesh->resource->vertices(),
              mesh->resource->indicies(),
              0, nullptr /* &globals */,
              Topology::TRIANGLES,
              mesh->resource->num_of_primitives());
          }
        } break;
      }
    }

    grcd->render_context->present(
      (RenderContext::Command::Key)0xFFFFFFFFFFFFFFFFull /* last */,
      grcd->swap_chain_and_resources->swap_chain());

    grcd->world->_visual_representation._mutex.unlock();
    typedef Array<butane::VisualRepresentation::Culled> Culled;
    make_delete(Culled, Allocators::scratch(), grcd->culled);
    Allocators::scratch().free((void*)data);
  }
} // Tasks
} // butane
