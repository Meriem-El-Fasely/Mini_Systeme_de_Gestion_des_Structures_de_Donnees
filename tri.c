#include "tri.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Fonction utilitaire pour mesurer le temps d'exécution
static void start_timer(clock_t *start) {
    *start = clock();
}

static void stop_timer(clock_t start, TriStats *stats) {
    clock_t end = clock();
    stats->time_taken = (double)(end - start) / CLOCKS_PER_SEC;
}

// ============================================================
// TRI POUR LES ENTIERS (int)
// ============================================================

void tri_bulle(int arr[], size_t n, TriStats *stats) {
    clock_t start;
    start_timer(&start);
    for (size_t i = 0; i < n - 1; i++) {
        for (size_t j = 0; j < n - i - 1; j++) {
            if (arr[j] > arr[j + 1]) {
                int temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
            }
        }
    }
    stop_timer(start, stats);
}

void tri_insertion(int arr[], size_t n, TriStats *stats) {
    clock_t start;
    start_timer(&start);
    for (size_t i = 1; i < n; i++) {
        int key = arr[i];
        int j = (int)i - 1;
        while (j >= 0 && arr[j] > key) {
            arr[j + 1] = arr[j];
            j = j - 1;
        }
        arr[j + 1] = key;
    }
    stop_timer(start, stats);
}

void tri_shell(int arr[], size_t n, TriStats *stats) {
    clock_t start;
    start_timer(&start);
    for (int gap = n / 2; gap > 0; gap /= 2) {
        for (int i = gap; i < n; i++) {
            int temp = arr[i];
            int j;
            for (j = i; j >= gap && arr[j - gap] > temp; j -= gap) {
                arr[j] = arr[j - gap];
            }
            arr[j] = temp;
        }
    }
    stop_timer(start, stats);
}

static int partition(int arr[], int low, int high) {
    int pivot = arr[high];
    int i = (low - 1);
    for (int j = low; j <= high - 1; j++) {
        if (arr[j] <= pivot) {
            i++;
            int temp = arr[i];
            arr[i] = arr[j];
            arr[j] = temp;
        }
    }
    int temp = arr[i + 1];
    arr[i + 1] = arr[high];
    arr[high] = temp;
    return (i + 1);
}

static void tri_quick(int arr[], int low, int high) {
    if (low < high) {
        int pi = partition(arr, low, high);
        tri_quick(arr, low, pi - 1);
        tri_quick(arr, pi + 1, high);
    }
}

void tri_quick_wrapper(int arr[], size_t n, TriStats *stats) {
    clock_t start;
    start_timer(&start);
    if (n > 0) {
        tri_quick(arr, 0, (int)n - 1);
    }
    stop_timer(start, stats);
}

// ============================================================
// TRI POUR LES RÉELS (float)
// ============================================================

void tri_bulle_float(float arr[], size_t n, TriStats *stats) {
    clock_t start;
    start_timer(&start);
    for (size_t i = 0; i < n - 1; i++) {
        for (size_t j = 0; j < n - i - 1; j++) {
            if (arr[j] > arr[j + 1]) {
                float temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
            }
        }
    }
    stop_timer(start, stats);
}

void tri_insertion_float(float arr[], size_t n, TriStats *stats) {
    clock_t start;
    start_timer(&start);
    for (size_t i = 1; i < n; i++) {
        float key = arr[i];
        int j = (int)i - 1;
        while (j >= 0 && arr[j] > key) {
            arr[j + 1] = arr[j];
            j = j - 1;
        }
        arr[j + 1] = key;
    }
    stop_timer(start, stats);
}

void tri_shell_float(float arr[], size_t n, TriStats *stats) {
    clock_t start;
    start_timer(&start);
    for (int gap = n / 2; gap > 0; gap /= 2) {
        for (int i = gap; i < n; i++) {
            float temp = arr[i];
            int j;
            for (j = i; j >= gap && arr[j - gap] > temp; j -= gap) {
                arr[j] = arr[j - gap];
            }
            arr[j] = temp;
        }
    }
    stop_timer(start, stats);
}

static int partition_float(float arr[], int low, int high) {
    float pivot = arr[high];
    int i = (low - 1);
    for (int j = low; j <= high - 1; j++) {
        if (arr[j] <= pivot) {
            i++;
            float temp = arr[i];
            arr[i] = arr[j];
            arr[j] = temp;
        }
    }
    float temp = arr[i + 1];
    arr[i + 1] = arr[high];
    arr[high] = temp;
    return (i + 1);
}

static void tri_quick_float(float arr[], int low, int high) {
    if (low < high) {
        int pi = partition_float(arr, low, high);
        tri_quick_float(arr, low, pi - 1);
        tri_quick_float(arr, pi + 1, high);
    }
}

