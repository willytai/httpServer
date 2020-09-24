#ifndef __UTILITIES__H__
#define __UTILITIES__H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "def.h"

size_t fileRead(const char* filename, char* buffer);
void myStrCpy(char* dst, const char* src, const long unsigned int start, const long unsigned int n);
void suffix(const char* filename, char* suf);
void getFileType(const char* suf, char* type);

#endif /* __UTILITIES__H__ */
