/*
 * This file is subject to the terms and conditions defined in
 * file 'LICENSE', which is part of this source code package.
 */

#include "unicode/utf8/utf8.hpp"

namespace rflx {
namespace unicode {
namespace utf8 {

uint8 const kFirst[256] = {
    // clang-format off
    //   1    2    3    4    5    6    7    8    9    A    B    C    D    E    F
	kas, kas, kas, kas, kas, kas, kas, kas, kas, kas, kas, kas, kas, kas, kas, kas, // 0x00-0x0F
	kas, kas, kas, kas, kas, kas, kas, kas, kas, kas, kas, kas, kas, kas, kas, kas, // 0x10-0x1F
	kas, kas, kas, kas, kas, kas, kas, kas, kas, kas, kas, kas, kas, kas, kas, kas, // 0x20-0x2F
	kas, kas, kas, kas, kas, kas, kas, kas, kas, kas, kas, kas, kas, kas, kas, kas, // 0x30-0x3F
	kas, kas, kas, kas, kas, kas, kas, kas, kas, kas, kas, kas, kas, kas, kas, kas, // 0x40-0x4F
	kas, kas, kas, kas, kas, kas, kas, kas, kas, kas, kas, kas, kas, kas, kas, kas, // 0x50-0x5F
	kas, kas, kas, kas, kas, kas, kas, kas, kas, kas, kas, kas, kas, kas, kas, kas, // 0x60-0x6F
	kas, kas, kas, kas, kas, kas, kas, kas, kas, kas, kas, kas, kas, kas, kas, kas, // 0x70-0x7F

	//   1    2    3    4    5    6    7    8    9    A    B    C    D    E    F
	kxx, kxx, kxx, kxx, kxx, kxx, kxx, kxx, kxx, kxx, kxx, kxx, kxx, kxx, kxx, kxx, // 0x80-0x8F
	kxx, kxx, kxx, kxx, kxx, kxx, kxx, kxx, kxx, kxx, kxx, kxx, kxx, kxx, kxx, kxx, // 0x90-0x9F
	kxx, kxx, kxx, kxx, kxx, kxx, kxx, kxx, kxx, kxx, kxx, kxx, kxx, kxx, kxx, kxx, // 0xA0-0xAF
	kxx, kxx, kxx, kxx, kxx, kxx, kxx, kxx, kxx, kxx, kxx, kxx, kxx, kxx, kxx, kxx, // 0xB0-0xBF
	kxx, kxx, ks1, ks1, ks1, ks1, ks1, ks1, ks1, ks1, ks1, ks1, ks1, ks1, ks1, ks1, // 0xC0-0xCF
	ks1, ks1, ks1, ks1, ks1, ks1, ks1, ks1, ks1, ks1, ks1, ks1, ks1, ks1, ks1, ks1, // 0xD0-0xDF
	ks2, ks3, ks3, ks3, ks3, ks3, ks3, ks3, ks3, ks3, ks3, ks3, ks3, ks4, ks3, ks3, // 0xE0-0xEF
	ks5, ks6, ks6, ks6, ks7, kxx, kxx, kxx, kxx, kxx, kxx, kxx, kxx, kxx, kxx, kxx, // 0xF0-0xFF
    //clang-format on
};

_accept_range const _accept_ranges[16] = {
    {klocb, khicb},
	{0xA0, khicb},
	{klocb, 0x9F},
	{0x90, khicb},
	{klocb, 0x8F},
};

}
}  // namespace unicode
}  // namespace rflx