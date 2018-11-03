#pragma once
#include <cassert>

#if defined(_DEBUG) or not defined(NDEBUG)
#define GS_ASSERT(...) assert((__VA_ARGS__))
#else
#define GS_ASSERT(...) ((void)0)
#endif

#ifdef STATISTICS
#define GS_COLLECT_STATS
#endif