#ifndef _DLL_DEF_H
#define _DLL_DEF_H

/**
 * Declares the public interface of a DLL. The interface of a DLL includes all of its
 * exported functions in the same order found in the DLL's definition in DLLS.BIN.
 *
 * DLL interfaces describe a DLL's export table as a list of function pointers called
 * the virtual function table (vtbl/vtable for short). The vtable is constructed at
 * runtime during DLL relocation with each function pointer in the table pointing to
 * the relocated address of the corresponding exported function.
 *
 * DLL interfaces form a single-inheritance hierarchy such that an interface can have
 * another interface as its "base". A DLL that implements an interface provides an
 * implementation for all functions in the inheritance hierarchy. Please note that
 * this is not exactly like interfaces in object-oriented programming. A DLL cannot
 * implement more than one interface (aside from through inheritance) and likewise
 * neither can an interface.
 *
 * All DLL interfaces must inherit from the special 'DLL' interface either directly
 * or indirectly. Its definition can be found below.
 */
#define DLL_INTERFACE(name)                 \
    typedef struct name {                   \
        struct name##_Vtbl *vtbl;           \
    } name;                                 \
    typedef struct name##_Vtbl name##_Vtbl; \
    struct name##_Vtbl

/**
 * Declares the base type of a DLL interface.
 * *Must* be used only once at the start of a DLL interface definition.
 *
 * Example:
 * DLL_INTERFACE(DLL_Foo) {
 *     DLL_INTERFACE_BASE(DLL_Bar);
 *     void (*func1)(void);
 *     // etc...
 * };
 */
#define DLL_INTERFACE_BASE(name) \
    struct name##_Vtbl base

/**
 * The base of all DLL interfaces.
 * All DLL interfaces *must* inherit this directly or indirectly, otherwise the
 * function pointers will not line up correctly in memory.
 */
DLL_INTERFACE(DLL) {
    // 32-bit value at the start of every DLL export table.
    // Always zero.
    unsigned long _pad;
};

#endif //_DLL_DEF_H
