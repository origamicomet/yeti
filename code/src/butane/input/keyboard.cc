// This file is part of Butane. See README.md and LICENSE.md for details.
// Copyright (c) 2012 Michael Williams <devbug@bitbyte.ca>

#include <butane/input/keyboard.h>

namespace butane {
  namespace Keyboard {
    static const struct {
      Button id;
      Hash<uint32_t, murmur_hash> key;
      const char* name;
    } _key_map[] = {
      {  0,          Hash<uint32_t, murmur_hash>("esc"),       "esc" },
      {  0,       Hash<uint32_t, murmur_hash>("escape"),    "escape" },
      {  1,           Hash<uint32_t, murmur_hash>("f1"),        "f1" },
      {  2,           Hash<uint32_t, murmur_hash>("f2"),        "f2" },
      {  3,           Hash<uint32_t, murmur_hash>("f3"),        "f3" },
      {  4,           Hash<uint32_t, murmur_hash>("f4"),        "f4" },
      {  5,           Hash<uint32_t, murmur_hash>("f5"),        "f5" },
      {  6,           Hash<uint32_t, murmur_hash>("f6"),        "f6" },
      {  7,           Hash<uint32_t, murmur_hash>("f7"),        "f7" },
      {  8,           Hash<uint32_t, murmur_hash>("f8"),        "f8" },
      {  9,           Hash<uint32_t, murmur_hash>("f9"),        "f9" },
      { 10,          Hash<uint32_t, murmur_hash>("f10"),       "f10" },
      { 11,          Hash<uint32_t, murmur_hash>("f11"),       "f11" },
      { 12,          Hash<uint32_t, murmur_hash>("f12"),       "f12" },
      { 13,            Hash<uint32_t, murmur_hash>("`"),         "`" },
      { 13,            Hash<uint32_t, murmur_hash>("~"),         "~" },
      { 14,            Hash<uint32_t, murmur_hash>("-"),         "-" },
      { 14,            Hash<uint32_t, murmur_hash>("_"),         "_" },
      { 15,            Hash<uint32_t, murmur_hash>("="),         "=" },
      { 15,            Hash<uint32_t, murmur_hash>("+"),         "+" },
      { 16,            Hash<uint32_t, murmur_hash>("0"),         "0" },
      { 16,            Hash<uint32_t, murmur_hash>("0"),         ")" },
      { 17,            Hash<uint32_t, murmur_hash>("1"),         "1" },
      { 17,            Hash<uint32_t, murmur_hash>("1"),         "!" },
      { 18,            Hash<uint32_t, murmur_hash>("2"),         "2" },
      { 18,            Hash<uint32_t, murmur_hash>("2"),         "@" },
      { 19,            Hash<uint32_t, murmur_hash>("3"),         "3" },
      { 19,            Hash<uint32_t, murmur_hash>("3"),         "#" },
      { 20,            Hash<uint32_t, murmur_hash>("4"),         "4" },
      { 20,            Hash<uint32_t, murmur_hash>("4"),         "$" },
      { 21,            Hash<uint32_t, murmur_hash>("5"),         "5" },
      { 21,            Hash<uint32_t, murmur_hash>("5"),         "%" },
      { 22,            Hash<uint32_t, murmur_hash>("6"),         "6" },
      { 22,            Hash<uint32_t, murmur_hash>("6"),         "^" },
      { 23,            Hash<uint32_t, murmur_hash>("7"),         "7" },
      { 23,            Hash<uint32_t, murmur_hash>("7"),         "&" },
      { 24,            Hash<uint32_t, murmur_hash>("8"),         "8" },
      { 24,            Hash<uint32_t, murmur_hash>("8"),         "*" },
      { 25,            Hash<uint32_t, murmur_hash>("9"),         "9" },
      { 25,            Hash<uint32_t, murmur_hash>("9"),         "(" },
      { 26,            Hash<uint32_t, murmur_hash>("["),         "[" },
      { 26,            Hash<uint32_t, murmur_hash>("{"),         "{" },
      { 27,            Hash<uint32_t, murmur_hash>("]"),         "]" },
      { 27,            Hash<uint32_t, murmur_hash>("}"),         "}" },
      { 28,            Hash<uint32_t, murmur_hash>(";"),         ";" },
      { 28,            Hash<uint32_t, murmur_hash>(":"),         ":" },
      { 29,            Hash<uint32_t, murmur_hash>("'"),         "'" },
      { 29,           Hash<uint32_t, murmur_hash>("\""),        "\"" },
      { 30,            Hash<uint32_t, murmur_hash>(","),         "," },
      { 30,            Hash<uint32_t, murmur_hash>("<"),         "<" },
      { 31,            Hash<uint32_t, murmur_hash>("."),         "." },
      { 31,            Hash<uint32_t, murmur_hash>(">"),         ">" },
      { 32,            Hash<uint32_t, murmur_hash>("/"),         "/" },
      { 32,            Hash<uint32_t, murmur_hash>("?"),         "?" },
      { 33,           Hash<uint32_t, murmur_hash>("\\"),        "\\" },
      { 33,            Hash<uint32_t, murmur_hash>("|"),         "|" },
      { 34,            Hash<uint32_t, murmur_hash>("a"),         "a" },
      { 35,            Hash<uint32_t, murmur_hash>("b"),         "b" },
      { 36,            Hash<uint32_t, murmur_hash>("c"),         "c" },
      { 37,            Hash<uint32_t, murmur_hash>("d"),         "d" },
      { 38,            Hash<uint32_t, murmur_hash>("e"),         "e" },
      { 39,            Hash<uint32_t, murmur_hash>("f"),         "f" },
      { 40,            Hash<uint32_t, murmur_hash>("g"),         "g" },
      { 41,            Hash<uint32_t, murmur_hash>("h"),         "h" },
      { 42,            Hash<uint32_t, murmur_hash>("i"),         "i" },
      { 43,            Hash<uint32_t, murmur_hash>("j"),         "j" },
      { 44,            Hash<uint32_t, murmur_hash>("k"),         "k" },
      { 45,            Hash<uint32_t, murmur_hash>("l"),         "l" },
      { 46,            Hash<uint32_t, murmur_hash>("m"),         "m" },
      { 47,            Hash<uint32_t, murmur_hash>("n"),         "n" },
      { 48,            Hash<uint32_t, murmur_hash>("o"),         "o" },
      { 49,            Hash<uint32_t, murmur_hash>("p"),         "p" },
      { 50,            Hash<uint32_t, murmur_hash>("q"),         "q" },
      { 51,            Hash<uint32_t, murmur_hash>("r"),         "r" },
      { 52,            Hash<uint32_t, murmur_hash>("s"),         "s" },
      { 53,            Hash<uint32_t, murmur_hash>("t"),         "t" },
      { 54,            Hash<uint32_t, murmur_hash>("u"),         "u" },
      { 55,            Hash<uint32_t, murmur_hash>("v"),         "v" },
      { 56,            Hash<uint32_t, murmur_hash>("w"),         "w" },
      { 57,            Hash<uint32_t, murmur_hash>("x"),         "x" },
      { 58,            Hash<uint32_t, murmur_hash>("y"),         "y" },
      { 59,            Hash<uint32_t, murmur_hash>("z"),         "z" },
      { 60,    Hash<uint32_t, murmur_hash>("backspace"), "backspace" },
      { 61,        Hash<uint32_t, murmur_hash>("enter"),     "enter" },
      { 61,       Hash<uint32_t, murmur_hash>("return"),    "return" },
      { 62,         Hash<uint32_t, murmur_hash>("ctrl"),      "ctrl" },
      { 63,          Hash<uint32_t, murmur_hash>("alt"),       "alt" },
      { 64,        Hash<uint32_t, murmur_hash>("shift"),     "shift" },
      { 65,     Hash<uint32_t, murmur_hash>("capslock"),  "capslock" },
      { 66,          Hash<uint32_t, murmur_hash>("tab"),       "tab" },
      { 67,        Hash<uint32_t, murmur_hash>("space"),     "space" },
      { 68,       Hash<uint32_t, murmur_hash>("insert"),    "insert" },
      { 69,       Hash<uint32_t, murmur_hash>("delete"),    "delete" },
      { 70,         Hash<uint32_t, murmur_hash>("home"),      "home" },
      { 71,          Hash<uint32_t, murmur_hash>("end"),       "end" },
      { 72,      Hash<uint32_t, murmur_hash>("page_up"),   "page_up" },
      { 73,    Hash<uint32_t, murmur_hash>("page_down"), "page_down" },
      { 74,         Hash<uint32_t, murmur_hash>("left"),      "left" },
      { 75,        Hash<uint32_t, murmur_hash>("right"),     "right" },
      { 76,           Hash<uint32_t, murmur_hash>("up"),        "up" },
      { 77,         Hash<uint32_t, murmur_hash>("down"),      "down" }
    };

    Button name(
      const char* name )
    {
      assert(name != nullptr);
      const Hash<uint32_t, murmur_hash> key(name);
      for (size_t i = 0; i < 101; ++i)
        if (_key_map[i].key == key)
          return _key_map[i].id;
      return 0xFFFFFFFFu;
    }

    static Input::State _buttons[78] = { Input::UP, };

    Input::State button(
      const Button button )
    {
      assert(button >= 0);
      assert(button <= 77);

      return _buttons[button];
    }

    void set_button(
      const Button button,
      const Input::State state )
    {
      assert(button >= 0);
      assert(button <= 77);

      _buttons[button] = state;
    }
  } // Keyboard
} // butane

