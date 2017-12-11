#include <iostream>
#include <string>

void print(int list[], unsigned size) {
    std::string output = "[";
    for (size_t i = 0; i < size; i++) {
        output += std::to_string(list[i]);
        if(i < size-1) { // size-1 is the last elem of the array
            output += ", ";
        }
    }
    output += "]";
    std::cout << output << std::endl;
}

void swap(int *a, int *b) {
    int tmp = *a;
    *a = *b;
    *b = tmp;
}

void compare(bool up, int list[], unsigned size) {
    unsigned dist = size/2;
    for (size_t i = 0; i < dist; i++) {
        if((list[i] > list[i+dist]) == up) {
            swap(&list[i], &list[i+dist]);
        }
    }
}

void merge(bool up, int list[], unsigned size) {
    if(size > 1) {
        compare(up, list, size);
        merge(up, list, size/2);
        merge(up, list+size/2, size/2);
    }
}

void sort(bool up, int list[], unsigned size) {
    if(size > 1) {
        sort(true, list, size/2);
        sort(false, list+size/2, size/2);
        merge(up, list, size);
    }
}

int main() {

    int list[] = {10, 30, 11, 20, 4, 330, 21, 110};
    sort(true, list, 8);
    std::cout << "result : ";
    print(list, 8);

    return 0;
}
