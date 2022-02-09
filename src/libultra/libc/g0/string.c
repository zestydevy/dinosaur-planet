#include "libc/string.h"

void *memcpy(void *dst, const void *src, size_t size)
{
    char *dstc = (char *)dst;
    const char *srcc = (const char *)src;
    while (size > 0)
    {
        *dstc++ = *srcc++;
        size--;
    }
    return (void *)dst;
}

size_t strlen(const char *s)
{
    const char *sc = s;
    while (*sc)
        sc++;
    return sc - s;
}

char *strchr(const char *s, s32 c)
{
    const char ch = c;
    while (*s != ch)
    {
        if (*s == 0)
            return NULL;
        s++;
    }
    return (char *)s;
}
