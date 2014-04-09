cse491-parallel
===============
Hw 4
Ben Taylor and Marty Scherr

A rail yard simulator
2 perpendicular sections of track with a turn table in the middle
Each section of track will be managed by a separate mpi process


Parallel timings:
[scherrma@dev-intel10 cse491-parallel]$ time mpirun -n 5 trainz 100 60 20 100000 > /dev/null

real    0m15.983s
user    1m13.100s
sys     0m1.237s
