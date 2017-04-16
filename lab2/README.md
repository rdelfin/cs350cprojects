Instructions for Compilation and Execution
==========================================

This project contains multiple files to be compiled. The command to compile them all together is:

```
$ gcc *.c -o lab2
```

This will generate an executable called `lab2`. It takes in exactly one argument: the path to the assembled y86 program. You can find a couple of sample programs (including the `quicksort` and `fibonacci` programs) in the `programs/` folder. They contain both the assembly (`*.ys`) and executable (`*.yo`) files.

Optional Cmake Compilation
--------------------------

This project supports the CMake build system. To compile it using said system, run the following commands from the root of the directory:
```
$ mkdir build/
$ cd build
$ cmake ..
$ make
```

This will generate an executable called `lab2` under the `build` directory.

*"With great power comes great responsibility"* - Uncle Ben
