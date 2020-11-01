#pragma once

#include "unicode/utf8/utf8.hpp"

namespace rflx {
namespace unicode {
namespace utf8 {

constexpr rune kSurrogateMin = 0xD800;
constexpr rune kSurrogateMax = 0xDFFF;

constexpr uint8 ktx = 0b10000000;
constexpr uint8 kt2 = 0b11000000;
constexpr uint8 kt3 = 0b11100000;
constexpr uint8 kt4 = 0b11110000;

constexpr uint8 kmaskx = 0b00111111;
constexpr uint8 kmask2 = 0b00011111;
constexpr uint8 kmask3 = 0b00001111;
constexpr uint8 kmask4 = 0b00000111;

constexpr rune krune1Max = (1 << 7) - 1;
constexpr rune krune2Max = (1 << 11) - 1;
constexpr rune krune3Max = (1 << 16) - 1;

// The default lowest and highest continuation byte.

constexpr uint8 klocb = 0b10000000;
constexpr uint8 khicb = 0b10111111;

// These names of these constants are chosen to give nice alignment in the
// table below. The first nibble is an index into acceptRanges or F for
// special one-byte cases. The second nibble is the Rune length or the
// Status for the special one-byte case.
constexpr uint8 kxx = 0xF1;  // invalid: size 1
constexpr uint8 kas = 0xF0;  // ASCII: size 1
constexpr uint8 ks1 = 0x02;  // accept 0, size 2
constexpr uint8 ks2 = 0x13;  // accept 1, size 3
constexpr uint8 ks3 = 0x03;  // accept 0, size 3
constexpr uint8 ks4 = 0x23;  // accept 2, size 3
constexpr uint8 ks5 = 0x34;  // accept 3, size 4
constexpr uint8 ks6 = 0x04;  // accept 0, size 4
constexpr uint8 ks7 = 0x44;  // accept 4, size 4

// first is information about the first byte in a UTF-8 sequence.
extern uint8 const kFirst[256];

struct _accept_range {
    uint8 lo;  // lowest value for second byte.
    uint8 hi;  // highest value for second byte.
};

extern _accept_range const _accept_ranges[16];

constexpr bool FullRune(span<uint8 const> p) {
    uint64 const n = p.size();
    if (n == 0) {
        return false;
    }

    uint8 const x = kFirst[p[0]];
    if (n >= static_cast<uint64>(x & 7)) {
        return true;  // ASCII, invalid or valid.
    }

    // Must be short or invalid.
    _accept_range const& accept = _accept_ranges[x >> 4];
    if ((n > 1) && ((p[1] < accept.lo) || (accept.hi < p[1]))) {
        return true;
    }

    return false;
}

constexpr bool FullRuneInString(string_view p) {
    return FullRune({p.data(), p.size()});
}

constexpr pair<rune, int8> DecodeRune(span<uint8 const> p) {
    uint64 const n = p.size();

    if (n < 1) {
        return {kRuneError, 0};
    }

    uint8 const p0 = p[0];
    uint8 const x = kFirst[p0];

    if (x >= kas) {
        // The following code simulates an additional check for x == xx and
        // handling the ASCII and invalid cases accordingly. This mask-and-or
        // approach prevents an additional branch.
        if (x == kxx) {
            return {kRuneError, 1};
        }
        return {p[0], 1};
    }

    uint64 const sz = static_cast<uint64>(x & 7);
    _accept_range const& accept = _accept_ranges[x >> 4];

    if (n < sz) {
        return {kRuneError, 1};
    }

    uint8 const b1 = p[1];

    if ((b1 < accept.lo) || (accept.hi < b1)) {
        return {kRuneError, 1};
    }

    if (sz <= 2) {
        return {{rune(p0 & kmask2) << 6 | rune(b1 & kmaskx)}, 2};
    }

    uint8 const b2 = p[2];

    if ((b2 < klocb) || (khicb < b2)) {
        return {kRuneError, 1};
    }

    if (sz <= 3) {
        return {{rune(p0 & kmask3) << 12 | rune(b1 & kmaskx) << 6 |
                 rune(b2 & kmaskx)},
                3};
    }

    uint8 const b3 = p[3];

    if ((b3 < klocb) || (khicb < b3)) {
        return {kRuneError, 1};
    }

    return {{rune(p0 & kmask4) << 18 | rune(b1 & kmaskx) << 12 |
             rune(b2 & kmaskx) << 6 | rune(b3 & kmaskx)},
            4};
}

constexpr pair<rune, int8> DecodeRuneInString(string_view string) {
    return DecodeRune({string.data(), string.size()});
}

constexpr pair<rune, int8> DecodeLastRune(span<uint8 const> p) {
    int64 const end = p.size();
    if (end == 0) {
        return {kRuneError, 0};
    }

    int64 start = end - 1;
    rune const r = p[start];
    if (r < kRuneSelf) {
        return {r, 1};
    }

    // guard against O(n^2) behavior when traversing
    // backwards through strings with long sequences of
    // invalid UTF-8.

    int64 const lim = (end <= kUTFMax) ? 0 : end - kUTFMax;

    for (--start; start >= lim; --start) {
        if (RuneStart(p[start])) {
            break;
        }
    }

    if (start < 0) {
        start = 0;
    }

    auto [new_r, size] = DecodeRune(p.subspan(start, end));

    if (start + size != end) {
        return {kRuneError, 1};
    }

    return {new_r, size};
}

constexpr pair<rune, int8> DecodeLastRuneInString(string_view s) {
    return DecodeLastRune({s.data(), s.size()});
}

constexpr int8 RuneLen(rune r) {
    if (r < krune1Max) {
        return 1;
    }

    if (r < krune2Max) {
        return 2;
    }

    if ((kSurrogateMin <= r) && (r <= kSurrogateMax)) {
        return -1;
    }

    if (r <= krune3Max) {
        return 3;
    }

    if (r <= kMaxRune) {
        return 4;
    }

    return -1;
}

constexpr int32 EncodeRune(span<uint8> p, rune r) {
    if (r <= krune1Max) {
        p[0] = static_cast<uint8>(r);
        return 1;
    }

    if (r <= krune2Max) {
        p[0] = kt2 | static_cast<uint8>(r >> 6);
        p[1] = ktx | (static_cast<uint8>(r) & kmaskx);
        return 2;
    }

    if ((r > kMaxRune) || ((kSurrogateMin <= r) && (r <= kSurrogateMax))) {
        r = kRuneError;
    }

    if (r <= krune3Max) {
        p[0] = kt3 | static_cast<uint8>(r >> 12);
        p[1] = ktx | (static_cast<uint8>(r >> 6) & kmaskx);
        p[2] = ktx | (static_cast<uint8>(r) & kmaskx);
        return 3;
    }

    p[0] = kt4 | static_cast<uint8>(r >> 18);
    p[1] = ktx | (static_cast<uint8>(r >> 12) & kmaskx);
    p[2] = ktx | (static_cast<uint8>(r >> 6) & kmaskx);
    p[3] = ktx | (static_cast<uint8>(r) & kmaskx);
    return 4;
}

constexpr int64 RuneCount(span<uint8 const> p) {
    uint64 const np = p.size();
    int64 n = 0;
    for (uint64 i = 0; i < np;) {
        ++n;
        uint8 const c = p[i];
        if (c < kRuneSelf) {
            // ASCII fast path
            ++i;
            continue;
        }
        uint8 const x = kFirst[c];
        if (x == kxx) {
            ++i;  // invalid.
            continue;
        }
        uint32 size = static_cast<uint32>(x & 7);
        if (i + size > np) {
            ++i;  // Short or invalid.
            continue;
        }
        _accept_range const& accept = _accept_ranges[x >> 4];
        if (uint8 const c = p[i + 1]; c < accept.lo || accept.hi < c) {
            size = 1;
        } else if (size == 2) {
        } else if (uint8 const c = p[i + 2]; c < klocb || khicb < c) {
            size = 1;
        } else if (size == 3) {
        } else if (uint8 const c = p[i + 3]; c < klocb || khicb < c) {
            size = 1;
        }
        i += size;
    }

    return n;
}

constexpr int64 RuneCountInString(string_view s) {
    return RuneCount({s.data(), s.size()});
}

constexpr bool RuneStart(uint8 b) {
    return (b & 0xC0) != 0x80;
}

constexpr bool Valid(span<uint8 const> p) {
    uint64 n = p.size();

    for (uint64 i = 0; i < n;) {
        uint8 const pi = p[i];
        if (pi < kRuneSelf) {
            ++i;
            continue;
        }

        uint8 const x = kFirst[pi];

        if (x == kxx) {
            return false;  // Illegal starter byte.
        }

        uint32 size = static_cast<uint32>(x & 7);

        if (i + size > n) {
            return false;  // Short or invalid.
        }

        _accept_range const& accept = _accept_ranges[x >> 4];

        if (uint8 const c = p[i + 1]; c < accept.lo || accept.hi < c) {
            return false;
        } else if (size == 2) {
        } else if (uint8 const c = p[i + 2]; c < klocb || khicb < c) {
            return false;
        } else if (size == 3) {
        } else if (uint8 const c = p[i + 3]; c < klocb || khicb < c) {
            return false;
        }
        i += size;
    }

    return true;
}

constexpr bool ValidString(string_view s) {
    return Valid({s.data(), s.size()});
}

constexpr bool ValidRune(rune r) {
    if (0 <= r && r < kSurrogateMin) {
        return true;
    }
    if (kSurrogateMax < r && r <= kMaxRune) {
        return true;
    }

    return false;
}

}  // namespace utf8
}  // namespace unicode
}  // namespace rflx