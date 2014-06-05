Simple-Compiler
===============

A compiler that will read an input program and represents it in an internal data structure. The data structure will contain instructions to be executed as well as a part that represents the memory of the program (space for variables). Then the compiler will execute the data structure. This means that the program will traverse the data structure and at every node it visits, it will execute the node by changing appropriate memory locations and deciding what is the next instruction to execute (program counter). The output of the compiler is the output that the input program should produce.
