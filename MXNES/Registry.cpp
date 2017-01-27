#include "Registry.h"

using namespace MXNES;

std::unordered_map<std::size_t, 
	Registry::_VoidPointerRAIIWrapper> Registry::_typeHashToInstancePointerMap;

void Registry::destroy_all_objects() {
	_typeHashToInstancePointerMap.clear();
}

bool Registry::_is_object_in_map(const std::size_t key) {
	return _typeHashToInstancePointerMap.find(key) != _typeHashToInstancePointerMap.end();
}