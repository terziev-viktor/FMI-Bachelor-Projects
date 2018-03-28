import numpy
import matplotlib.pyplot as plt

# read data
X = []
Y = []

for line in open('data_poly.csv'):
    x, y = line.split(',')
    x = float(x)
    y = float(y)
    X.append([1, x, x**2])
    Y.append(y)

# convert to numpy arrays

X = numpy.array(X)
Y = numpy.array(Y)

# calclulate Yprediciton
w = numpy.linalg.solve(X.T.dot(X), X.T.dot(Y))
Yprediciton = numpy.dot(X, w)

# view data
plt.scatter(X[:, 1], Y) # real values
plt.plot(sorted(X[:, 1]), sorted(Yprediciton)) # predictions, if data is not sorted plt will draw too many lines
plt.show()

