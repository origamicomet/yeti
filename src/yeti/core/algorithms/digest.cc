//===-- yeti/core/algorithms/digest.cc ------------------*- mode: C++11 -*-===//
//
//                 _____               _     _   _
//                |   __|___ _ _ ___ _| |___| |_|_|___ ___
//                |   __| . | | |   | . | .'|  _| | . |   |
//                |__|  |___|___|_|_|___|__,|_| |_|___|_|_|
//
//       This file is distributed under the terms described in LICENSE.
//
//===----------------------------------------------------------------------===//

#include "yeti/core/algorithms/digest.h"

// For streaming variants.
#include "yeti/core/platform/filesystem.h"

namespace yeti {
namespace core {

static YETI_INLINE u32 rotl(u32 v, u32 r) {
  return (v << r) | (v >> (32 - r));
}

static YETI_INLINE u32 rotr(u32 v, u32 r) {
  return (v >> r) | (v << (32 - r));
}

// Based on Steve Reid's <steve@edmweb.com> public domain implementation.
namespace sha1 {
  struct Context {
    u32 state[5];
    u32 count[2];
    u8  buffer[64];
  };

  // These perform the inital expand.
  // Steve got the idea of expanding during the round function from SSLeay.
#if YETI_ARCHITECTURE == YETI_ARCHITECTURE_X86 || \
    YETI_ARCHITECTURE == YETI_ARCHITECTURE_X86_64
  #define blk0(i) (block->l[i] = (rotl(block->l[i], 24) & 0xFF00FF00) \
                               | (rotl(block->l[i], 8)  & 0x00FF00FF))
#elif 0
  // Big endian.
  #define blk0(i) (block->l[i])
#endif

  #define blk(i) \
    (block->l[i&15] = rotl(block->l[(i+13)&15] ^ block->l[(i+8)&15] \
                                               ^ block->l[(i+2)&15] \
                                               ^ block->l[i&15], 1))

  // These are the different operations used in SHA-11.
  #define R0(v,w,x,y,z,i) z+=((w&(x^y))^y)+blk0(i)+0x5A827999+rotl(v,5);w=rotl(w,30);
  #define R1(v,w,x,y,z,i) z+=((w&(x^y))^y)+blk(i)+0x5A827999+rotl(v,5);w=rotl(w,30);
  #define R2(v,w,x,y,z,i) z+=(w^x^y)+blk(i)+0x6ED9EBA1+rotl(v,5);w=rotl(w,30);
  #define R3(v,w,x,y,z,i) z+=(((w|x)&y)|(w&x))+blk(i)+0x8F1BBCDC+rotl(v,5);w=rotl(w,30);
  #define R4(v,w,x,y,z,i) z+=(w^x^y)+blk(i)+0xCA62C1D6+rotl(v,5);w=rotl(w,30);

