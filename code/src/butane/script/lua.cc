// This file is part of Butane. See README.md and LICENSE.md for details.
// Copyright (c) 2012 Michael Williams <devbug@bitbyte.ca>

#include <butane/script/lua.h>

namespace butane {
namespace Lua {
  Script::Stack::Stack(
    Script& script
  ) : butane::Script::Stack(script)
  {}

  Script::Stack::~Stack()
  {}

  void Script::Stack::push(
    void ) const
  {
    lua_pushnil(((Script&)_script)._state);
  }

  void Script::Stack::pop(
    void ) const
  {
    lua_pop(((Script&)_script)._state, 1);
  }

  void Script::Stack::push(
    void* ptr ) const
  {
    lua_pushlightuserdata(((Script&)_script)._state, ptr);
  }

  void Script::Stack::pop(
    void*& ptr ) const
  {
    Script& script = ((Script&)_script);
    if (!lua_islightuserdata(script._state, -1))
      script.error("Trying to pop non-pointer value!");
    ptr = lua_touserdata(script._state, -1);
    lua_pop(script._state, 1);
  }

  void Script::Stack::push(
    bool boolean ) const
  {
    lua_pushboolean(((Script&)_script)._state, boolean ? 1 : 0);
  }

  void Script::Stack::pop(
    bool& boolean ) const
  {
    Script& script = ((Script&)_script);
    if (!lua_isboolean(script._state, -1))
      script.error("Trying to pop non-boolean value!");
    boolean = (lua_toboolean(script._state, -1) != 0);
    lua_pop(script._state, 1);
  }

  void Script::Stack::push(
    int integer ) const
  {
    lua_pushinteger(((Script&)_script)._state, integer);
  }

  void Script::Stack::pop(
    int& integer ) const
  {
    Script& script = ((Script&)_script);
    if (!lua_isnumber(script._state, -1))
      script.error("Trying to pop non-integer value!");
    integer = lua_tointeger(script._state, -1);
    lua_pop(script._state, 1);
  }

  void Script::Stack::push(
    float number ) const
  {
    lua_pushnumber(((Script&)_script)._state, number);
  }

  void Script::Stack::pop(
    float& number ) const
  {
    Script& script = ((Script&)_script);
    if (!lua_isnumber(script._state, -1))
      script.error("Trying to pop non-number value!");
    number = lua_tonumber(script._state, -1);
    lua_pop(script._state, 1);
  }

  void Script::Stack::push(
    double number ) const
  {
    lua_pushnumber(((Script&)_script)._state, number);
  }

  void Script::Stack::pop(
    double& number ) const
  {
    Script& script = ((Script&)_script);
    if (!lua_isnumber(script._state, -1))
      script.error("Trying to pop non-number value!");
    number = lua_tonumber(script._state, -1);
    lua_pop(script._state, 1);
  }

  void Script::Stack::push(
    const char* string ) const
  {
    lua_pushstring(((Script&)_script)._state, string);
  }

  void Script::Stack::push(
    const String& string ) const
  {
    lua_pushstring(((Script&)_script)._state, string.raw());
  }

  void Script::Stack::pop(
    String& string ) const
  {
    Script& script = ((Script&)_script);
    if (!lua_isstring(script._state, -1))
      script.error("Trying to pop non-string value!");
    string = String(Allocators::scratch(), lua_tostring(script._state, -1));
    lua_pop(script._state, 1);
  }

  void Script::Stack::push(
    Function function ) const
  {
    Script& script = ((Script&)_script);
    lua_pushlightuserdata(script._state, (void*)&script);
    lua_pushlightuserdata(script._state, (void*)function);
    lua_pushcclosure(script._state, &Script::__forwarding_closure, 2);
  }

  void Script::Stack::pop(
    Function& function ) const
  {
    Script& script = ((Script&)_script);
    if (!lua_iscfunction(script._state, -1))
      script.error("Trying to pop non-function value!");
    if (!lua_getupvalue(script._state, -1, 2))
      script.error("Trying to pop non-function value!");
    function = (Script::Function)lua_touserdata(script._state, -1);
    lua_pop(script._state, 2);
  }

