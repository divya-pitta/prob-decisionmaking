#include <iostream>
#include <fstream>
#include <unordered_map>
#include <map>
#include <vector>
#include <algorithm>
using namespace std;

class Evidence{
public:
    map<char, vector<int>> CorrectlyGuessed;
    vector<char> IncorrectGuesses;
};

std::multimap<double, string> ReadFromFile(double& totalCount)
{

    std::multimap<double, string> wordCount;
    ifstream infile("hw1_word_counts_05.txt");
    string word;
    double count;
    while(infile >> word >> count){
        wordCount.insert(make_pair(count, word));
        totalCount+=count;
    }
    return wordCount;
}

void CalculatePriorProbabilities(std::multimap<double, string> wordCount,
                                 double totalCount,
                                 std::map<string, double>& priorProb, std::map<string,
                                 std::map<char, vector<int> > >&alphabetPosition){
    std::multimap<double, string> priorProbFirst; //Used to sort the words wrt their probability to get most frequent and least frequent words

    for(std::multimap<double, string>::iterator i=wordCount.begin(); i!=wordCount.end(); i++){
        priorProbFirst.insert(make_pair(i->first/totalCount, i->second));
        priorProb.insert(make_pair(i->second, i->first/totalCount));
        for(int j=0; j<i->second.length(); j++){
            alphabetPosition[i->second][i->second.at(j)].push_back(j);
        }
    }

    //Printing for part a:
    int j=1;

    for(std::multimap<double, string>::iterator i=wordCount.begin(), k=priorProbFirst.begin(); j<=14; j++, i++, k++){
        cout << i->first << "-" << i->second << endl;
        vector<int> temp = alphabetPosition[i->second][i->second.at(2)];
        }
    for(std::multimap<double,string>::iterator i=--wordCount.end(), k=--priorProbFirst.end(); j>0; j--, i--, k--){
        cout << i->first << "-" << i->second << endl;
        //cout << k->first << "-" << k->second << endl;
    }
}
void InitializePredictiveProbMap(std::map<char, double>& predictiveProb){
    predictiveProb['A'] = 0.0;
    predictiveProb['B'] = 0.0;
    predictiveProb['C'] = 0.0;
    predictiveProb['D'] = 0.0;
    predictiveProb['E'] = 0.0;
    predictiveProb['F'] = 0.0;
    predictiveProb['G'] = 0.0;
    predictiveProb['H'] = 0.0;
    predictiveProb['I'] = 0.0;
    predictiveProb['J'] = 0.0;
    predictiveProb['K'] = 0.0;
    predictiveProb['L'] = 0.0;
    predictiveProb['M'] = 0.0;
    predictiveProb['N'] = 0.0;
    predictiveProb['O'] = 0.0;
    predictiveProb['P'] = 0.0;
    predictiveProb['Q'] = 0.0;
    predictiveProb['R'] = 0.0;
    predictiveProb['S'] = 0.0;
    predictiveProb['T'] = 0.0;
    predictiveProb['U'] = 0.0;
    predictiveProb['V'] = 0.0;
    predictiveProb['W'] = 0.0;
    predictiveProb['X'] = 0.0;
    predictiveProb['Y'] = 0.0;
    predictiveProb['Z'] = 0.0;
}
char CalculatePredictiveProbability(std::map<string, double> priorProb,
                                      std::map<string, map<char, vector<int> > > alphabetPosition,
                                      std::map<char, double>& predictiveProb,
                                      Evidence& evidence)
{
    char nextBestGuess;

    InitializePredictiveProbMap(predictiveProb);
    std::multimap<double, char> sortedPredictiveProb; //To maintain the probability of alphabets in a sorted order, just reverse of the above
    for(std::map<char, double>::iterator i = predictiveProb.begin(); i!=predictiveProb.end(); i++)
    {
        char c = i->first;
        if(evidence.CorrectlyGuessed.find(c)==evidence.CorrectlyGuessed.end()){
            for(std::map<string, double>::iterator j = priorProb.begin(); j!=priorProb.end(); j++)
        {
            if(alphabetPosition[j->first].find(c)!=alphabetPosition[j->first].end()){
                i->second += j->second;
            }
        }
            if(i->second<1.0 && i->second!=1.0){
                sortedPredictiveProb.insert(make_pair(i->second, i->first));
            }
        }
    }
    //cout << sanityCheck << endl;
    std::map<double, char>::iterator iter;
    double max = 0;
    for(iter = sortedPredictiveProb.begin(); iter!=sortedPredictiveProb.end(); iter++){
        cout << iter->second << " : " << iter->first << endl;
    }
    --iter;
    for(;iter!=sortedPredictiveProb.begin(); iter--){
        if(iter->first<1)
            break;
        else
            continue;
    }
    nextBestGuess = iter->second;

    cout << iter->second << " : " << iter->first << endl;
    return nextBestGuess;
}

