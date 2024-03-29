import numpy as np
from numpy.linalg import inv

data00 = []
data01 = []

f = open('hw4_nasdaq00.txt')
for word in f.read().split():
    data00.append(float(word))
f = open('hw4_nasdaq01.txt')
for word in f.read().split():
    data01.append(float(word))

data00count = len(data00)

X  = [[0 for x in range(3)] for y in range(data00count)]
Y = data00

for i in range(0, data00count):
    k=1
    while(k<=3):
        if(i-k>=0):
            X[i][k-1] = Y[i-k]
        k = k+1
X = X[3:]
Y = Y[3:]

theta, residuals, rank, s = np.linalg.lstsq(X, Y)
print(theta)
Xt = np.transpose(X)
A = np.matmul(Xt, X)
print(A)

Ainv = inv(A)
B = np.matmul(Xt, Y)
print(B)

Weight = np.matmul(Ainv, B)
Wt = np.transpose(Weight)

Ycalc = np.matmul(X, Wt)

MSE = ((Y - Ycalc)**2).mean()

print(Weight)
print("Mean Squared Error on Nasdaq00: " +str(MSE))

data01count = len(data01)
X  = [[0 for x in range(3)] for y in range(data01count)]
Y = data01

for i in range(0, data01count):
    k=1
    while(k<=3):
        if(i-k>=0):
            X[i][k-1] = Y[i-k]
        k = k+1

X=X[3:]
Y = Y[3:]
Ycalc = np.matmul(X, Wt)
MSE = ((Y - Ycalc)**2).mean()
print("Mean Squared Error on Nasdaq01: " +str(MSE))