import numpy
import matplotlib.pyplot as plt
import sys

class Vect:
    def __init__(self, X, Y):
        self.x = X
        self.y = Y

def OrientedArea(A, B, C):
    return (B.x - A.x) * (C.y - B.y) - (C.x - B.x) * (B.y - A.y)

X = [0, 5, 0, 0]
Y = [0, 0, 5, 0]
Px = 1
Py = 1

plt.xlabel('X')
plt.ylabel('Y')
plt.plot(X, Y, color='blue')
plt.scatter(Px, Py, color='red')
plt.show()
