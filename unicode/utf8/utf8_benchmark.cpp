#include "benchmark/benchmark.h"
#include "types.hpp"
#include "unicode/utf8/utf8.hpp"

namespace rflx {
namespace unicode {
namespace utf8 {

namespace {

void BenchmarkRuneCountTenASCIIChars(benchmark::State& state) {
  string_literal s = "0123456789";
  span<uint8 const> b{reinterpret_cast<uint8 const*>(s.Data()), s.Size()};
  for (auto _ : state) {
    benchmark::DoNotOptimize(b);
    benchmark::DoNotOptimize(RuneCount(b));
  }
}

BENCHMARK(BenchmarkRuneCountTenASCIIChars)->Range(1, 256);

void BenchmarkRuneCountTenJapaneseChars(benchmark::State& state) {
  string_literal s = "日本語日本語日本語日";
  span<uint8 const> b{reinterpret_cast<uint8 const*>(s.Data()), s.Size()};
  for (auto _ : state) {
    benchmark::DoNotOptimize(b);
    benchmark::DoNotOptimize(RuneCount(b));
  }
}

BENCHMARK(BenchmarkRuneCountTenJapaneseChars)->Range(1, 256);

void BenchmarkRuneCountInStringTenASCIIChars(benchmark::State& state) {
  string_literal s = "0123456789";
  string_view b{reinterpret_cast<uint8 const*>(s.Data()), s.Size()};
  for (auto _ : state) {
    benchmark::DoNotOptimize(b);
    benchmark::DoNotOptimize(RuneCountInString(b));
  }
}

BENCHMARK(BenchmarkRuneCountInStringTenASCIIChars)->Range(1, 256);

void BenchmarkRuneCountInStringTenJapaneseChars(benchmark::State& state) {
  string_literal s = "日本語日本語日本語日";
  string_view b{reinterpret_cast<uint8 const*>(s.Data()), s.Size()};
  for (auto _ : state) {
    benchmark::DoNotOptimize(b);
    benchmark::DoNotOptimize(RuneCountInString(b));
  }
}

BENCHMARK(BenchmarkRuneCountInStringTenJapaneseChars)->Range(1, 256);

void BenchmarkValidTenASCIIChars(benchmark::State& state) {
  string_literal s = "0123456789";
  span<uint8 const> b{reinterpret_cast<uint8 const*>(s.Data()), s.Size()};
  for (auto _ : state) {
    benchmark::DoNotOptimize(b);
    benchmark::DoNotOptimize(Valid(b));
  }
}

BENCHMARK(BenchmarkValidTenASCIIChars)->Range(1, 256);

void BenchmarkValidTenJapaneseChars(benchmark::State& state) {
  string_literal s = "日本語日本語日本語日";
  span<uint8 const> b{reinterpret_cast<uint8 const*>(s.Data()), s.Size()};
  for (auto _ : state) {
    benchmark::DoNotOptimize(b);
    benchmark::DoNotOptimize(Valid(b));
  }
}

BENCHMARK(BenchmarkValidTenJapaneseChars)->Range(1, 256);

void BenchmarkValidStringTenASCIIChars(benchmark::State& state) {
  string_literal s = "0123456789";
  string_view b{reinterpret_cast<uint8 const*>(s.Data()), s.Size()};
  for (auto _ : state) {
    benchmark::DoNotOptimize(b);
    benchmark::DoNotOptimize(ValidString(b));
  }
}

BENCHMARK(BenchmarkValidStringTenASCIIChars)->Range(1, 256);

void BenchmarkValidStringTenJapaneseChars(benchmark::State& state) {
  string_literal s = "日本語日本語日本語日";
  string_view b{reinterpret_cast<uint8 const*>(s.Data()), s.Size()};
  for (auto _ : state) {
    benchmark::DoNotOptimize(b);
    benchmark::DoNotOptimize(ValidString(b));
  }
}

BENCHMARK(BenchmarkValidStringTenJapaneseChars)->Range(1, 256);

void BenchmarkEncodeASCIIRune(benchmark::State& state) {
  array<uint8, kUTFMax> buf;
  for (auto _ : state) {
    benchmark::DoNotOptimize(buf);
    benchmark::DoNotOptimize(EncodeRune({buf.data(), buf.size()}, 'a'));
  }
}

BENCHMARK(BenchmarkEncodeASCIIRune)->Range(1, 256);

void BenchmarkEncodeJapaneseRune(benchmark::State& state) {
  array<uint8, kUTFMax> buf;
  for (auto _ : state) {
    benchmark::DoNotOptimize(buf);
    benchmark::DoNotOptimize(EncodeRune({buf.data(), buf.size()}, 0x672C));
  }
}

BENCHMARK(BenchmarkEncodeJapaneseRune)->Range(1, 256);

void BenchmarkDecodeASCIIRune(benchmark::State& state) {
  array<uint8, kUTFMax> buf;
  EncodeRune({buf.data(), buf.size()}, 0x61);
  for (auto _ : state) {
    benchmark::DoNotOptimize(buf);
    benchmark::DoNotOptimize(DecodeRune({buf.data(), buf.size()}));
  }
}

BENCHMARK(BenchmarkDecodeASCIIRune)->Range(1, 256);

void BenchmarkDecodeJapaneseRune(benchmark::State& state) {
  array<uint8, kUTFMax> buf;
  EncodeRune({buf.data(), buf.size()}, 0x672C);
  for (auto _ : state) {
    benchmark::DoNotOptimize(buf);
    benchmark::DoNotOptimize(DecodeRune({buf.data(), buf.size()}));
  }
}

BENCHMARK(BenchmarkDecodeJapaneseRune)->Range(1, 256);

void BenchmarkFullASCIIRune(benchmark::State& state) {
  array<uint8, kUTFMax> buf;
  EncodeRune({buf.data(), buf.size()}, 0x61);
  for (auto _ : state) {
    benchmark::DoNotOptimize(buf);
    benchmark::DoNotOptimize(FullRune({buf.data(), buf.size()}));
  }
}

BENCHMARK(BenchmarkFullASCIIRune)->Range(1, 256);

void BenchmarkFullJapaneseRune(benchmark::State& state) {
  array<uint8, kUTFMax> buf;
  EncodeRune({buf.data(), buf.size()}, 0x672C);
  for (auto _ : state) {
    benchmark::DoNotOptimize(buf);
    benchmark::DoNotOptimize(FullRune({buf.data(), buf.size()}));
  }
}

BENCHMARK(BenchmarkFullJapaneseRune)->Range(1, 256);

}  // namespace

}  // namespace utf8
}  // namespace unicode
}  // namespace rflx