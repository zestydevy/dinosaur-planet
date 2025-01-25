#ifndef _DLL_DEF_H
#define _DLL_DEF_H

#define DLL_INTERFACE_BEGIN(name)                   \
    typedef struct DLLInst_##name DLLInst_##name;   \
    struct DLLInst_##name {                         \
        struct {                                    \
            unsigned long _pad;

#define DLL_INTERFACE_END() \
        } *exports;         \
    };

#endif //_DLL_DEF_H
