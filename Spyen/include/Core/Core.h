#pragma once

#ifdef SPYEN_BUILD_DLL
#define SPYEN_API __declspec(dllexport)
#else
#define SPYEN_API __declspec(dllimport)
#endif
#include <cstdint>

