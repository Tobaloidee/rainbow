/// Copyright 2010-11 Bifrost Games. All rights reserved.
/// \author Tommy Nguyen

#include "Random.h"

#ifdef RAINBOW_UNIX
#	pragma GCC diagnostic push
#	pragma GCC diagnostic ignored "-Wlong-long"
#endif

#ifndef RAINBOW_IOS
#	include <dSFMT/dSFMT.c>
#endif

#ifdef RAINBOW_UNIX
#	pragma GCC diagnostic pop
#endif