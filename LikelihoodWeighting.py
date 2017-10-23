import random
import numpy as np
import matplotlib.pyplot as plt

n =10
alpha = 0.1
samples = 100000
Z = 128
queryi = 8;
print(n)
print(alpha)
print(samples)

def fofbeta(sample):
    sum = 0;
    for i in range(0, n):
        sum = sum + (sample[i]*(2**i))
    return sum

def prob(sample, fbeta):
    p = abs(Z - fbeta)
    return ((1-alpha)/(1+alpha))*(alpha**p)

def likelihoodEstimate(sampleSize):
    denominator = 0;
    numerator = 0;
    for num in range(0, sampleSize):
        sample = {}
        for bi in range(0, n):
            sample[bi] = random.randint(0, 1);
        fbeta = fofbeta(sample)
        p = prob(sample, fbeta)
        denominator += p
        if (sample[queryi - 1] == 1):
            numerator += p
    return numerator/denominator;

t = np.arange(0., 5., 0.2)

# red dashes, blue squares and green triangles
plt.plot(t, t, 'r--', t, t**2, 'bs', t, t**3, 'g^')
plt.show()

for num in range(100, 20000, 1000):
    plt.plot(num, likelihoodEstimate(num), 'r')
plt.draw()
plt.show()



