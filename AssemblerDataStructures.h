using namespace std;

    //OPTAB

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

    //SYMTAB

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

    // REGISTER

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

    //BLOCK

class Block {
    string name, length, address, *loctr;
    int number;

public:

    void setName(string name) { this->name = name; }
    void setNumber(int number) { this->number = number; }
    void setLength(string length) { this->length = length; }
    void setAddress(string address) { this->address = address; }
    void setLoctrSize(int size) {
        loctr = new string[size];
    }

    string getName() { return name; }
    int getNumber() { return number; }
    string getLength() { return length; }
    string getAddress() { return address; }
    string *getLoctr() { return loctr; }

};

string* getBlockLoctrSize(string File_Path, int blockLoctrSize[], int BlockSize) {
    ifstream fin;
    fin.open(File_Path);

    int i, index = 0;

    for(i = 0; i < BlockSize; i++) {
        blockLoctrSize[i] = 0;
    }

    string line, word[3];
    string* blockName = new string[BlockSize];

    blockName[0] = "DEFAULT";

    while(fin) {
        getline(fin, line);
        stringstream iss(line);
        i = 0;
        while (iss >> word[i]) {
            word[i];
            i++;
        }

        string label, instr, operand = "NAN";
        if(i == 3) {
            label = word[0];
            instr = word[1];
            operand = word[2];
        } else if(i == 2) {
            instr = word[0];
            operand = word[1];
        } else if(i == 1) {
            instr = word[0];
            if(instr == "USE") {
                operand = "DEFAULT";
            }
        }
        int j = 0, flag = 1;
        if(instr == "USE") {
            while(blockName[j] != "") {
                if(blockName[j] == operand) {
                    index = j;
                    flag = 0;
                }
                j++;
            }
            if(flag == 1) {
                blockName[j] = operand;
                index = j;
            }
            blockLoctrSize[index]--;
        }

        if(instr == "BASE" || instr == "LTORG") {
            blockLoctrSize[index]--;
        }

        blockLoctrSize[index]++;
    }
    blockLoctrSize[index]--;
    fin.close();

    for(int k = 1; k < BlockSize; k++) {
        blockLoctrSize[k]--;
    }

    return blockName;
}

int* BLOCK(string File_Path, Block block[], int BlockSize) {
    int* blockLoctrSize = new int[BlockSize];

    string* blockName = getBlockLoctrSize(File_Path, blockLoctrSize, BlockSize);

    int i = 0;
    for(i = 0; i < BlockSize; i++) {
        block[i].setLoctrSize(blockLoctrSize[i]);
        block[i].setName(blockName[i]);
        block[i].setNumber(i);
    }
    return blockLoctrSize;
}

int searchNumberBlock(Block block[], int BlockSize, string name) {
    int i = 0;
    while(i < BlockSize) {
        if(block[i].getName() == name) {
            return block[i].getNumber();
        }
        i++;
    }
    return -1;
}
