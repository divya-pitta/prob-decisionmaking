import random
import numpy as np
import matplotlib.pyplot as plt

n =10
alpha = 0.1
samples = 100000
Z = 128
queryi = 10;
print(n)
print(alpha)
print(samples)

def fofbeta(sample):
    sum = 0;
    for i in range(1, n+1):
        sum = sum + (sample[i]*(2**(i-1)))
    return sum

def prob(fbeta):
    p = abs(Z - fbeta)
    return ((1-alpha)/(1+alpha))*(alpha**p)

def likelihoodEstimate():
    sampleSize = 1000000;
    denominator = 0;
    numerator = 0;
    x = []
    y = []
    for num in range(0, sampleSize):
        sample = {}
        for bi in range(1, n+1):
            #print(bi)
            sample[bi] = random.randint(0, 1);
        fbeta = fofbeta(sample)
        p = prob(fbeta)
        denominator += p
        if (sample[queryi] == 1):
            numerator += p
        if(denominator==0.0):
            continue;
        x.append(num)
        y.append(numerator/denominator)
    x
    y
    plt.plot(x, y)
    plt.show()

print("--Starting likelihood estimation--")
likelihoodEstimate()
print("--End of estimation, plotting graph--")