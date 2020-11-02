#pragma once

#include "types.hpp"

namespace rflx {
class string_view;
}

std::ostream& operator<<(std::ostream& out, rflx::string_view const& s);

namespace rflx {

class string_iterator;

class string {
 public:
  using traits_type = std::char_traits<uint8>;
  using value_type = uint8;
  using pointer = uint8*;
  using const_pointer = const uint8*;
  using reference = uint8&;
  using const_reference = const uint8&;
  using const_iterator = string_iterator;
  using iterator = const_iterator;
  using size_type = size_t;
  using difference_type = std::ptrdiff_t;

  constexpr string();
  constexpr string(uint64 count, uint8 ch);
  constexpr string(uint8 const*);
  constexpr string(uint8 const* s, uint64 count);
  constexpr string(std::initializer_list<uint8> iList);

  constexpr string(string const&) = delete;
  constexpr string& operator=(string const&) = delete;

  constexpr string(string&&) noexcept;
  constexpr string& operator=(string&&) noexcept;

  constexpr ~string();

  constexpr const_iterator begin() const noexcept;
  constexpr const_iterator end() const noexcept;

  constexpr string Clone();

  constexpr uint64 Size() const;
  constexpr uint8 const* Data() const;

  constexpr string operator+(string const& s) const;

 private:
  constexpr string(uint8 const* data, uint64 size, std::nullptr_t);

  uint8 const* data_;
  uint64 size_;
};

class string_literal : public string {
 public:
  constexpr string_literal(const char*);
  constexpr string_literal(std::initializer_list<uint8> iList);

  constexpr string_literal(string_literal const&) = delete;
  constexpr string_literal& operator=(string_literal const&) = delete;

  constexpr string_literal(string_literal&&) noexcept;
  constexpr string_literal& operator=(string_literal&&) noexcept;

  constexpr ~string_literal();
};

class string_view {
 public:
  constexpr string_view();
  constexpr string_view(uint8 const* data, uint64 size);
  constexpr string_view(uint8 const* data);

  constexpr string_view(string_view const& o) noexcept;
  constexpr string_view& operator=(string_view const& o) noexcept;

  constexpr string_view(string_view&& o) noexcept;
  constexpr string_view& operator=(string_view&& o) noexcept;

  constexpr ~string_view();

  constexpr bool operator==(string_view const& o) const;
  constexpr bool operator!=(string_view const& o) const;

  constexpr string_iterator begin() const noexcept;
  constexpr string_iterator end() const noexcept;

  constexpr bool Empty() const;
  constexpr uint64 Size() const;
  constexpr uint8 const* Data() const;

  constexpr string_view Substr(uint64 pos) const;
  constexpr string_view Substr(uint64 pos, uint64 size) const;

 private:
  friend std::ostream& ::operator<<(std::ostream& out, string_view const& s);
  uint8 const* data_;
  uint64 size_;
};

class string_iterator {
 public:
  using self_type = string_iterator;
  using value_type = rune;
  using iterator_category = std::forward_iterator_tag;
  using difference_type = int;

  explicit constexpr string_iterator(string_view sv);
  constexpr self_type operator++();
  constexpr self_type operator++(int);
  constexpr value_type operator*() const;
  constexpr value_type operator->() const;
  constexpr bool operator==(self_type const& o) const;
  constexpr bool operator!=(self_type const& o) const;

 private:
  string_view const sv_;
};

}  // namespace rflx

#include "unicode/utf8/strings_impl.hpp"