void tri_quick_wrapper_float(float arr[], size_t n, TriStats *stats) {
    clock_t start;
    start_timer(&start);
    if (n > 0) {
        tri_quick_float(arr, 0, (int)n - 1);
    }
    stop_timer(start, stats);
}

// ============================================================
// TRI POUR LES CARACTÈRES (char)
// ============================================================

void tri_bulle_char(char arr[], size_t n, TriStats *stats) {
    clock_t start;
    start_timer(&start);
    for (size_t i = 0; i < n - 1; i++) {
        for (size_t j = 0; j < n - i - 1; j++) {
            if (arr[j] > arr[j + 1]) {
                char temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
            }
        }
    }
    stop_timer(start, stats);
}

void tri_insertion_char(char arr[], size_t n, TriStats *stats) {
    clock_t start;
    start_timer(&start);
    for (size_t i = 1; i < n; i++) {
        char key = arr[i];
        int j = (int)i - 1;
        while (j >= 0 && arr[j] > key) {
            arr[j + 1] = arr[j];
            j = j - 1;
        }
        arr[j + 1] = key;
    }
    stop_timer(start, stats);
}

void tri_shell_char(char arr[], size_t n, TriStats *stats) {
    clock_t start;
    start_timer(&start);
    for (int gap = n / 2; gap > 0; gap /= 2) {
        for (int i = gap; i < n; i++) {
            char temp = arr[i];
            int j;
            for (j = i; j >= gap && arr[j - gap] > temp; j -= gap) {
                arr[j] = arr[j - gap];
            }
            arr[j] = temp;
        }
    }
    stop_timer(start, stats);
}

static int partition_char(char arr[], int low, int high) {
    char pivot = arr[high];
    int i = (low - 1);
    for (int j = low; j <= high - 1; j++) {
        if (arr[j] <= pivot) {
            i++;
            char temp = arr[i];
            arr[i] = arr[j];
            arr[j] = temp;
        }
    }
    char temp = arr[i + 1];
    arr[i + 1] = arr[high];
    arr[high] = temp;
    return (i + 1);
}

static void tri_quick_char(char arr[], int low, int high) {
    if (low < high) {
        int pi = partition_char(arr, low, high);
        tri_quick_char(arr, low, pi - 1);
        tri_quick_char(arr, pi + 1, high);
    }
}

void tri_quick_wrapper_char(char arr[], size_t n, TriStats *stats) {
    clock_t start;
    start_timer(&start);
    if (n > 0) {
        tri_quick_char(arr, 0, (int)n - 1);
    }
    stop_timer(start, stats);
}

// ============================================================
// TRI POUR LES CHAÎNES (char*)
// ============================================================

void tri_bulle_string(char *arr[], size_t n, TriStats *stats) {
    clock_t start;
    start_timer(&start);
    for (size_t i = 0; i < n - 1; i++) {
        for (size_t j = 0; j < n - i - 1; j++) {
            if (strcmp(arr[j], arr[j + 1]) > 0) {
                char *temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
            }
        }
    }
    stop_timer(start, stats);
}

void tri_insertion_string(char *arr[], size_t n, TriStats *stats) {
    clock_t start;
    start_timer(&start);
    for (size_t i = 1; i < n; i++) {
        char *key = arr[i];
        int j = (int)i - 1;
        while (j >= 0 && strcmp(arr[j], key) > 0) {
            arr[j + 1] = arr[j];
            j = j - 1;
        }
        arr[j + 1] = key;
    }
    stop_timer(start, stats);
}

void tri_shell_string(char *arr[], size_t n, TriStats *stats) {
    clock_t start;
    start_timer(&start);
    for (int gap = n / 2; gap > 0; gap /= 2) {
        for (int i = gap; i < n; i++) {
            char *temp = arr[i];
            int j;
            for (j = i; j >= gap && strcmp(arr[j - gap], temp) > 0; j -= gap) {
                arr[j] = arr[j - gap];
            }
            arr[j] = temp;
        }
    }
    stop_timer(start, stats);
}

static int partition_string(char *arr[], int low, int high) {
    char *pivot = arr[high];
    int i = (low - 1);
    for (int j = low; j <= high - 1; j++) {
        if (strcmp(arr[j], pivot) <= 0) {
            i++;
            char *temp = arr[i];
            arr[i] = arr[j];
            arr[j] = temp;
        }
    }
    char *temp = arr[i + 1];
    arr[i + 1] = arr[high];
    arr[high] = temp;
    return (i + 1);
}

static void tri_quick_string(char *arr[], int low, int high) {
    if (low < high) {
        int pi = partition_string(arr, low, high);
        tri_quick_string(arr, low, pi - 1);
        tri_quick_string(arr, pi + 1, high);
    }
}

void tri_quick_wrapper_string(char *arr[], size_t n, TriStats *stats) {
    clock_t start;
    start_timer(&start);
    if (n > 0) {
        tri_quick_string(arr, 0, (int)n - 1);
    }
    stop_timer(start, stats);
}
