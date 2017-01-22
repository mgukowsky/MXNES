#pragma once

#include "defs.h"
#include <iostream>
#include <memory>
#include <typeinfo>
#include <unordered_map>

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
			_typeHashToInstancePointerMap[key] = new T;
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
		return *(reinterpret_cast<T*>(_typeHashToInstancePointerMap[key]));
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
			delete reinterpret_cast<T*>(_typeHashToInstancePointerMap[key]);
			_typeHashToInstancePointerMap.erase(key);
		}
	}

private:

	//Delete constructors/destructor to explicitly make this a static class
	Registry() = delete;
	~Registry() = delete;
	Registry(const Registry&) = delete;
	Registry& operator=(const Registry&) = delete;
	Registry(const Registry&&) = delete;
	Registry& operator=(const Registry&&) = delete;

	static bool _is_object_in_map(const std::size_t key);

	static std::unordered_map<std::size_t, void*> _typeHashToInstancePointerMap;

};

} //namespace MXNES