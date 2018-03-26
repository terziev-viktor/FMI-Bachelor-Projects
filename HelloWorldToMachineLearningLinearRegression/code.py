import numpy
import pandas
import matplotlib.pyplot as plt
df = pandas.read_csv('data_1d.csv') # load data from csv
X = numpy.array(df['X']) # convert data to numpy arrays
Y = numpy.array(df['Y'])

# apply the equations
n = X.size
denominator = n * X.dot(X) - X.sum()**2
a = (n * X.dot(Y) - Y.sum() * X.sum()) / denominator
b = (Y.sum() * X.dot(X) - X.sum()*X.dot(Y)) / denominator

Yprediction = a*X + b

# how good is the model ?
d1 = Y - Yprediction
d2 = Y - Y.mean()
SSres = d1.dot(d1)
SStot = d2.dot(d2)

RSq = 1 - SSres / SStot
print(RSq) # the closest to 1 the better (0.9911 in this case)

inp = input("Enter new X to predict a Y: ")
print("I guess: " + str(a*float(inp) + b))

plt.scatter(X, Y)
plt.xlabel("X")
plt.ylabel("Y")
plt.plot(X, Yprediction, color='red')
plt.show()