  void Script::Stack::push(
    const char* type,
    void* ptr ) const
  {
    Script& script = ((Script&)_script);
    assert(type != nullptr);

    const char* qt /* qualified_type */ =
      Script::__qualified_name(script._state, type);

    if (!qt)
      fail("Unable to create a '%s' (unable to qualify type!)", type);

    *((void**)lua_newuserdata(script._state, sizeof(void*))) = ptr;
    lua_getfield(script._state, -2, qt);
    lua_remove(script._state, -3);
    lua_setmetatable(script._state, -2);
  }
} // Lua
} // butane

namespace butane {
namespace Lua {
  Script::Arguments::Arguments(
    Script& script
  ) : butane::Script::Arguments(script)
  {}

  Script::Arguments::~Arguments()
  {}

  Script::Arguments::operator size_t() const
  {
    return (size_t)lua_gettop(((Script&)_script)._state);
  }

  void Script::Arguments::to(
    size_t arg, void*& ptr ) const
  {
    Script& script = ((Script&)_script);
    if (arg >= lua_gettop(script._state))
      script.error("Trying to access undefined argument!");
    if (!lua_islightuserdata(script._state, arg + 1))
      script.error("Trying to access non-pointer argument!");
    ptr = lua_touserdata(script._state, arg + 1);
  }

  void Script::Arguments::to(
    size_t arg, bool& boolean ) const
  {
    Script& script = ((Script&)_script);
    if (arg >= lua_gettop(script._state))
      script.error("Trying to access undefined argument!");
    if (!lua_isboolean(script._state, arg + 1))
      script.error("Trying to access non-boolean argument!");
    boolean = (lua_toboolean(script._state, arg + 1) != 0) ? true : false;
  }

  void Script::Arguments::to(
    size_t arg, int& integer ) const
  {
    Script& script = ((Script&)_script);
    if (arg >= lua_gettop(script._state))
      script.error("Trying to access undefined argument!");
    if (!lua_isnumber(script._state, arg + 1))
      script.error("Trying to access non-integer argument!");
    integer = lua_tointeger(script._state, arg + 1);
  }

  void Script::Arguments::to(
    size_t arg, float& number ) const
  {
    Script& script = ((Script&)_script);
    if (arg >= lua_gettop(script._state))
      script.error("Trying to access undefined argument!");
    if (!lua_isnumber(script._state, arg + 1))
      script.error("Trying to access non-number argument!");
    number = lua_tonumber(script._state, arg + 1);
  }

  void Script::Arguments::to(
    size_t arg, double& number ) const
  {
    Script& script = ((Script&)_script);
    if (arg >= lua_gettop(script._state))
      script.error("Trying to access undefined argument!");
    if (!lua_isnumber(script._state, arg + 1))
      script.error("Trying to access non-number argument!");
    number = lua_tonumber(script._state, arg + 1);
  }

  void Script::Arguments::to(
    size_t arg, const char*& string ) const
  {
    Script& script = ((Script&)_script);
    if (arg >= lua_gettop(script._state))
      script.error("Trying to access undefined argument!");
    if (!lua_isstring(script._state, arg + 1))
      script.error("Trying to access non-string argument!");
    string = lua_tostring(script._state, arg + 1);
  }

  void Script::Arguments::to(
    size_t arg, String& string ) const
  {
    Script& script = ((Script&)_script);
    if (arg >= lua_gettop(script._state))
      script.error("Trying to access undefined argument!");
    if (!lua_isstring(script._state, arg + 1))
      script.error("Trying to access non-string argument!");
    string = String(Allocators::scratch(), lua_tostring(script._state, arg + 1));
  }

  void Script::Arguments::to(
    size_t arg, const char* type, void*& ptr ) const
  {
    assert(type != nullptr);
    Script& script = ((Script&)_script);
    if (arg >= lua_gettop(script._state))
      script.error("Trying to access undefined argument!");
    if (!lua_isuserdata(script._state, arg + 1))
      script.error("Trying to access non-%s argument!", type);
    ptr = *((void**)lua_touserdata(script._state, arg + 1));
  }
} // Lua
} // butane

namespace butane {
namespace Lua {
  Script::Type::Type(
    Script& script
  ) : butane::Script::Type(script)
  {
  }

  Script::Type::~Type()
  {
  }

