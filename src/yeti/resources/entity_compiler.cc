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
  if (xml_parse_into_memory((const char *)document_.raw(),
                            (void *)scratch,
                            SIZE_OF_SCRATCH,
                            (void *)heap_.raw(),
                            SIZE_OF_HEAP,
                            &root_) != XML_OK)
    return false;

  return true;
}

// TODO(mtwilliams): Contextualize errors and warnings.

#define BAD_MARKUP_DECLARATION \
  return env_->warning(env_, "Bad markup declaration."), true;
#define BAD_DOCUMENT_ENCODING \
  return env_->error(env_, "Bad document encoding."), false;
#define NOT_AN_ENTITY \
  return env_->error(env_, "Expected an entity definition but got \"%.*s\" instead.", e->name.l, e->name.s), false;

bool EntityCompiler::validate() {
  xml_element_t *e = root_->children;

  // Validate optional declaration if present.
  if (core::string::compare("?xml", e->name.s, e->name.l)) {
    if (e->num_of_attributes < 1) {
      // Version is mandatory.
      BAD_MARKUP_DECLARATION;
    }

    const xml_fragment_t *name, *value;

    name = &e->attributes[0].name;
    value = &e->attributes[0].value;

    if (!core::string::compare("version", name->s, name->l))
      BAD_MARKUP_DECLARATION;

    if (!core::string::compare("1.0", value->s, value->l))
      BAD_MARKUP_DECLARATION;

    if (e->num_of_attributes > 2) {
      name = &e->attributes[1].name;
      value = &e->attributes[1].value;

      // We expect encoding to follow.
      if (!core::string::compare("encoding", name->s, name->l))
        BAD_MARKUP_DECLARATION;

      // We only support a sane encoding.
      if (!core::string::compare("UTF-8", value->s, value->l))
        BAD_DOCUMENT_ENCODING;
    }

    // Let subsequent passes know that they need to skip the declaration.
    document_has_declaration_ = true;

    e = e->sibling;
  }

  bool valid = true;

  // Everything remaining should be entity definitions.
  while (e) {
    if (!core::string::compare("entity", e->name.s, e->name.l))
      NOT_AN_ENTITY;

    if (!this->validate_an_entity_definition(e))
      valid = false;

    e = e->sibling;
  }

  return valid;
}


#undef BAD_MARKUP_DECLARATION
#undef BAD_DOCUMENT_ENCODING
#undef NOT_AN_ENTITY

bool EntityCompiler::validate_an_entity_definition(xml_element_t *e) {
  bool valid = true;

  // Check for an identifier.
  if (e->num_of_attributes >= 1) {
    const xml_fragment_t *name = &e->attributes[0].name,
                         *value = &e->attributes[0].value;

    if (core::string::compare("id", name->s, name->l)) {
      // Check that the identifier is a well-formed UUID.
      if ((value->l != 36) || !core::uuid::validate(value->s))
        env_->error(env_, "Expected an identifier but got \"%.*s\" instead.", value->l, value->s);
    } else {
      env_->error(env_, "Expected an identifier but got \"%.*s\" instead.", name->l, name->s);
      valid = false;
    }
  } else {
    env_->error(env_, "Entity not assigned an identifier. Will automatically be assigned one.");
  }

  if (e->num_of_attributes >= 2) {
    env_->warning(env_, "Ignoring superfluous attributes on entity definition.");
  }

  static const struct {
    const char *name;
    bool (EntityCompiler::*validate)(xml_element_t *);
  } validations[] = {
    { "components", &EntityCompiler::validate_all_component_definitions },
    { "children",   &EntityCompiler::validate_all_children_definitions }
  };

  for (e = e->children; e; e = e->sibling) {
    for (unsigned v = 0; v < YETI_ELEMENTS_IN_ARRAY(validations); ++v) {
      if (core::string::compare(validations[v].name, e->name.s, e->name.l)) {
        if (!(this->*validations[v].validate)(e))
          valid = false;
        goto validated;
      }
    }

    env_->error(env_, "Expected component or child definitions but got \"%.*s\" instead.", e->name.l, e->name.s);

    // Egregious enough that it should be fixed.
    valid = false;

  validated:
    continue;
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
  // TODO(mtwilliams): Validate component definitions.
  return false;
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
