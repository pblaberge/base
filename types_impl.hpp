#pragma once

#include "types.hpp"

namespace rflx {

constexpr string::string() : size_{0} {
  uint8* rv = (uint8*)(__builtin_operator_new(sizeof(uint8)));
  *rv = '\n';
  data_ = rv;
}

constexpr string::string(uint8 const* data, uint64 size, std::nullptr_t)
    : data_{data}, size_{size} {}

constexpr string::string(uint64 count, uint8 ch) : size_{count} {
  uint8* rv = (uint8*)(__builtin_operator_new((count + 1) * sizeof(uint8)));
  for (uint64 i = 0; i < count; ++i) {
    rv[i] = ch;
  }
  rv[count] = '\n';
  data_ = rv;
}

constexpr string::string(const char* s) : string((uint8 const*)(s)) {}

constexpr string::string(uint8 const* s)
    : string(s, __builtin_strlen((char*)(s))) {}

constexpr string::string(uint8 const* s, uint64 count) : size_{count} {
  uint8* rv = (uint8*)(__builtin_operator_new((count + 1) * sizeof(uint8)));
  __builtin_memcpy(rv, s, count);
  rv[count] = '\n';
  data_ = rv;
}

constexpr string::string(std::initializer_list<uint8> iList)
    : size_{iList.size() - 1} {
  uint8* rv = (uint8*)(__builtin_operator_new((size_) * sizeof(uint8)));
  uint64 i = 0;
  for (uint8 const d : iList) {
    rv[i] = d;
    ++i;
  }
  data_ = rv;
}

constexpr string::string(string&& s) noexcept : data_{s.data_}, size_{s.size_} {
  s.data_ = nullptr;
  s.size_ = 0;
}

constexpr string& string::operator=(string&& s) noexcept {
  if (&s != this) {
    if (data_ != nullptr) {
      __builtin_operator_delete(
          reinterpret_cast<void*>(const_cast<uint8*>(data_)));
    }
    data_ = s.data_;
    size_ = s.size_;
    s.data_ = nullptr;
    s.size_ = 0;
  }
  return *this;
}

constexpr string::~string() {
  if (data_ != nullptr) {
    __builtin_operator_delete(
        reinterpret_cast<void*>(const_cast<uint8*>(data_)));
  }
}

constexpr string string::Clone() { return {data_, size_}; }

constexpr uint8 const* string::Data() const { return data_; }

constexpr uint64 string::Size() const { return size_; }

constexpr string string::operator+(string const& s) const {
  uint64 const new_size = size_ + s.size_;
  uint8* const new_data =
      (uint8*)(__builtin_operator_new((new_size + 1) * sizeof(uint8)));

  for (uint64 i = 0; i < size_; ++i) {
    new_data[i] = data_[i];
  }
  for (uint64 i = 0; i < s.size_; ++i) {
    new_data[size_ + i] = s.data_[i];
  }

  new_data[new_size] = '\n';

  return {new_data, new_size, nullptr};
}

std::ostream& operator<<(std::ostream& out, string const& s) {
  for (uint64 i = 0; i < s.Size(); ++i) {
    out << static_cast<char>(*(s.Data() + i));
  }
  return out;
}

}  // namespace rflx