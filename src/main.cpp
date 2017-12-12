#include <iostream>
#include <string>
#include "BitonicSort.hpp"


int main() {

    int list[] = {10, 30, 11, 20, 4, 330, 21, 110};
    sort(true, list, 8);
    std::cout << "result : ";
    print(list, 8);
    // ---------------------
    int list1[] = {5, 9};
    sort(true, list1, 2);
    print(list1, 2);
    // ---------------------
    int list2[] = {5, 9};
    sort(false, list2, 2);
    print(list2, 2);
    // --------------------
    int list3[] = {5, 9, 10, 7};
    sort(true, list3, 4);
    print(list3, 4);
    // ---------------------
    int list4[] = {5, 9, 10, 7};
    sort(false, list4, 4);
    print(list4, 4);
    
    return 0;
}
