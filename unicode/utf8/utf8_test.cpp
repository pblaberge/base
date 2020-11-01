#include "unicode/utf8/utf8.hpp"

#include "gtest/gtest.h"

namespace rflx {
namespace unicode {
namespace utf8 {

struct Utf8Map {
  rune r;
  string str;
};

std::array<Utf8Map const, 32> const utf8map = {{
    {0x0000, {0x00, 0x00}},
    {0x0001, "\x01"},
    {0x007e, "\x7e"},
    {0x007f, "\x7f"},
    {0x0080, "\xc2\x80"},
    {0x0081, "\xc2\x81"},
    {0x00bf, "\xc2\xbf"},
    {0x00c0, "\xc3\x80"},
    {0x00c1, "\xc3\x81"},
    {0x00c8, "\xc3\x88"},
    {0x00d0, "\xc3\x90"},
    {0x00e0, "\xc3\xa0"},
    {0x00f0, "\xc3\xb0"},
    {0x00f8, "\xc3\xb8"},
    {0x00ff, "\xc3\xbf"},
    {0x0100, "\xc4\x80"},
    {0x07ff, "\xdf\xbf"},
    {0x0400, "\xd0\x80"},
    {0x0800, "\xe0\xa0\x80"},
    {0x0801, "\xe0\xa0\x81"},
    {0x1000, "\xe1\x80\x80"},
    {0xd000, "\xed\x80\x80"},
    {0xd7ff, "\xed\x9f\xbf"},  // last code point before surrogate half.
    {0xe000, "\xee\x80\x80"},  // first code point after surrogate half.
    {0xfffe, "\xef\xbf\xbe"},
    {0xffff, "\xef\xbf\xbf"},
    {0x10000, "\xf0\x90\x80\x80"},
    {0x10001, "\xf0\x90\x80\x81"},
    {0x40000, "\xf1\x80\x80\x80"},
    {0x10fffe, "\xf4\x8f\xbf\xbe"},
    {0x10ffff, "\xf4\x8f\xbf\xbf"},
    {0xFFFD, "\xef\xbf\xbd"},
}};

Utf8Map surrogateMap[] = {
    {0xd800, "\xed\xa0\x80"},  // surrogate min decodes to (RuneError, 1)
    {0xdfff, "\xed\xbf\xbf"},  // surrogate max decodes to (RuneError, 1)
};

string testStrings[] = {
    "",
    "abcd",
    "☺☻☹",
    "日a本b語ç日ð本Ê語þ日¥本¼語i日©",
    "日a本b語ç日ð本Ê語þ日¥本¼語i日©日a本b語ç日ð本Ê語þ日¥本¼語i日©日a本b語ç日ð本"
    "Ê語þ日¥本¼語i日©",
    "\x80\x80\x80\x80",
};

std::array<string, 37> invalidSequenceTests = {{
    "\xed\xa0\x80\x80",  // surrogate min
    "\xed\xbf\xbf\x80",  // surrogate max

    // xx
    "\x91\x80\x80\x80",

    // s1
    "\xC2\x7F\x80\x80",
    "\xC2\xC0\x80\x80",
    "\xDF\x7F\x80\x80",
    "\xDF\xC0\x80\x80",

    // s2
    "\xE0\x9F\xBF\x80",
    "\xE0\xA0\x7F\x80",
    "\xE0\xBF\xC0\x80",
    "\xE0\xC0\x80\x80",

    // s3
    "\xE1\x7F\xBF\x80",
    "\xE1\x80\x7F\x80",
    "\xE1\xBF\xC0\x80",
    "\xE1\xC0\x80\x80",

    // s4
    "\xED\x7F\xBF\x80",
    "\xED\x80\x7F\x80",
    "\xED\x9F\xC0\x80",
    "\xED\xA0\x80\x80",

    // s5
    "\xF0\x8F\xBF\xBF",
    "\xF0\x90\x7F\xBF",
    "\xF0\x90\x80\x7F",
    "\xF0\xBF\xBF\xC0",
    "\xF0\xBF\xC0\x80",
    "\xF0\xC0\x80\x80",

    // s6
    "\xF1\x7F\xBF\xBF",
    "\xF1\x80\x7F\xBF",
    "\xF1\x80\x80\x7F",
    "\xF1\xBF\xBF\xC0",
    "\xF1\xBF\xC0\x80",
    "\xF1\xC0\x80\x80",

    // s7
    "\xF4\x7F\xBF\xBF",
    "\xF4\x80\x7F\xBF",
    "\xF4\x80\x80\x7F",
    "\xF4\x8F\xBF\xC0",
    "\xF4\x8F\xC0\x80",
    "\xF4\x90\x80\x80",
}};

struct RuneCountTest {
  std::string_view in;
  int32 out;
};

std::array<RuneCountTest, 6> runecounttests = {{
    {"abcd", 4},
    {"☺☻☹", 3},
    {"1,2,3,4", 7},
    {{"\xe2\x00", 2}, 2},
    {{"\xe2\x80", 2}, 2},
    {{"a\xe2\x80", 3}, 3},
}};

struct RuneLenTest {
  rune r;
  int32 size;
};

std::array<RuneLenTest, 10> runelentests = {{
    {0, 1},
    {0x65, 1},
    {0xe9, 2},
    {0x263A, 3},
    {kRuneError, 3},
    {kMaxRune, 4},
    {0xD800, -1},
    {0xDFFF, -1},
    {kMaxRune + 1, -1},
    {static_cast<rune>(-1), -1},
}};

struct ValidTest {
  std::string_view in;
  bool out;
};

std::array<char, 2> valid_bytes_0{66, static_cast<char>(250)};
std::array<char, 3> valid_bytes_1{66, static_cast<char>(250), 67};

std::array<ValidTest, 18> validTests = {{
    {"", true},
    {"a", true},
    {"abc", true},
    {"Ж", true},
    {"ЖЖ", true},
    {"брэд-ЛГТМ", true},
    {"☺☻☹", true},
    {"aa\xe2", false},
    {{valid_bytes_0.data(), 2}, false},
    {{valid_bytes_1.data(), 3}, false},
    {"a\uFFFDb", true},
    {"\xF4\x8F\xBF\xBF", true},       // U+10FFFF
    {"\xF4\x90\x80\x80", false},      // U+10FFFF+1; out of range
    {"\xF7\xBF\xBF\xBF", false},      // 0x1FFFFF; out of range
    {"\xFB\xBF\xBF\xBF\xBF", false},  // 0x3FFFFFF; out of range
    {"\xc0\x80", false},              // U+0000 encoded in two bytes: incorrect
    {"\xed\xa0\x80", false},          // U+D800 high surrogate (sic)
    {"\xed\xbf\xbf", false},          // U+DFFF low surrogate (sic)
}};

struct ValidRuneTest {
  rune r;
  bool ok;
};

std::array<ValidRuneTest, 12> validrunetests = {{
    {0, true},
    {0x65, true},
    {0xE9, true},
    {0x263A, true},
    {kRuneError, true},
    {kMaxRune, true},
    {0xD7FF, true},
    {0xD800, false},
    {0xDFFF, false},
    {0xE000, true},
    {kMaxRune + 1, false},
    {static_cast<rune>(-1), false},
}};

TEST(utf8, TestFullRune) {
  for (Utf8Map const& m : utf8map) {
    span<uint8 const> const b{m.str.Data(), m.str.Size()};
    if (!FullRune(b)) {
      FAIL() << "FullRune(" << m.str << ") (" << m.r << ") = false, want true";
    }
    string_view const s{m.str.Data(), m.str.Size()};
    if (!FullRuneInString(s)) {
      FAIL() << "FullRuneInString(" << m.str << ") (" << m.r
             << ") = false, want true";
    }
  }
  for (std::string const& str : std::array<std::string, 2>{"\xc0", "\xc1"}) {
    span<uint8 const> const b{reinterpret_cast<uint8 const*>(str.c_str()),
                              str.size()};
    if (!FullRune(b)) {
      FAIL() << "FullRune(" << str << ") = false, want true";
    }
    string_view const s{reinterpret_cast<uint8 const*>(str.c_str()),
                        str.size()};
    if (!FullRuneInString(s)) {
      FAIL() << "FullRuneInString(" << str << ") = false, want true";
    }
  }
}

TEST(utf8, TestEncodeRune) {
  for (uint64 i = 0; i < utf8map.size(); ++i) {
    Utf8Map const& m = utf8map[i];
    span<uint8 const> const b{m.str.Data(), m.str.Size()};
    std::array<uint8, 10> buf;
    int32 const n = EncodeRune({buf.data(), buf.size()}, m.r);
    span<uint8> b1{buf.data(), static_cast<uint64>(n)};
    ASSERT_EQ(b.size(), b1.size());
    if (b != b1) {
      FAIL() << "EncodeRune(" << m.r << ") failed for [" << i << "]";
    }
  }
}

TEST(utf8, TestDecodeRune) {
  for (Utf8Map const& m : utf8map) {
    {
      span<uint8 const> const b{m.str.Data(), m.str.Size()};
      {
        auto const [r, size] = DecodeRune(b);
        if (r != m.r || size != b.size()) {
          FAIL() << "DecodeRune(" << m.str << " = " << r << ", " << size
                 << " want " << r << ", " << b.size();
        }
      }

      {
        string_view const s{m.str.Data(), m.str.Size()};
        auto const [r, size] = DecodeRuneInString(s);
        if (r != m.r || size != s.size()) {
          FAIL() << "DecodeRuneInString(" << m.str << " = " << r << ", " << size
                 << " want " << r << ", " << s.size();
        }
      }
    }

    {
      // there's an extra byte that bytes left behind - make sure trailing
      // byte works
      span<uint8 const> const b{m.str.Data(), m.str.Size() + 1};
      {
        auto const [r, size] = DecodeRune(b);
        if (r != m.r || size != (b.size() - 1)) {
          FAIL() << "DecodeRune(" << m.str << " = " << r << ", " << size
                 << " want " << r << ", " << b.size();
        }
      }

      string_view const s{m.str.Data(), m.str.Size() + 1};
      auto const [r, size] = DecodeRuneInString(s);
      if (r != m.r || size != b.size() - 1) {
        FAIL() << "DecodeRuneInString(" << m.str << " = " << r << ", " << size
               << " want " << r << ", " << s.size();
      }
    }

    std::vector<uint8> b{m.str.begin(), m.str.end()};
    // make sure missing bytes fail
    int8 wantsize = 1;
    if (wantsize >= b.size()) {
      wantsize = 0;
    }
    {
      auto const [r, size] = DecodeRune({b.data(), b.size() - 1});
      if (r != kRuneError || size != wantsize) {
        FAIL() << "DecodeRune(" << m.str << " = " << r << ", " << size
               << " want " << kRuneError << ", " << wantsize;
      }
    }
    {
      string_view const s{m.str.Data(), m.str.Size() - 1};

      auto const [r, size] = DecodeRuneInString(s);
      if (r != kRuneError || size != wantsize) {
        FAIL() << "DecodeRuneInString(" << m.str << " = " << r << ", " << size
               << " want " << kRuneError << ", " << wantsize;
      }
    }

    // make sure bad sequences fail
    if (b.size() == 1) {
      b[0] = 0x80;
    } else {
      b[b.size() - 1] = 0x7F;
    }
    {
      auto const [r, size] = DecodeRune({b.data(), b.size()});
      if (r != kRuneError || size != 1) {
        FAIL() << "DecodeRune(" << m.str << " = " << r << ", " << size
               << " want " << kRuneError << ", " << 1;
      }
    }

    string_view s{b.data(), b.size()};
    auto const [r, size] = DecodeRuneInString(s);
    if (r != kRuneError || size != 1) {
      FAIL() << "DecodeRuneInString(" << m.str << " = " << r << ", " << size
             << " want " << kRuneError << ", " << 1;
    }
  }
}

TEST(utf8, TestDecodeSurrogateRune) {
  for (Utf8Map const& m : surrogateMap) {
    span<uint8 const> const b{m.str.Data(), m.str.Size()};
    {
      auto const [r, size] = DecodeRune(b);
      if (r != kRuneError || size != 1) {
        FAIL() << "DecodeRune(" << m.str << " = " << r << ", " << size
               << " want " << kRuneError << ", " << 1;
      }
    }
    string_view const s{m.str.Data(), m.str.Size()};
    auto const [r, size] = DecodeRuneInString(s);
    if (r != kRuneError || size != 1) {
      FAIL() << "DecodeRuneInString(" << m.str << " = " << r << ", " << size
             << " want " << kRuneError << ", " << 1;
    }
  }
}

void testSequence(string_view s) {
  struct info {
    int32 index;
    rune r;
  };

  std::vector<info> index{s.size()};
  span<uint8 const> const b{reinterpret_cast<uint8 const*>(s.data()), s.size()};
  int32 si = 0;
  int32 j = 0;

  string_view s0 = s;

  while (!s0.empty()) {
    auto [r, i] = DecodeRune(s0);
    index[j] = info{si, r};
    si += i;
    ++j;
    s0 = s0.substr(i);
  }
  --j;
  for (si = s.size(); si > 0;) {
    auto [r1, size1] = DecodeLastRune(b.subspan(0, si));
    if (r1 != index[j].r) {
      FAIL() << "DecodeLastRune(" << std::string{s.begin(), s.end()} << ", "
             << si << ") = " << r1 << ", want " << index[j].r;
      return;
    }
    si -= size1;
    if (si != index[j].index) {
      FAIL() << "DecodeLastRune(" << std::string{s.begin(), s.end()}
             << ") index mismatch at " << si << ", want " << index[j].index;
      return;
    }
    --j;
  }
  if (si != 0) {
    FAIL() << "DecodeLastRune(" << std::string{s.begin(), s.end()}
           << ") finished at " << si << ", not 0";
  }
}

TEST(utf8, TestSequencing) {
  for (string const& ts : testStrings) {
    for (Utf8Map const& m : utf8map) {
      for (string const& s :
           std::array<string, 3>{ts + m.str, m.str + ts, ts + m.str + ts}) {
        testSequence({s.Data(), s.Size()});
      }
    }
  }
}

rune runtimeDecodeRune(string_view s) {
  auto [r, size] = DecodeRuneInString(std::move(s));
  return r;
}

TEST(utf8, TestDecodeInvalidSequence) {
  for (string const& str : invalidSequenceTests) {
    span<uint8 const> const b{str.Data(), str.Size()};
    auto const [r1, size1] = DecodeRune(b);
    if (rune want = kRuneError; r1 != want) {
      FAIL() << "DecodeRune(" << str << " = " << r1 << ", want " << want;
      return;
    }
    string_view const s{str.Data(), str.Size()};
    auto const [r2, size2] = DecodeRuneInString(s);
    if (rune want = kRuneError; r2 != want) {
      FAIL() << "DecodeRune(" << str << " = " << r2 << ", want " << want;
      return;
    }
    if (r1 != r2) {
      FAIL() << "DecodeRune(" << std::string{s.begin(), s.end()} << ") = " << r1
             << " mismatch with DecodeRuneInString("
             << std::string{s.begin(), s.end()} << ") = " << r2;
      return;
    }
    rune r3 = runtimeDecodeRune(s);
    if (r2 != r3) {
      FAIL() << "DecodeRune(" << std::string{s.begin(), s.end()} << ") = " << r2
             << " mismatch with runtimeDecodeRune("
             << std::string{s.begin(), s.end()} << ") = " << r3;
      return;
    }
  }
}

// Check that negative runes encode as U+FFFD.
TEST(utf8, TestNegativeRune) {
  std::array<uint8, kUTFMax> errorbuf_array;
  std::array<uint8, kUTFMax> buf_array;

  span<uint8 const> errorbuf{
      errorbuf_array.data(),
      static_cast<uint64>(EncodeRune(
          {errorbuf_array.data(), errorbuf_array.size()}, kRuneError))};
  span<uint8 const> buf{buf_array.data(),
                        static_cast<uint64>(EncodeRune(
                            {buf_array.data(), buf_array.size()}, -1))};

  if (errorbuf != buf) {
    FAIL() << "incorrect encodeing for rune -1";
  }
}

TEST(utf8, TestRuneCount) {
  for (RuneCountTest const& tt : runecounttests) {
    if (int64 out = RuneCountInString(
            {reinterpret_cast<uint8 const*>(tt.in.data()), tt.in.size()});
        out != tt.out) {
      FAIL() << "RuneCountInString(" << tt.in << ") = " << out << ", want "
             << tt.out;
    }
    if (int64 out = RuneCount(
            {reinterpret_cast<uint8 const*>(tt.in.data()), tt.in.size()});
        out != tt.out) {
      FAIL() << "RuneCount(" << tt.in << ") = " << out << ", want " << tt.out;
    }
  }
}

TEST(utf8, TestRuneLen) {
  for (RuneLenTest const& tt : runelentests) {
    if (int8 size = RuneLen(tt.r); size != tt.size) {
      FAIL() << "RuneLen(" << tt.r << ") = " << size << ", want " << tt.size;
    }
  }
}

TEST(utf8, TestValid) {
  for (ValidTest const& tt : validTests) {
    if (Valid({reinterpret_cast<uint8 const*>(tt.in.data()), tt.in.size()}) !=
        tt.out) {
      FAIL() << "Valid(" << tt.in << ") = " << !tt.out << ", want " << tt.out;
    }
    if (ValidString({reinterpret_cast<uint8 const*>(tt.in.data()),
                     tt.in.size()}) != tt.out) {
      FAIL() << "ValidString(" << tt.in << ") = " << !tt.out << ", want "
             << tt.out;
    }
  }
}

TEST(utf8, TestValidRune) {
  for (ValidRuneTest const& tt : validrunetests) {
    if (bool ok = ValidRune(tt.r); ok != tt.ok) {
      FAIL() << "ValidRune(" << tt.r << ") = " << ok << ", want " << tt.ok;
    }
  }
}

}  // namespace utf8
}  // namespace unicode
}  // namespace rflx