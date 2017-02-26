#pragma once

#include "common/SmartMemoryManager.hpp"

namespace MXNES {
namespace UTIL {

//Utility class to hold misc data with automatic memory management
template<typename T, u64 size>
struct Blob final : private Dependency<Core> {
	Blob() 
		: pBuff(nullptr) {

		pBuff = _memoryManager.allocate_managed_memory(size);
	}

	~Blob() = default;
	MXNES_DISABLE_ALTERNATE_CONSTRUCTORS(Blob);

	u8 *pBuff;

private:
	SmartMemoryManager<T> _memoryManager;
};

} //namespace UTIL
} //namespace MXNES