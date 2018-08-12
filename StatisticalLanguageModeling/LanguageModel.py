from math import log
from matplotlib import pyplot as plt

eps = 1e-50

# Reading words from vocab file
words = []
f = open('hw4_vocab.txt')
for word in f.read().split():
    words.append(word)
count = len(words)

# Reading counts from unigram file
ucount = []
f = open('hw4_unigram.txt')
for word in f.read().split():
    ucount.append(int(word))

# Calculating max likelihood estimate of words in unigram modelszszs
total = 0;
for i in range(0, len(words)-1):
    #print(words[i] + "-" + str(ucount[i]))
    total+=ucount[i]
uml = []
for i in range(0, len(words)-1):
    uml.append(ucount[i]/total);
    if(words[i].startswith('M')):
        print(words[i] + " : " + str(uml[i]))

#Reading from the bigram file

bicount = [[0 for x in range(count)] for y in range(count)]
temp = []
f = open('hw4_bigram.txt')
for word in f.read().split():
    temp.append(int(word))
print(len(temp))
for i in range(0, len(temp)-1, 3):
    bicount[temp[i]-1][temp[i+1]-1] = temp[i+2]

#Calculating probabilities of words most likely to occur after word 'THE"

the_posn = words.index('THE')
print(words[the_posn])

print(bicount[the_posn])
count_the = 0;
for i in range(0, len(bicount[the_posn])-1):
    count_the += bicount[the_posn][i]
print("Bigram count for the: " +str(count_the))
print("Unigram count for the: " + str(ucount[the_posn]))

sortedposns = sorted(range(len(bicount[the_posn])), key=lambda k: bicount[the_posn][k])
print(sortedposns)
print("Hello")
for i in range (len(sortedposns)-1, len(sortedposns)-11, -1):
    #print(i)
    print("THE " + words[sortedposns[i]] + " probability : " + str(bicount[the_posn][sortedposns[i]]/count_the))

#Using both models to calculate log likelihoods of the sentences

sentence1 = "THE STOCK MARKET FELL BY ONE HUNDRED POINTS LAST WEEK"
sentence2 = "THE SIXTEEN OFFICIALS SOLD FIRE INSURANCE"

def loglikhelihood(sentence):
    print(sentence)
    w1 = sentence.split(' ')
    ull = 0;
    bll = 0;
    for w in w1:
        index = words.index(w)
        prob = ucount[index] / total
        ull += log(prob)

    print("Unigram log likelihood sentence: " + str(ull));

    startIndex = words.index('<s>')
    firstWordIndex = words.index(w1[0])
    bll += log(bicount[startIndex][firstWordIndex] / sum(bicount[startIndex]))

    for i in range(0, len(w1) - 1):
        curr = words.index(w1[i])
        nxt = words.index(w1[i + 1])
        prob = bicount[curr][nxt]/sum(bicount[curr])
        if(prob < eps):
            print(words[curr]+ " - " +words[nxt])
            prob = eps
        bll += log(prob)

    print("Bigram log likelihood sentence: " + str(bll))

loglikhelihood(sentence1)
loglikhelihood(sentence2)

def mixtureProb(l, uniprob, biprob):
    return (l*(uniprob)+(1-l)*(biprob))

def weightedmode(sentence):
    print(sentence)
    w1 = sentence.split(' ')
    uniprob = []
    biprob = []
    lam=0.0
    startIndex = words.index('<s>')
    firstWordIndex = words.index(w1[0])
    biprob.append(bicount[startIndex][firstWordIndex]/sum(bicount[startIndex]))
    for i in range(0, len(w1)):
        curr = words.index(w1[i])
        if(i+1 < len(w1)):
            print(w1[i])
            print(w1[i + 1] + "--")
            nxt = words.index(w1[i + 1])
            uniprob.append(ucount[curr] / total)
            biprob.append(bicount[curr][nxt] / sum(bicount[curr]))
        else:
            uniprob.append(ucount[curr]/total)

    X = []
    Y = []
    for l in range(0, 1001):
        print(lam)
        X.append(lam)
        ll = 0
        for i in range(0, len(biprob)):
            prob = mixtureProb(lam, uniprob[i], biprob[i])
            print(prob)
            if(prob<eps):
                ll+=log(eps)
            else:
                ll += log(prob)
        print(str(ll)+'log likelihood');
        Y.append(ll)
        lam += 0.001
    max_value = max(Y)
    print(max_value)
    max_index = Y.index(max_value)
    print(X[max_index])
    plt.plot(X, Y)
    plt.show()

weightedmode(sentence2)
