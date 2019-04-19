void getSize(string File_Path, int *SYMTABSize, int *litTabSize, int *BlockSize) {
    ifstream fin;
    fin.open(File_Path);

    int i;
    string line, word[3], blockCheck = "", litCheck = "";
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
        }

        if(i == 3) {
            *SYMTABSize = *SYMTABSize + 1;
        }
        if(operand.at(0) == '=') {
            if(litCheck.find(operand) != std::string::npos) {
                continue;
            } else {
                litCheck.append(operand);
                *litTabSize = *litTabSize + 1;
            }
        }
        if(instr == "USE" && operand != "NAN") {
            if (blockCheck.find(operand) != std::string::npos) {
                continue;
            } else {
                blockCheck.append(operand);
                *BlockSize = *BlockSize + 1;
            }
        }
    }
    fin.close();
}

void WriteObjectProgram(string opcode[], string programName, string programAddress, string programLength) {
    ofstream fout;
    fout.open("ObjectProgram.txt");

    fout<<"H-"<<programName<<"-"<<programAddress<<"-"<<programLength<<endl;

    int i = 0;
    while(opcode[i] != "") {
        fout<<"T";
    }
}

string DecimalToHex(int loc) {
    string hexString;
    stringstream ss;

    ss << hex << loc;
    hexString = ss.str();

    return hexString;
}

int HexToDecimal(string hexString) {
    unsigned int dec;
    stringstream ss;

    ss << hex << hexString;
    ss >> dec;
    int decimalString = static_cast<int>(dec);
    return decimalString;
}

string spaces(string s1) {
    while(s1.length() < 6) {
        s1.append(" ");
    }
    return s1;
}

string Zeroes(string s1) {
    int i = 0;
    string s2;
    while(i < 2-s1.length()) {
        s2.append("0");
        i++;
    }
    return s2.append(s1);
}

string addZeroes(string s1) {
    int i = 0;
    string s2;
    while(i < 6-s1.length()) {
        s2.append("0");
        i++;
    }
    return s2.append(s1);
}


string asciiToHex(string s1) {
    int i = 0;
    string hexString;
    while(i < s1.length()) {
        hexString.append(DecimalToHex(static_cast<int>(s1.at(i))));
        i++;
    }

    return hexString;
}

string HexToBinary(string hexString) {;
	int value;
	istringstream ost(hexString);
	ost >> hex >> value;

	bitset<32> bits(value);

	string binaryString (bits.to_string());

    return binaryString;
}

string BinaryToHex(string Binary) {
    long int longint = 0;

    int len = Binary.length();
    for(int i=0;i<len;i++) {
        longint += ( Binary[len-i-1]-48) * pow(2,i);
    }
    stringstream ss;
    string temp;

    ss <<setbase(16)<<longint;
    temp = ss.str();

    while(temp.length() != 3) {
        temp = "0" + temp;
    }

    return temp;
}

string addZeroesDisplacement(string disp, string flag) {
    int i = 0, limit;
    string s1;
    if(flag == "0") {
        limit = 3;
    } else {
        limit = 5;
    }
    while(s1.length() + disp.length() < limit) {
        s1.append("0");
    }
    return s1.append(disp);
}
