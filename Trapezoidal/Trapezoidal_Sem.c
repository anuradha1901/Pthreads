/* Assignment 2: Problem 2 (Trapezoid method to approximate the aread under the curve ln(x) between 1 and e^4)
Name: Anuradha Agarwal
Location: /home/605/agarwa/assign2/Trapezoidal/
Last Updated: March 13, 2023
Notes: I have used the pthreads semaphore template provided as a part of the lecture notes. I am using the clock()
function to calculate the time.
*/

// Importing necessary libraries.
// pthread.h contains all the functions/types we need to run this program using multiple threads.
// Here we have an extra library to import which is semaphore as it is not included in pthread.h


#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <pthread.h>
#include <time.h>
#include <semaphore.h>

// Initializing global variables and functions

double a;                           // Lower limit
double b;                           // Upper limit
double h;                           // Step size
double n;                           // Number of steps which will be defined to be 1024 in the main
double sum = 0.0;                   // Storing the total sum (final)
int thread_count;                   // Number of threads provided by the user
sem_t sem_pointer;		    // Creating the semaphore pointer so calling it is easy throughout. I'm doing this more to stay consistent with how I programmed the mutex version of this code.

// Functions

double func(double x);
void* trapezoidal_integration(void* rank);


// The main program begins here

int main(int argc, char *argv[]) {
	// Checking if enough number of arguments are provided
	 if (argc != 2) {
       		 printf("Please provide number of threads \n");
       		 exit(1);
    	}

	// Converting number of threads from string format to number
    	thread_count = strtol(argv[1], NULL, 0);
    	pthread_t* thread_handles = malloc(thread_count * sizeof(pthread_t));

    	// Hard coding the values from the assignment
    	a = 1.0;
    	b = exp(4.0);
    	n = 1024;
    	h = (b - a) / n;

    	// Initializing the semaphore variable
    	sem_init(&sem_pointer, 0, 1);                    // Here the second and third arguments are constants

    	// Creating threads and passing the trapezoidal_integration function to each of them
    	long thread;

	// Starting time Reference: http://www.craieprogramming.org/211/notes/timing.html
        clock_t start, end;
        start = clock();

    	for (thread = 0; thread < thread_count; thread++) {
        	pthread_create(&thread_handles[thread], NULL, trapezoidal_integration, (void*)thread);
    	}


    	// Waiting for threads to complete
    	for (thread = 0; thread < thread_count; thread++) {
        	pthread_join(thread_handles[thread], NULL);
    	}


    	// Result
	double result = sum * h;

	// End time

        end = clock();

    	printf("The approximate aread under the curve ln(x) on [1,e^4] using the trapezoidal rule is %f\n", result);

	double total_time = ((double) (end - start)) / CLOCKS_PER_SEC;
        printf("Total time taken by the program using semaphores: %f seconds\n", total_time);

    	free(thread_handles); // freeing memory
    	sem_destroy(&sem_pointer);    // destroying the semaphore variable
	return 0;
}

// function func takes in a double x as it's input and outputs log(x)

double func(double x) {
    	return log(x);
}


// function trapezoidal_integration calculates the area under the curve using the trapezoidal rule

void* trapezoidal_integration(void* rank) {
   	long my_rank = (long) rank;                             // current rank
        int n_per_thread = n/thread_count;                      // number of steps per thread
        int my_first_i = my_rank * n_per_thread;                // first step
        int my_last_i = (my_rank + 1) * n_per_thread;           // last step
        double sum_per_thread = 0.0;                            // initializing local sum for each thread

	// The last thread will compute the rest of the steps
        // This is to ensure the case where dividing the intervals among the threads is not perfect (n is not divisible by thread_count)


    	if (my_rank == thread_count - 1) {
        	my_last_i = n;
    	}

	
	// Calculating lower limit and upper limit of intergration for each thread
        double a_per_thread = a + my_first_i * h;
        double b_per_thread = a + my_last_i * h;

        // Trapezoidal rule
        sum_per_thread = sum_per_thread + (func(a_per_thread) + func(b_per_thread))/2.0;

        for (int step = my_first_i + 1; step < my_last_i; step++){
                double x = a + step * h;
                sum_per_thread += func(x);
        }

	
	// Equivalent to locking the mutex --> adding the sum --> unlocking
    	sem_wait(&sem_pointer);
    	sum += sum_per_thread;
    	sem_post(&sem_pointer);

    	return NULL;
}

