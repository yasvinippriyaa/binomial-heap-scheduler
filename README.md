# binomial-heap-scheduler
"jobs.txt" is a file that contains arrival time and burst time of a number of processes.
Scheduling criteria to prioritise these jobs is the ratio of burst time to waiting time.
The file "binomial-heap-scheduler.cpp" takes "jobs.txt", schedules them using binomial heap and returns the total time taken to execute all the processes.
The file "minheap-scheduler.cpp" takes "jobs.txt", schedules them using minheap and returns the total time taken to execute all the processes. 
For large number of jobs, it is observed that binomial heap scheduling takes less amount of time than minheap scheduling to execute these jobs.