  int Script::Type::__ctor(
    lua_State* state )
  {
    Script& script =
      (*(Script*)lua_touserdata(state, lua_upvalueindex(1)));
    Constructor ctor =
      (Constructor)lua_touserdata(state, lua_upvalueindex(2));
    lua_remove(state, 1);
    *((void**)lua_newuserdata(state, sizeof(void*))) =
      ctor(script, Script::Arguments(script));
    lua_pushvalue(state, lua_upvalueindex(3));
    lua_setmetatable(state, -2);
    return 1;
  }

  int Script::Type::__dtor(
    lua_State* state )
  {
    Script& script =
      (*(Script*)lua_touserdata(state, lua_upvalueindex(1)));
    Destructor dtor =
      (Destructor)lua_touserdata(state, lua_upvalueindex(2));
    dtor(script, lua_touserdata(state, 1));
    return 0;
  }

  int Script::Type::__forwarding_closure(
    lua_State* state )
  {
    Script& script =
      (*(Script*)lua_touserdata(state, lua_upvalueindex(1)));
    Method method =
      (Method)lua_touserdata(state, lua_upvalueindex(2));
    void* self = *((void**)lua_touserdata(state, 1));
    lua_remove(state, 1);
    return (int)method(script, self, Script::Arguments(script));
  }

  int Script::Type::__get(
    lua_State* state )
  {
    Script& script =
      (*(Script*)lua_touserdata(state, lua_upvalueindex(1)));
    lua_pushfstring(state, "__get_%s", lua_tostring(state, 2));
    lua_rawget(state, lua_upvalueindex(2));
    Getter getter = (Getter)lua_touserdata(state, -1);
    if (!getter)
      script.error("Unable to get '%s' (no getter!)", lua_tostring(state, 2));
    lua_pop(state, 1);
    void* self = *((void**)lua_touserdata(state, 1));
    lua_remove(state, 1);
    return (int)getter(script, self, Script::Arguments(script));
  }

  int Script::Type::__set(
    lua_State* state )
  {
    Script& script =
      (*(Script*)lua_touserdata(state, lua_upvalueindex(1)));
    lua_pushfstring(state, "__set_%s", lua_tostring(state, 2));
    lua_rawget(state, lua_upvalueindex(2));
    Setter setter = (Setter)lua_touserdata(state, -1);
    if (!setter)
      script.error("Unable to set '%s' (no setter!)", lua_tostring(state, 2));
    lua_pop(state, 1);
    void* self = *((void**)lua_touserdata(state, 1));
    lua_insert(state, 1);
    lua_pop(state, 2);
    return (int)setter(script, self, Script::Arguments(script));
  }

  butane::Script::Type& Script::Type::getter(
    const char* member,
    Getter getter )
  {
    assert(member != nullptr);
    assert(getter != nullptr);

    lua_pushfstring(((Script&)_script)._state, "__get_%s", member);
    lua_pushlightuserdata(((Script&)_script)._state, (void*)getter);
    lua_settable(((Script&)_script)._state, -3);

    return *this;
  }

  butane::Script::Type& Script::Type::setter(
    const char* member,
    Setter setter )
  {
    assert(member != nullptr);
    assert(setter != nullptr);

    lua_pushfstring(((Script&)_script)._state, "__set_%s", member);
    lua_pushlightuserdata(((Script&)_script)._state, (void*)setter);
    lua_settable(((Script&)_script)._state, -3);

    return *this;
  }

  butane::Script::Type& Script::Type::accessors(
    const char* member,
    Getter getter,
    Setter setter )
  {
    return this->getter(member, getter).setter(member, setter);
  }

  butane::Script::Type& Script::Type::operation(
    const char* operation,
    Operator method )
  {
    assert(operation != nullptr);
    assert(method != nullptr);

    if (strcmp("add", operation) == 0)
      return this->method("__add", method);
    if (strcmp("sub", operation) == 0)
      return this->method("__sub", method);
    if (strcmp("mul", operation) == 0)
      return this->method("__mul", method);
    if (strcmp("div", operation) == 0)
      return this->method("__div", method);

    fail("Unable to expose operation '%s' (unknown or unsupported operation!)", operation);
    return *this;
  }

