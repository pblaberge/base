#include "unicode/utf16/utf16.hpp"

namespace rflx {
namespace unicode {
namespace utf16 {

slice<uint16> Encode(span<rune> const s) {
  uint64 n = s.size();
  for (rune const v : s) {
    if (v >= kSurrogateSelf) {
      ++n;
    }
  }

  slice<uint16> a;
  a.resize(n);
  n = 0;
  for (rune const v : s) {
    if ((0 <= v && v < kSurrogate1) ||
        (kSurrogate3 <= v && v < kSurrogateSelf)) {
      // normal rune
      a[n] = static_cast<uint16>(v);
      ++n;
    } else if (kSurrogateSelf <= v && v <= kMaxRune) {
      // needs surrogate sequence
      auto const [r1, r2] = EncodeRune(v);
      a[n] = static_cast<uint16>(r1);
      a[n + 1] = static_cast<uint16>(r2);
      n += 2;
    } else {
      a[n] = static_cast<uint16>(kReplacementChar);
      ++n;
    }
  }
  a.resize(n);
  return a;
}

slice<rune> Decode(span<uint16> const s) {
  slice<rune> a;
  a.resize(s.size());
  uint64 n = 0;
  for (uint64 i = 0; i < s.size(); ++i) {
    rune const r = s[i];
    if ((r < kSurrogate1) || (kSurrogate3 <= r)) {
      // normal rune
      a[n] = static_cast<rune>(r);
    } else if ((kSurrogate1 <= r) && (r < kSurrogate2) && (i + 1 < s.size()) &&
               (kSurrogate2 <= s[i + 1]) && (s[i + 1] < kSurrogate3)) {
      // valid surrogate sequence
      a[n] = DecodeRune(rune(r), rune(s[i + 1]));
      ++i;
    } else {
      // invalid surrogate sequence
      a[n] = kReplacementChar;
    }
    ++n;
  }
  a.resize(n);
  return a;
}

}  // namespace utf16
}  // namespace unicode
}  // namespace rflx