# Template Data Structures
A library implementing various algorithms data structures with C++ templates.
 
## Goal
The goal of this project is to learn how to use C++ templates effectively (especially alongisde other object-oriented concepts), and is not meant to be an optimized implementation of such data structures. Many of the implemented algorithms and structures will be those commonly taught in an algorithms/data structures course, such as sorting algorithms, stacks, queues, trees, etc..

## Getting Started
These are header-only and can be used when included in a project with no additional build steps.

### Supported types and classes
All data structures should support any ```<typename T>```.

Currently, sorting algorithms only operate on ```vector<T>```, but will support any ```<typename T>``` for which the inequality operators (```<``` and ```>```) are defined. Search algorithms use iterators and will therefore operate on any STL-compliant container with the appropriate iterator. Binary search, for example, requires a random-access iterator because the iterator requires the less-than operator. As such, a ```linked_list<T>``` from this project will generate an error. Because ```std::iterator_traits``` is used to check this, the error will be generated at compile time. 

### Portability
This project has been compiled and tested on MSVC and GCC. Other compilers, such as Clang, have not been tested. However, portability should not be an issue as all code used is standard C++ and does not use compiler-specific features.