  butane::Script::Type& Script::Type::method(
    const char* name,
    Method method )
  {
    assert(name != nullptr);
    assert(method != nullptr);

    lua_pushlightuserdata(((Script&)_script)._state, (void*)&_script);
    lua_pushlightuserdata(((Script&)_script)._state, (void*)method);
    lua_pushcclosure(((Script&)_script)._state, &Type::__forwarding_closure, 2);
    lua_setfield(((Script&)_script)._state, -2, name);

    return *this;
  }

  butane::Script::Type& Script::Type::method(
    const char* name,
    Function function )
  {
    assert(name != nullptr);
    assert(function != nullptr);

    lua_pushlightuserdata(((Script&)_script)._state, (void*)&_script);
    lua_pushlightuserdata(((Script&)_script)._state, (void*)function);
    lua_pushcclosure(((Script&)_script)._state, &Script::__forwarding_closure, 2);
    lua_setfield(((Script&)_script)._state, -2, name);

    return *this;
  }

  void Script::Type::expose(
    const char* name )
  {
    assert(name != nullptr);

    const char* qn /* qualified_name */ =
      Script::__qualified_name(((Script&)_script)._state, name);

    if (!qn) {
      fail("Unable to expose '%s' (unable to qualify name!)", name);
      return; }

    lua_insert(((Script&)_script)._state, -2);
    lua_pushvalue(((Script&)_script)._state, -1);
    lua_setmetatable(((Script&)_script)._state, -1);
    lua_setfield(((Script&)_script)._state, -2, name);
    lua_pop(((Script&)_script)._state, 1);

    make_delete(Type, Allocators::heap(), this);
  }
} // Lua
} // butanes

namespace butane {
namespace Lua {
  Script::Script()
    : _state(lua_newstate((lua_Alloc)&Script::__alloc, (void*)this))
    , _stack(*this)
    , _memory_usage(0)
  {
    lua_pushlightuserdata(_state, (void*)this);
    lua_setfield(_state, LUA_REGISTRYINDEX, "butane::Script");

    static const luaL_Reg libs[] = {
      { "", luaopen_base },
      { LUA_LOADLIBNAME, luaopen_package },
      { LUA_TABLIBNAME, luaopen_table },
      { LUA_STRLIBNAME, luaopen_string },
      { LUA_MATHLIBNAME, luaopen_math },
    #if defined(BUTANE_DEBUG_BUILD) || defined(BUTANE_DEVELOPMENT_BUILD)
      { LUA_DBLIBNAME, luaopen_debug },
    #endif
      { NULL, NULL }
    };

    for (const luaL_Reg* lib = &libs[0]; lib->func; ++lib) {
      luaL_requiref(_state, lib->name, lib->func, 1);
      lua_pop(_state, 1);
    }
  }

  Script::~Script()
  {
    if (_state)
      lua_close(_state);
  }

  void* Script::__alloc(
    Script* script,
    void* ptr,
    size_t old_size,
    size_t new_size )
  {
    script->_memory_usage -= old_size;
    script->_memory_usage += new_size;
    return Allocators::heap().realloc(ptr, new_size);
  }

  int Script::__forwarding_closure(
    lua_State* state )
  {
    Script& script =
      (*(Script*)lua_touserdata(state, lua_upvalueindex(1)));
    Script::Function function =
      (Script::Function)lua_touserdata(state, lua_upvalueindex(2));
    return (int)function(script, Script::Arguments(script));
  }

  int Script::__error_handler(
    lua_State* state )
  {
    Script& script = *((Script*)lua_touserdata(state, lua_upvalueindex(1)));
    script.error(lua_tostring(state, 1));
    return 0;
  }

  const char* Script::__qualified_name(
    lua_State* state,
    const char* name )
  {
    assert(state != nullptr);
    assert(name != nullptr);

    const char* qn /*qualified_name*/ = name;
    lua_rawgeti(state, LUA_REGISTRYINDEX, LUA_RIDX_GLOBALS);

    while (qn) {
      const char* seperator = find(qn, ".");
      if (!seperator)
        break;

      lua_pushlstring(state, qn, (size_t)(seperator - qn));
      lua_gettable(state, -2);

      if (lua_isnil(state, -1)) {
        lua_pop(state, 1);
        lua_createtable(state, 0, 0);
        lua_pushlstring(state, qn, (size_t)(seperator - qn));
        lua_pushvalue(state, -2);
        lua_settable(state, -4);
      }

      lua_replace(state, -2);
      qn = next(seperator);
    }

    if (!qn)
      lua_pop(state, 1);

    return qn;
  }

