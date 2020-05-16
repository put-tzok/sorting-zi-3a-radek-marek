#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

unsigned int ns[] = {100, 1000, 2500, 5000, 10000, 12500, 15000, 20000};

void fill_increasing(int *t, unsigned int n) {
    for (int i = 0; i < n; i++) {
        t[i] = i;
    }
}

void fill_decreasing(int *t, unsigned int n) {
    for (int i = 0; i < n; i++) {
        t[i] = n - i;
    }
}

void fill_vshape(int *t, unsigned int n) {
    int j = 0;
    int m = n - 1;
    for (int i = 0; i <= (n / 2); i++) {
        t[i] = m - j;
        t[m - i] = t[i] - 1;
        j = j + 2;
    }
}

void swap(int *t, int i, int j) {
    int temp = t[j];
    t[j] = t[i];
    t[i] = temp;
}

// Selection sort functions
int selection_sort_argmin(int *t, unsigned n, int i) {
    int min = i;
    for (int j = i + 1; j < n; j++) {
        if (t[j] < t[min]) {
            min = j;
        }
    }
    return min;
}

void selection_sort(int *t, unsigned n) {
    for (int i = 0; i < n; i++) {
        int j = selection_sort_argmin(t, n, i);
        swap(t, i, j);
    }
}

// Insertion Sort Functions
void insertion_sort(int *t, unsigned int n) {
    int i, key, j;
    for (i = 1; i < n; i++) {
        key = t[i];
        j = i - 1;

        while (j >= 0 && t[j] > key) {
            t[j + 1] = t[j];
            j = j - 1;
        }
        t[j + 1] = key;
    }
}

// Quick Sort Functions
int partition(int *t, int p, int r) {
    int x = t[r];
    int i = p - 1;
    for (int j = p; j <= r - 1; j++) {
        if (t[j] < x) {
            i = i + 1;
            swap(t, i, j);
        }
    }
    i = i + 1;
    swap(t, i, r);
    return i;
}

void quick_sort_(int *t, int p, int r) {
    if (p < r) {
        int q = partition(t, p, r);
        quick_sort_(t, p, q - 1);
        quick_sort_(t, q + 1, r);
    }
}

void quick_sort(int *t, unsigned int n) {
    quick_sort_(t, 0, n - 1);
}

// Heap Sort Functions
void heapify(int *t, int n, int i) {
    int largest = i;
    int l = 2 * i + 1;
    int r = 2 * i + 2;
    if (l < n && t[l] > t[largest]) largest = l;
    if (r < n && t[r] > t[largest]) largest = r;
    if (largest != i) {
        swap(t, i, largest);
        heapify(t, n, largest);
    }
}


void heap_sort(int *t, unsigned int n) {
    for (int i = n / 2 - 1; i >= 0; i--) {
        heapify(t, n, i);
    }
    for (int i = n - 1; i >= 0; i--) {
        swap(t, 0, i);
        heapify(t, i, 0);
    }
}


void fill_random(int *t, unsigned int n) {
    for (unsigned int i = 0; i < n; i++) {
        t[i] = rand();
    }
}

void is_random(int *t, unsigned int n) {
    return;
}

void is_increasing(int *t, unsigned int n) {
    for (unsigned int i = 1; i < n; i++) {
        assert(t[i] > t[i - 1]);
    }
}

void is_decreasing(int *t, unsigned int n) {
    for (unsigned int i = 1; i < n; i++) {
        assert(t[i] < t[i - 1]);
    }
}

void is_vshape(int *t, unsigned int n) {
    int *begin = t;
    int *end = t + n - 1;

    while (end - begin > 1) {
        assert(*begin > *end);
        begin++;
        assert(*end > *begin);
        end--;
    }
}

void is_sorted(int *t, unsigned int n) {
    for (unsigned int i = 1; i < n; i++) {
        assert(t[i] >= t[i - 1]);
    }
}

void (*fill_functions[])(int *, unsigned int) = {fill_random, fill_increasing, fill_decreasing, fill_vshape};

void (*check_functions[])(int *, unsigned int) = {is_random, is_increasing, is_decreasing, is_vshape};

void (*sort_functions[])(int *, unsigned int) = {selection_sort, insertion_sort, quick_sort, heap_sort};

char *fill_names[] = {"Random", "Increasing", "Decreasing", "V-Shape"};
char *sort_names[] = {"SelectionSort", "InsertionSort", "QuickSort", "HeapSort"};

int main() {
    for (unsigned int i = 0; i < sizeof(sort_functions) / sizeof(*sort_functions); i++) {
        void (*sort)(int *, unsigned int) = sort_functions[i];

        for (unsigned int j = 0; j < sizeof(fill_functions) / sizeof(*fill_functions); j++) {
            void (*fill)(int *, unsigned int) = fill_functions[j];
            void (*check)(int *, unsigned int) = check_functions[j];

            for (unsigned int k = 0; k < sizeof(ns) / sizeof(*ns); k++) {
                unsigned int n = ns[k];
                int *t = malloc(n * sizeof(*t));

                fill(t, n);
                check(t, n);

                clock_t begin = clock();
                sort(t, n);
                clock_t end = clock();
                is_sorted(t, n);

                printf("%s\t%s\t%u\t%f\n", sort_names[i], fill_names[j], n,
                       (double) (end - begin) / (double) CLOCKS_PER_SEC);
                free(t);
            }
        }
    }
    return 0;
}