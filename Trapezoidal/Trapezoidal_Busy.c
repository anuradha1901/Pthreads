/* Assignment 2: Problem 2 (Trapezoid method to approximate the aread under the curve ln(x) between 1 and e^4)
Name: Anuradha Agarwal
Location: /home/605/agarwa/assign2/Trapezoidal/
Last Updated: March 13, 2023
Notes: I have used the pthreads busy waiting template provided as a part of the lecture notes.
I'm using clock() function to find the time. 
*/


// Importing necessary libraries.
// pthread.h contains all the functions/types we need to run this program using multiple threads.


#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <pthread.h>
#include <time.h>


// Initializing global variables and functions

double a;                           // Lower limit
double b;                           // Upper limit
double h;                           // Step size
double n;                           // Number of steps which will be defined to be 1024 in the main
double sum = 0.0;                   // Storing the total sum (final)
int thread_count;                   // Number of threads provided by the user
int flag;			    // Declaring flag as a global variable


// Functions

double func(double x);
void* trapezoidal_integration(void* rank);


int main(int argc, char *argv[]){

        // Checking if enough number of arguments are provided
        if (argc != 2){
                printf("Please provide number of threads \n");
                exit(1);
        }


        // Converting number of threads from string format to number
        thread_count = strtol(argv[1], NULL, 0);
        pthread_t* thread_handles = malloc(thread_count*sizeof(pthread_t));


        // Hard coding the values from the assignment
        a = 1.0;
        b = exp(4.0);
        n = 1024;
        h = (b-a)/n;               // Step size


        // Initializing the busy waiting variable
        flag = 0;

        // Creating threads and passing the trapezoidal_integration function to each of them
        long thread;


        // Starting time Reference: http://www.craieprogramming.org/211/notes/timing.html
        // struct rusage r_usage;
        // getrusage(RUSAGE_SELF, &r_usage);
        // double start_time = (double)r_usage.ru_utime.tv_sec + (double)r_usage.ru_utime.tv_usec / 1000000;

	clock_t start, end;
        start = clock();


        for (thread=0; thread<thread_count; thread++){
                pthread_create(&thread_handles[thread], NULL, trapezoidal_integration, (void *)(long)thread);
        }


        // Wait for all threads to finish
        for (thread=0; thread<thread_count; thread++){
                pthread_join(thread_handles[thread], NULL);
        }

        double result = sum * h;         // final result


        // End time

        // getrusage(RUSAGE_SELF, &r_usage);
        // double end_time = (double)r_usage.ru_utime.tv_sec + (double)r_usage.ru_utime.tv_usec / 1000000;

	end = clock();

        printf("The approximate aread under the curve ln(x) on [1,e^4] using the trapezoidal rule is %f\n", result);

        double total_time = ((double) (end - start)) / CLOCKS_PER_SEC;
        printf("Total time taken by the program using busy waiting: %f seconds\n", total_time);
        
	free(thread_handles); // freeing memory
        return 0;

}

// function func takes in a double x as it's input and outputs log(x)

double func(double x){
        return log(x);
}


// function trapezoidal_integration calculates the area under the curve using the trapezoidal rule

void* trapezoidal_integration(void* rank){
        long my_rank = (long) rank;                             // current rank
        int n_per_thread = n/thread_count;                      // number of steps per thread
        int my_first_i = my_rank * n_per_thread;                // first step
        int my_last_i = (my_rank + 1) * n_per_thread;           // last step
        double sum_per_thread = 0.0;                            // initializing local sum for each thread

        // The last thread will compute the rest of the steps
        // This is to ensure the case where dividing the intervals among the threads is not perfect (n is not divisible by thread_count)
        if (my_rank == thread_count - 1){
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

        
	// Busy waiting flag updation
	while (flag != my_rank);
	sum += sum_per_thread;
	flag++;	 
		
        return NULL;
}

