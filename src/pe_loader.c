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

#if defined(OS_WINDOWS)
int _dos_header_load(const char *buffer, PIMAGE_DOS_HEADER *header) {
    int rc = ERR_GENERIC;

    CHECK_ERROR(
        buffer[0] != 'M' || buffer[1] != 'Z',
        "[!] Invalid header\n"
    );

    *header = (PIMAGE_DOS_HEADER)buffer;
    rc = ERR_SUCCESS;
cleanup:

    INFO("[_dos_header_load] rc: %d", rc);
    return rc;
}

int _nt_header_load(const char *buffer, PIMAGE_NT_HEADER *header) {
    int rc = ERR_GENERIC;

    CHECK_ERROR(
        buffer[0] != 'P' || buffer[1] != 'E' || buffer[2] != 0 || buffer[3] != 0,
        "[!] Invalid header\n"
    );

    *header = (PIMAGE_NT_HEADER)buffer;

    #if defined(BITS_32)
    CHECK_ERROR(
        (*header)->FileHeader.Machine != 0x014c,
        "[!] Bitness mismatch, expected 32-bit\n"
    );
    DEBUG("[+] Bitness: 32-bit");
    #elif defined(BITS_64)
    CHECK_ERROR(
        (*header)->FileHeader.Machine != 0x8664,
        "[!] Bitness mismatch, expected 64-bit\n"
    );
    DEBUG("[+] Bitness: 64-bit");
    #endif

    CHECK_ERROR(
        (*header)->FileHeader.NumberOfSections > 96,
        "[!] Invalid number of sections\n"
    );
    DEBUG("[+] Number of sections: %d", (*header)->FileHeader.NumberOfSections);

    CHECK_ERROR(
        (*header)->FileHeader.SizeOfOptionalHeader == 0,
        "[!] Invalid optional header size, may be a COFF object\n"
    );
    DEBUG("[+] Optional header size: %d", (*header)->FileHeader.SizeOfOptionalHeader);

    #if defined(BITS_32)
    CHECK_ERROR(
        (*header)->OptionalHeader.Magic != 0x10b,
        "[!] Invalid optional header magic\n"
    );
    #elif defined(BITS_64)
    CHECK_ERROR(
        (*header)->OptionalHeader.Magic != 0x20b,
        "[!] Invalid optional header magic\n"
    );
    #endif
    DEBUG("[+] Optional header magic: 0x%x", (*header)->OptionalHeader.Magic);

    (*header)->DataDirectory = malloc(sizeof(IMAGE_DATA_DIRECTORY) * (*header)->OptionalHeader.NumberOfRvaAndSizes);
    unsigned int i = 0;
    for (i = 0; i < (*header)->OptionalHeader.NumberOfRvaAndSizes; i++) {
        (*header)->DataDirectory[i] = (PIMAGE_DATA_DIRECTORY)(buffer + sizeof(IMAGE_OPTIONAL_HEADER) + sizeof(IMAGE_DATA_DIRECTORY) * i);
    }
    DEBUG("[+] Number of data directories: %d", (*header)->OptionalHeader.NumberOfRvaAndSizes);

    rc = ERR_SUCCESS;
cleanup:

    INFO("[_nt_header_load] rc: %d", rc);
    return rc;
}

int _load_sections(const char *buffer, PIMAGE_NT_HEADER nt_header, PIMAGE_SECTION_HEADER **sections) {
    int rc = ERR_GENERIC;
    PIMAGE_SECTION_HEADER *_sections = NULL;
    unsigned int i = 0;

    _sections = malloc(sizeof(IMAGE_SECTION_HEADER) * nt_header->FileHeader.NumberOfSections);
    CHECK_ERROR(
        _sections == NULL,
        "[!] Failed to allocate memory\n"
    );

    for (i = 0; i < nt_header->FileHeader.NumberOfSections; i++) {
        *(_sections + i) = (PIMAGE_SECTION_HEADER)(buffer + sizeof(IMAGE_SECTION_HEADER) * i);
    }
 
    *sections = _sections;
    rc = ERR_SUCCESS;
cleanup:
    FREE_IF(rc, _sections);

    INFO("[_load_sections] rc: %d", rc);
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
        size < sizeof(IMAGE_DOS_HEADER),
        "[!] Invalid buffer size\n"
    );
    CHECK_ERROR(
        _dos_header_load(buffer, &_loader->dos_header),
        "[!] Failed to load DOS header\n"
    );
    buffer = buffer + _loader->dos_header->e_lfanew;
    size -= _loader->dos_header->e_lfanew;

    CHECK_ERROR(
        _nt_header_load(buffer, &_loader->nt_header),
        "[!] Failed to load NT header\n"
    );
    buffer = buffer + sizeof(IMAGE_FILE_HEADER) + 4;
    size -= sizeof(IMAGE_FILE_HEADER) + 4;

    unsigned int optional_header_size = _loader->nt_header->FileHeader.SizeOfOptionalHeader;
    unsigned int actual_size = sizeof(IMAGE_OPTIONAL_HEADER) + sizeof(IMAGE_DATA_DIRECTORY) * _loader->nt_header->OptionalHeader.NumberOfRvaAndSizes;
    CHECK_ERROR(
        optional_header_size != actual_size,
        "[!] Invalid optional header size (expected %d, got %d)\n",
        optional_header_size, actual_size
    );
    buffer = buffer + optional_header_size;
    size -= optional_header_size;

    CHECK_ERROR(
        _load_sections(
            buffer,
            _loader->nt_header,
            &_loader->sections
        ),
        "[!] Failed to load sections\n"
    );

    int i = 0;
    for(i = 0; i < _loader->nt_header->FileHeader.NumberOfSections; i++) {
        INFO("Section %d: %s", i, (char *)&_loader->sections[i]->Name);
    }

    *loader = _loader;
    rc = ERR_SUCCESS;
cleanup:
    FREE_IF(rc, _loader);

    INFO("[loader_init] rc: %d", rc);
    return rc;
}
#endif
