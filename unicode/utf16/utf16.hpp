#pragma once

#include "types.hpp"

namespace rflx {
namespace unicode {
namespace utf16 {

// IsSurrogate reports whether the specified Unicode code point
// can appear in a surrogate pair.
constexpr bool IsSurrogate(rune r);

// DecodeRune returns the UTF-16 decoding of a surrogate pair.
// If the pair is not a valid UTF-16 surrogate pair, DecodeRune returns
// the Unicode replacement code point U+FFFD.
constexpr rune DecodeRune(rune r1, rune r2);

// EncodeRune returns the UTF-16 surrogate pair r1, r2 for the given rune.
// If the rune is not a valid Unicode code point or does not need encoding,
// EncodeRune returns U+FFFD, U+FFFD.
constexpr pair<rune, rune> EncodeRune(rune r);

// Encode returns the UTF-16 encoding of the Unicode code point sequence s.
slice<uint16> Encode(span<rune> const s);

// Decode returns the Unicode code point sequence represented
// by the UTF-16 encoding s.
slice<rune> Decode(span<uint16> const s);

}  // namespace utf16
}  // namespace unicode
}  // namespace rflx

#include "unicode/utf16/utf16_impl.hpp"