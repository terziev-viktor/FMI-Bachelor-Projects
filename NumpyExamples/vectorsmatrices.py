import numpy as np

a = np.array([[1, 2, 3], [3,4,5]]) # array of arrays
M = np.matrix([[1, 2, 3], [3,4,5]])

print(a)
print(M) # the math object matrix
print(M.T) # M^t
print()
# creating vectors fast
Z = np.zeros(10) # zero vector
print(Z)
Z = np.zeros((5, 5)) # 5x5 zero matrix
print(Z)
Z = np.ones((5, 5)) # a 5x5 matrix containing only 1s
print(Z)

Z = np.random.random((5, 5)) # random 0 to 1
print(Z)
Z = np.random.randn(5,5) # random floats sampled from a univariate (Gaussian) distribution of mean 0 and variance 1
print(Z)

# Matrix operations
A = np.matrix([[1,2,3], [3,4,5], [6,7,8]]) # 3x3 matrix
E = np.matrix([[1,0,0], [0,1,0],[0,0,1]]) # One

Ainv = np.linalg.inv(A) # inverse of the matrix A
Einv = np.linalg.inv(E) # inverse of 1
print(A)
print(Ainv)
print(A.dot(Ainv)) # to see if its currect
print(np.linalg.det(A)) # determinant of A
# solve Ax = B
A = np.array([[1,1], [1.5, 4]])
B = np.array([2200, 5050])
print(np.linalg.solve(A, B))