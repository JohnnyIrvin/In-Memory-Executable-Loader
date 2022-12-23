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
#include <stdio.h>
#include <stdlib.h>

#include "include/errors.h"
#include "include/loader.h"
#include "include/macros.h"

#if defined(OS_LINUX)

int _elf_header_load(const char *buffer, PELF_HEADER *elf_header) {
    int rc = ERR_GENERIC;
    PELF_HEADER _elf_header = NULL;

    _elf_header = malloc(sizeof(ELF_HEADER));
    CHECK_ERROR(
        _elf_header == NULL,
        "[!] Failed to allocate memory\n"
    );
    _elf_header = (PELF_HEADER)buffer;
    CHECK_ERROR(
        _elf_header->e_ident[0] != '\x7f' ||
        _elf_header->e_ident[1] != 'E' ||
        _elf_header->e_ident[2] != 'L' ||
        _elf_header->e_ident[3] != 'F',
        "[!] File is not an ELF file\n"
    );

    *elf_header = _elf_header;
    rc = ERR_SUCCESS;
cleanup:
    FREE_IF(rc, _elf_header);

    INFO("[_elf_header_load] rc: %d", rc);
    return rc;
}

int loader_init(PLOADER *loader, const char *buffer, unsigned int size) {
    int rc = ERR_GENERIC;
    PLOADER _loader = NULL;

    _loader = malloc(sizeof(LOADER));
    CHECK_ERROR(
        _loader == NULL,
        "[!] Failed to allocate memory\n"
    );
    CHECK_ERROR(
        size < sizeof(ELF_HEADER),
        "[!] File is too small to be an ELF file\n"
    );
    CHECK_ERROR(
        _elf_header_load(buffer, &_loader->elf_header) != ERR_SUCCESS,
        "[!] Failed to load ELF header\n"
    );

    *loader = _loader;
    rc = ERR_SUCCESS;
cleanup:
    FREE_IF(rc, _loader);

    INFO("[loader_init] rc: %d", rc);
    return rc;
}

#endif // OS_LINUX
