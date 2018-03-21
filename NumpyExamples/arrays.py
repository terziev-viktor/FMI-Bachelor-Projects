
import numpy as np
from datetime import datetime as dt
L = [1, 2, 3] # regular python list
A = np.array([1, 2, 3]) # numpy array acts as the math objetc vector

for e in L: # printing the list is trivial
    print(e)
    
for e in A: # printing a vector is trivial
    print(e)

print(L + L ) # concatination
print ( A + A ) # vector addition
 
# print(L**2) you cant power a list
print(A**2)
print(np.sqrt(A)) # square root of the vector
print(np.exp(A)) # exponent
print(np.log(A)) # log of the vector

# scalar/dot product of vectors
a = np.array([1, 2, 3])
b = np.array([3 , 4, 5])

dotAB = a.dot(b) # dot product
normA = np.linalg.norm(a) # ||a||
normB = np.linalg.norm(b) # ||b||
cosPHI = dotAB / (normA * normB) # cos of angle between vectors a and b
print('cos<a,b>=', end='')
print(cosPHI)
print(np.arccos(cosPHI))
