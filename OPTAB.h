// #include<bits/stdc++.h>
// #include <iostream>
// #include <cstring>
// #include <fstream>

using namespace std;

class Instruction {
    string opname, format, opcode;

public:
    void set(string opname, string format, string opcode) {
        this->opname = opname;
        this->format = format;
        this->opcode = opcode;
    }
     string getopcode() {
        return opcode;
    }
    string getopname() {
        return opname;
    }
    string getformat() {
        return format;
    }
};

void OPTAB(Instruction optab[]) {

    ifstream fin;
    string line, word[3];
    fin.open("OPTAB.txt");
    int i, j = 0;
    while(fin) {
        getline(fin, line);
        stringstream iss(line);
        i = 0;
        while (iss >> word[i]) {
            word[i];
            i++;
        }
        if(line == "") {
            break;
        }
        if(word[1] == "3/4") {
            optab[j].set(word[0], word[1], word[2]);
        } else if(word[1] == "2") {
            optab[j].set(word[0], word[1], word[2]);
        } else {
            optab[j].set(word[0], word[1], word[2]);
        }
        j++;
    }
    fin.close();
}

string searchOPTAB(Instruction optab[], string search_opname) {
    for(int i = 0; i < 59; i++) {
        if(optab[i].getopname() == search_opname) {
            return (optab[i].getopcode());
        }
    }
    return "NOT FOUND";
}

string searchFormatOPTAB(Instruction optab[], string search_opname) {
    for(int i = 0; i < 59; i++) {
        if(optab[i].getopname() == search_opname) {
            return (optab[i].getformat());
        }
    }
    return "NOT FOUND";
}
//
// int main() {
//     Instruction optab[60];
//     OPTAB(optab);
//     cout<<searchOPTAB(optab, "LDA")<<endl;
//
//     return 0;
// }
