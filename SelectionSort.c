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

void selection_sort(int arr[], int n)
{
    for (int i = 0; i < n -1; i++)
    {
        int candidate = i;
        for (int j = i + 1; j < n ; j++)
        {
            if (arr[j] < arr[candidate])
            {
                candidate = j;
            }
        }
        // Swap candidate and arr[i]
        int temp = arr[i];
        arr[i] = arr[candidate];
        arr[candidate] = temp;
    }
}

void benchmark_sort(int start_size, int inc_size, int max_size, const char *filename) {
    FILE *file = fopen(filename, "w");
    if (file == NULL) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    srand(time(NULL));
    printf("Benchmarking selection sort...\n");
    printf("Array size\tTime (milliseconds)\n");

    for (int n = start_size; n <= max_size; n += inc_size) {
        int *arr = malloc(n * sizeof(int));

        // Randomly initialize the array
        for (int i = 0; i < n; i++) {
            arr[i] = rand() % 100; // Random numbers between 0 and 99
        }

        struct timespec t_start, t_stop;

        // Time the selection sort
        clock_gettime(CLOCK_REALTIME, &t_start);
        selection_sort(arr, n);
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
    const int max_size = 100000;    // Maximum array size
    const char *filename = "SelectionSort_results.dat"; // Output .dat file name

    benchmark_sort(start_size, inc_size, max_size, filename);

    return 0;
}

