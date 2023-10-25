#ifndef HASHUTIL_H_
#define HASHUTIL_H_

#include <stdint.h>
#include <stdlib.h>
#include <sys/types.h>

#include <string>

#include <random>

namespace hashing {

class SimpleMixSplit {

 public:
  uint64_t seed;
  SimpleMixSplit() {
    ::std::random_device random;
    seed = random();
    seed <<= 32;
    seed |= random();
  }

  inline static uint64_t murmur64(uint64_t h) {
    h ^= h >> 33;
    h *= UINT64_C(0xff51afd7ed558ccd);
    h ^= h >> 33;
    h *= UINT64_C(0xc4ceb9fe1a85ec53);
    h ^= h >> 33;
    return h;
  }

  inline uint64_t operator()(uint64_t key) const {
    return murmur64(key + seed);
  }
};

}

#endif  // CUCKOO_FILTER_HASHUTIL_H_