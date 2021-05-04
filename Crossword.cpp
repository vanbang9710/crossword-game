#include <bits/stdc++.h>

using namespace std;

const char DATA_FILE[] = "Crossword.txt";
typedef vector<string> vstring;
typedef vector<int> vint;

void coutCrossword(vstring &crossword)
{
    for (int i = 1; i < crossword.size(); ++i)
        cout << setw(3) << i << ".  " << crossword[i] << '\n';
}

void getCrossword(int &crossLen, vstring &crossword, vstring &question,
                  vint &keyPos, const char* fileName)
{
    ifstream fcin(fileName);
    if (fcin.is_open() == 0) {
        fcin.close(); return;
    }
    fcin >> crossLen;
    if (crossLen < 1) {
        fcin.close(); return;
    }
    crossword.resize(crossLen); question.resize(crossLen), keyPos.resize(crossLen);
    for (int i = 0; i < crossLen; ++i) {
        getline(fcin, question[i]);
        getline(fcin, question[i]);
        getline(fcin, crossword[i]);
        fcin >> keyPos[i];
    }
    fcin.close(); return;
}

void processCrossword(int &crossLen, vstring &crossword, vint &keyPos,
                      vstring &knownWord, int &maxKeyPos)
{
    for (int i = 1; i < crossLen; ++i) {
        maxKeyPos = max(maxKeyPos, keyPos[i]);
    }
    for (int i = 1; i < crossLen; ++i) {
        knownWord[i].append(maxKeyPos - keyPos[i], ' ');
        knownWord[i].append(crossword[i].size(), '*');
        knownWord[i][maxKeyPos - 1] = '|';
    }
}

void renderGame(vstring &knownWord, int point)
{
    coutCrossword(knownWord);
    cout << "Point: " << point << '\n';
}

void respondToAns(int &crossLen, vstring &crossword, vint &keyPos, vstring &knownWord, int &maxKeyPos,
                  vector<bool> &answeredQues, string &ans, int &ques, int &point, int &wordCount)
{
    bool needUpdate = 0;
    if (ans != crossword[ques]) {
        if (ques) {
            needUpdate = 1;
            cout << "Wrong! The correct answer is: " << crossword[ques];
        }
        else cout << "Wrong!";
    }
    else {
        needUpdate = 1;
        point += ((ques == 0) ? wordCount : 1);
        cout << "Right! Your point now is: " << point;
    }
    cout << '\n';
    //Update
    if (needUpdate == 0) return;
    --wordCount;
    answeredQues[ques] = 1;
    if (ques) {
        int startPos = maxKeyPos - keyPos[ques];
        for (int i = knownWord[ques].size() - 1; i >= startPos; --i)
            knownWord[ques][i] = crossword[ques][i - startPos];
    }
    else {
        for (int i = 1; i < crossLen; ++i)
            knownWord[i][maxKeyPos - 1] = crossword[0][i - 1];
    }
}

int main()
{
    //Init crossword
    int crossLen;
    vstring crossword, question;
    vint keyPos;
    getCrossword(crossLen, crossword, question, keyPos, DATA_FILE);
    if (crossword.size() < 1) {
        cout << "Invalid crossword file " << DATA_FILE;
        return -1;
    }
    vstring knownWord(crossLen);
    int maxKeyPos = 0;
    vector<bool> answeredQues(crossLen, 0);
    processCrossword(crossLen, crossword, keyPos, knownWord, maxKeyPos);
    //Interactive part
    int wordCount = crossLen, point = 0;
    do {
        renderGame(knownWord, point);
        cout << "Type the number of a question to answer it(0 to answer the keyword)\n";
        int ques;
        string tmp;
        while (1)
        {
            cin >> tmp;
            bool valid = 1;
            for (auto &c : tmp)
                if (c < '0' || c > '9') valid = 0;
            if (valid == 0) {
                cout << "Invalid input. Please type again\n";
                continue;
            }
            ques = 0;
            for (auto &c : tmp) ques = ques * 10 + c - '0';
            if (ques < 0 || ques >= crossLen || answeredQues[ques]) {
                cout << "Invalid input. Please type again\n";
                continue;
            }
            break;
        }
        if (ques != 0) cout << question[ques] << '\n';
        cout << "The number of character is " << crossword[ques].size() << ". Please type the answer\n";
        string ans;
        cin >> ans;
        tmp = ans;
        transform(tmp.begin(), tmp.end(), ans.begin(), ::tolower); //make lower case string
        respondToAns(crossLen, crossword, keyPos, knownWord, maxKeyPos, answeredQues, ans, ques, point, wordCount);
    } while (wordCount && (wordCount != 1 || answeredQues[0])); //dung khi het ques hoac chi con keyword
    cout << "Congratulation! Your final point is: " << point;

    return 0;
}
