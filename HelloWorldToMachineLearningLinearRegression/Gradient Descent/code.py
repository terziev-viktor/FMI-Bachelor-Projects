import numpy
import pandas
import matplotlib.pyplot as plt
from sklearn.model_selection import train_test_split


fieldsX = ['Temperature (C)', 'Apparent Temperature (C)']
fieldY = ['Humidity']
dfX = pandas.read_csv('weatherHistory.csv', usecols=fieldsX)
dfY = pandas.read_csv('weatherHistory.csv', usecols=fieldY)

dfX, Xtest, dfY, Ytest = train_test_split(dfX, dfY, test_size=0.1)

N = dfY.shape[0]# df.shape = tuple that looks like this: (rows, cols)
N_test = Xtest.shape[0]
D = 3
Y = []
X = []
Y_test = []
X_test = []

for i in dfY.values:
    Y.append(i[0])

for i in dfX.values:
    X.append([1.0, i[0], i[1]])

for i in Xtest.values:
    X_test.append([1.0, i[0], i[0]])

for i in Ytest.values:
    Y_test.append(i[0])

X_test = numpy.array(X_test)
Y_test = numpy.array(Y_test)
X = numpy.array(X)
Y = numpy.array(Y)

w = numpy.random.randn(D) / numpy.sqrt(D)
learning_rate = 0.0000000001
T = 1000
costsX = []
costsY = []
l = 100.0
for i in range(T):
    Yhat = numpy.dot(X, w)
    delta = Yhat - Y
    w = w - learning_rate*(X.T.dot(delta) - l*w)
    costsX.append(float(i))
    costsY.append(delta.dot(delta) / N)

Ypredition = numpy.dot(X_test, w)

print(w)
plt.plot(costsX, costsY, label='mean squared error drop')
plt.legend()
plt.show()
plt.plot(sorted(Y_test), label='target')
plt.plot(sorted(Ypredition), label='prediction')
plt.legend()
plt.show()







