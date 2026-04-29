#ifndef _MACROS_H
#define _MACROS_H

#define ALIGNED(x) __attribute__((aligned(x)))

#define ARRAYCOUNT(a) (sizeof(a) / sizeof(a[0]))
#define ARRAYCOUNT_S(a) (long)ARRAYCOUNT(a)
#define OFFSETOF(type, field) ((size_t)&(((type*)0)->field))

#ifdef __sgi
#define STUBBED_PRINTF 
#elif __clang__
#define STUBBED_PRINTF(...) _Pragma("clang diagnostic push") \
                            _Pragma("clang diagnostic ignored \"-Wunused-value\"") \
                            (__VA_ARGS__) \
                            _Pragma("clang diagnostic pop")
#else
#define STUBBED_PRINTF 
#endif

#define DO_PRAGMA(x) _Pragma(#x)

#ifdef __sgi
#define PRAGMA_IGNORE_PUSH(warning)
#define PRAGMA_IGNORE_POP()
#elif __GNUC__
#define PRAGMA_IGNORE_PUSH(warning) _Pragma("GCC diagnostic push") \
                            DO_PRAGMA(GCC diagnostic ignored warning)
#define PRAGMA_IGNORE_POP() _Pragma("GCC diagnostic pop")
#elif __clang__
#define PRAGMA_IGNORE_PUSH(warning) _Pragma("clang diagnostic push") \
                            DO_PRAGMA(clang diagnostic ignored warning)
#define PRAGMA_IGNORE_POP() _Pragma("clang diagnostic pop")
#endif

#define UNUSED __attribute__((unused))

#define STACKSIZE(x) (x / sizeof(u64))

#ifdef __sgi
#define __attribute__(x)
#endif

// A few systems in the game use an array as a cache table. This gives you the asset ID
#define ASSETCACHE_ID(x)    ((x << 1) + 0)
// A few systems in the game use an array as a cache table. This gives you the pointer to the asset
#define ASSETCACHE_PTR(x)   ((x << 1) + 1)

#endif
