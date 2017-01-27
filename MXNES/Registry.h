#pragma once

#include "defs.h"

#pragma warning(push, 0)
#include <iostream>
#include <memory>
#include <typeinfo>
#include <unordered_map>
#pragma warning(pop)

namespace MXNES {

class Registry {
public:
	template<typename T>
	static bool register_object() {
		const std::size_t key = typeid(T).hash_code();
		if (_is_object_in_map(key)) {
			std::cerr << "Tried to add instance of "
				<< typeid(T).name() 
				<< " but it was already in the registry";
			return false;
		}
		else {
			_typeHashToInstancePointerMap.emplace(key, new T);
			return false;
		}
	}
	
	template<typename T>
	static T& retrieve_object() {
		const std::size_t key = typeid(T).hash_code();
		
		//Create the object if it is not yet in the map
		if (!_is_object_in_map(key)) {
			/*std::cout << "Tried to retrieve instance of "
				<< typeid(T).name() 
				<< " but it was not in the registry; adding entry.";*/
			register_object<T>();
		}
		return *(reinterpret_cast<T*>(_typeHashToInstancePointerMap[key]._vptr));
	}

	template<typename T>
	static T& create_object() {
		register_object<T>();
		return retrieve_object<T>();
	}

	template<typename T>
	static void destroy_object() {
		const std::size_t key = typeid(T).hash_code();
		if (!_is_object_in_map(key)) {
			std::cerr << "Tried to destroy instance of "
				<< typeid(T).name()
				<< " but it was not in the registry.";
		}
		else {
			_typeHashToInstancePointerMap.erase(key);
		}
	}

	static void destroy_all_objects();

private:

	struct _VoidPointerRAIIWrapper {
		_VoidPointerRAIIWrapper() : _vptr(nullptr) {}
		_VoidPointerRAIIWrapper(void *vptrArg) : _vptr(vptrArg) {}
		~_VoidPointerRAIIWrapper() { 
			if (_vptr != nullptr) 
				delete _vptr; 
		}

		void *_vptr;
	};

	//Delete constructors/destructor to explicitly make this a static class
	Registry() = delete;
	~Registry() = delete;
	MXNES_DISABLE_ALTERNATE_CONSTRUCTORS(Registry);

	static bool _is_object_in_map(const std::size_t key);

	static std::unordered_map<std::size_t, _VoidPointerRAIIWrapper> _typeHashToInstancePointerMap;

};

} //namespace MXNES