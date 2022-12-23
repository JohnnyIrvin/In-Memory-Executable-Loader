// Copyright (c) 2022 Johnathan P. Irvin

// Permission is hereby granted, free of charge, to any person obtaining
// a copy of this software and associated documentation files (the
// "Software"), to deal in the Software without restriction, including
// without limitation the rights to use, copy, modify, merge, publish,
// distribute, sublicense, and/or sell copies of the Software, and to
// permit persons to whom the Software is furnished to do so, subject to
// the following conditions:

// The above copyright notice and this permission notice shall be
// included in all copies or substantial portions of the Software.

// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
// EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
// MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
// NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
// LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
// OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
// WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
#ifndef MACROS_H
#define MACROS_H
#include <stdio.h>

#define RED "\033[1;31m"
#define YELLOW "\033[1;33m"
#define GREEN "\033[1;32m"
#define BLUE "\033[1;34m"
#define GRAY "\033[1;30m"
#define RESET "\033[0m"

#if defined(_WIN32) || defined(_WIN64)
#define OS_WINDOWS
#elif defined(__linux__)
#define OS_LINUX
#endif

// Bitness
#if __SIZEOF_POINTER__ == 8
#define BITS_64
#elif __SIZEOF_POINTER__ == 4
#define BITS_32
#endif

#define LOG(severity, color, format, ...) \
  fprintf(stderr, "%s%s: " format RESET "\n", color, severity, ##__VA_ARGS__)
#define ERROR(format, ...) LOG("ERROR", RED, format, ##__VA_ARGS__)
#define WARNING(format, ...) LOG("WARNING", YELLOW, format, ##__VA_ARGS__)
#define INFO(format, ...) LOG("INFO", GRAY, format, ##__VA_ARGS__)
#define DEBUG(format, ...) LOG("DEBUG", BLUE, format, ##__VA_ARGS__)
#define SUCCESS(format, ...) LOG("SUCCESS", GREEN, format, ##__VA_ARGS__)

#define CHECK_ERROR(cond, format, ...) \
    if (cond) { \
      ERROR(format, ##__VA_ARGS__); \
      goto cleanup; \
    }
#define FREE(ptr) \
    if (ptr) { \
      free(ptr); \
      ptr = NULL; \
    }
#define FREE_IF(cond, ptr) \
    if (cond) { \
      FREE(ptr); \
    }
#endif // MACROS_H
