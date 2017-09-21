//===-- yeti/core/platform/entropy.cc -------------------*- mode: C++11 -*-===//
//
//                 _____               _     _   _
//                |   __|___ _ _ ___ _| |___| |_|_|___ ___
//                |   __| . | | |   | . | .'|  _| | . |   |
//                |__|  |___|___|_|_|___|__,|_| |_|___|_|_|
//
//       This file is distributed under the terms described in LICENSE.
//
//===----------------------------------------------------------------------===//

#include "yeti/core/platform/entropy.h"

#if YETI_PLATFORM == YETI_PLATFORM_WINDOWS
  #include <windows.h>
  #include <wincrypt.h>
#elif YETI_PLATFORM == YETI_PLATFORM_MAC
#elif YETI_PLATFORM == YETI_PLATFORM_LINUX
#elif YETI_PLATFORM == YETI_PLATFORM_IOS
#elif YETI_PLATFORM == YETI_PLATFORM_ANDROID
#endif

namespace yeti {
namespace core {

namespace {
#if YETI_PLATFORM == YETI_PLATFORM_WINDOWS
  class StrongCryptoServiceProvider {
   public:
    StrongCryptoServiceProvider() {
      ::CryptAcquireContext(&csp, NULL, NULL, PROV_RSA_FULL, CRYPT_VERIFYCONTEXT);
    }

    ~StrongCryptoServiceProvider() {
      ::CryptReleaseContext(csp, 0);
    }

   public:
    HCRYPTPROV csp;
  };

  static StrongCryptoServiceProvider strong_crypto_service_provider_;
#endif
}

void entropy(void *buffer, size_t amount) {
#if YETI_PLATFORM == YETI_PLATFORM_WINDOWS
  ::CryptGenRandom(strong_crypto_service_provider_.csp, amount, (BYTE *)buffer);
#elif YETI_PLATFORM == YETI_PLATFORM_MAC || \
      YETI_PLATFORM == YETI_PLATFORM_LINUX
  // TODO(mtwilliams): Read from `/dev/urandom`.
  YETI_TRAP();
#endif
}

} // core
} // yeti
