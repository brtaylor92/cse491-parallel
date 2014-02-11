cse491-parallel
===============
CSE 491 section 4, homework 2
By Ben Taylor and Marty Scherr

timings:
10 core 2p machine


rand:
Single threaded took 4344ms (4.34452s)
1 threads took 4758ms (4.75834s)
Speedup: 0.913032    (fraction of linear: 0.913032)

10 threads took 1171ms (1.17173s)
Speedup: 3.70777    (fraction of linear: 0.370777)

20 threads took 952ms (0.952221s)
Speedup: 4.56251    (fraction of linear: 0.228125)

30 threads took 976ms (0.976392s)
Speedup: 4.44956    (fraction of linear: 0.148319)


mult:
Single threaded took 974ms (0.974285s)
1 threads took 982ms (0.982585s)
Speedup: 0.991553    (fraction of linear: 0.991553)

10 threads took 118ms (0.118999s)
Speedup: 8.18734    (fraction of linear: 0.818734)

20 threads took 69ms (0.069368s)
Speedup: 14.0452    (fraction of linear: 0.702258)

30 threads took 69ms (0.06951s)
Speedup: 14.0165    (fraction of linear: 0.467216)


add:
Single threaded took 1120ms (1.1205s)
1 threads took 1103ms (1.10327s)
Speedup: 1.01562    (fraction of linear: 1.01562)

10 threads took 1008ms (1.00849s)
Speedup: 1.11107    (fraction of linear: 0.111107)

20 threads took 1010ms (1.01038s)
Speedup: 1.10899    (fraction of linear: 0.0554496)

30 threads took 1009ms (1.00986s)
Speedup: 1.10957    (fraction of linear: 0.0369855)
