from subprocess import run
from itertools import product

# settings
threads_numbers = 2, 8
dx_sizes = .1, .01, .001, .0001, .00001, .000001

run(["make", "clean"])
run(["make"])

for args in product(map(str, dx_sizes), map(str, threads_numbers)):
    run(["./pthreads_calka", *args, "0", "3.14"])
