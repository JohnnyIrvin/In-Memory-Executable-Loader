#!/bin/bash
# Copyright (c) 2022 Johnathan P. Irvin

# Permission is hereby granted, free of charge, to any person obtaining
# a copy of this software and associated documentation files (the
# "Software"), to deal in the Software without restriction, including
# without limitation the rights to use, copy, modify, merge, publish,
# distribute, sublicense, and/or sell copies of the Software, and to
# permit persons to whom the Software is furnished to do so, subject to
# the following conditions:

# The above copyright notice and this permission notice shall be
# included in all copies or substantial portions of the Software.

# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
# EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
# MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
# NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
# LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
# OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
# WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
rm bin/*

CC="x86_64-w64-mingw32-gcc" cmake CMakelists.txt -B win64
CC="i686-w64-mingw32-gcc" cmake CMakelists.txt -B win32

# Use gcc -m64 for 64-bit and gcc -m32 for 32-bit
CC="gcc" cmake CMakelists.txt -B linux64 -DCMAKE_C_FLAGS="-m64" 
CC="gcc" cmake CMakelists.txt -B linux32 -DCMAKE_C_FLAGS="-m32"

make -C win64
make -C win32
make -C linux64
make -C linux32

mv win64/bin/in-memory-executable-loader.exe bin/win64.exe
mv win32/bin/in-memory-executable-loader.exe bin/win32.exe
mv linux64/bin/in-memory-executable-loader bin/linux64
mv linux32/bin/in-memory-executable-loader bin/linux32

rm -rf win64
rm -rf win32
rm -rf linux64
rm -rf linux32

