#ifndef _BSS_H
#define _BSS_H

/*
    .bss sections aren't mapped in splat yet and declaring uninitialized variables
    without that mapping (specifically for statics) causes those variables to be 
    at the wrong address. Instead, use these macros to state that an uninitialized
    variable *should* be defined in the file. The macro will instead mark it as
    extern (so this won't work for function local statics).
*/

#if 1
// This variable should be declared in this file as an uninitialized global.
#define BSS_GLOBAL extern
// This variable should be declared in this file as an uninitialized static.
#define BSS_STATIC extern
#else
#define BSS_GLOBAL 
#define BSS_STATIC static
#endif

#endif
