#ifndef _SYS_STRING_H
#define _SYS_STRING_H

#include "PR/ultratypes.h"

char *strcpy(char *dest, char *src);
char *strcat(char *dest, const char *src);
/**
 * Case insensitive strcmp.
 */
int strcasecmp(const char *lhs, const char *rhs);
/**
 * Case insensitive strncmp.
 */
int strncasecmp(const char *lhs, const char *rhs, size_t count);
void *memset(void *dest, int ch, size_t count);

#endif
