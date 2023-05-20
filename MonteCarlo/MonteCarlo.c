/* Assignment 2: Problem 1 (MonteCarlo method to approximate value of pi)
Name: Anuradha Agarwal
Location: /home/605/agarwa/assign2/MonteCarlo/
Last Updated: March 13, 2023
Notes: I have used the pthreads template provided as a part of the lecture notes.
No mutial exclutions are implemented here and clock() function is used to calculate
time.
*/


// Importing necessary libraries.
// pthread.h contains all the functions/types we need to run this program using multiple threads.

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h> 
#include <math.h>

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
	srand((unsigned int)time(NULL));                           /* initializing random number generator, time is used here as the seed value so that when the program runs again, new random numbers are generated. I initially tried using srand inside the Monte_carlo_func with rank as the seed value and was doubtful about the random numbers and I always got the same answer from each of the threads */
	double pi_val =  3.14159265358979;

	// Reference for srand(): https://www.tutorialspoint.com/rand-and-srand-in-c-cplusplus

	// Starting time Reference: http://www.craieprogramming.org/211/notes/timing.html
	clock_t start, end;
	start = clock();

	// creating threads and allocating the function to them and providing paramenters
	// The following loop is for everthing but the master thread, essentially creating and giving work to other workers

	for (thread=0; thread<thread_count; thread++){
		pthread_create(&thread_handles[thread], NULL, Monte_carlo_func, (void *)thread);
	}


	// After all the threads are done running 

	for (thread=0; thread<thread_count; thread++){
		pthread_join(thread_handles[thread], NULL);
	}
	

	// Esitmating pi from the formula given in the assignment 
	
	double pi_approx = 4.0*(number_in_circle)/(double)(number_of_tosses);

	// End time
        end = clock();

	// Approximate error (not including this in the time as it is not a requirement of the homework)
	double pi_error = fabs(pi_approx - pi_val);

	printf("Value of Pi is approximately: %f\n", pi_approx);
	printf("Absolute error with respect to pi with 16 digits of accuracy is: %f\n", pi_error);	


	double total_time = ((double) (end - start)) / CLOCKS_PER_SEC;
        printf("Total time taken: %f seconds\n", total_time);


	// Freeing the memory

	free(thread_handles);
	return 0;
}

void *Monte_carlo_func(void *rank){

	// The section of code here is from the pthreads lecture notes to divide threads

	long long int my_rank = (long long int) rank;
	long long int number_of_tosses_per_thread = number_of_tosses / thread_count;
	long long int my_first_toss = my_rank * number_of_tosses_per_thread;
	long long int my_last_toss = (my_rank + 1) * number_of_tosses_per_thread;


	// Making sure that the last thread gets all the remaining number of tosses

	if (my_rank == thread_count - 1) {
        	my_last_toss = number_of_tosses;
    	}


	// In this section rand_r function is called and is taking the pointer of the seed initiated for each thread of the program. I am here ensuring that each thread generated different sequences. 
	
	long long int number_in_circle_thread = 0;                  /* initializing number_in_circle for each thread = 0 */     

	// Throwing darts randomly at a square dartboard. Estimating the value of pi with a random number generator
	double x;	                                            /* x coordinate */
	double y;                                                   /* y coordinate */
	unsigned int seed = my_rank + 1;

	for (long long int toss = my_first_toss; toss < my_last_toss; toss++){

		/* generating random numbers reference: https://stackoverflow.com/questions/1557208/generating-random-number-between-1-1-in-c# */
		

		/* Here rand_r is generating new random number depending on the seed value. We are converting random generated numbers to numbers between [-1,1]. RAND_MAX here is the max value that can be returned by the function. By dividing the rand_r with RAND_MAX gives a number between 0 and 1. Multiplying that value by 2 and subtracting 1 would give us a value between [-1,1]. I used the reference above for the math here */


		x = ((double)rand_r(&seed) / (RAND_MAX)) * 2 - 1;
		y = ((double)rand_r(&seed) / (RAND_MAX)) * 2 - 1;

		// if distance squared <= 1 update the counter of the number_in_circle_thread variable, essentialy we want to see which points lie inside the circle.

		if (x*x + y*y <=1){
			number_in_circle_thread++;
		}
	}

	// updating total number_in_circle
	
	number_in_circle += number_in_circle_thread;
	return NULL;
	
}

