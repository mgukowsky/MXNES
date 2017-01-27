#pragma once

#pragma warning(push, 0)
#include <string>
#pragma warning(pop)

#include "defs.h"

namespace MXNES {

//Interface for a given system component.
//To avoid the deadly diamond, inheriting classes should ALWAYS
//be marked with 'final'.
class Component {
protected:

	Component() = default;
	~Component() = default;

	MXNES_DISABLE_ALTERNATE_CONSTRUCTORS(Component);

	/**
	 * @brief reset Reinitializes all members of the object which are not dependencies
	 *
	 * @return none
	 */
	virtual void reset() = 0;

	/**
	 * @brief to_string Creates a string representation of the object's state, which can
	 *		be used later on to restore it
	 *
	 * @return The string representation of the object's state
	 */
	virtual std::string to_string() const = 0;

	/**
	 * @brief from_string Restores the object's non-dependency members to the state 
	 *		described in the given string
	 *
	 * @param source A valid string representation of the object's new state
	 * @return none
	 */
	virtual void from_string(const std::string source) = 0;
};

} //namespace MXNES