// @DECOMP_OPT_FLAGS=-dollar

#ifdef __sgi
// official name: diCpuTraceCurrentStack
void* diCpuTraceCurrentStack(void) {
    return __$sp;
}
#elif (__GNUC__ || __clang__)
void* diCpuTraceCurrentStack(void) {
    register void *sp;
    asm volatile("move %0, $sp\n" : "=r"(sp));
    return sp;
}
#endif
