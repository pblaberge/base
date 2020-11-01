#include "types.hpp"

std::ostream& operator<<(std::ostream& out, rflx::string const& s) {
  for (rflx::uint64 i = 0; i < s.Size(); ++i) {
    out << static_cast<char>(*(s.Data() + i));
  }
  return out;
}

std::ostream& operator<<(std::ostream& out, rflx::string_view const& s) {
  for (rflx::uint64 i = 0; i < s.size_; ++i) {
    out << static_cast<char>(*(s.data_ + i));
  }
  return out;
}