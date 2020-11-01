/*
 * This file is subject to the terms and conditions defined in
 * file 'LICENSE', which is part of this source code package.
 */

#pragma once

#include <iostream>
#include <string_view>
#include <utility>
#include <vector>

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

using std::pair;

using rune = uint32;

class string {
 public:
  using traits_type = std::char_traits<uint8>;
  using value_type = uint8;
  using pointer = uint8*;
  using const_pointer = const uint8*;
  using reference = uint8&;
  using const_reference = const uint8&;
  using const_iterator = const uint8*;
  using iterator = const_iterator;
  using const_reverse_iterator = std::reverse_iterator<const_iterator>;
  using reverse_iterator = const_reverse_iterator;
  using size_type = size_t;
  using difference_type = std::ptrdiff_t;

  constexpr string();
  constexpr string(uint64 count, uint8 ch);
  constexpr string(uint8 const*);
  constexpr string(char const*);
  constexpr string(uint8 const* s, uint64 count);
  constexpr string(std::initializer_list<uint8> iList);

  constexpr string(string const&) = delete;
  constexpr string& operator=(string const&) = delete;

  constexpr string(string&&) noexcept;
  constexpr string& operator=(string&&) noexcept;

  constexpr ~string();

  constexpr const_iterator begin() const noexcept { return data_; }
  constexpr const_iterator end() const noexcept { return data_ + size_; }

  constexpr string Clone();

  constexpr uint64 Size() const;
  constexpr uint8 const* Data() const;

  constexpr string operator+(string const& s) const;

 private:
  constexpr string(uint8 const* data, uint64 size, std::nullptr_t);

  uint8 const* data_;
  uint64 size_;
};

using string_view = std::basic_string_view<uint8 const>;

}  // namespace rflx

#include "types_impl.hpp"