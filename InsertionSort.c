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

// Utility function to swap two elements
void swap(int *a, int *b)
{
    int temp = *a;
    *a = *b;
    *b = temp;
}

// Function to perform insertion sort using swaps
void insertion_sort(int arr[], int n)
{
    for (int i = 1; i < n; i++)
    {
        for (int j = i; j > 0 && arr[j] < arr[j - 1]; j--)
        {
            swap(&arr[j], &arr[j - 1]);
        }
    }
}

// Benchmark function for insertion sort
void benchmark_sort(int start_size, int inc_size, int max_size, const char *filename) {
    FILE *file = fopen(filename, "w");
    if (file == NULL) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    srand(time(NULL));
    printf("Benchmarking insertion sort...\n");
    printf("Array size\tTime (milliseconds)\n");

    for (int n = start_size; n <= max_size; n += inc_size) {
        int *arr = malloc(n * sizeof(int));

        // Randomly initialize the array
        for (int i = 0; i < n; i++)
        {
            arr[i] = rand() % 100; // Random numbers between 0 and 99
        }

        struct timespec t_start, t_stop;

        // Time the insertion sort
        clock_gettime(CLOCK_REALTIME, &t_start);
        insertion_sort(arr, n);
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
    const int max_size = 50000;    // Maximum array size
    const char *filename = "InsertionSort_results.dat"; // Output .dat file name

    benchmark_sort(start_size, inc_size, max_size, filename);

    return 0;
}

