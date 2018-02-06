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

namespace yeti {

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
{
  yeti_assert_debug(env_ != NULL);
  yeti_assert_debug(input_ != NULL);
  yeti_assert_debug(output_ != NULL);
}

EntityCompiler::~EntityCompiler() {
}

bool EntityCompiler::run() {
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

bool EntityCompiler::load() {
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

// TODO(mtwilliams): Contextualize errors and warnings.

bool EntityCompiler::validate() {
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

bool EntityCompiler::validate_an_entity_definition(xml_element_t *e) {
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
      // Egregious enough that it can be ignored.
      env_->error(env_, "Expected component or children definition but got \"%.*s\" instead.", e->name.l, e->name.s);
      valid = false;
    }
  }

  return valid;
}

bool EntityCompiler::validate_all_component_definitions(xml_element_t *e) {
  bool valid = true;

  for (e = e->children; e; e = e->sibling)
    if (!this->validate_a_component_definition(e))
      valid = false;

  return valid;
}

bool EntityCompiler::validate_a_component_definition(xml_element_t *e) {
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

bool EntityCompiler::validate_all_children_definitions(xml_element_t *e) {
  bool valid = true;

  for (e = e->children; e; e = e->sibling)
    if (!this->validate_an_entity_definition(e))
      valid = false;

  return valid;
}

#if 0
static xml_element_t *xml_find(xml_element_t *e, const char *path) {
  // TODO(mtwilliams): Implement a subset of XPath with mutually recursive
  // functions.

  for (xml_element_t *c = e->children; c; c = c->sibling)
    if (core::string::compare(path, c->name.s, c->name.l))
      return c;

  return NULL;
}
#endif

bool EntityCompiler::compile() {
  xml_element_t *e = root_->children;

  if (document_has_declaration_)
    // Skip declaration.
    e = e->sibling;

  while (e) {
    // TODO(mtwilliams): Compile definitions into intermediate format.
    e = e->sibling;
  }

  return false;
}

bool EntityCompiler::bake() {
  // TODO(mtwilliams): Bake intermediate format to file.
  return false;
}

} // yeti
