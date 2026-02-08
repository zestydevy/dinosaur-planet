#ifndef __MACROS_H__
#define __MACROS_H__

#define ALIGNED(x) __attribute__((aligned(x)))

#define ARRLEN(x) ((s32)(sizeof(x) / sizeof(x[0])))

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

#define UNUSED __attribute__((unused))

#ifdef __sgi
#define __attribute__(x)
#endif

#define ALIGN8(val) (((val) + 7) & ~7)

#define STACK(stack, size) \
    u64 stack[ALIGN8(size) / sizeof(u64)]

#define STACK_START(stack) \
    ((u8*)(stack) + sizeof(stack))

#endif
