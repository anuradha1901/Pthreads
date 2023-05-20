# Assignment 2: Problem 2
## Name: Anuradha Agarwal
## Class: COMP 605 - Scientific Computing
## Location: /home/605/agarwa/assign2/Trapezoidal/
## Last Updated: 3/13/2023

--

This folder contains the following files and folders:
- Trapezoidal_Busy.c: Trapezoidal rule with pthreads and busy waiting
- Trapezoidal_Busy: Executable for busy_waiting
- Trapezoidal_Mutex.c: Trapezoidal rule with pthreads and mutexes
- Trapezoidal_Mutex: Executable for mutexes
- Trapezoidal_Sem.c: Trapezoidal rule with semaphores
- Trapezoidal_Sem: Executable for semaphores
- Extra/ : This folder contains Trapezoidal_serial.c (will be used 
		to compare to parallel)and it's respective executable.


--

## Description of the problem:
Trapezoidal_Busy.c, Trapezoidal_Mutex.c, Trapezoidal_Sem.c use the Trapezoidal
method to approximate the area under the curve ln(x) b/w 1 and e^x. They use 
busy waiting, mutexes and semaphores respectively to reduce errors. The inputs 
of these programs are the number of threads and they need to be passed as command 
line arguments. 

The main function takes the number of threads, creates the treads and assigns 
each thread a section of the area to compute. The trapezoidal_integration 
function calculates the area for each of the threads and adds to the sum in an 
order that is handled by busy waiting, mutexes and semaphores.
--

## To run:

```
        ./Trapezoidal_Busy #_of_threads 
```

```
	./Trapezoidal_Mutex #_of_threads
```

```
	./Trapezoidal_Sem #_of_threads
```
--
## Analysis:

In this section I will compare the runtimes of these programs as well as see how
they work: 

- Part1: In this part of the analysis I will talk about how each of the program 
	calculates the sum. Here I'm considering 5 threads as the argument. 
	- Busy waiting: The following is one the output of an arbitrary run:
		Thread 0 calculated sum_per_thread
		Thread 0 incremented flag to 1
		Thread 2 calculated sum_per_thread
		Thread 3 calculated sum_per_thread
		Thread 1 calculated sum_per_thread
		Thread 1 incremented flag to 2
		Thread 2 incremented flag to 3
		Thread 3 incremented flag to 4
		Thread 4 calculated sum_per_thread
		Thread 4 incremented flag to 5
 
		As we can see here even though thread 2 and thread 3 have already
		finished calculating their respective sums, they still have to 
		wait for thread 1 to finish it's calculation, add to the global
		sum and increment the flag for the next thread. This is the 
		reason why busy waiting is a little slow in comparision to others,
		because of all the waiting time. 

	- Mutexes: The following is one of the output of an arbitrary run:
		Thread 0 has calculated sum_per_thread
		Thread 0 has acquired mutex and updated the sum
		Thread 0 has released mutex and completed its task
		Thread 4 has calculated sum_per_thread
		Thread 4 has acquired mutex and updated the sum
		Thread 4 has released mutex and completed its task
		Thread 2 has calculated sum_per_thread
		Thread 2 has acquired mutex and updated the sum
		Thread 2 has released mutex and completed its task
		Thread 3 has calculated sum_per_thread
		Thread 3 has acquired mutex and updated the sum
		Thread 3 has released mutex and completed its task
		Thread 1 has calculated sum_per_thread
		Thread 1 has acquired mutex and updated the sum
		Thread 1 has released mutex and completed its task

		In comparision to busy waiting, we see that there is no set 
		order in terms of which thread updates the sum. Since there
		is no unnecesary waiting involved in this method it is known to
		be faster then busy waiting. 

	- Semaphores: The following is one of the output of an arbitrary run:
		Thread 0 has calculated sum_per_thread
		Thread 0 is updating the sum
		Thread 0 added sum_per_thread to sum
		Thread 3 has calculated sum_per_thread
		Thread 3 is updating the sum
		Thread 3 added sum_per_thread to sum
		Thread 2 has calculated sum_per_thread
		Thread 2 is updating the sum
		Thread 4 has calculated sum_per_thread
		Thread 4 is updating the sum
		Thread 4 added sum_per_thread to sum
		Thread 2 added sum_per_thread to sum
		Thread 1 has calculated sum_per_thread
		Thread 1 is updating the sum
		Thread 1 added sum_per_thread to sum

		In comparision to mutexes, we see that multiple threads are able
		to add to the sum but in a more controlled way where they enter but 
		wait so there isn't an extra step to unlock. In comparision to 
		busy waiting we see that there is no set order to which thread gets
		to update the global sum first. We can conclude that mutexes and 
		semaphore update the global variables in a similar manner. The only
		difference is the lock and unlock system.