Evidence GetEvidence(){
    Evidence e;

    vector<char> incorrectGuesses;
    incorrectGuesses.push_back('A');
    incorrectGuesses.push_back('E');
    incorrectGuesses.push_back('M');
    incorrectGuesses.push_back('N');
    incorrectGuesses.push_back('T');

    map<char, vector<int>> correctGuess;
    vector<int> positionVector;
    positionVector.push_back(2);
    correctGuess.insert(make_pair('O', positionVector));
    /*positionVector.clear();
    positionVector.push_back(4);
    correctGuess.insert(make_pair('S', positionVector));*/
    e.CorrectlyGuessed = correctGuess;
    e.IncorrectGuesses = incorrectGuesses;

    return e;
}

bool EvidenceSatisfied(Evidence evidence, string word, std::map<string, std::map<char, vector<int> > >& letterMap){
    for(int i=0; i<evidence.IncorrectGuesses.size(); i++){
        size_t found = word.find(evidence.IncorrectGuesses[i]);
        if(found!=std::string::npos){
            return false;
        }
    }

    for(std::map<char, vector<int> >::iterator iter = evidence.CorrectlyGuessed.begin(); iter!=evidence.CorrectlyGuessed.end(); iter++){
        vector<int> correctlyGuessedLetterPositions;
        for(int i=0; i<word.length(); i++){
            if(word[i]==iter->first){
                correctlyGuessedLetterPositions.push_back(i);
            }
        }
        if(iter->second.size()!=correctlyGuessedLetterPositions.size()){
            return false;
        }
        else{
            for(int i=0; i<iter->second.size(); i++){
                if(correctlyGuessedLetterPositions[i]!=iter->second[i]){
                    return false;
                }
            }
        }

    }
    return true;
}

char NextBestGuessGivenEvidence(Evidence evidence,
                                std::map<string, double>& priorProb,
                                std::map<string, std::map<char, vector<int> > >& letterMap,
                                std::map<char, double>& predictiveProb)
    {
        char nextBestGuess;
        map<string, double> evidenceProb; //Will be 0 if the evidence is not satisfied;
        double totalEvidenceProb = 0.0;

        for(map<string, double>::iterator i = priorProb.begin(); i!=priorProb.end(); i++){
            if(EvidenceSatisfied(evidence, i->first, letterMap)){
                evidenceProb.insert(make_pair(i->first, i->second));
                totalEvidenceProb+=i->second;
            }
        }

        map<string, double> posteriorProb;

        for(map<string, double>::iterator iter = evidenceProb.begin(); iter!=evidenceProb.end(); iter++){
            posteriorProb.insert(make_pair(iter->first, iter->second/totalEvidenceProb));
        }

        nextBestGuess = CalculatePredictiveProbability(posteriorProb, letterMap, predictiveProb, evidence);

        cout << "Next Best Guess: " << nextBestGuess << endl;

        return nextBestGuess;
    }

int main()
{
    std::multimap<double, string> wordCount;
    std::map<string, double> priorProb; //To map words to their probabilities
    std::map<string, std::map<char, vector<int> > > alphabetPosition; //To map words to the letters they contain and their positions
    std::map<char, double> predictiveProb; //To maintain the predictive probability of an alphabet

    double totalCount = 0;
    wordCount = ReadFromFile(totalCount);

    CalculatePriorProbabilities(wordCount, totalCount, priorProb, alphabetPosition);

    //char nextBestGuess = CalculatePredictiveProbability(priorProb, alphabetPosition, predictiveProb);

    char nextBestGuess;
    Evidence evidence = GetEvidence();

    InitializePredictiveProbMap(predictiveProb);

    nextBestGuess = NextBestGuessGivenEvidence(evidence, priorProb, alphabetPosition, predictiveProb);

    return 0;
}


