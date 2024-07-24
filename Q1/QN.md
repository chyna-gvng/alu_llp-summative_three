### Q1. Performance Monitoring

We are interested in understanding how the PC keeps the usage of the CPU and the Memory, you are required to write a program that keeps track of the CPU usage percentage, and the Memory usage percentage. The process records the percentage every 2 seconds.

The percentage and the timestamp (hh:mm:ss) have to recorded in a file (.txt)on the disk.

Once the program executes, you will equally be required to inspect the system calls under trigger. You will only be taking open, close, write, and read system calls. The script will be redirecting the output of the trace command to another file named syscalls.txt

#### Notes
- ```Strace``` used.
- Run with ```sudo``` to avoid operation restriction on strace.