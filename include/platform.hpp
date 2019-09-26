/*
platform agnostic "simple include"
intended for a simple preprocessor if switch
*/
#ifndef PLATFORM_HPP
#define PLATFORM_HPP

#ifdef __linux__
#include "platform/linux_platform.hpp"
#endif


#endif
