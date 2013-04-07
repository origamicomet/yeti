// This file is part of Butane. See README.md and LICENSE.md for details.
// Copyright (c) 2012 Michael Williams <devbug@bitbyte.ca>

#include <bt/lua/script.h>

namespace bt {
namespace Lua {
  Script::Stack::Stack( Script* script )
    : bt::Script::Stack(script)
  {
  }

  Script::Stack::~Stack()
  {
  }

  void Script::Stack::push( void )
  {
    lua_pushnil(((Script*)_script)->_state);
  }

  void Script::Stack::push( void* ptr )
  {
    lua_pushlightuserdata(((Script*)_script)->_state, ptr);
  }

  void Script::Stack::push( bool boolean )
  {
    lua_pushboolean(((Script*)_script)->_state, boolean ? 1 : 0);
  }

  void Script::Stack::push( int integer )
  {
    lua_pushinteger(((Script*)_script)->_state, integer);
  }

  void Script::Stack::push( float number )
  {
    lua_pushnumber(((Script*)_script)->_state, number);
  }

  void Script::Stack::push( double number )
  {
    lua_pushnumber(((Script*)_script)->_state, number);
  }

  void Script::Stack::push( const char* string )
  {
    lua_pushstring(((Script*)_script)->_state, string);
  }

  static int __forwarding_closure( lua_State* state ) {
    Script* script =
      (Script*)lua_touserdata(state, lua_upvalueindex(1));
    Script::Function func =
      (Script::Function)lua_touserdata(state, lua_upvalueindex(2));
    return (int)func(script, script->_arguments);
  }

  void Script::Stack::push( Script::Function func )
  {
    lua_pushlightuserdata(((Script*)_script)->_state, (void*)_script);
    lua_pushlightuserdata(((Script*)_script)->_state, (void*)func);
    lua_pushcclosure(((Script*)_script)->_state, &__forwarding_closure, 2);
  }

  void Script::Stack::pop( void )
  {
    if (!lua_isnil(((Script*)_script)->_state, -1))
      luaL_error(((Script*)_script)->_state, "Trying to pop non-nil value!");
    lua_pop(((Script*)_script)->_state, 1);
  }

  void Script::Stack::pop( void*& ptr )
  {
    if (!lua_islightuserdata(((Script*)_script)->_state, -1))
      luaL_error(((Script*)_script)->_state, "Trying to pop non-pointer value!");
    ptr = lua_touserdata(((Script*)_script)->_state, -1);
    lua_pop(((Script*)_script)->_state, 1);
  }

  void Script::Stack::pop( bool& boolean )
  {
    if (!lua_isboolean(((Script*)_script)->_state, -1))
      luaL_error(((Script*)_script)->_state, "Trying to pop non-boolean value!");
    boolean = (lua_toboolean(((Script*)_script)->_state, -1) != 0);
    lua_pop(((Script*)_script)->_state, 1);
  }

  void Script::Stack::pop( int& integer )
  {
    if (!lua_isnumber(((Script*)_script)->_state, -1))
      luaL_error(((Script*)_script)->_state, "Trying to pop non-integer value!");
    integer = lua_tointeger(((Script*)_script)->_state, -1);
    lua_pop(((Script*)_script)->_state, 1);
  }

  void Script::Stack::pop( float& number )
  {
    if (!lua_isnumber(((Script*)_script)->_state, -1))
      luaL_error(((Script*)_script)->_state, "Trying to pop non-number value!");
    number = lua_tonumber(((Script*)_script)->_state, -1);
    lua_pop(((Script*)_script)->_state, 1);
  }

  void Script::Stack::pop( double& number )
  {
    if (!lua_isnumber(((Script*)_script)->_state, -1))
      luaL_error(((Script*)_script)->_state, "Trying to pop non-number value!");
    number = lua_tonumber(((Script*)_script)->_state, -1);
    lua_pop(((Script*)_script)->_state, 1);
  }

  void Script::Stack::pop( String& string )
  {
    if (!lua_isstring(((Script*)_script)->_state, -1))
      luaL_error(((Script*)_script)->_state, "Trying to pop non-number value!");
    string = String(Allocator::scratch(), lua_tostring(((Script*)_script)->_state, -1));
    lua_pop(((Script*)_script)->_state, 1);
  }

