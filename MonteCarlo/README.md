# Assignment 2: Problem 1 
## Name: Anuradha Agarwal
## Class: COMP 605 - Scientific Computing 
## Location: /home/605/agarwa/assign2/MonteCarlo
## Last Updated: 3/13/2023

--

This folder contains the following files and folders:
- MonteCarlo.c : Main file for problem 1 of assignment 2
- MonteCarlo : Executable 
- Extra/ : This folder contains MonteCarlo_serial.c (will be used to compare to parallel)and MonteCarlo_tosses_per_head.c which was the first version of code written. Both these files have their respective executables. 


--

## Description of the problem: 
MonteCarlo.c implements the Monte Carlo method to approximate the value of pi using multiple threads. This program takes two arguments through the command line where the first argument is the number of threads and the second argument is the number of tosses. 

This program begins by creating multiple threads and assigns a fraction of the tosses to each of the threads. Then each thread randomly generates a sequence of co-ordinates and counts the number of co-ordinates that fall inside the circle that is inscribed in the square. The formula mentioned in the assignment will be then used to approximate the value of pi. 

--

## To run:

```
	./MonteCarlo #_of_threads #_of_tosses
```
--
## Analysis:

In this section I will compare how the parallel program performs with respect to serial along with how the timing and error changes with repect to change in number of 
tosses and change in number of threads. Time here is calculated using thee clock() function.

- Table 1: Compares the time it takes with different number of threads and different number of tosses:

	| Threads -> | Serial   |    2     |    4     |    8     |    64    |   128    | 
	|	     |	        |	   |	      |		 |	    |	       |
	|	 |   | 	        |	   |          |	         |	    |	       |
	| Tosses v   |          |          |          |          |          |          |          
	|------------|----------|----------|----------|----------|----------|----------|
	|    10      | 0.000002 | 0.000094 | 0.000119 | 0.000297 | 0.002223 | 0.003360 |
	|    100     | 0.000004 | 0.000098 | 0.000159 | 0.000404 | 0.001690 | 0.003435 |
	|    1000    | 0.000024 | 0.000124 | 0.000212 | 0.000476 | 0.001725 | 0.003478 |
	|    10000   | 0.000251 | 0.000215 | 0.000233 | 0.000677 | 0.001448 | 0.003989 |
	|    100000  | 0.002775 | 0.001703 | 0.001682 | 0.000846 | 0.002098 | 0.004475 |
	|    1000000 | 0.021477 | 0.016658 | 0.017425 | 0.016727 | 0.017917 | 0.019417 |
	|------------|----------|----------|----------|----------|----------|----------|

Keeping number of threads constant here we see that as number of tosses increase the time (in seconds) increases gradually which is expected, as the problem 
is getting more computational expensive. For number of tosses 10, 100, 1000 and even some cases in 10000 we see that the serial code does a better job compared 
to the parallel and as we keep increasing the number of threads the time increases more and more. This shows that for lesser number of tosses the overhead is
going beyond the serial time. We also see when number of tosses is equal to 100000, 8 threads is optimal and there is about a 3 times speedup here. Similarly when 
number of tosses is equal to 1000000, choosing any number of threads until 128 is faster than serial. The optimal in that case is also 8 threads.  

In conclusion, the execution time increases as the number of tosses increases, which is because of computationally expensive random numbers being generated 
and more calculations being performed. Secondly for higer number of tosses, parallel implementation usually perfoms better than serial. But the speedup doesn't 
continue to increase in a linear fashion with repect to number of threads. Beyond 8 threads we see that due to overhead, doesn't result in improvement. This 
shows that it is important to try different threads and sizes of problem to determine the equilibrium.


- Table 2: Compares the error with different number of threads and different number of tosses:

	| Threads -> | Serial   |    2     |    4     |    8     |    64    |   128    | 
	|	     |	        |	   |	      |		 |	    |	       |
	|	 |   | 	        |	   |          |	         |	    |	       |
	| Tosses v   |          |          |          |          |          |          |          
	|------------|----------|----------|----------|----------|----------|----------|
	|    10      | 0.458407 | 0.058407 | 0.058407 | 0.858407 | 0.458407 | 1.141593 |
	|    100     | 0.221593 | 0.378407 | 0.298407 | 0.021593 | 0.098407 | 0.221593 |
	|    1000    | 0.001593 | 0.005593 | 0.010407 | 0.086407 | 0.022407 | 0.029593 |
	|    10000   | 0.028867 | 0.011193 | 0.011993 | 0.009593 | 0.027207 | 0.012007 |
	|    100000  | 0.005993 | 0.003553 | 0.001047 | 0.000967 | 0.004247 | 0.001833 |
	|    1000000 | 0.000039 | 0.002457 | 0.000869 | 0.000805 | 0.047105 | 0.023777 |
	|------------|----------|----------|----------|----------|----------|----------| 


Some things to be noted from the table above is that the error decreases as the number of tosses increase for most cases. This is because the more number 
of toses means more accurate approximation. We also notice that this is not the case all the time especially for larger number of threads as here I'm not
using any of the mutual exclusion processes like mutexes or busy waiting, so there is a larger error due to multiple threads updating the data at the same 
time. We also see that the optimal number of threads for each number of tosses vary. Where for 10 tosses, 128 threads result in the largest error whereas 
when we use 1000 toses, 8 threads result in the largest error. One thing to note is that it is very hard to find a pattern because of the obvious reason 
of not using mutual exclutions. 
