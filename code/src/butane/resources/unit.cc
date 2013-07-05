// This file is part of Butane. See README.md and LICENSE.md for details.
// Copyright (c) 2012 Michael Williams <devbug@bitbyte.ca>

#include <butane/resources/unit.h>

namespace butane {
  static Allocator& __allocator_initializer() {
    static ProxyAllocator allocator("unit resources", Allocators::heap());
    return allocator;
  }

  static const thread_safe::Static< Allocator >
    __ts_allocator(&__allocator_initializer);

  static Allocator& allocator() {
    return __ts_allocator();
  }

  static const Resource::Type& __type_initializer() {
    static const Resource::Type type(
      "unit", "unit",
      (Resource::Type::Load)&UnitResource::load,
      (Resource::Type::Unload)&UnitResource::unload,
      (Resource::Type::Compile)&UnitResource::compile);
    return type;
  }

  static const thread_safe::Static< const Resource::Type >
    __ts_type(&__type_initializer);

  const Resource::Type& UnitResource::type() {
    return __ts_type();
  }

  UnitResource::UnitResource(
    const Resource::Id id
  ) : butane::Resource(UnitResource::type(), id)
    , _scene_graph(nullptr)
  {
  }

  UnitResource::~UnitResource()
  {
  }

  UnitResource* UnitResource::load(
    const Resource::Id id,
    const Resource::Stream& stream )
  {
    const LogScope _("UnitResource::load");

    const MemoryResidentData* mrd =
      ((const MemoryResidentData*)stream.memory_resident_data());

    UnitResource* unit =
      make_new(UnitResource, allocator())(id);

    const size_t scene_graph_len =
      sizeof(SceneGraph::Serialized) +
      (mrd->scene_graph->num_of_nodes * sizeof(SceneGraph::Node::Serialized));

    unit->_scene_graph =
      (SceneGraph::Serialized*)allocator().alloc(scene_graph_len);

    copy(
      (void*)unit->_scene_graph,
      (const void*)((SceneGraph::Serialized*)mrd->scene_graph),
      scene_graph_len);

    return unit;
  }

  void UnitResource::unload(
    UnitResource* unit )
  {
    const LogScope _("UnitResource::unload");

    assert(unit != nullptr);
    make_delete(UnitResource, allocator(), unit);
  }

  static bool node_type_from_string(
    const char* str,
    SceneGraph::Node::Type& type )
  {
    if (strcmp("EMPTY", str) == 0) {
      type = SceneGraph::Node::EMPTY;
      return true;
    } else if(strcmp("CAMERA", str) == 0) {
      type = SceneGraph::Node::CAMERA;
      return true;
    } else if (strcmp("MESH", str) == 0) {
      type = SceneGraph::Node::MESH;
      return true; }
    return false;
  }

  static bool camera_type_from_string(
    const char* str,
    SceneGraph::Node::Camera::Type& type )
  {
    if (strcmp("ORTHOGRAPHIC", str) == 0) {
      type = SceneGraph::Node::Camera::ORTHOGRAPHIC;
      return true;
    } else if(strcmp("PERSPECTIVE", str) == 0) {
      type = SceneGraph::Node::Camera::PERSPECTIVE;
      return true; }
    return false;
  }

