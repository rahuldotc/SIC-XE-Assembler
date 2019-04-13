void getSize(int *LOCTRSize, int *SYMTABSize) {
    ifstream fin;
    fin.open("/home/rahul/Documents/P.R.O.J.E.C.T.S/SampleProgram.txt");

    int i;
    string line, word[3];
    while(fin) {
        getline(fin, line);
        stringstream iss(line);
        i = 0;
        while (iss >> word[i]) {
            word[i];
            i++;
        }
        if(line != "") {
            *LOCTRSize = *LOCTRSize + 1;
        }
        if(i == 3) {
            *SYMTABSize = *SYMTABSize + 1;
        }
    }
    fin.close();
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
    // cout << (decimalString);
    return decimalString;
}

string addZeroes(string s1) {
    int i = 0;
    string s2;
    while(i < 4-s1.length()) {
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
    (flag == "0") ? (limit = 3) : (limit = 5);
    while(s1.length() + disp.length() < limit) {
        s1.append("0");
    }
    return s1.append(disp);
}