  void Script::Stack::pop( Script::Function& func )
  {
    if (!lua_iscfunction(((Script*)_script)->_state, -1))
      luaL_error(((Script*)_script)->_state, "Trying to pop non-function value!");
    if (!lua_getupvalue(((Script*)_script)->_state, -1, 2))
      luaL_error(((Script*)_script)->_state, "Trying to pop an external function value!\n");
    func = (Script::Function)lua_touserdata(((Script*)_script)->_state, -1);
    lua_pop(((Script*)_script)->_state, 2);
  }

  // ============================================================

  Script::Arguments::Arguments( Script* script )
    : bt::Script::Arguments(script)
  {
  }

  Script::Arguments::~Arguments()
  {
  }

  Script::Arguments::operator size_t()
  {
    return (size_t)lua_gettop(((Script*)_script)->_state);
  }

  // void* Script::Arguments::to<void*>( size_t index )
  // {
  //   if (!lua_islightuserdata(((Script*)_script)->_state, index + 1))
  //     luaL_argerror(((Script*)_script)->_state, index + 1, "expected a pointer");
  //   return lua_touserdata(((Script*)_script)->_state, index + 1);
  // }

  // bool Script::Arguments::to<bool>( size_t index )
  // {
  //   if (!lua_isboolean(((Script*)_script)->_state, index + 1))
  //     luaL_argerror(((Script*)_script)->_state, index + 1, "expected a boolean");
  //   return lua_toboolean(((Script*)_script)->_state, index + 1);
  // }

  // int Script::Arguments::to<int>( size_t index )
  // {
  //   if (!lua_isnumber(((Script*)_script)->_state, index + 1))
  //     luaL_argerror(((Script*)_script)->_state, index + 1, "expected an integer");
  //   return lua_tointeger(((Script*)_script)->_state, index + 1);
  // }

  // float Script::Arguments::to<float>( size_t index )
  // {
  //   if (!lua_isnumber(((Script*)_script)->_state, index + 1))
  //     luaL_argerror(((Script*)_script)->_state, index + 1, "expected a number");
  //   return (float)lua_tonumber(((Script*)_script)->_state, index + 1);
  // }

  // double Script::Arguments::to<double>( size_t index )
  // {
  //   if (!lua_isnumber(((Script*)_script)->_state, index + 1))
  //     luaL_argerror(((Script*)_script)->_state, index + 1, "expected a number");
  //   return (double)lua_tonumber(((Script*)_script)->_state, index + 1);
  // }

  // const char* Script::Arguments::to<const char*>( size_t index )
  // {
  //   if (!lua_isstring(((Script*)_script)->_state, index + 1))
  //     luaL_argerror(((Script*)_script)->_state, index + 1, "expected a string");
  //   return lua_tostring(((Script*)_script)->_state, index + 1);
  // }

  // Script::Function Script::Arguments::to<Script::Function>( size_t index )
  // {
  //   if (!lua_iscfunction(((Script*)_script)->_state, index + 1))
  //     luaL_argerror(((Script*)_script)->_state, index + 1, "expected a function");
  //   if (!lua_getupvalue(((Script*)_script)->_state, index + 1, 2))
  //     luaL_argerror(((Script*)_script)->_state, index + 1, "expected a non-external function");
  //   Script::Function func =
  //     (Script::Function)lua_touserdata(((Script*)_script)->_state, -1);
  //   lua_pop(((Script*)_script)->_state, 1);
  //   return func;
  // }

  // ============================================================

  static int __at_panic( lua_State* state ) {
    const char* error_msg = lua_tostring(state, -1);
    if (!error_msg)
      error_msg = "unknown error";
    lua_getfield(state, LUA_REGISTRYINDEX, "bt::Script");
    Script* script = (Script*)lua_touserdata(state, -1);
    Callstack cs;
    script->callstack(cs);
    script->_on_error(script, error_msg, cs);
    exit(EXIT_FAILURE);
    return 0;
  }

  static int __error_handler( lua_State* state ) {
    Script* script =
      (Script*)lua_touserdata(state, lua_upvalueindex(1));
    Callstack* cs =
      (Callstack*)lua_touserdata(state, lua_upvalueindex(2));
    if (cs)
      script->callstack(*cs);
    return 1;
  }

