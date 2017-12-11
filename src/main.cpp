#include <iostream>
#include <string>

void swap(int *a, int *b) {
    int tmp = *a;
    *b = *a;
    *a = tmp;
}

void compare(bool up, int *list, unsigned size) {
    std::cout << "in comp" << std::endl;
    unsigned dist = size/2;
    for (size_t i = 0; i < dist; i++) {
        if((*(list+i) > *(list+i+dist)) == up) {
            swap(list+i, list+i+dist);
        }
    }
}

void sort(bool up, int *list, unsigned size) {
    std::cout << "in sort. Size : " << std::to_string(size) << std::endl;
    if(size > 2) {
        sort(true, list, size/2);
        sort(false, list+size/2, size);
        compare(up, list, size);
    }
}

void print(int *list, unsigned size) {
    std::cout << "in print" << std::endl;
    std::string output = "[";
    for (size_t i = 0; i < size; i++) {
        output += std::to_string(*(list+i));
        if(i < size-1) { // size-1 is the last elem of the array
            output += ", ";
        }
    }
    output += "]";
    std::cout << output << std::endl;
}

int main() {
    int list[] = {10, 30, 11, 20, 4, 330, 21, 110};
    print(list, 8);
    sort(true, list, 8);
    return 0;
}
