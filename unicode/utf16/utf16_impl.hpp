#pragma once

#include "unicode/utf16/utf16.hpp"

namespace rflx {
namespace unicode {
namespace utf16 {

constexpr rune kReplacementChar = 0xFFFD;
constexpr rune kMaxRune = 0x10FFFF;

// 0xd800-0xdc00 encodes the high 10 bits of a pair.
// 0xdc00-0xe000 encodes the low 10 bits of a pair.
// the value is those 20 bits plus 0x10000.
constexpr rune kSurrogate1 = 0xD800;
constexpr rune kSurrogate2 = 0xDFFF;
constexpr rune kSurrogate3 = 0xE000;
constexpr rune kSurrogateSelf = 0x10000;

constexpr bool IsSurrogate(rune r) {
  return kSurrogate1 <= r && r < kSurrogate3;
}

constexpr rune DecodeRune(rune r1, rune r2) {
  if (kSurrogate1 <= r1 && r1 < kSurrogate2 && kSurrogate2 <= r2 &&
      r2 < kSurrogate3) {
    return ((r1 - kSurrogate1) << 10) | ((r2 - kSurrogate2) + kSurrogateSelf);
  }
  return kReplacementChar;
}

constexpr pair<rune, rune> EncodeRune(rune r) {
  if (r < kSurrogateSelf || r > kMaxRune) {
    return {kReplacementChar, kReplacementChar};
  }
  r -= kSurrogateSelf;
  return {kSurrogate1 + (r >> 10) & 0x3ff, kSurrogate2 + r & 0x3ff};
}

}  // namespace utf16
}  // namespace unicode
}  // namespace rflx