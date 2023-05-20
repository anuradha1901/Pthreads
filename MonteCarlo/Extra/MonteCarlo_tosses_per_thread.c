/* Assignment 2: Problem 1 (MonteCarlo method to approximate value of pi)
Name: Anuradha Agarwal
Location: /home/605/agarwa/assign2/MonteCarlo/
Last Updated: March 7, 2023
Notes: I have used the pthreads template provided as a part of the lecture notes.
*/


// Importing necessary libraries.
// pthread.h contains all the functions/types we need to run this program using multiple threads.

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>


// Initializing global variables and functions

int thread_count;                      /* Thread_count here is a global variable which is accessible to all the threads. It is implicitly initialized to 0 */
long long int number_of_tosses;        /* Number of tosses for each thread */
long long int number_in_circle = 0;    /* Shared total count in the circle */
void *Monte_carlo_func(void *rank);    /* Monte Carlo function that runs on threads (Pointer to void) */


// We will now begin the main function
int main(int argc, char *argv[]){

        // agrc here is the number of arguments and argv is the array of arguments

        long thread;
        pthread_t* thread_handles;     /* Defining handles */

        // checking if the user gave all the required  arguments

        if (argc != 3){
                printf("Please provide number of threads as your first argument and number of tosses as your second argument. \n");
                exit(1);
        }

        thread_count = strtol(argv[1], NULL, 0);                   /* Number of threads */
        thread_handles = malloc(thread_count*sizeof(pthread_t));   /* Memory allocation */
        number_of_tosses = strtol(argv[2], NULL, 0);               /* Number of tosses */

        // creating threads and allocating the function to them and providing paramenters
        // The following loop is for everthing but the master thread, essentially creating and giving work to other workers

        for (thread=0; thread<thread_count; thread++){
                pthread_create(&thread_handles[thread], NULL, Monte_carlo_func, (void *)(long)thread);
        }


        // After all the threads are done running

        for (thread=0; thread<thread_count; thread++){
                pthread_join(thread_handles[thread], NULL);
        }

        // Esitmating pi from the formula given in the assignment

        double pi_approx = 4.0*(number_in_circle)/(double)(number_of_tosses*thread_count);
        printf("Value of Pi is approximately: %f\n", pi_approx);

        // Freeing the memory

        free(thread_handles);
        return 0;
}

void *Monte_carlo_func(void *rank){

        // generating pseudo random number: for seed (reference: https://www.ibm.com/docs/en/zos/2.2.0?topic=functions-srand-set-seed-rand-function))

        srand((long)rank);                            /* seed */
        long long int number_in_circle_thread = 0;    /* initializing number_in_circle for each thread = 0 */

        // Throwing darts randomly at a square dartboard. Estimating the value of pi with a random number generator

        for (long long int toss = 0; toss < number_of_tosses; toss++){

                /* generating randome numbers reference: https://stackoverflow.com/questions/1557208/generating-random-number-between-1-1-in-c#:~:text=We%20get%20random%20numbers%20from,by%20adding%20it%20to%20%2D1%20.&text=On%20the%20limitations%20of%20this,number%20you%20want%2C%20%2D1. */

                double x = (double)rand() / RAND_MAX;    /* generates a random number between 0 and 1 */
                x = 2 * x - 1;                           /* scales the random number to be between -1 and 1 */

                double y = (double)rand() / RAND_MAX;    /* generates a random number between 0 and 1 */
                y = 2 * y - 1;                           /* scales the random number to be between -1 and 1 */

                // if distance squared <= 1 update the counter of the number_in_circle_thread variable

                if (x*x + y*y <=1){
                        number_in_circle_thread++;
                }
        }

        // updating total number_in_circle

        number_in_circle += number_in_circle_thread;
        return NULL;

}
