import numpy as np
import matplotlib.pyplot as plt

x = np.linspace(0, 10, 100) # create list of 500 x coordinates
y = np.sin(x) # apply f(x) = sin(x) for each x

plt.plot(x, y) # draws dots and connects each 2 with a line
plt.title('sin(x)')
plt.xlabel('x')
plt.ylabel('f(x) = sin(x)')
plt.show()
plt.scatter(x, y) # draws dots
plt.show()

nums = np.random.random(1000) # get 1000 random numbers [0, 1]
bins_count = 20
plt.hist(nums, bins=bins_count) # histogram of random numbers
plt.show()
