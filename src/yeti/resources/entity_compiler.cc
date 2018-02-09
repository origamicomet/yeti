//===-- yeti/resources/entity_compiler.cc ---------------*- mode: C++11 -*-===//
//
//                             __ __     _   _
//                            |  |  |___| |_|_|
//                            |_   _| -_|  _| |
//                              |_| |___|_| |_|
//
//       This file is distributed under the terms described in LICENSE.
//
//===----------------------------------------------------------------------===//

#include "entity_compiler.h"

// Format is defined in a separate file to better track changes.
#include "entity_resource_format.h"

// We need public types related to entities and components.
#include "yeti/entity.h"
#include "yeti/component.h"

namespace yeti {

// TODO(mtwilliams): Contextualize errors and warnings.
// TODO(mtwilliams): Inheritance.

namespace entity_compiler {
  // We only allow names composed of `a-z`, `0-9`, and `_` characters.
  static bool is_acceptable_name(const char *s, size_t l) {
    for (; l--; s++)
      if ((*s < 'a' && *s > 'z') && (*s < '0' && *s > '9') && (*s != '_'))
        return false;
    return true;
  }
}

EntityCompiler::EntityCompiler(const resource_compiler::Environment *env,
                               const resource_compiler::Input *input,
                               const resource_compiler::Output *output)
  : env_(env)
  , input_(input)
  , output_(output)
  , document_(core::global_heap_allocator())
  , heap_(core::global_heap_allocator(), SIZE_OF_HEAP)
  , root_(NULL)
  , document_has_declaration_(false)
  , entity_definitions_(core::global_heap_allocator())
  , component_definitions_(core::global_heap_allocator())
  , instance_definitions_(core::global_heap_allocator())
  , data_(core::global_heap_allocator())
{
  yeti_assert_debug(env_ != NULL);
  yeti_assert_debug(input_ != NULL);
  yeti_assert_debug(output_ != NULL);
}

EntityCompiler::~EntityCompiler()
{
}

bool EntityCompiler::run()
{
  // Load description into memory then parse into a tree.
  if (!load())
    return false;

  // Walk freshly minted tree to validate.
  if (!validate())
    return false;

  // Walk again to compile into an intermediate representation.
  if (!compile())
    return false;

  // Convert intermediate representation to optimized runtime format.
  if (!bake())
    return false;

  return true;
}

bool EntityCompiler::load()
{
  // Load markup into memory.
  const u64 size = core::fs::size(input_->source);
  document_.resize(size+1);
  core::fs::read(input_->source, (void *)document_.raw(), size);

  // Null-terminate for parser.
  document_[size] = '\0';

  // Use stack as scratch buffer is we don't need a lot of memory.
  u8 scratch[SIZE_OF_SCRATCH];

  // Parse into tree.
  return (xml_parse_into_memory((const char *)document_.raw(),
                                (void *)scratch,
                                SIZE_OF_SCRATCH,
                                (void *)heap_.raw(),
                                SIZE_OF_HEAP,
                                &root_) == XML_OK);
}

bool EntityCompiler::validate()
{
  xml_element_t *e = root_->children;

  // Let subsequent passes know there's a markup declaration.
  document_has_declaration_ = core::string::compare("?xml", e->name.s, e->name.l);

  // Skip markup declaration if present.
  if (document_has_declaration_)
    e = e->sibling;

  bool valid = true;

  // Everything else should be entity definitions.
  while (e) {
    const bool is_entity_definition = core::string::compare("entity", e->name.s, e->name.l);

    if (is_entity_definition) {
      if (!this->validate_an_entity_definition(e))
        valid = false;
    } else {
      env_->error(env_, "Expected an entity definition but got \"%.*s\" instead.", e->name.l, e->name.s);
      valid = false;
    }

    e = e->sibling;
  }

  return valid;
}

bool EntityCompiler::validate_an_entity_definition(xml_element_t *e)
{
  bool valid = true;

  if (e->num_of_attributes >= 1) {
    const xml_fragment_t *name = &e->attributes[0].name,
                         *value = &e->attributes[0].value;

    if (core::string::compare("id", name->s, name->l)) {
      if (!core::uuid::validate(value->s)) {
        env_->error(env_, "Expected an identifier but got \"%.*s\" instead.", value->l, value->s);
        valid = false;
      }
    } else {
      env_->error(env_, "Expected an identifier but got \"%.*s\" instead.", name->l, name->s);
      valid = false;
    }
  } else {
    env_->error(env_, "Entity not assigned an identifier. Will automatically be assigned one.");
  }

  if (e->num_of_attributes >= 2) {
    const xml_fragment_t *name = &e->attributes[0].name,
                         *value = &e->attributes[0].value;

    if (core::string::compare("name", name->s, name->l)) {
      if (!entity_compiler::is_acceptable_name(name->s, name->l)) {
        env_->error(env_, "Names can only contain alphanumerics and underscores but got \"%.*s\".", value->l, value->s);
        valid = false;
      }
      if (name->l > 255) {
        env_->error(env_, "Given name \"%.*s\" is too long.", name->l, name->s);
        valid = false;
      }
    } else {
      env_->error(env_, "Expected a name but got \"%.*s\" instead.", name->l, name->s);
      valid = false;
    }
  }

  if (e->num_of_attributes >= 3) {
    env_->warning(env_, "Ignoring superfluous attributes on entity definition.");
  }

  // Validate components and children if any.
  for (e = e->children; e; e = e->sibling) {
    if (core::string::compare("components", e->name.s, e->name.l)) {
      if (!this->validate_all_component_definitions(e))
        valid = false;
    } else if (core::string::compare("children", e->name.s, e->name.l)) {
      if (!this->validate_all_children_definitions(e))
        valid = false;
    } else {
      // Egregious enough that it cannot be ignored.
      env_->error(env_, "Expected component or children definition but got \"%.*s\" instead.", e->name.l, e->name.s);
      valid = false;
    }
  }

  return valid;
}

bool EntityCompiler::validate_all_component_definitions(xml_element_t *e)
{
  bool valid = true;

  for (e = e->children; e; e = e->sibling)
    if (!this->validate_a_component_definition(e))
      valid = false;

  return valid;
}

bool EntityCompiler::validate_a_component_definition(xml_element_t *e)
{
  bool valid = true;

  if (e->num_of_attributes >= 1) {
    const xml_fragment_t *name = &e->attributes[0].name,
                         *value = &e->attributes[0].value;

    if (core::string::compare("id", name->s, name->l)) {
      if (!core::uuid::validate(value->s)) {
        env_->error(env_, "Expected an identifier for component but got \"%.*s\" instead.", value->l, value->s);
        valid = false;
      }
    } else {
      env_->error(env_, "Expected an identifier for component but got \"%.*s\" instead.", name->l, name->s);
      valid = false;
    }
  } else {
    env_->error(env_, "Component not assigned an identifier. Will automatically be assigned one.");
  }

  // Ignore superfluous attributes.
  if (e->num_of_attributes >= 2) {
    env_->warning(env_, "Ignoring superfluous attributes on component definition.");
  }

  return valid;
}

bool EntityCompiler::validate_all_children_definitions(xml_element_t *e)
{
  bool valid = true;

  for (e = e->children; e; e = e->sibling)
    if (!this->validate_an_entity_definition(e))
      valid = false;

  return valid;
}

static xml_element_t *xml_find(xml_element_t *e, const char *path) {
  // TODO(mtwilliams): Implement a subset of XPath with mutually recursive
  // functions.

  for (xml_element_t *c = e->children; c; c = c->sibling)
    if (core::string::compare(path, c->name.s, c->name.l))
      return c;

  return NULL;
}

bool EntityCompiler::compile()
{
  xml_element_t *e = root_->children;

  if (document_has_declaration_)
    // Skip declaration.
    e = e->sibling;

  do {
    if (!this->compile_an_entity(e))
      return false;
  } while (e = e->sibling);

  return true;
}

// TODO(mtwilliams): Link together component definitions by type as we compile
// using a hash table pointing to the head of an intrusive linked-list.

bool EntityCompiler::compile_an_entity(xml_element_t *e, u32 parent)
{
  EntityDef *def = &entity_definitions_.emplace();

  // Preemptively store index for children *after* allocating since we want
  // indices in [1, n].
  const u32 index = entity_definitions_.size();

  if (e->num_of_attributes >= 1) {
    core::uuid::parse(e->attributes[0].value.s, def->identifier);
  } else {
    // Assign an identifier since none was given.
    core::uuid::generate(def->identifier);
  }

  if (e->num_of_attributes >= 2) {
    // We defer pooling until baking.
    def->name = e->attributes[1].value.s;
    def->length_of_name = u8(e->attributes[1].value.l);
  } else {
    def->name = NULL;
    def->length_of_name = 0;
  }

  // We can't store pointers because they can be invalidated when resizing our
  // intermediate arrays, so we store an index instead.
  def->index_of_parent = parent;

  // Compile components.
  if (xml_element_t *components = xml_find(e, "components")) {
    for (xml_element_t *component = components->children; component; component = component->sibling) {
      if (!this->compile_a_component(component, index)) {
        if (def->name) {
          env_->error(env_, "Compilation of \"%.*s\" failed due to component.", def->length_of_name, def->name);
        } else {
          char identifier[36+1];
          core::uuid::present(def->identifier, identifier);
          env_->error(env_, "Compilation of \"%s\" failed due to component.", identifier);
        }
        return false;
      }
    }
  }

  // Then compile children.
  if (xml_element_t *children = xml_find(e, "children"))
    for (xml_element_t *child = children->children; child; child = child->sibling)
      if (!this->compile_an_entity(child, index))
        return false;

  return true;
}

bool EntityCompiler::compile_a_component(xml_element_t *e, u32 entity)
{
  Component::Id type;
  const Component *component;

  // Copy name so we can null-terminate it.
  char *name_of_type = (char *)alloca(e->name.l + 1);
  core::memory::copy((const void *)e->name.s, (void *)name_of_type, e->name.l);
  name_of_type[e->name.l] = '\0';

  // Then lookup component by name.
  type = component_registry::id_from_name(name_of_type);
  component = component_registry::component_by_id(type);

  if (!component) {
    env_->error(env_, "Don't know how to compile \"%.*s\" components.", e->name.l, e->name.s);
    return false;
  }

  ComponentDef *cdef;

  if (!(cdef = this->find_component_definition(type)))
    cdef = this->add_component_definition(type, component->version);

  InstanceDef *idef = &instance_definitions_.emplace();

  this->link_instance_definition(cdef, idef);

  if (e->num_of_attributes >= 1) {
    core::uuid::parse(e->attributes[0].value.s, idef->identifier);
  } else {
    // Assign an identifier since none was given.
    core::uuid::generate(idef->identifier);
  }

  if (e->num_of_attributes >= 2) {
    // We defer pooling until baking.
    idef->name = e->attributes[1].value.s;
    idef->length_of_name = u8(e->attributes[1].value.l);
  } else {
    idef->name = NULL;
    idef->length_of_name = 0;
  }

  idef->offset_to_data = data_.size();
  idef->amount_of_data = 0;

  idef->index_of_entity = entity;

  component_compiler::Environment env;
  env.compiler = this;
  env.info = &EntityCompiler::info;
  env.warning = &EntityCompiler::warning;
  env.error = &EntityCompiler::error;

  component_compiler::Input input;
  input.root = e;

  component_compiler::Output output;
  output.write = &write;

  const bool successful = component->compile(&env, &input, &output);

  idef->amount_of_data = data_.size() - idef->offset_to_data;

  // Track the total amount of data generated by all instances to simplify baking.
  cdef->data += idef->amount_of_data;

  return successful;
}


EntityCompiler::ComponentDef *EntityCompiler::find_component_definition(u32 type)
{
  for (ComponentDef *def = component_definitions_.begin(); def < component_definitions_.end(); ++def)
    if (def->type == type)
      return def;

  return NULL;
}

EntityCompiler::ComponentDef *EntityCompiler::add_component_definition(u32 type, u32 version)
{
  ComponentDef *def = &component_definitions_.emplace();

  def->type    = type;
  def->version = version;
  def->head    = -1;
  def->tail    = -1;
  def->count   = 0;
  def->data    = 0;

  return def;
}

void EntityCompiler::link_instance_definition(ComponentDef *cdef, InstanceDef *idef)
{
  const u32 index = instance_definitions_.index_from_pointer(idef);

  if (cdef->count++) {
    // Append to linked-list.
    instance_definitions_[cdef->tail].next = index;
    // Update tail for future appends so we can append in O(1).
    cdef->tail = index;
  } else {
    // Setup linked-list of instances.
    cdef->head = cdef->tail = index;
  }

  // Given instance definition is always tail.
  idef->next = -1;
}

bool EntityCompiler::bake()
{
  const u32 n_entities = entity_definitions_.size();
  const u32 n_components = component_definitions_.size();

  entity_resource_format::Header header;

  header.num_of_entities = n_entities;
  header.num_of_components = n_components;

  const u32 size_of_header = sizeof(entity_resource_format::Header);
  const u32 size_of_entities = n_entities * sizeof(entity_resource_format::Entity);
  const u32 size_of_components = n_components * sizeof(entity_resource_format::Component);
  const u32 size_of_hierarchy = n_entities * sizeof(entity_resource_format::Hierarchy);

  header.offset_to_entities = size_of_header;
  header.offset_to_hierarchy = size_of_entities + header.offset_to_entities;
  header.offset_to_components = size_of_hierarchy + header.offset_to_hierarchy;

  // Zero to ensure deterministic builds.
  core::memory::zero((void *)header.padding, sizeof(header.padding));

  // Write out header.
  core::fs::write(output_->memory_resident_data, (const void *)&header, sizeof(header));

  // Hierarchy is collected into memory and then written wholesale.
  core::Array<u32> hierarchy(core::global_heap_allocator());

  for (EntityDef *edef = entity_definitions_.begin(); edef < entity_definitions_.end(); ++edef) {
    entity_resource_format::Entity entity;

    // Copy identifier.
    core::memory::copy((const void *)edef->identifier, (void *)entity.identifier, 16);

    // Collect hierarchy.
    hierarchy.push(edef->index_of_parent);

    // Write out entity.
    core::fs::write(output_->memory_resident_data, (const void *)&entity, sizeof(entity));
  }

  // Write out hierarchy.
  core::fs::write(output_->memory_resident_data, (const void *)hierarchy.raw(), hierarchy.size() * sizeof(u32));

  u32 offset = header.offset_to_hierarchy + size_of_hierarchy;

  for (ComponentDef *cdef = component_definitions_.begin(); cdef < component_definitions_.end(); ++cdef) {
    entity_resource_format::Component component;

    component.type = cdef->type;
    component.version = cdef->version;

    component.count = cdef->count;

    const u32 size_of_mapping = component.count * sizeof(u32);
    const u32 size_of_instances = component.count * sizeof(entity_resource_format::Instance);
    const u32 size_of_data = component.count;

    // TODO(mtwilliams): Align to 16-byte boundary.
    component.offset_to_mapping = offset;
    component.offset_to_instances = size_of_mapping + component.offset_to_mapping;
    component.offset_to_data = size_of_instances + component.offset_to_instances;

    core::fs::write(output_->memory_resident_data, (const void *)&component, sizeof(component));

    offset = size_of_data + component.offset_to_data;
  }

  for (ComponentDef *cdef = component_definitions_.begin(); cdef < component_definitions_.end(); ++cdef) {
    const u32 n_instances = cdef->count;

    // Mapping from instances to entities is built in memory and then written wholesale.
    core::Array<u32> mappings(core::global_heap_allocator());

    for (u32 index = cdef->head; index != -1; index = instance_definitions_[index].next)
      mappings.push(instance_definitions_[index].index_of_entity);

    // Write out mappings.
    core::fs::write(output_->memory_resident_data, (const void *)mappings.raw(), mappings.size() * sizeof(u32));

    for (u32 index = cdef->head; index != -1; index = instance_definitions_[index].next) {
      InstanceDef *idef = &instance_definitions_[index];

      entity_resource_format::Instance instance;

      // Copy identifier.
      core::memory::copy((const void *)idef->identifier, (void *)instance.identifier, 16);

      // Write out instance.
      core::fs::write(output_->memory_resident_data, (const void *)&instance, sizeof(instance));
    }

    for (u32 index = cdef->head; index != -1; index = instance_definitions_[index].next) {
      InstanceDef *idef = &instance_definitions_[index];

      // Write out data.
      core::fs::write(output_->memory_resident_data, (const void *)&data_[idef->offset_to_data], idef->amount_of_data);
    }
  }

  return true;
}

void EntityCompiler::info(const component_compiler::Environment *env, const char *format, ...)
{
  EntityCompiler *compiler = env->compiler;
  va_list va;
  va_start(va, format);
  compiler->forward(format, va, compiler->env_->info);
  va_end(va);
}

void EntityCompiler::warning(const component_compiler::Environment *env, const char *format, ...)
{
  EntityCompiler *compiler = env->compiler;
  va_list va;
  va_start(va, format);
  compiler->forward(format, va, compiler->env_->warning);
  va_end(va);
}

void EntityCompiler::error(const component_compiler::Environment *env, const char *format, ...)
{
  EntityCompiler *compiler = env->compiler;
  va_list va;
  va_start(va, format);
  compiler->forward(format, va, compiler->env_->error);
  va_end(va);
}

void EntityCompiler::forward(const char *format, va_list ap, Forwardee callback)
{
  // Format.
  const int size = vsnprintf(NULL, 0, format, ap) + 1;
  char *message = (char *)alloca(size);
  vsnprintf(message, size, format, ap);

  // Forward to given callback.
  callback(env_, message);
}

void EntityCompiler::write(const component_compiler::Environment *env,
                           const void *buffer,
                           size_t amount)
{
  auto &pool = env->compiler->data_;
  const size_t offset = env->compiler->data_.size();
  pool.grow(amount);
  core::memory::copy(buffer, (void *)&pool.raw()[offset], amount);
}

} // yeti