  Script::Script()
    : _state(lua_newstate((lua_Alloc)&Script::alloc, (void*)this))
    , _memory_usage(0)
    , _stack(Stack(this))
    , _arguments(Arguments(this))
  {
    lua_pushlightuserdata(_state, (void*)this);
    lua_setfield(_state, LUA_REGISTRYINDEX, "bt::Script");
    lua_atpanic(_state, &__at_panic);

    static const luaL_Reg libs[] = {
      { "", luaopen_base },
      { LUA_LOADLIBNAME, luaopen_package },
      { LUA_TABLIBNAME, luaopen_table },
      { LUA_STRLIBNAME, luaopen_string },
      { LUA_MATHLIBNAME, luaopen_math },
    #if !defined(BT_RELEASE_BUILD)
      { LUA_DBLIBNAME, luaopen_debug },
    #endif
      { NULL, NULL }
    };

    for (const luaL_Reg* lib = &libs[0]; lib->func; ++lib) {
      lua_pushcfunction(_state, lib->func);
      lua_pushstring(_state, lib->name);
      lua_call(_state, 1, 0);
    }

    ScriptInterface::expose(this);
  }

  Script::~Script()
  {
    if (_state)
      lua_close(_state);
  }

  static const char* first_occurrence_of(
    const char* haystack,
    char needle )
  {
    assert(haystack != nullptr);

    char ch;
    while ((ch = *haystack++)) {
      if (ch == needle)
        return (haystack - 1);
    }

    return nullptr;
  }

  static const char* lua_namespace(
    lua_State* state,
    const char* name )
  {
    assert(state != nullptr);
    assert(name != nullptr);

    const char* ns /*namespace*/ = name;
    lua_rawgeti(state, LUA_REGISTRYINDEX, LUA_RIDX_GLOBALS);

    while (true) {
      const char* dot = first_occurrence_of(ns, '.');
      if (!dot)
        break;
      
      lua_pushlstring(state, ns, (size_t)(dot - ns));
      lua_gettable(state, -2);

      if (lua_isnil(state, -1)) {
        lua_pop(state, 1);
        lua_createtable(state, 0, 0);
        lua_pushlstring(state, ns, (size_t)(dot - ns));
        lua_pushvalue(state, -2);
        lua_settable(state, -4);
      }

      lua_replace(state, -2);
      ns = dot + 1;
    }

    return ns;
  }

  bool Script::call(
    const char* name,
    size_t num_arguments,
    size_t& num_returns )
  {
    Callstack cs;

    const char* ns = lua_namespace(_state, name);
    lua_getfield(_state, -1, ns);
    lua_insert(_state, lua_gettop(_state) - (int)(num_arguments + 1));
    lua_pop(_state, 1);

    lua_pushlightuserdata(_state, (void*)this);
    lua_pushlightuserdata(_state, (void*)&cs);
    lua_pushcclosure(_state, &__error_handler, 2);
    const int ehi = lua_gettop(_state) - (int)(num_arguments + 2);
    lua_insert(_state, ehi);

    const int orig_top = lua_gettop(_state);

    if (lua_pcall(_state, num_arguments, LUA_MULTRET, ehi) != 0) {
      _on_error(this, lua_tostring(_state, -1), cs);
      lua_pop(_state, 1);
      return false;
    }

    num_returns = lua_gettop(_state) - (orig_top - num_arguments - 1);
    lua_pop(_state, 1);
    return true;
  }

  void Script::expose(
    const char* name,
    Script::Function func )
  {
    assert(name != nullptr);
    assert(func != nullptr);

    const char* ns = lua_namespace(_state, name);

    lua_pushstring(_state, ns);
    lua_pushlightuserdata(_state, (void*)this);
    lua_pushlightuserdata(_state, (void*)func);
    lua_pushcclosure(_state, &__forwarding_closure, 2);
    lua_settable(_state, -3);
    lua_pop(_state, 1);
  }

