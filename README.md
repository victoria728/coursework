This program analyses electrical waveform data, 
the program calculates the rms, detects faults, and writes a report

the program structure is:

 main.c          // Program entry point
 io.c / io.h     // File input/output handling
 waveform.c / waveform.h  // Signal analysis logic
 CMakeLists.txt  // Build configuration
results.txt     // Output report (generated)
input.csv       // Example input file

 How you run the program: 
You compile the program by using GCC
gcc main.c io.c waveform.c -o analyser -lm

the program expects a CSV file with the following columns
timestamp
phaseA,phaseB,phaseC
current
frequency
powerFactor
thd

at the end the program generates a file after running it which is:
results.txt
this file includes 
Per-phase analysis (A, B, C)
System health status
Fault detection results
Top voltage magnitudes

How It Basically Works:
Load CSV data into memory
Analyse each phase:
Compute electrical metrics
Detect anomalies using thresholds
Sort samples based on voltage magnitude
Generate report

Error Handling
The program handles:
Missing or invalid input files 
Memory allocation failures 
Incorrect CSV format

Technologies Used
C Programming Language
Standard Libraries:
stdio.h
stdlib.h
math.h
string.h

Key Notes
The program uses dynamic memory allocation (malloc/free)
Sorting is implemented using insertion sort
Bitwise flags are used for efficient fault detection

Author 
Victoria Anyanwu 
