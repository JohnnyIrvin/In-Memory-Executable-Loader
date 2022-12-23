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

int load_file(const char *filename, char **buffer, unsigned int *size) {
    int rc = ERR_GENERIC;
    FILE *file = NULL;

    file = fopen(filename, "rb");
    CHECK_ERROR(
        file == NULL,
        "[!] Failed to open file\n"
    );

    fseek(file, 0, SEEK_END);
    *size = ftell(file);
    fseek(file, 0, SEEK_SET);

    *buffer = malloc(*size);
    CHECK_ERROR(
        *buffer == NULL,
        "[!] Failed to allocate memory\n"
    );

    fread(*buffer, 1, *size, file);
    CHECK_ERROR(
        ferror(file),
        "[!] Failed to read file\n"
    );

    rc = ERR_SUCCESS;
cleanup:
    if (file) {
        fclose(file);
    }

    FREE_IF(rc, *buffer)

    INFO("[load_file] rc: %d", rc);
    return rc;
}

int main(int argc, char *argv[]) {
    int rc = ERR_GENERIC;
    char *buffer = NULL;
    PLOADER loader = NULL;
    unsigned int size = 0;

    CHECK_ERROR(
        argc < 2,
        "[!] Usage: %s <filename>",
        argv[0]
    );
    CHECK_ERROR(
        load_file(argv[1], &buffer, &size),
        "[!] Failed to load file: %s",
        argv[1]
    );
    CHECK_ERROR(
        loader_init(&loader, buffer, size),
        "[!] Failed to initialize loader"
    );

    rc = ERR_SUCCESS;
cleanup:
    FREE(buffer)

    INFO("[main] rc: %d", rc);
    return rc;
}
