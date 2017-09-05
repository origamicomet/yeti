//===-- yeti/renderer/layer.cc --------------------------*- mode: C++11 -*-===//
//
//                             __ __     _   _
//                            |  |  |___| |_|_|
//                            |_   _| -_|  _| |
//                              |_| |___|_| |_|
//
//       This file is distributed under the terms described in LICENSE.
//
//===----------------------------------------------------------------------===//

#include "yeti/renderer/layer.h"

namespace yeti {
namespace renderer {

Layer::Id layer::id_from_name(const char *name) {
  yeti_assert_debug(name != NULL);
  return foundation::murmur_hash_32(name);
}

Layer::Order layer::order_from_string(const char *order) {
  if (strcmp(order, "do_not_care") == 0)
    return Layer::DO_NOT_CARE;
  if (strcmp(order, "submission") == 0)
    return Layer::SUBMISSION;
  if (strcmp(order, "state") == 0)
    return Layer::STATE;
  if (strcmp(order, "front_to_back") == 0)
    return Layer::FRONT_TO_BACK;
  if (strcmp(order, "back_to_front") == 0)
    return Layer::BACK_TO_FRONT;
  return Layer::UNKNOWN;
}

const char *layer::order_to_string(const Layer::Order order) {
  switch (order) {
    case Layer::DO_NOT_CARE: return "do_not_care";
    case Layer::SUBMISSION: return "submission";
    case Layer::STATE: return "state";
    case Layer::FRONT_TO_BACK: return "front_to_back";
    case Layer::BACK_TO_FRONT: return "back_to_front";
  }

  return NULL;
}

} // renderer
} // yeti
