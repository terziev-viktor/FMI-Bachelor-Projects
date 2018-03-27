import numpy
import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D

X = []
Y = []

# y = w0x0 + w1x1 + w2x2
# where y is the dependant
# x1, x2 are the independent variables
# w0, w1, w2 are the numbers we want to find such that the error in the predictions is minimum
# x0 should be 1 so we can calculate w0

# read data
for line in open('data_2d.csv'):
    x1, x2, y = line.split(',')
    X.append([1, float(x1), float(x2)]) # append 1 for every w0
    Y.append(float(y))

X = numpy.array(X)
Y = numpy.array(Y)

# calculate Ypred
w = numpy.linalg.solve(numpy.dot(X.T, X), numpy.dot(X.T, Y))
Ypred = numpy.dot(X, w)

# plot the result
fig = plt.figure()
ax = fig.add_subplot(111, projection='3d')
ax.scatter(X[:,0], X[:,1], Y)
ax.plot(X[:,0], X[:,1], Ypred, color='red') # its an ugly image but it gives a good idea of the model
plt.show()

# calc R^2
d1 = Y - Ypred
d2 = Y - Y.mean()

Rsq = 1 - d1.dot(d1) / d2.dot(d2)
print(Rsq)