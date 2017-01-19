#pragma once

#include <cmath>
#include <cstring>
#include <cstdint>

#ifdef _MSC_VER
#include <Windows.h>
#define MXNES_FORCEINLINE		__forceinline
#else
#define MXNES_FORCEINLINE		inline
#endif

#ifdef MXNES_TESTBUILD
#define MXNES_DECLARE_TESTRUNNER_ACCESS		friend class Testrunner
#else
#define MXNES_DECLARE_TESTRUNNER_ACCESS
#endif

//The following aligned pointers are guaranteed to be atomic on x86 architectures; see Intel Software Developer's
//Manual, Volume 3A, Section 8.1.1. N.B. the pointer itself is atomic, NOT the value it points to.
//USAGE:
//	MXNES_ATOMIC_POINTER	u8 *fooPtr;
//	MXNES_ATOMIC_POIONTER	s32 * volatile barPtr;

#ifdef _M_IX86
#define MXNES_ATOMIC_POINTER		alignas(4)
#else //Otherwise, align at 8 bytes, since this will be considered aligned on both 32- and 64-bit architectures
#define MXNES_ATOMIC_POINTER		alignas(8)
#endif

namespace MXNES {

typedef std::int8_t			s8;
typedef std::int16_t		s16;
typedef std::int32_t		s32;
typedef std::int64_t		s64;

typedef std::uint8_t		u8;
typedef std::uint16_t		u16;
typedef std::uint32_t		u32;
typedef std::uint64_t		u64;

namespace UTIL {

	template<typename T>
	void zero_memory(T *pBlock, const std::size_t size = sizeof(T)) {
		std::memset(pBlock, 0, size);
	}

} //namespace MXNES::UTIL

namespace DEFAULT {
	const u16 APP_WIDTH = 800, APP_HEIGHT = 600;
	const u32 FRAMERATE_MS = std::lround(1000.0 / 60.0);
}

} //namespace MXNES