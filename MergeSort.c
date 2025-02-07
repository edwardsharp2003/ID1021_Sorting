#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <unistd.h>
#include <sys/time.h>

long nano_seconds(struct timespec *t_start, struct timespec *t_stop) {
    return (t_stop->tv_nsec - t_start->tv_nsec) +
           (t_stop->tv_sec - t_start->tv_sec) * 1000000000;
}

void merge(int *org, int *aux, int lo, int mid, int hi)
{
    // Copy elements from the original array to the auxiliary array
    for (int k = lo; k <= hi; k++)
    {
        aux[k] = org[k];
    }

    int i = lo;        // Pointer for the left subarray
    int j = mid + 1;   // Pointer for the right subarray

    // Merge elements back into the original array
    for (int k = lo; k <= hi; k++)
    {
        if (i > mid)
        {            // Left part exhausted, take from the right
            org[k] = aux[j++];
        }
        else if (j > hi)
        {      // Right part exhausted, take from the left
            org[k] = aux[i++];
        }
        else if (aux[i] <= aux[j])
        {  // Smaller element from the left part
            org[k] = aux[i++];
        }
        else
        {                  // Smaller element from the right part
            org[k] = aux[j++];
        }
    }
}

void sort(int *org, int *aux, int lo, int hi) {
    if (lo >= hi) {
        return; // Base case: One-element subarray is already sorted
    }

    int mid = (lo + hi) / 2;

    // Recursively sort left and right halves
    sort(org, aux, lo, mid);
    sort(org, aux, mid + 1, hi);

    // Merge the two sorted halves
    merge(org, aux, lo, mid, hi);
}

void merge_sort(int *org, int n) {
    if (n <= 1) {
        return; // No sorting needed for arrays of size 0 or 1
    }

    int *aux = (int *)malloc(n * sizeof(int)); // Allocate auxiliary array
    if (!aux) {
        perror("Error allocating memory for auxiliary array");
        exit(EXIT_FAILURE);
    }

    sort(org, aux, 0, n - 1); // Sort the entire array with bounds [0, n-1]
    free(aux);                // Free auxiliary memory
}


// Benchmark function for merge sort
void benchmark_sort(int start_size, int inc_size, int max_size, const char *filename) {
    FILE *file = fopen(filename, "w");
    if (file == NULL) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    srand(time(NULL));
    printf("Benchmarking merge sort...\n");
    printf("Array size\tTime (microseconds)\n");

    for (int n = start_size; n <= max_size; n += inc_size) {
        int *arr = malloc(n * sizeof(int));

        // Randomly initialize the array
        for (int i = 0; i < n; i++)
        {
            arr[i] = rand() % 100; // Random numbers between 0 and 99
        }

        struct timespec t_start, t_stop;

        // Time the merge sort
        clock_gettime(CLOCK_REALTIME, &t_start);
        merge_sort(arr, n);
        clock_gettime(CLOCK_REALTIME, &t_stop);

        long duration = nano_seconds(&t_start, &t_stop);
        duration = duration * 0.000001; // change factor depending on unit (* 1 for nanoseconds)

        // Print to console
        printf("%d\t\t%ld\n", n, duration);

        // Write to .dat file
        fprintf(file, "%d %ld\n", n, duration);

        free(arr); // Free allocated memory
    }

    fclose(file);
    printf("Benchmark data written to file: %s\n", filename);
}

int main() {
    // Parameters
    const int start_size = 1000;   // Initial array size
    const int inc_size = 1000;     // Increment in array size
    const int max_size = 1000000;    // Maximum array size
    const char *filename = "MergeSort_results.dat"; // Output .dat file name

    benchmark_sort(start_size, inc_size, max_size, filename);

    return 0;
}

