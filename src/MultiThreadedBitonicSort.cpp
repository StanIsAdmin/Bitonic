#include <iostream>
#include <string>
#include <pthread.h>

struct data {
    bool up;
    int *list; // [] not used as we would have to specied the size.
    unsigned size;
};

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

void compare(struct data *arg) {
    unsigned dist = arg->size/2;
    for (size_t i = 0; i < dist; i++) {
        if((arg->list[i] > arg->list[i+dist]) == arg->up) {
            swap(&arg->list[i], &arg->list[i+dist]);
        }
    }
}

void *merge(void* parametre) {
    struct data *arg = (struct data*) parametre;
    pthread_t threads[2];
    void* status;
    struct data args[2] = {{true, arg->list, arg->size/2}, {false, arg->list+arg->size/2, arg->size/2}};
    if(arg->size > 1) {
        compare(arg);
        pthread_create(&threads[0], NULL, merge, (void*)&args[0]);
        pthread_create(&threads[1], NULL, merge, (void*)&args[1]);
        pthread_join(threads[0], &status);
        pthread_join(threads[1], &status);
    }
}

void *sort(void* parametre) {
    struct data *arg = (struct data*) parametre;
    pthread_t threads[2];
    void* status;
    struct data args[2] = {{true, arg->list, arg->size/2}, {false, arg->list+arg->size/2, arg->size/2}};
    if(arg->size > 1) {
        pthread_create(&threads[0], NULL, sort, (void*)&args[0]);
        pthread_create(&threads[1], NULL, sort, (void*)&args[1]);
        pthread_join(threads[0], &status);
        pthread_join(threads[1], &status);
        merge((void*)&arg);
    }
}

void *sort(bool up, int list[], unsigned size) {
    struct data arg = {up, list, size};
    sort((void*)&arg);
}

int main() {

    int list[] = {10, 30, 11, 20, 4, 330, 21, 110};
    sort(true, list, 8);
    std::cout << "result : ";
    print(list, 8);

    return 0;
}