- Part 2: In this part of the analysis I will talk more about how the timing is 
	  different for the programs. All the times mentioned below are in seconds.
	  The serial version of this code takes 0.000058 seconds to run.(time is 
	  calculated using the clock() function. 


		|# of threads |  Busy  |  Mutex |  Sem   |
		|-------------|--------|--------|--------|
		|     2       |0.000184|0.000101|0.000083|
		|     4       |0.000134|0.000112|0.000183|
		|     8       |0.000241|0.000205|0.000197|
		|     16      |0.000721|0.000394|0.000613|
		|     32      |0.000990|0.000854|0.000992|
		|     64      |0.001769|0.001717|0.001946|
		|     128     |0.003656|0.003509|0.003444|
                |----------------------------------------|

	Here we see that all of the times with parallelized code takes longer than 
	serial. This could probably be the case because of the fixed n value which 
	might not be the most ideal n or the problem in general is too simple to be 
	parallelized. I'm not entirely sure if the threads that are
	doing the tasks are being used by other users at the same time or it could 
	just be the case where the overhead is much bigger than the serial. But, to 
	compare how busy waiting, mutex and semaphores perform we see that Mutexes 
	solve the problem faster than busy waiting for all the different number of 
	threads.  We also see that Sem does equally good as mutex but sometimes takes
	longer than mutexes. The fastest amongs all is the one that uses semaphores
	and 2 number of threads. For busy waiting looks like the ideal number of 
	threads is 4, for mutex the ideal number of threads is 2 and for semaphores
	the ideal number of threads is 2. One thing to keep in mind here is the size
	of the problem remains the same which is n = 1024. 

--

- Part 3: Advantages and disadvantages (from notes and pratical implementation)
	- Busy Waiting: For smaller values of n and smaller number of threads busy 
			waiting worked at par with other implementations. Whereas for
			larger numner of threads busy waiting took the longes because
			of how it sequentially lets threads to update the values. This
			way a lot of time is wasted waiting. 
	- Mutexes: For most cases, mutexes took the least time to solve the problem.The
		   fact that it lets any thread update the shared value but makes sure only
		   one thread has access to the shared variable makes it faster than busy 
		   waiting. Implementing mutexes was fairly easy but definitely harder than 
		   busy waiting. I can see how wrong implementation of mutexes could cause 
		   errors in the process where threads won't be able to access the global 
		   variable and update. In this problem each thread essentially had the same
		   importance but if there was a different scenario where each thread has a 
		   different tast, there might be some sort of competetion in a sense where if 
		   a thread which has a low priority task gets to update the global variable
	 	   and the one with a higher priority has to wait. 
	- Semaphores: An advantage for this is that multiple threads can have access to the 
		      resources. I can only think of disadvantages being very similar to mutexes.
		      Semaphores are more useful when there are more than 1 critical sections.
		      From class on 3/14 there might also be the race condition issue. 
		      
- Overall though the time is different for all three of the implementations, the values are very
close to one another. This could be the case that all the threads have the same level of performance.
Or, the problem is not complex enough to see the differences in the times. 



