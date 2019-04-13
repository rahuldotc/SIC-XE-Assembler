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
class Symbol {
    string label, location;
public:
    void set(string label, string location) {
        this->label = label;
        this->location = location;
    }
    string getLabel() {
        return label;
    }
    string getLocation() {
        return location;
    }
};

string searchLocationSymtab(Symbol symtab[], string label, int SYMTABSize) {
    int i = 0;
    while(i < SYMTABSize) {
        if(label == symtab[i].getLabel()) {
            return symtab[i].getLocation();
        }
        i++;
    }
    return "NOT FOUND";
}

class Register {
    string name, opcode;
public:
    void set(string name, string opcode) {
        this->name = name;
        this->opcode = opcode;
    }
    string getopcode() {
        return opcode;
    }
    string getname() {
        return name;
    }
};

void REGISTER(Register registers[]) {
    registers[0].set("A", "0");
    registers[1].set("X", "1");
    registers[2].set("L", "2");
    registers[3].set("B", "3");
    registers[4].set("S", "4");
    registers[5].set("T", "5");
    registers[6].set("F", "6");
    registers[7].set("PC", "8");
    registers[8].set("SW", "9");
}

string searchRegisters(Register registers[], string name) {
    int i = 0;
    while(name != registers[i].getname()) {
        i++;
    }
    return registers[i].getopcode();
}
