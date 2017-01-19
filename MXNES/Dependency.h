#pragma once

#include "Registry.h"

namespace MXNES {

/**
 * Component-like class which allows for Dependency Injection (DI)
 *
 * DI is performed by having a given class retrieve its dependencies by inheriting
 * this class. 
 *
 * Because the Registry is static and Dependency instances are not, the order of
 * initialization is deterministic.
 *
 * To help avoid inheritance hierarchy issues, classes which have inherited dependencies
 * should be marked as "final"
 *
 * Usage:
 *		class ServiceA {
 *		public:
 * 			ServiceA() {}
 * 			~ServiceA() {}
 * 			void sayHi() { std::cout << "hi from ServiceA" << std::endl; }
 *		};
 *		class ServiceB {
 *		public:
 * 			ServiceB() {}
 * 			~ServiceB() {}
 * 			void sayHi() { std::cout << "hi from ServiceB" << std::endl; }
 *		};
 * 
 *		class DerivedAB : public Dependency<ServiceA>, public Dependency<ServiceB> {
 *		public:
 * 			DerivedAB() {}
 * 			~DerivedAB() {}
 * 			void sayHi() {
 *				//Use explicit template arguments to disambiguate between dependencies
 * 				Dependency<ServiceA>::dep.sayHi();
 * 				Dependency<ServiceB>::dep.sayHi();
 * 			}
 *		};
 * 
 *		int main(int argc, char **argv) {
 * 			DerivedAB dab;
 * 			dab.sayHi();
 * 
 * 			return 0;
 *		}
 */
template<typename T>
class Dependency {
protected:
	//Registry::retrieve_object() will create the instance of the dependency if it
	//does not yet exist
	Dependency() 
		: dep(Registry::retrieve_object<T>()) {}

	~Dependency() = default;

	/**
	 * The actual instance of the dependency
	 */
	T &dep;
};

} //namespace MXNES