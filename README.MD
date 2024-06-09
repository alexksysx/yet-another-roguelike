# Requirements

## Windows
To build project on Windows you need:
[msys2](https://www.msys2.org/) with gcc, g++, ar and make (optional) installed.

To install tools run in msys2 terminal:
`$ pacman -S mingw-w64-ucrt-x86_64-gcc`

## Linux
TODO

# Build

## Build with make 
run `make` command

## Build with nobuild.c (recommended)
Compile build script:  
`gcc -o nob.exe nobuild.c`    

Run build script:
`nob.exe`