  bool UnitResource::compile(
    const Resource::Compiler::Input& input,
    const Resource::Compiler::Output& output )
  {
    const LogScope _("UnitResource::compile");

    size_t sjson_len = 0;
    const char* sjson =
      (const char*)File::read(input.data, Allocators::heap(), &sjson_len);

    Array<uint8_t> blob(Allocators::scratch(), 1 << 16 /* 64kb */);
    blob.resize(blob.reserved());
    zero((void*)blob.raw(), blob.size());
    sjson::Parser parser(Allocators::heap(), (void*)&blob[0], blob.size());

    if (!parser.parse(sjson, sjson_len)) {
      Allocators::heap().free((void*)sjson);
      output.log("Malformed sjson; parsing failed.");
      return false; }

    Allocators::heap().free((void*)sjson);

    const sjson::Object* root =
      (const sjson::Object*)&blob[0];

    const sjson::Array* nodes = (const sjson::Array*)root->find("nodes");
    if (!nodes) {
      output.log("Malformed input: 'nodes' not specified!");
      return false; }
    if (!nodes->is_array()) {
      output.log("Malformed input: expected 'nodes' to be an array!");
      return false; }
    if (nodes->size() > SceneGraph::maximum_num_of_nodes) {
      output.log("Malformed input: a maximum of %u nodes are supported!", SceneGraph::maximum_num_of_nodes);
      return false; }
    if (nodes->size() == 0) {
      output.log("Malformed input: expected at least one node!");
      return false; }

    const size_t mrd_len =
      sizeof(MemoryResidentData) + sizeof(SceneGraph::Serialized) +
      sizeof(SceneGraph::Node::Serialized) * nodes->size();

    MemoryResidentData* mrd =
      (MemoryResidentData*)Allocators::heap().alloc(mrd_len);

    {
      int64_t offset = sizeof(MemoryResidentData);
      mrd->scene_graph = relative_ptr<SceneGraph::Serialized*>(offset - offsetof(MemoryResidentData, scene_graph));
    }

    {
      int64_t offset = sizeof(SceneGraph::Serialized);
      mrd->scene_graph->num_of_nodes = nodes->size();
      mrd->scene_graph->nodes = relative_ptr<SceneGraph::Node::Serialized*>(offset - offsetof(SceneGraph::Serialized, nodes));
    }

    for (uint id = 0; id < nodes->size(); ++id) {
      SceneGraph::Node::Serialized& sn = mrd->scene_graph->nodes[id];
      const sjson::Object* node = (const sjson::Object*)nodes->at(id);
      if (!node->is_object()) {
        output.log("Malformed input: the node %u not an object!", id);
        goto failure; }

      sn.id = (SceneGraph::Node::Id)id;

      /* sn.type = */ {
        const sjson::String* value = (const sjson::String*)node->find("type");
        if (!value) {
          output.log("Malformed input: the node %u is missing 'type'!", id);
          goto failure; }
        if (!value->is_string()) {
          output.log("Malformed input: 'type' in the node %u is not a string!", id);
          goto failure; }
        if (!node_type_from_string(value->raw(), sn.type)) {
          output.log("Malformed input: the type '%s' in the node %u is not recognized!", value->raw(), id);
          goto failure; }
      }

      /* sn.parent = */ {
        const sjson::Number* value = (const sjson::Number*)node->find("parent");
        if (!value) {
          output.log("Malformed input: the node %u is missing 'parent'!", id);
          goto failure; }
        if (!value->is_number()) {
          output.log("Malformed input: 'parent' in the node %u is not an id!", id);
          goto failure; }
        int parent = value->value();
        if (parent == id) {
          output.log("Malformed input: the parent '%d' in the node %u is self referencing!", parent, id);
          goto failure; }
        if ((parent > 0) && ((parent > nodes->size()) || (parent > id))) {
          output.log("Malformed input: the parent '%d' in the node %u references an undefined node!", parent, id);
          goto failure; }
        sn.link = (parent < 0) ? SceneGraph::Node::unlinked : (SceneGraph::Node::Id)parent;
      }

      /* sn.name = */ {
        const sjson::String* value = (const sjson::String*)node->find("fully_qualified_name");
        if (!value) {
          output.log("Malformed input: the node %u is missing 'fully_qualified_name'!", id);
          goto failure; }
        if (!value->is_string()) {
          output.log("Malformed input: 'fully_qualified_name' in the node %u is not a string!", id);
          goto failure; }
        sn.name = SceneGraph::Node::Name(value->raw());
      }

      /* sn.pose = */ {
        const sjson::Object* pose = (const sjson::Object*)node->find("pose");
        if (!pose) {
          output.log("Malformed input: the node %u is missing 'pose'!", id);
          goto failure; }
        if (!pose->is_object()) {
          output.log("Malformed input: 'pose' in the node %u is not an object!", id);
          goto failure; }

        /* sn.pose.position = */ {
          const sjson::Array* value = (const sjson::Array*)pose->find("position");
          if (!value) {
            output.log("Malformed input: the node %u is missing 'pose.position'!", id);
            goto failure; }
          if (!value->is_array() || (value->size() != 3) || (!value->at(0)->is_number()) || (!value->at(1)->is_number()) || (!value->at(2)->is_number())) {
            output.log("Malformed input: 'pose.position' in the node %u is not a vector!", id);
            goto failure; }
          sn.pose.position = Vec3f(
            ((const sjson::Number*)value->at(0))->value(),
            ((const sjson::Number*)value->at(1))->value(),
            ((const sjson::Number*)value->at(2))->value());
        }

        /* sn.pose.rotation = */ {
          const sjson::Array* value = (const sjson::Array*)pose->find("rotation");
          if (!value) {
            output.log("Malformed input: the node %u is missing 'pose.rotation'!", id);
            goto failure; }
          if (!value->is_array() || (value->size() != 4) || (!value->at(0)->is_number()) || (!value->at(1)->is_number()) || (!value->at(2)->is_number()) || (!value->at(3)->is_number())) {
            output.log("Malformed input: 'pose.rotation' in the node %u is not a quaternion!", id);
            goto failure; }
          sn.pose.rotation = Quatf(
            ((const sjson::Number*)value->at(0))->value(),
            ((const sjson::Number*)value->at(1))->value(),
            ((const sjson::Number*)value->at(2))->value(),
            ((const sjson::Number*)value->at(3))->value());
        }

        /* sn.pose.scale = */ {
          const sjson::Array* value = (const sjson::Array*)pose->find("scale");
          if (!value) {
            output.log("Malformed input: the node %u is missing 'pose.scale'!", id);
            goto failure; }
          if (!value->is_array() || (value->size() != 3) || (!value->at(0)->is_number()) || (!value->at(1)->is_number()) || (!value->at(2)->is_number())) {
            output.log("Malformed input: 'pose.scale' in the node %u is not a vector!", id);
            goto failure; }
          sn.pose.scale = Vec3f(
            ((const sjson::Number*)value->at(0))->value(),
            ((const sjson::Number*)value->at(1))->value(),
            ((const sjson::Number*)value->at(2))->value());
        }
      }

      /* sn.data = */ {
        switch (sn.type) {
          case SceneGraph::Node::CAMERA: {
            const sjson::Object* data = (const sjson::Object*)node->find("data");
            if (!data) {
              output.log("Malformed input: the node %u is missing 'data'!", id);
              goto failure; }
            if (!data->is_object()) {
              output.log("Malformed input: 'data' in the node %u is not an object!", id);
              goto failure; }

            SceneGraph::Node::Camera::Serialized* camera =
              (SceneGraph::Node::Camera::Serialized*)&sn.as;

            /* sn.as.camera.type = */ {
              const sjson::String* value = (const sjson::String*)data->find("type");
              if (!value) {
                output.log("Malformed input: the node %u is missing 'data.type'!", id);
                goto failure; }
              if (!value->is_string()) {
                output.log("Malformed input: 'data.type' in the node %u is not a string!", id);
                goto failure; }
              if (!camera_type_from_string(value->raw(), camera->type)) {
                output.log("Malformed input: the camera type '%s' in the node %u is not recognized!", value->raw(), id);
                goto failure; }
            }

            /* sn.as.camera.near = */ {
              const sjson::Number* value = (const sjson::Number*)data->find("near");
              if (!value) {
                output.log("Malformed input: the node %u is missing 'data.near'!", id);
                goto failure; }
              if (!value->is_number()) {
                output.log("Malformed input: 'data.near' in the node %u is not a number!", id);
                goto failure; }
              camera->near = (float)value->value();
            }

            /* sn.as.camera.far = */ {
              const sjson::Number* value = (const sjson::Number*)data->find("far");
              if (!value) {
                output.log("Malformed input: the node %u is missing 'data.far'!", id);
                goto failure; }
              if (!value->is_number()) {
                output.log("Malformed input: 'data.far' in the node %u is not a number!", id);
                goto failure; }
              camera->far = (float)value->value();
            }

            switch (camera->type) {
              case SceneGraph::Node::Camera::ORTHOGRAPHIC: {
                /* sn.as.camera.orthographic.min_x = */ {
                  const sjson::Number* value = (const sjson::Number*)data->find("min_x");
                  if (!value) {
                    output.log("Malformed input: the node %u is missing 'data.min_x'!", id);
                    goto failure; }
                  if (!value->is_number()) {
                    output.log("Malformed input: 'data.min_x' in the node %u is not a number!", id);
                    goto failure; }
                  camera->orthographic.min_x = (float)value->value();
                }

                /* sn.as.camera.orthographic.max_x = */ {
                  const sjson::Number* value = (const sjson::Number*)data->find("max_x");
                  if (!value) {
                    output.log("Malformed input: the node %u is missing 'data.max_x'!", id);
                    goto failure; }
                  if (!value->is_number()) {
                    output.log("Malformed input: 'data.max_x' in the node %u is not a number!", id);
                    goto failure; }
                  camera->orthographic.max_x = (float)value->value();
                }

                /* sn.as.camera.orthographic.min_y = */ {
                  const sjson::Number* value = (const sjson::Number*)data->find("min_y");
                  if (!value) {
                    output.log("Malformed input: the node %u is missing 'data.min_y'!", id);
                    goto failure; }
                  if (!value->is_number()) {
                    output.log("Malformed input: 'data.min_y' in the node %u is not a number!", id);
                    goto failure; }
                  camera->orthographic.min_y = (float)value->value();
                }

                /* sn.as.camera.orthographic.max_y = */ {
                  const sjson::Number* value = (const sjson::Number*)data->find("max_y");
                  if (!value) {
                    output.log("Malformed input: the node %u is missing 'data.max_y'!", id);
                    goto failure; }
                  if (!value->is_number()) {
                    output.log("Malformed input: 'data.max_y' in the node %u is not a number!", id);
                    goto failure; }
                  camera->orthographic.max_y = (float)value->value();
                }
              } break;

              case SceneGraph::Node::Camera::PERSPECTIVE: {
                /* sn.as.camera.perspective.field_of_view = */ {
                  const sjson::Number* value = (const sjson::Number*)data->find("field_of_view");
                  if (!value) {
                    output.log("Malformed input: the node %u is missing 'data.field_of_view'!", id);
                    goto failure; }
                  if (!value->is_number()) {
                    output.log("Malformed input: 'data.field_of_view' in the node %u is not a number!", id);
                    goto failure; }
                  camera->perspective.field_of_view = (float)value->value();
                }

                /* sn.as.camera.perspective.aspect_ratio = */ {
                  const sjson::Number* value = (const sjson::Number*)data->find("aspect_ratio");
                  if (!value) {
                    output.log("Malformed input: the node %u is missing 'data.aspect_ratio'!", id);
                    goto failure; }
                  if (!value->is_number()) {
                    output.log("Malformed input: 'data.aspect_ratio' in the node %u is not a number!", id);
                    goto failure; }
                  camera->perspective.aspect_ratio = (float)value->value();
                }
              } break;
            }
          } break;

          case SceneGraph::Node::MESH: {
            SceneGraph::Node::Mesh::Serialized* mesh =
              (SceneGraph::Node::Mesh::Serialized*)&sn.as;

            String path = String::format(
              Allocators::scratch(), "%s/%s",
              output.path,
              ((const sjson::String*)node->find("fully_qualified_name"))->raw());

            replace_all(path.raw(), '.', '/');

            mesh->resource = Resource::Id(MeshResource::type(), path);
            mesh->material = 0;
          } break;
        }
      }
    }

    if (!File::write(output.memory_resident_data, (const void*)mrd, mrd_len)) {
      output.log("Unable to write memory-resident data!");
      goto failure; }

    Allocators::heap().free((void*)mrd);
    return true;

  failure:
    Allocators::heap().free((void*)mrd);
    return false;
  }
} // butane
