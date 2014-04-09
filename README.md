cse491-parallel
===============
Hw 4
Ben Taylor and Marty Scherr

A rail yard simulator
2 perpendicular sections of track with a turn table in the middle
Each section of track will be managed by a separate mpi process


Parallel timings:
=================
arguments: -n numThreads trainz trackLen trainsPerGrid trainSpeed timeSteps

[scherrma@dev-intel10 cse491-parallel]$ time mpirun -n 5 trainz 100 75 20 100000 > /dev/null

real    0m8.566s

user    0m23.476s

sys     0m14.263s


[scherrma@dev-intel10 cse491-parallel]$ time mpirun -n 5 trainz 100 75 10 100000 > /dev/null               

real    0m6.833s

user    0m20.227s

sys     0m13.541s

Serial timings:
===============
arguments: trainz trackLen totalTrains trainSpeed timeSteps

[scherrma@dev-intel10 cse491-parallel]$ time ./trainz 100 300 10 100000 > /dev/null

real    0m12.610s

user    0m12.343s

sys     0m0.146s

[scherrma@dev-intel10 cse491-parallel]$ time ./trainz 100 300 20 100000 > /dev/null

real    0m14.774s

user    0m13.741s

sys     0m0.151s