  // Hashes a single 512-bit block. This is the core of the algorithm.
  static void transform(u32 state[5], const u8 buffer[64]) {
    i32 a, b, c, d, e;

    typedef union {
      u8  c[64];
      u32 l[16];
    } Block;

    // An array to appear as a pointer.
    Block block[1];

    // Copy before munging.
    memcpy(block, buffer, 64);

    // Copy state to working variables.
    a = state[0];
    b = state[1];
    c = state[2];
    d = state[3];
    e = state[4];

    // 4 rounds of 20 operations each. Loop unrolled.
    R0(a, b, c, d, e, 0);
    R0(e, a, b, c, d, 1);
    R0(d, e, a, b, c, 2);
    R0(c, d, e, a, b, 3);
    R0(b, c, d, e, a, 4);
    R0(a, b, c, d, e, 5);
    R0(e, a, b, c, d, 6);
    R0(d, e, a, b, c, 7);
    R0(c, d, e, a, b, 8);
    R0(b, c, d, e, a, 9);
    R0(a, b, c, d, e, 10);
    R0(e, a, b, c, d, 11);
    R0(d, e, a, b, c, 12);
    R0(c, d, e, a, b, 13);
    R0(b, c, d, e, a, 14);
    R0(a, b, c, d, e, 15);
    R1(e, a, b, c, d, 16);
    R1(d, e, a, b, c, 17);
    R1(c, d, e, a, b, 18);
    R1(b, c, d, e, a, 19);
    R2(a, b, c, d, e, 20);
    R2(e, a, b, c, d, 21);
    R2(d, e, a, b, c, 22);
    R2(c, d, e, a, b, 23);
    R2(b, c, d, e, a, 24);
    R2(a, b, c, d, e, 25);
    R2(e, a, b, c, d, 26);
    R2(d, e, a, b, c, 27);
    R2(c, d, e, a, b, 28);
    R2(b, c, d, e, a, 29);
    R2(a, b, c, d, e, 30);
    R2(e, a, b, c, d, 31);
    R2(d, e, a, b, c, 32);
    R2(c, d, e, a, b, 33);
    R2(b, c, d, e, a, 34);
    R2(a, b, c, d, e, 35);
    R2(e, a, b, c, d, 36);
    R2(d, e, a, b, c, 37);
    R2(c, d, e, a, b, 38);
    R2(b, c, d, e, a, 39);
    R3(a, b, c, d, e, 40);
    R3(e, a, b, c, d, 41);
    R3(d, e, a, b, c, 42);
    R3(c, d, e, a, b, 43);
    R3(b, c, d, e, a, 44);
    R3(a, b, c, d, e, 45);
    R3(e, a, b, c, d, 46);
    R3(d, e, a, b, c, 47);
    R3(c, d, e, a, b, 48);
    R3(b, c, d, e, a, 49);
    R3(a, b, c, d, e, 50);
    R3(e, a, b, c, d, 51);
    R3(d, e, a, b, c, 52);
    R3(c, d, e, a, b, 53);
    R3(b, c, d, e, a, 54);
    R3(a, b, c, d, e, 55);
    R3(e, a, b, c, d, 56);
    R3(d, e, a, b, c, 57);
    R3(c, d, e, a, b, 58);
    R3(b, c, d, e, a, 59);
    R4(a, b, c, d, e, 60);
    R4(e, a, b, c, d, 61);
    R4(d, e, a, b, c, 62);
    R4(c, d, e, a, b, 63);
    R4(b, c, d, e, a, 64);
    R4(a, b, c, d, e, 65);
    R4(e, a, b, c, d, 66);
    R4(d, e, a, b, c, 67);
    R4(c, d, e, a, b, 68);
    R4(b, c, d, e, a, 69);
    R4(a, b, c, d, e, 70);
    R4(e, a, b, c, d, 71);
    R4(d, e, a, b, c, 72);
    R4(c, d, e, a, b, 73);
    R4(b, c, d, e, a, 74);
    R4(a, b, c, d, e, 75);
    R4(e, a, b, c, d, 76);
    R4(d, e, a, b, c, 77);
    R4(c, d, e, a, b, 78);
    R4(b, c, d, e, a, 79);

    // Add the working variables back to state.
    state[0] += a;
    state[1] += b;
    state[2] += c;
    state[3] += d;
    state[4] += e;

    // Wipe variables.
    a = b = c = d = e = 0;
    memset(block, '\0', sizeof(block));
  }

  static void initialize(Context *context) {
    context->state[0] = 0x67452301;
    context->state[1] = 0xEFCDAB89;
    context->state[2] = 0x98BADCFE;
    context->state[3] = 0x10325476;
    context->state[4] = 0xC3D2E1F0;
    context->count[0] = context->count[1] = 0;
  }

  static void update(Context *context, const u8 *data, u32 length) {
    u32 i, j;

    j = context->count[0];

    if ((context->count[0] += length << 3) < j)
      context->count[1] += 1;

    context->count[1] += (length >> 29);

    j = (j >> 3) & 63;

    if ((j + length) > 63) {
      memcpy(&context->buffer[j], data, (i = 64 - j));
      transform(context->state, context->buffer);
      for (; i + 63 < length; i += 64)
        transform(context->state, &data[i]);
      j = 0;
    } else {
      i = 0;
    }

    memcpy(&context->buffer[j], &data[i], length - i);
  }

  // Pad and return digest.
  static void final(Context *context, u8 digest[20]) {
    unsigned i;

    u8 fc[8];
    u8 c;

    for (i = 0; i < 8; i++)
      fc[i] = (u8)((context->count[(i >= 4 ? 0 : 1)] >> ((3 - (i & 3)) * 8)) & 255);

    c = 0200;
    update(context, &c, 1);
    while ((context->count[0] & 504) != 448) {
      c = 0000;
      update(context, &c, 1);
    }

    update(context, fc, 8);
    for (i = 0; i < 20; i++)
      digest[i] = (u8)((context->state[i >> 2] >> ((3 - (i & 3)) * 8)) & 255);

    // Wipe variables.
    memset(context, '\0', sizeof(*context));
    memset(&fc, '\0', sizeof(fc));
  }

  void compute(const void *buf, u32 buf_len, u8 digest[20]) {
    Context context;

    initialize(&context);
    update(&context, (const u8 *)buf, buf_len);
    final(&context, digest);
  }

  void compute(File *file, u8 digest[20]) {
    Context context;

    initialize(&context);

    u8 buffer[4096];

    while (const u32 read = fs::read(file, (void *)&buffer[0], sizeof(buffer)))
      update(&context, &buffer[0], read);

    final(&context, digest);
  }

  void present(const u8 digest[20], char pretty[41]) {
    for (unsigned i = 0; i < 20; ++i)
      sprintf(&pretty[i * 2], "%02x", digest[i]);
  }

  #undef blk0
  #undef blk

  #undef R0
  #undef R1
  #undef R2
  #undef R3
  #undef R4
}

} // core
} // yeti