  bool Script::load(
    const char* name,
    const void* code,
    size_t code_len )
  {
    assert(name != nullptr);
    assert(code != nullptr);
    assert(code_len != 0);

    if (luaL_loadbuffer(_state, (const char*)code, code_len, name) != 0) {
      warn("luaL_loadbuffer failed:\n\n%s\n", lua_tostring(_state, -1));
      lua_pop(_state, 1);
      return false; }

    lua_pushlightuserdata(_state, (void*)this);
    lua_pushcclosure(_state, &Script::__error_handler, 1);
    lua_insert(_state, -2);

    if (lua_pcall(_state, 0, LUA_MULTRET, -2) != 0)
      return false;

    return true;
  }

  bool Script::load(
    const String& name,
    const void* code,
    size_t code_len )
  {
    return load(name.raw(), code, code_len);
  }

  bool Script::call(
    const char* name,
    size_t num_of_arguments,
    size_t& num_of_returns )
  {
    assert(name != nullptr);

    const char* qn /* qualified_name */ =
      Script::__qualified_name(_state, name);

    if (!qn)
      return false;

    lua_getfield(_state, -1, qn);
    lua_insert(_state, lua_gettop(_state) - (int)(num_of_arguments + 1));
    lua_pop(_state, 1);

    lua_pushlightuserdata(_state, (void*)this);
    lua_pushcclosure(_state, &Script::__error_handler, 1);
    const int ehi = lua_gettop(_state) - (int)(num_of_arguments + 2);
    lua_insert(_state, ehi);

    const int top_ = lua_gettop(_state);
    if (lua_pcall(_state, num_of_arguments, LUA_MULTRET, ehi) != 0) {
      lua_pop(_state, 1);
      return false; }

    num_of_returns = lua_gettop(_state) - (top_ - num_of_arguments - 1);
    lua_pop(_state, 1);

    return true;
  }

  bool Script::call(
    const String& name,
    size_t num_of_arguments,
    size_t& num_of_returns )
  {
    return call(name.raw(), num_of_arguments, num_of_returns);
  }

  butane::Script::Type& Script::type(
    Type::Constructor ctor,
    Type::Destructor dtor )
  {
    assert(ctor != nullptr);
    assert(dtor != nullptr);

    lua_createtable(_state, 0, 4);

    lua_pushlightuserdata(_state, (void*)this);
    lua_pushlightuserdata(_state, (void*)ctor);
    lua_pushvalue(_state, -3);
    lua_pushcclosure(_state, &Type::__ctor, 3);
    lua_setfield(_state, -2, "__call");

    lua_pushlightuserdata(_state, (void*)this);
    lua_pushlightuserdata(_state, (void*)dtor);
    lua_pushcclosure(_state, &Type::__dtor, 2);
    lua_setfield(_state, -2, "__gc");

    lua_pushlightuserdata(_state, (void*)this);
    lua_pushvalue(_state, -2);
    lua_pushcclosure(_state, &Type::__get, 2);
    lua_setfield(_state, -2, "__index");

    lua_pushlightuserdata(_state, (void*)this);
    lua_pushvalue(_state, -2);
    lua_pushcclosure(_state, &Type::__set, 2);
    lua_setfield(_state, -2, "__newindex");

    return *(make_new(Type, Allocators::heap())(*this));
  }

  void Script::expose(
    const char* name,
    Function function )
  {
    assert(name != nullptr);
    assert(function != nullptr);

    const char* qn /* qualified_name */ =
      Script::__qualified_name(_state, name);

    if (!qn) {
      fail("Unable to expose '%s' (unable to qualify name!)", name);
      return; }

    lua_pushstring(_state, qn);
    lua_pushlightuserdata(_state, (void*)this);
    lua_pushlightuserdata(_state, (void*)function);
    lua_pushcclosure(_state, &Script::__forwarding_closure, 2);
    lua_settable(_state, -3);
    lua_pop(_state, 1);
  }

