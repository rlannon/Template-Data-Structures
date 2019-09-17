# Template Data Structures
A library implementing various algorithms data structures with C++ templates.
 
## Goal
The goal of this project is to learn how to use C++ templates effectively (especially alongisde other object-oriented concepts), and is not meant to be an optimized implementation of such data structures. Many of the implemented algorithms and structures will be those commonly taught in an algorithms/data structures course, such as sorting algorithms, stacks, queues, trees, etc..

## Getting Started
These are header-only and can be used when included in a project with no additional build steps.

### Supported types and classes
All data structures should support any ```<typename T>```.

Algorithms will support any class or type for which the equality/inequality operators are defined. Currently, sorting algorithms operate on a ```vector<T>```, though I intend on overloading them to operate on arrays as well.

### Portability
This project has been compiled and tested on MSVC and GCC. Other compilers, such as Clang, have not been tested. However, portability should not be an issue as all code used is standard C++ and does not use compiler-specific features.