  void Script::callstack( Callstack& cs )
  {
    lua_Debug ar;

    int level = 0;
    while (lua_getstack(_state, level++, &ar) == 1) {
      if (lua_getinfo(_state, "nlf", &ar) == 0)
        fail("lua_getinfo failed!\n");

      Callstack::Frame frame;

      if (strcmp("Lua", ar.what) == 0) {
        strncpy(&frame.name[0], &ar.name[0], sizeof(frame.name) - 1);
        frame.name[sizeof(frame.name) - 1] = '\0';
        strncpy(&frame.file[0], &ar.source[0], sizeof(frame.file) - 1);
        frame.file[sizeof(frame.file) - 1] = '\0';
        frame.line = (ar.currentline == -1) ? 0 : ar.currentline;
      } else if (strcmp("C", ar.what) == 0) {
        void* addr = lua_tocfunction(_state, -1);
        if (lua_getupvalue(_state, -1, 2))
          addr = lua_touserdata(_state, -1);
        lua_pop(_state, 1);

        if (!address_to_name(addr, &frame.name[0], sizeof(frame.name))) {
          snprintf(
            &frame.name[0], sizeof(frame.name),
            "#<Function:%016" PRIx64 ">",
            (uint64_t)addr);
        }

        if (!address_to_file_path(addr, &frame.file[0], sizeof(frame.file)))
          frame.file[0] = '\0';

        if (!address_to_line_number(addr, frame.line))
          frame.line = 0;
      } else if ((strcmp("main", ar.what) == 0) ||
                 (strcmp("tail", ar.what) == 0))
      {
        strncpy(&frame.name[0], &ar.what[0], sizeof(frame.name) - 1);
        frame.name[sizeof(frame.name) - 1] = '\0';
        strncpy(&frame.file[0], &ar.source[0], sizeof(frame.file) - 1);
        frame.file[sizeof(frame.file) - 1] = '\0';
        frame.line = (ar.currentline == -1) ? 0 : ar.currentline;
      } else {
        strncpy(&frame.name[0], &ar.name[0], sizeof(frame.name) - 1);
        frame.name[sizeof(frame.name) - 1] = '\0';
        strncpy(&frame.file[0], &ar.source[0], sizeof(frame.file) - 1);
        frame.file[sizeof(frame.file) - 1] = '\0';
        frame.line = (ar.currentline == -1) ? 0 : ar.currentline;
      }

      lua_pop(_state, 1);
      cs.frames.push_back(frame);
    }
  }

  void* Script::alloc(
    Script* script,
    void* ptr,
    size_t old_size,
    size_t new_size )
  {
    script->_memory_usage -= old_size;
    script->_memory_usage += new_size;
    return Allocator::heap().realloc(ptr, new_size);
  }

  static const char* lua_status_to_string( int status ) {
    switch (status) {
      case LUA_OK:
        return "LUA_OK";
      case LUA_YIELD:
        return "LUA_YIELD";
      case LUA_ERRRUN:
        return "LUA_ERRRUN";
      case LUA_ERRSYNTAX:
        return "LUA_ERRSYNTAX";
      case LUA_ERRMEM:
        return "LUA_ERRMEM";
      case LUA_ERRGCMM:
        return "LUA_ERRGCMM";
      case LUA_ERRERR:
        return "LUA_ERRERR";
      default:
        return nullptr;
    }
  }

  String Script::to_string() {
    const int status = lua_status(_state);
    const char* status_ = lua_status_to_string(status);
    char buf[33] = { 0, };
    return String(Allocator::scratch(),
      "#<Lua::Script:%016" PRIx64 "> "
      "[state=%016" PRIx64 ",memory_usage=%ub,status=%s]",
      (uint64_t)this,
      (uint64_t)_state,
      _memory_usage,
      status_ ? status_ : itoa(status, &buf[0], 10));
  }

  // ============================================================

  Script* Script::load(
    const void* code,
    size_t code_len,
    const char* name )
  {
    assert(code != nullptr);

    Script* script = MAKE_NEW(Script, Allocator::heap())();

    if (luaL_loadbuffer(script->_state, (const char*)code, code_len, name) != 0) {
      MAKE_DELETE(Script, Allocator::heap(), script);
      return nullptr;
    }

    Callstack cs;

    lua_pushlightuserdata(script->_state, (void*)script);
    lua_pushlightuserdata(script->_state, (void*)&cs);
    lua_pushcclosure(script->_state, &__error_handler, 2);
    lua_insert(script->_state, -2);

    if (lua_pcall(script->_state, 0, LUA_MULTRET, -2) != 0) {
      script->_on_error(script, lua_tostring(script->_state, -1), cs);
      MAKE_DELETE(Script, Allocator::heap(), script);
      return nullptr;
    }

    return script;
  }

} // Lua
} // bt

namespace bt {
  Script* Script::load(
    const void* code,
    size_t code_len,
    const char* name )
  {
    return Lua::Script::load(code, code_len, name);
  }
} // bt