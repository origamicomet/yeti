//===-- yeti/resources/entity_compiler.h ----------------*- mode: C++11 -*-===//
//
//                             __ __     _   _
//                            |  |  |___| |_|_|
//                            |_   _| -_|  _| |
//                              |_| |___|_| |_|
//
//       This file is distributed under the terms described in LICENSE.
//
//===----------------------------------------------------------------------===//
//
// TODO(mtwilliams): Document the purpose of this file.
//
//===----------------------------------------------------------------------===//

#ifndef _YETI_ENTITY_COMPILER_H_
#define _YETI_ENTITY_COMPILER_H_

#include "yeti/resource.h"
#include "yeti/resource_compiler.h"

namespace yeti {

class YETI_PRIVATE EntityCompiler {
 YETI_DISALLOW_COPYING(EntityCompiler);

 private:
  static const size_t SIZE_OF_SCRATCH = 65535;
  static const size_t SIZE_OF_HEAP = 1 * 1024 * 1024;

 private:
  struct EntityDef {
    u8 identifier[16];
    const char *name;
    u8 length_of_name;
    u32 index_of_parent;
  };

  struct ComponentDef {
    u32 type;
    u32 version;
    u32 head;
    u32 tail;
    u32 count;
    u32 data;
  };

  struct InstanceDef {
    u8 identifier[16];
    const char *name;
    u8 length_of_name;
    u32 offset_to_data;
    u32 amount_of_data;
    u32 index_of_entity;
    u32 next;
  };

 public:
  EntityCompiler(const resource_compiler::Environment *env,
                 const resource_compiler::Input *input,
                 const resource_compiler::Output *output);

  ~EntityCompiler();

 public:
  bool run();

 private:
  bool load();
  bool validate();
  bool compile();
  bool bake();

 private:
  bool validate_an_entity_definition(xml_element_t *e);
  bool validate_all_component_definitions(xml_element_t *e);
  bool validate_a_component_definition(xml_element_t *e);
  bool validate_all_children_definitions(xml_element_t *e);

 private:
  bool compile_an_entity(xml_element_t *e, u32 parent = 0);
  bool compile_a_component(xml_element_t *e, u32 entity);

 private:
  ComponentDef *find_component_definition(u32 type);
  ComponentDef *add_component_definition(u32 type, u32 version);
  void link_instance_definition(ComponentDef *cdef, InstanceDef *idef);

 private:
  typedef void (*Forwardee)(const ResourceCompiler::Environment *env, const char *format, ...);

  /// \internal Shims used to forward component compiler logs to resource compiler.
  /// @{
  static void info(const component_compiler::Environment *env, const char *format, ...);
  static void warning(const component_compiler::Environment *env, const char *format, ...);
  static void error(const component_compiler::Environment *env, const char *format, ...);
  /// @}

  /// \internal Forwards a log to @callback.
  void forward(const char *format, va_list ap, Forwardee callback);

 private:
  /// \internal Copies data from component compilation to `data_`.
  static void write(const component_compiler::Environment *env,
                    const void *buffer,
                    size_t amount);

 private:
  const resource_compiler::Environment *env_;
  const resource_compiler::Input *input_;
  const resource_compiler::Output *output_;

 private:
  // Buffer storing entire document.
  core::Array<u8> document_;

  // Heap passed to parser for tree.
  core::Array<u8> heap_;

  // Root element composing the tree.
  xml_element_t *root_;

  // Document has an optional markup declaration.
  bool document_has_declaration_;

  // Intermediates.
  core::Array<EntityDef> entity_definitions_;
  core::Array<ComponentDef> component_definitions_;
  core::Array<InstanceDef> instance_definitions_;

  // Data from compiled instances.
  core::Array<u8> data_;
};

} // yeti

#endif // _YETI_ENTITY_COMPILER_H_