  void Script::expose(
    const String& name,
    Function function )
  {
    return expose(name.raw(), function);
  }

  void Script::set(
    const char* name )
  {
    assert(name != nullptr);

    const char* qn /* qualified_name */ =
      Script::__qualified_name(_state, name);

    if (!qn) {
      error("Unable to set '%s' (unable to qualify name!)", name);
      return; }

    lua_pushnil(_state);
    lua_setfield(_state, -2, qn);
    lua_pop(_state, 1);
  }

  void Script::set(
    const char* name,
    void* ptr )
  {
    assert(name != nullptr);

    const char* qn /* qualified_name */ =
      Script::__qualified_name(_state, name);

    if (!qn) {
      error("Unable to set '%s' (unable to qualify name!)", name);
      return; }

    lua_pushlightuserdata(_state, ptr);
    lua_setfield(_state, -2, qn);
    lua_pop(_state, 1);
  }

  void Script::get(
    const char* name,
    void*& ptr ) const
  {
    assert(name != nullptr);

    const char* qn /* qualified_name */ =
      Script::__qualified_name(_state, name);

    if (!qn) {
      error("Unable to get '%s' (unable to qualify name!)", name);
      return; }

    lua_getfield(_state, -1, qn);
    if(!lua_islightuserdata(_state, -1))
      error("Trying to get non-pointer value!");
    ptr = lua_touserdata(_state, -1);
    lua_pop(_state, 2);
  }

  void Script::set(
    const char* name,
    bool boolean )
  {
    assert(name != nullptr);

    const char* qn /* qualified_name */ =
      Script::__qualified_name(_state, name);

    if (!qn) {
      error("Unable to set '%s' (unable to qualify name!)", name);
      return; }

    lua_pushboolean(_state, boolean);
    lua_setfield(_state, -2, qn);
    lua_pop(_state, 1);
  }

  void Script::get(
    const char* name,
    bool& boolean ) const
  {
    assert(name != nullptr);

    const char* qn /* qualified_name */ =
      Script::__qualified_name(_state, name);

    if (!qn) {
      error("Unable to get '%s' (unable to qualify name!)", name);
      return; }

    lua_getfield(_state, -1, qn);
    if (!lua_isboolean(_state, -1))
      error("Trying to get non-boolean value!");
    boolean = lua_toboolean(_state, -1);
    lua_pop(_state, 2);
  }

  void Script::set(
    const char* name,
    int integer )
  {
    assert(name != nullptr);

    const char* qn /* qualified_name */ =
      Script::__qualified_name(_state, name);

    if (!qn) {
      error("Unable to set '%s' (unable to qualify name!)", name);
      return; }

    lua_pushinteger(_state, integer);
    lua_setfield(_state, -2, qn);
    lua_pop(_state, 1);
  }

  void Script::get(
    const char* name,
    int& integer ) const
  {
    assert(name != nullptr);

    const char* qn /* qualified_name */ =
      Script::__qualified_name(_state, name);

    if (!qn) {
      error("Unable to get '%s' (unable to qualify name!)", name);
      return; }

    lua_getfield(_state, -1, qn);
    if (!lua_isnumber(_state, -1))
      error("Trying to get non-integer value!");
    integer = lua_tointeger(_state, -1);
    lua_pop(_state, 2);
  }

  void Script::set(
    const char* name,
    float number )
  {
    assert(name != nullptr);

    const char* qn /* qualified_name */ =
      Script::__qualified_name(_state, name);

    if (!qn) {
      error("Unable to set '%s' (unable to qualify name!)", name);
      return; }

    lua_pushnumber(_state, number);
    lua_setfield(_state, -2, qn);
    lua_pop(_state, 1);
  }

  void Script::get(
    const char* name,
    float& number ) const
  {
    assert(name != nullptr);

    const char* qn /* qualified_name */ =
      Script::__qualified_name(_state, name);

    if (!qn) {
      error("Unable to get '%s' (unable to qualify name!)", name);
      return; }

    lua_getfield(_state, -1, qn);
    if (!lua_isnumber(_state, -1))
      error("Trying to get non-number value!");
    number = lua_tonumber(_state, -1);
    lua_pop(_state, 2);
  }

