//===-- yeti/core/misc/uuid.cc --------------------------*- mode: C++11 -*-===//
//
//                 _____               _     _   _
//                |   __|___ _ _ ___ _| |___| |_|_|___ ___
//                |   __| . | | |   | . | .'|  _| | . |   |
//                |__|  |___|___|_|_|___|__,|_| |_|___|_|_|
//
//       This file is distributed under the terms described in LICENSE.
//
//===----------------------------------------------------------------------===//

#include "yeti/core/misc/uuid.h"

// Used to generate UUID v4.
#include "yeti/core/algorithms/random.h"

namespace yeti {
namespace core {

// TODO(mtwilliams): Improve validation.
// TODO(mtwilliams): Improve randomness.

void uuid::generate(u8 uuid[16]) {
  // Fill with random bytes.
  random_n(uuid, sizeof(uuid));

  // Mark as version four variant one as it is.
  uuid[6] = (uuid[6] & 0x0F) | 0x40;
  uuid[8] = (uuid[8] & 0x3F) | 0x80;
}

bool uuid::parse(const char *string, u8 uuid[16]) {
  unsigned n = 0;

  sscanf(string, "%2hhx%2hhx%2hhx%2hhx-"
                 "%2hhx%2hhx-"
                 "%2hhx%2hhx-"
                 "%2hhx%2hhx-"
                 "%2hhx%2hhx%2hhx%2hhx%2hhx%2hhx%n",
                 &uuid[0], &uuid[1], &uuid[2], &uuid[3],
                 &uuid[4], &uuid[5],
                 &uuid[6], &uuid[7],
                 &uuid[8], &uuid[9],
                 &uuid[10], &uuid[11], &uuid[12], &uuid[13], &uuid[14], &uuid[15],
                 &n);

  return (n == 36);
}

bool uuid::validate(const char *string) {
  u8 dummy[16];
  return uuid::parse(string, dummy);
}

void uuid::present(const u8 uuid[20], char pretty[36+1]) {
  sprintf(pretty, "%2hhx%2hhx%2hhx%2hhx-"
                  "%2hhx%2hhx-"
                  "%2hhx%2hhx-"
                  "%2hhx%2hhx-"
                  "%2hhx%2hhx%2hhx%2hhx%2hhx%2hhx",
                  uuid[0], uuid[1], uuid[2], uuid[3],
                  uuid[4], uuid[5],
                  uuid[6], uuid[7],
                  uuid[8], uuid[9],
                  uuid[10], uuid[11], uuid[12], uuid[13], uuid[14], uuid[15]);
}

} // core
} // yeti
