/*
 * This file is subject to the terms and conditions defined in
 * file 'LICENSE', which is part of this source code package.
 */

#pragma once

#include "types.hpp"

namespace rflx {
namespace unicode {
namespace utf8 {

constexpr uint64 kUTFMax = 4;
constexpr rune kRuneSelf = 0x80;
constexpr rune kRuneError = 0xFFFD;
constexpr rune kMaxRune = 0x10FFFF;

// FullRune reports whether the uint8 in p begin with a full UTF-8 encoding of a
// rune. An invalid encoding is considered a full Rune since it will convert as
// a width-1 error rune.
constexpr bool FullRune(span<uint8 const> p);

// FullRuneInString is like FullRune but its input is a string view.
constexpr bool FullRuneInString(string_view s);

// DecodeRune unpacks the first UTF-8 encoding in p and returns the rune and
// its width in bytes. If p is empty it returns (RuneError, 0). Otherwise, if
// the encoding is invalid, it returns (RuneError, 1). Both are impossible
// results for correct, non-empty UTF-8.
//

// An encoding is invalid if it is incorrect UTF-8, encodes a rune that is
// out of range, or is not the shortest possible UTF-8 encoding for the
// value. No other validation is performed.
constexpr pair<rune, int8> DecodeRune(span<uint8 const> p);

// DecodeRuneInString is like DecodeRune but its input is a string. If s is
// empty it returns (RuneError, 0). Otherwise, if the encoding is invalid, it
// returns (RuneError, 1). Both are impossible results for correct, non-empty
// UTF-8.
//
// An encoding is invalid if it is incorrect UTF-8, encodes a rune that is
// out of range, or is not the shortest possible UTF-8 encoding for the
// value. No other validation is performed.
constexpr pair<rune, int8> DecodeRuneInString(string_view string);

// DecodeLastRune unpacks the last UTF-8 encoding in p and returns the rune and
// its width in bytes. If p is empty it returns (RuneError, 0). Otherwise, if
// the encoding is invalid, it returns (RuneError, 1). Both are impossible
// results for correct, non-empty UTF-8.
//
// An encoding is invalid if it is incorrect UTF-8, encodes a rune that is
// out of range, or is not the shortest possible UTF-8 encoding for the
// value. No other validation is performed.
constexpr pair<rune, int8> DecodeLastRune(span<uint8 const> p);

// DecodeLastRuneInString is like DecodeLastRune but its input is a string. If
// s is empty it returns (RuneError, 0). Otherwise, if the encoding is invalid,
// it returns (RuneError, 1). Both are impossible results for correct,
// non-empty UTF-8.
//
// An encoding is invalid if it is incorrect UTF-8, encodes a rune that is
// out of range, or is not the shortest possible UTF-8 encoding for the
// value. No other validation is performed.
constexpr pair<rune, int8> DecodeLastRuneInString(string_view s);

// RuneLen returns the number of bytes required to encode the rune.
// It returns -1 if the rune is not a valid value to encode in UTF-8.
constexpr int8 RuneLen(rune r);

// EncodeRune writes into p (which must be large enough) the UTF-8 encoding of
// the rune. It returns the number of bytes written.
constexpr int32 EncodeRune(span<uint8> p, rune r);

// RuneCount returns the number of runes in p. Erroneous and short
// encodings are treated as single runes of width 1 byte.
constexpr int64 RuneCount(span<uint8 const> p);

// RuneCountInString is like RuneCount but its input is a string.
constexpr int64 RuneCountInString(string_view s);

// RuneStart reports whether the byte could be the first byte of an encoded,
// possibly invalid rune. Second and subsequent bytes always have the top two
// bits set to 10.
constexpr bool RuneStart(uint8 byte);

// Valid reports whether p consists entirely of valid UTF-8-encoded runes.
constexpr bool Valid(span<uint8 const> p);

// ValidString reports whether s consists entirely of valid UTF-8-encoded runes.
constexpr bool ValidString(string_view s);

// ValidRune reports whether r can be legally encoded as UTF-8.
// Code points that are out of range or a surrogate half are illegal.
constexpr bool ValidRune(rune r);

}  // namespace utf8
}  // namespace unicode
}  // namespace rflx

#include "unicode/utf8/utf8_impl.hpp"