/*
 * This file is subject to the terms and conditions defined in
 * file 'LICENSE', which is part of this source code package.
 */

#pragma once

#include <array>
#include <iostream>
#include <iterator>
#include <utility>
#include <vector>

#include "absl/container/flat_hash_map.h"
#include "absl/types/span.h"

namespace rflx {

using uint8 = unsigned char;
using uint16 = unsigned short;
using uint32 = unsigned int;
using uint64 = unsigned long;

using int8 = signed char;
using int16 = signed short;
using int32 = signed int;
using int64 = signed long;

template <typename T>
using span = absl::Span<T>;

template <typename T>
using slice = std::vector<T>;

template <class K, class V>
using map = absl::flat_hash_map<K, V>;

template <class T, std::size_t N>
using array = std::array<T, N>;

using std::pair;

using rune = uint32;

}  // namespace rflx
