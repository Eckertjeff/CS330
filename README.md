# CS330
Coursework from CS330 Operating Systems and Networking.

Program 1:
Simple program that creates 26 threads, each thread then prints one character 100 times.  When the thread has finished printing 100 characters, it prints a newline character and then returns.  The main function then waits to join each thread before returning EXIT_SUCCESS.

Program 2:
This program simulates the Dining Philosopher's problem.  The program creates 10 philosophers named Roger.  Then it creates 10 semaphores, or chopsticks for each philosopher to use.

Each philosopher then tries to aquire two chopsticks positioned to their immediate left, and right.  If only one chopstick is aquired, the philosopher returns it and trys again.  Otherwise the philosopher is able to eat once, return the chopsticks, and then start the process over again.

The program then launches a single thread used to print the number of times each philosopher has had a chance to eat in a hexidecimal value (as to reduce use of screen space).

Program 3:
Same general idea as program 2, just uses a different algorithm for aquiring the semaphores.

Program 4:
This program simulates three different virtual memory management systems: First-in First-out (FIFO), Least Recently Used (LRU), and Optimal.  The program recieves input from a file similar to in.txt.  The first number is how many "frames" the system can hold in memory at once, the following numbers are the pages that will be requested from the harddrive.

The program reads one line at a time, taking the first value as the framecount to be used, and then puts the remaining values in an array.  The program then proceeds to run the line through the three aforementioned page replacement policies and keeps track of how many times a page fault occurs, then displays it to the user. This process repeats until it reaches the end of the file.
