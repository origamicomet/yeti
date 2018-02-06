//===-- yeti/core/algorithms/random.cc ------------------*- mode: C++11 -*-===//
//
//                 _____               _     _   _
//                |   __|___ _ _ ___ _| |___| |_|_|___ ___
//                |   __| . | | |   | . | .'|  _| | . |   |
//                |__|  |___|___|_|_|___|__,|_| |_|___|_|_|
//
//       This file is distributed under the terms described in LICENSE.
//
//===----------------------------------------------------------------------===//

#include "yeti/core/algorithms/random.h"

#include <stdlib.h>
#include <time.h>

namespace yeti {
namespace core {

// We use xorshift1024* pseudo-random number generator per-thread to generate
// random numbers. Nothing fancy, but it's fast.

class PseudoRandomNumberGenerator {
 YETI_DISALLOW_COPYING(PseudoRandomNumberGenerator)

 public:
  PseudoRandomNumberGenerator();
  ~PseudoRandomNumberGenerator();

 private:
  void seed();

 public:
  u64 next();

 private:
  u64 state_[16];
  u64 p_;
};

PseudoRandomNumberGenerator::PseudoRandomNumberGenerator() {
  this->seed();
}

PseudoRandomNumberGenerator::~PseudoRandomNumberGenerator() {
}

void PseudoRandomNumberGenerator::seed() {
  // TODO(mtwilliams): Seed from a better source of entropy?
  ::srand(::time(NULL));

  for (unsigned i = 0; i < 16; ++i)
    state_[i] = ((u64)::rand() << 32ull) | (u64)::rand();

  p_ = 0;
}

u64 PseudoRandomNumberGenerator::next() {
  u64 s0 = state_[p_];
  u64 s1 = state_[p_ = (p_ + 1) & 15];
  s1 ^= s1 << 31;
  state_[p_] = s1 ^ s0 ^ (s1 >> 11) ^ (s0 >> 30);
  return state_[p_] * 1181783497276652981ull;
}

static YETI_THREAD_LOCAL PseudoRandomNumberGenerator prng_for_thread_;

template <> u64 random() { return prng_for_thread_.next(); }
template <> u32 random() { return u32(prng_for_thread_.next() >> 32ull); }
template <> u16 random() { return u16(prng_for_thread_.next() >> 48ull); }
template <> u8  random() { return u8(prng_for_thread_.next()  >> 56ull); }

template <> f64 random() { return f64(random<u64>()) / f64(~0ull); }
template <> f32 random() { return f32(random<u64>()) / f32(~0ull); }

template <typename T>
T random(const T min, const T max) {
  // See this http://stackoverflow.com/a/6852396 for the reasoning behind this.

  const u64 range   = 1 + max - min;
  const u64 buckets = ~0ull / range;
  const u64 limit   = buckets * range;

  u64 candidate;

  do {
    candidate = random<u64>();
  } while (candidate >= limit);

  return min + (candidate / buckets);
}

template <> u64 random(const u64 min, const u64 max);
template <> u32 random(const u32 min, const u32 max);
template <> u16 random(const u16 min, const u16 max);
template <> u8  random(const u8  min, const u8  max);

template <> f64 random(const f64 min, const f64 max) {
  return random<u64>() / (f64(~0ull) / (max - min)) + min;
}

template <> f32 random(const f32 min, const f32 max) {
  return random<u64>() / (f32(~0ull) / (max - min)) + min;
}

void random_n(u8 *buffer, size_t n) {
  const unsigned k = n / 8;

  // Fill in 64-bit increments.
  for (unsigned i = n / 8; i; --i, buffer += 8)
    *(u64 *)buffer = prng_for_thread_.next();

  u64 leftover = prng_for_thread_.next();

  // Fill remaining bits from next 64-bit number in sequence.
  switch (n % 8) {
    case 7: buffer[6] = leftover >>= 8;
    case 6: buffer[5] = leftover >>= 8;
    case 5: buffer[4] = leftover >>= 8;
    case 4: buffer[3] = leftover >>= 8;
    case 3: buffer[2] = leftover >>= 8;
    case 2: buffer[1] = leftover >>= 8;
    case 1: buffer[0] = leftover >>= 8;
  }
}

} // core
} // yeti
