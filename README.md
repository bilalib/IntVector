# IntVector
Represents a vector of integers as a single integer to save memory.

This is done by limiting the values an element of the vector can take. If we know we only need numbers 1 through 10, an IntVector can store 19 elements in a single ```unsigned long long```. This will use about 10x less memory than if every value was stored in an integer vector, or 5x less than a vector of ```short```. The drawbacks are the limit on element values, and the use of arithmetic computations for each operation. Conversely, this will allow faster copying, hashing, and comparisons.


## CompressedVector
Compressed vector implements all the functionalities of a normal vector, except it uses IntVectors to achieve the same level of compression. It includes all the standard vector functions omitted in IntVector.


## IntVector
The IntVector class is the core. Given a maximum number ```max_elt```, an IntVector has a fixed size, and each index can hold the values ```0,1,2,...,max_elt-1```.

The user may decide the container the IntVector uses, any unsigned int type such as ```unsigned int, unsigned short, unsigned long long```, etc. The size of an IntVector depends on ```max_elt``` and this chosen int type. In particular, let b=```max_elt```, and let n be the biggest number the chosen int type can hold. Then the size of an IntVector will be log<sub>b</sub>(n). The member function ```size()``` will return this value.

Below are the common functionalities of a vector that IntVector does not implement. These were omitted because the purpose of this class is memory efficiency, and it would require storing more values or severely time-inefficient calculations to implement these functions. Instead, ```CompressedVector```, which has a variable size, implements full functionality of ```std::vector``` using ```IntVector```.
* iterators
* back()
* pop_back()
* reserve() / resize()
