import numpy as np
from datetime import datetime as dt

a = np.random.randn(100)
b = np.random.randn(100)
T = 100000

# dot product using for loop
def slow_dot_product(a, b):
    result = 0
    for e, f in zip(a,b):
        result += e*f
    return result

t0 = dt.now()
for t in range(T):
    slow_dot_product(a, b)

t1 = dt.now() - t0 # slow_dot_product's time
t0 = dt.now()
for t in range(T):
    a.dot(b)
t2 = dt.now() - t0 # numpy's time
print("t1 / t2 =", t1.total_seconds() / t2.total_seconds()) # total diff in seconds between for loops and numpy func
