Instructions for Compilation and Execution
==========================================

This project contains multiple files to be compiled. The command to compile them all together is:

```
$ g++ -o hw6 src/*.cpp -I include -std=c++11
```

This will generate an executable called `hw6`. It takes in exactly one argument: the path to the assembly file.

Optional Cmake Compilation
--------------------------

This project supports the CMake build system. To compile it using said system, run the following commands from the root of the directory:
```
$ mkdir build/
$ cd build
$ cmake ..
$ make
```

This will generate an executable called `hw6` under the `build` directory.