  void Script::set(
    const char* name,
    double number )
  {
    assert(name != nullptr);

    const char* qn /* qualified_name */ =
      Script::__qualified_name(_state, name);

    if (!qn) {
      error("Unable to set '%s' (unable to qualify name!)", name);
      return; }

    lua_pushnumber(_state, number);
    lua_setfield(_state, -2, qn);
    lua_pop(_state, 1);
  }

  void Script::get(
    const char* name,
    double& number ) const
  {
    assert(name != nullptr);

    const char* qn /* qualified_name */ =
      Script::__qualified_name(_state, name);

    if (!qn) {
      error("Unable to get '%s' (unable to qualify name!)", name);
      return; }

    lua_getfield(_state, -1, qn);
    if (!lua_isnumber(_state, -1))
      error("Trying to get non-number value!");
    number = lua_tonumber(_state, -1);
    lua_pop(_state, 2);
  }

  void Script::set(
    const char* name,
    const char* string )
  {
    assert(name != nullptr);

    const char* qn /* qualified_name */ =
      Script::__qualified_name(_state, name);

    if (!qn) {
      error("Unable to set '%s' (unable to qualify name!)", name);
      return; }

    lua_pushstring(_state, string);
    lua_setfield(_state, -2, qn);
    lua_pop(_state, 1);
  }

  void Script::set(
    const char* name,
    const String& string )
  {
    assert(name != nullptr);

    const char* qn /* qualified_name */ =
      Script::__qualified_name(_state, name);

    if (!qn) {
      error("Unable to set '%s' (unable to qualify name!)", name);
      return; }

    lua_pushstring(_state, string.raw());
    lua_setfield(_state, -2, qn);
    lua_pop(_state, 1);
  }

  void Script::get(
    const char* name,
    String& string ) const
  {
    assert(name != nullptr);

    const char* qn /* qualified_name */ =
      Script::__qualified_name(_state, name);

    if (!qn) {
      error("Unable to get '%s' (unable to qualify name!)", name);
      return; }

    lua_getfield(_state, -1, qn);
    if (!lua_isstring(_state, -1))
      error("Trying to get non- value!");
    string = lua_tostring(_state, -1);
    lua_pop(_state, 2);
  }

  const butane::Script::Stack& Script::stack()
  {
    return _stack;
  }
} // Lua
} // butane

namespace butane {
  namespace Lua {
    static void* __compiler_alloc(
      void* /* ud */,
      void* ptr,
      size_t old_size,
      size_t new_size )
    {
      return Allocators::heap().realloc(ptr, new_size);
    }

    struct Writer {
      bool (*write)(
        void* closure,
        const void* bytes,
        size_t num_of_bytes );
      void* closure;
    };

    static int __compiler_writer(
      lua_State* state,
      const void* p,
      size_t sz,
      void* ud )
    {
      const Writer* writer = (const Writer*)ud;
      return (writer->write(writer->closure, p, sz) ? 0 : 1);
    }

    bool Script::compile(
      const char* name,
      const char* code,
      size_t code_len,
      bool (*write)(
        void* closure,
        const void* bytes,
        size_t num_of_bytes ),
      void (*error)(
        void* closure,
        const char* error ),
      void* closure )
    {
      assert(name != nullptr);
      assert(code != nullptr);
      assert(code_len != 0);
      assert(write != nullptr);
      assert(error != nullptr);

      lua_State* state = lua_newstate((lua_Alloc)&__compiler_alloc, nullptr);

      if (luaL_loadbuffer(state, (const char*)code, code_len, name) != 0) {
        error(closure, lua_tostring(state, -1));
        lua_pop(state, 1);
        lua_close(state);
        return false; }

      Writer writer;
      writer.write = write;
      writer.closure = closure;

      if (lua_dump(state, &__compiler_writer, (void*)&writer)) {
        lua_close(state);
        return false; }

      lua_close(state);
      return true;
    }
  } // Lua

  bool Script::compile(
    const char* name,
    const char* code,
    size_t code_len,
    bool (*write)(
      void* closure,
      const void* bytes,
      size_t num_of_bytes ),
    void (*error)(
      void* closure,
      const char* error ),
    void* closure )
  {
    return Lua::Script::compile(name, code, code_len, write, error, closure);
  }
} // butane
