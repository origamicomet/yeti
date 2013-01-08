#if !defined(_WIN32)
  #error Including lwe/platforms/windows.h in non-windows build is forbidden.
#endif

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

// #undef near
// #undef far
// #undef min
// #undef max