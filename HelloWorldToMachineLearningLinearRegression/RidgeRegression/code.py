import numpy
import matplotlib.pyplot as plt

# f(x) = x^2 + 3x^3
# f'(x) = 2x + 9x^2

w = 0.001
rate = 0.01

print(w)
for i in range(200):
    w = w - rate*(2*w + 9*w**2)
    print(w)


X = numpy.linspace(-10, 10, 50)
Y = []

def f(x):
    return 2*x + 3*x**3

def fprime(x):
    return 2*x + 9*x**2

# plot f(x) = x^2 + 3*x^3
for i in range(50):
    Y.append(f(X[i]))

plt.plot(X, Y, label='f(x) = x^2')
plt.legend()
plt.show()
