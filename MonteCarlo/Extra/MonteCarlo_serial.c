/* Assignment 2: Problem 1 (MonteCarlo method to approximate value of pi)
Name: Anuradha Agarwal
Location: /home/605/agarwa/assign2/MonteCarlo/
Last Updated: March 13, 2023
Notes: Serial implementation
*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

long long int number_of_tosses;
long long int number_in_circle = 0;

int main(int argc, char *argv[]) {

    // Checking if the user gave all the required arguments
    if (argc != 2) {
        printf("Please provide number of tosses as an argument.\n");
        exit(1);
    }

    double pi_val =  3.14159265358979;

    number_of_tosses = strtol(argv[1], NULL, 0);
    srand((unsigned int)time(NULL));

    // Starting time
    clock_t start_time = clock();

    double x, y;
    for (long long int toss = 0; toss < number_of_tosses; toss++) {
        // Generating two random numbers between -1 and 1
        x = ((double)rand() / RAND_MAX) * 2 - 1;
        y = ((double)rand() / RAND_MAX) * 2 - 1;

        // If the point lies inside the unit circle, increment the count
        if (x*x + y*y <= 1) {
            number_in_circle++;
        }
    }

    // End time
    clock_t end_time = clock();

    // Estimating pi
    double pi_approx = 4.0 * (double)number_in_circle / (double)number_of_tosses;
    printf("Value of Pi is approximately: %f\n", pi_approx);

    double pi_error = fabs(pi_approx - pi_val);
    printf("Absolute error with respect to pi with 16 digits of accuracy is: %f\n", pi_error);

    double total_time = ((double)(end_time - start_time)) / CLOCKS_PER_SEC;
    printf("Total time taken: %f seconds\n", total_time);

    return 0;
}

