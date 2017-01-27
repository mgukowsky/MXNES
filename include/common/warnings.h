#pragma once

#ifdef _MSC_VER

//Warnings we will never care about if /Wall is enabled

//Tell the compiler to trust our decisions about struct alignment and padding
#pragma warning(disable:4324)

//Tell the compiler to allow '..' in relative include paths
#pragma warning(disable:4464)

//Allow the compiler to remove unreferenced inline functions
#pragma warning(disable:4514)

//XML documentation nonsense
#pragma warning(disable:4634)

//We will allow the compiler to not inline a function if it insists that doing so is a bad idea
#pragma warning(disable:4710)

//This warning doesn't play well with the current MSVC implementation of the alignas() specifier,
//so we turn it off; turn it on to see the areas where the compiler is adding padding
#pragma warning(disable:4820)

#endif //ifdef _MSC_VER