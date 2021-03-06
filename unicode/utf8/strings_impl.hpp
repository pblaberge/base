#pragma once

#include "unicode/utf8/strings.hpp"
#include "unicode/utf8/utf8.hpp"

namespace rflx {

namespace unicode::utf8 {

constexpr pair<rune, int8> DecodeRuneInString(string_view string);

}

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

constexpr string::const_iterator string::begin() const noexcept {
  return string_iterator{string_view{data_, size_}};
}

constexpr string::const_iterator string::end() const noexcept {
  return string_iterator{string_view{data_ + size_, 0}};
}

constexpr string_literal::string_literal(const char* s)
    : string((uint8 const*)(s)) {}

constexpr string_literal::string_literal(std::initializer_list<uint8> iList)
    : string(std::move(iList)) {}

constexpr string_literal::string_literal(string_literal&&) noexcept = default;
constexpr string_literal& string_literal::operator=(string_literal&&) noexcept =
    default;

constexpr string_literal::~string_literal() = default;

constexpr string_view::string_view() : data_{nullptr}, size_{0} {}

constexpr string_view::string_view(string_view const& o) noexcept = default;
constexpr string_view& string_view::operator=(string_view const& o) noexcept =
    default;

constexpr string_view::string_view(string_view&& o) noexcept = default;
constexpr string_view& string_view::operator=(string_view&& o) noexcept =
    default;

constexpr string_view::~string_view() = default;

constexpr string_view::string_view(uint8 const* data, uint64 size)
    : data_{data}, size_{size} {}
constexpr string_view::string_view(uint8 const* data)
    : string_view(data, __builtin_strlen((char const*)(data))) {}

constexpr bool string_view::operator==(string_view const& o) const {
  return (data_ == o.data_) && (size_ == o.size_);
}

constexpr bool string_view::operator!=(string_view const& o) const {
  return !operator==(o);
}

constexpr string_iterator string_view::begin() const noexcept {
  return string_iterator{string_view{data_, size_}};
}

constexpr string_iterator string_view::end() const noexcept {
  return string_iterator{string_view{data_ + size_, 0}};
}

constexpr bool string_view::Empty() const { return size_ == 0; }
constexpr uint64 string_view::Size() const { return size_; }
constexpr uint8 const* string_view::Data() const { return data_; }

constexpr string_view string_view::Substr(uint64 pos) const {
  if (pos >= size_) {
    return {};
  }
  return Substr(pos, size_ - pos);
}

constexpr string_view string_view::Substr(uint64 pos, uint64 size) const {
  if (pos >= size_) {
    return {};
  }

  if (pos + size > size_) {
    return {};
  }

  return {data_ + pos, size};
}

constexpr string_iterator::string_iterator(string_view sv)
    : sv_{std::move(sv)} {};

constexpr bool string_iterator::operator==(
    string_iterator::self_type const& o) const {
  return sv_ == o.sv_;
}
constexpr bool string_iterator::operator!=(
    string_iterator::self_type const& o) const {
  return sv_ != o.sv_;
}

constexpr string_iterator::self_type string_iterator::operator++() {
  pair<rune, int8> p = ::rflx::unicode::utf8::DecodeRuneInString(sv_);
  return string_iterator::self_type(sv_.Substr(p.second));
}

constexpr string_iterator::self_type string_iterator::operator++(int) {
  pair<rune, int8> p = ::rflx::unicode::utf8::DecodeRuneInString(sv_);
  return string_iterator::self_type(sv_.Substr(p.second));
}

constexpr string_iterator::value_type string_iterator::operator*() const {
  pair<rune, int8> p = ::rflx::unicode::utf8::DecodeRuneInString(sv_);
  return p.first;
}

constexpr string_iterator::value_type string_iterator::operator->() const {
  pair<rune, int8> p = ::rflx::unicode::utf8::DecodeRuneInString(sv_);
  return p.first;
}

}  // namespace rflx

std::ostream& operator<<(std::ostream& out, rflx::string const& s);
