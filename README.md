Using the MPI library, write a program that calculates a histogram for a given string of data x[1], x[2], x[3], ...x[k] 
and threshold values A[1], A[2], ...A[N] (k >> N). A histogram is an array H[1..N] with the property that H[i] is the
number of x such that A[i-1] <= x < A[i] for i = 2, ..., N and H[1] is equal to the number of values of x satisfying the condition x < A[1].<br><br>
The parallel histogram computation is performed by N sorting processes SEG(1..N), the main process P and the statistical process 
STAT. Process P sends threshold values from table A to the sorting processes so that A[i] goes to SEG[i], and then sends a string 
of x values to SEG[1]. After sending the last value, process P sends an end signal and receives results from the STAT process. 
Each sorting process checks whether the received x value is less than its threshold value. If so, it sends a signal to the STAT 
process, otherwise it passes x to the next SEG process. The STAT process counts the received signals in the H table, which it 
finally sends to the P process.
