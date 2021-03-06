#pragma once

#include "common/Core.hpp"
#include "common/Dependency.hpp"

#pragma warning(push, 0)
#include <cstdlib>
#include <memory>
#include <new>
#include <vector>
#pragma warning(pop)

namespace MXNES {

/**
 * Component class which, when provided to another object, can
 * handle automatic memory management for the lifetime of that object
 */
template<typename T>
class SmartMemoryManager final : private Dependency<Core> {
public:
	SmartMemoryManager() = default;
	~SmartMemoryManager() = default;

	MXNES_DISABLE_ALTERNATE_CONSTRUCTORS(SmartMemoryManager);

	/**
	 * @brief allocate_managed_memory Performs a scalar allocation
	 *		of elements of type T. All objects allocated by this function will
	 *		be garbage-collected when the given instance of this class is destroyed.
	 *		If allocation fails, then the application is TERMINATED.
	 *
	 * @param numElements The number of elements of type T to allocate
	 * @return A pointer to the newly allocated memory
	 */
	T* allocate_managed_memory(const std::size_t numElements) {
		T *pNewMemory = new (std::nothrow) T[numElements];
		if (pNewMemory == nullptr) {
			Dependency<Core>::dep.logerr(std::string("Failed to allocate ")
				+ std::to_string(numElements * sizeof(T))
				+ " bytes of memory");

			Dependency<Core>::dep.alert_err(ERRORS::FATAL_ERROR_STR);

			//At this point it is dangerous to continue, so we need to kill the app
			std::abort();
		}
		else {
			_managedObjects.emplace_back(pNewMemory);
		}
		UTIL::zero_memory(pNewMemory, sizeof(T) * numElements);
		return pNewMemory;
	}

	/**
	 * @brief free_all Releases all memory managed by the instance
	 *
	 * @return none
	 */
	void free_all() {
		_managedObjects.clear();
	}

private:
	std::vector<std::unique_ptr<T[]>> _managedObjects;
};

} //namespace MXNES