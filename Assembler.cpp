#include <bits/stdc++.h>
#include <iostream>
#include <cstring>
#include <fstream>
// #include <string.h>
#include "OPTAB.h"

using namespace std;

int MAX_SIZE = 100;

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

string searchRegisters(Register registers[], string name) {
    int i = 0;
    while(name != registers[i].getname()) {
        i++;
    }
    return registers[i].getopcode();
}

string searchLocationSymtab(Symbol symtab[], string label) {
    int i = 0, SYMTABSize = 0, temp = 0;
    getSize(&temp, &SYMTABSize);
    while(i < SYMTABSize) {
        if(label == symtab[i].getLabel()) {
            return symtab[i].getLocation();
        }
        i++;
    }
    return "NOT FOUND";
    // if(label == symtab[i].getLabel()) {
        // cout<<"helooo";
    // }// else {
        // cout<<"HELOOOO@";
        // return symtab[i].getLocation();
    // }
}

string DecimalToHex(int loc) {
    string hexString;
    stringstream ss;

    ss << hex << loc;
    hexString = ss.str();
    // cout << res << endl;

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

void Pass1(Instruction optab[], Symbol symtab[], int loc[], string locationCounter[]) {
    ifstream fin;
    string line, word[3], size;
    fin.open("/home/rahul/Documents/P.R.O.J.E.C.T.S/SampleProgram.txt");
    int i, j = 0, k = 0;
    while(fin) {
        getline(fin, line);
        stringstream iss(line);
        i = 0;
        while (iss >> word[i]) {
            word[i];
            i++;
        }

        string label, instr, operand;
        if(i == 3) {
            label = word[0];
            instr = word[1];
            operand = word[2];
        } else if(i == 2) {
            instr = word[0];
            operand = word[1];
        } else if(i == 1) {
            instr = word[0];
        } else {
            continue;
        }
        if(instr == "START") {
            // cout<<"hello";
            loc[k] = 0;
            // cout<<loc[k]<<endl;
            locationCounter[k] = DecimalToHex(loc[k]);
            locationCounter[k] = addZeroes(DecimalToHex(loc[k]));
            loc[k+1] = 0;
            k++;
        } else if(instr == "BASE") {
            continue;
        } else if(instr == "WORD") {
            loc[k] = loc[k-1] + 3;
        } else if(instr == "RESW") {
            loc[k] = loc[k-1] + 3*(HexToDecimal((operand)));
        } else if(instr == "RESB") {
            loc[k] = loc[k-1] + (stoi(operand));
        } else if(instr == "BYTE") {
            if(operand == "X'05'" || operand == "X'F1'") {
                loc[k] = loc[k-1] + 1;
            } else {
                loc[k] = loc[k-1] + (operand.length() - 3);
            }
        } else if(instr == "END") {
            break;
        } else {

            //getting rid of '+' in format 4 instructions
            string instr2;
            if(instr.at(0) == '+') {
                instr2 = instr.substr(1, instr.length());
            } else {
                instr2 = instr;
            }

            size = searchFormatOPTAB(optab, instr2);
            if(size == "3/4" && instr.at(0) == '+') {
                loc[k] = loc[k-1] + 4;
            } else if(size == "3/4") {
                loc[k] = loc[k-1] + 3;
            } else if(size == "2") {
                loc[k] = loc[k-1] + 2;
            } else {
                loc[k] = loc[k-1] + 1;
            }
        }

        locationCounter[k] = DecimalToHex(loc[k]);
        locationCounter[k] = addZeroes(DecimalToHex(loc[k]));
        if(label != "") {
            symtab[j].set(label, locationCounter[k-1]);
            j++;
        }

        // cout<<k-1<<" "<<loc[k-1]<<" "<<locationCounter[k-1]<<" "<<instr.at(0)<<" "<<instr<<" "<<operand<<" "<<endl;;

        // cout<<locationCounter[0]
        // locationCounter[k] = addZeroes(DecimalToHex(loc[k]));
        k++;
    }
    fin.close();

}

string asciiToHex(string s1) {
    // char *s = s1;
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

// string addZeroesRegister(string op) {
//     string s1;
//     while(s1.length() + op.length() < 4) {
//         s1.append("0");
//     }
//     return s1.append(op);
// }

void Pass2(Instruction optab[], Symbol symtab[], string locationCounter[], Register registers[]) {

    string opcode[100], baseCounter, objectCode, N, I, X, B, P, E;
    // cout<<"Heloo";
    ifstream fin;
    string line, word[3], size, s1;
    fin.open("/home/rahul/Documents/P.R.O.J.E.C.T.S/SampleProgram.txt");
    int i, j = 0, k = 0;
    while(fin) {
        getline(fin, line);
        if(line == "") {
            continue;
        }
        stringstream iss(line);
        i = 0;
        while (iss >> word[i]) {
            word[i];
            i++;
        }

        string label, instr, operand;
        if(i == 3) {
            label = word[0];
            instr = word[1];
            operand = word[2];
        } else if(i == 2) {
            instr = word[0];
            operand = word[1];
        } else if(i == 1) {
            instr = word[0];
            // operand = "";
        } else {
            continue;
        }
        cout<<"k = "<<k<<" j = "<<j<<label<<" "<<instr<<" "<<operand<<endl;
        if(instr == "START" || instr == "RESB" || instr == "RESW" || instr == "END") {
            k++;
           continue;
       } else if(instr == "BASE") {
           baseCounter = searchLocationSymtab(symtab, operand);
           continue;
       } else if(instr == "BYTE") {
            s1 = operand.substr(2, (operand.length() - 3));
            if(operand.at(0) == 'C') {
                opcode[j] = asciiToHex(s1);
            } else if(operand.at(0) == 'X') {
                opcode[j] = s1;
            }
        } else if(instr == "WORD") {
            opcode[j] = addZeroes(DecimalToHex(stoi(operand)));
        } else {

            //INSTRUCTION CORRECTION
            string instr2 = instr;
            if(instr.at(0) == '+') {
                instr2 = instr.substr(1, instr.length());
            }

            string size = searchFormatOPTAB(optab, instr2);
            // objectCode = searchOPTAB(optab, instr2);

            if(size == "3/4") {
                // objectCode = objectCode.substr(0, objectCode.length() - 3);

                if(instr == "RSUB") {
                    objectCode = "4f0000";
                    opcode[j] = objectCode;
                    k++; j++;
                    continue;
                }


                //OPERAND CORRECTION
                string operand2 = operand;
                if(operand.at(0) == '#' || operand.at(0) == '@') {
                    operand2 = operand.substr(1, operand.length());
                }
                cout<<operand2;

                if(operand2.at(operand2.length()-1) == 'X' && operand2.at(operand2.length()-2) == ',') {
                    X = "1";
                    operand2 = operand2.substr(0, operand2.length()-2);
                } else {
                    X = "0";
                }
       //
                // B, P Condition Flags
                string tempDisp0 = searchLocationSymtab(symtab, operand2);
                if(tempDisp0 != "NOT FOUND") {
                    int tempDisp = HexToDecimal(searchLocationSymtab(symtab, operand2)) - HexToDecimal(locationCounter[k+1]);
                    if(tempDisp > 4095 || tempDisp < -4095) {
                        B = "1";
                        P = "0";
                    } else {
                        P = "1";
                        B = "0";
                    }
                }

                // B = "1"; P = "0";
                if(operand.at(0) == '#') {
                    N = "0"; I = "1";
                    B = "0";
                    P = "0";
                    if(searchLocationSymtab(symtab, operand2) != "NOT FOUND") {
                        P = "1";
                    }
                } else if(operand.at(0) == '@') {
                    N = "1"; I = "0";
                } else {
                    N = "1"; I = "1";
                }

                if(instr.at(0) == '+') {
                    E = "1";
                    P = "0";
                    B = "0";
                    // instr = instr.substr(1, instr.length());
                } else {
                    E = "0";
                }
       //
                objectCode = searchOPTAB(optab, instr2);
                objectCode = HexToBinary(objectCode);
                objectCode = objectCode.substr(24, 29);
                objectCode = objectCode.substr(0, 6);

                //Appending flags to the intermediate object code in binary
                objectCode = objectCode.append(N);
                objectCode = objectCode.append(I);
                objectCode = objectCode.append(X);
                objectCode = objectCode.append(B);
                objectCode = objectCode.append(P);
                objectCode = objectCode.append(E);

                // cout<<"k = "<<k<<" N = "<<N<<" I = "<<I<<" X = "<<X<<" B = "<<B<<" P = "<<P<<" E = "<<E<<endl;
                // cout<<E<<endl;
                if(E == "0") {
                    string disp;
                    if(N == "1") {
                        if(B == "0" && P == "1") {
                            disp = DecimalToHex(HexToDecimal(searchLocationSymtab(symtab, operand2)) - HexToDecimal(locationCounter[k+1]));
                            objectCode = BinaryToHex(objectCode);
                            disp = addZeroesDisplacement(disp, E);
                            if(disp.length() > 3) {
                                disp = disp.substr(5, disp.length());
                            }
                            objectCode = objectCode.append(disp);
                        } else if(B =="1" && P == "0"){                                                   //else if(B =="1" && P == "0")
                            disp = DecimalToHex(HexToDecimal(searchLocationSymtab(symtab, operand2)) - HexToDecimal(baseCounter));
                            objectCode = BinaryToHex(objectCode);
                            disp = addZeroesDisplacement(disp, E);
                            objectCode = objectCode.append(disp);
                        }
                    } else if(N == "0" && I == "1") {                                                        //else if(N == "0" && I == "1")
                        disp = operand2;
                        if(searchLocationSymtab(symtab, disp) != "NOT FOUND") {
                            disp = searchLocationSymtab(symtab, disp);
                        }
                        if(P == "1") {
                            disp = DecimalToHex(HexToDecimal(searchLocationSymtab(symtab, operand2)) - HexToDecimal(locationCounter[k+1]));
                            objectCode = BinaryToHex(objectCode);
                            disp = addZeroesDisplacement(disp, E);
                            if(disp.length() > 3) {
                                disp = disp.substr(5, disp.length());
                            }
                        } else {
                            disp = addZeroesDisplacement(DecimalToHex(stoi(disp)), E);
                            objectCode = BinaryToHex(objectCode);
                        }
                        objectCode = objectCode.append(disp);
                     }
                } else if(E == "1"){
                    string operand3 = operand2;
                    string addr;
                    if(searchLocationSymtab(symtab, operand3) != "NOT FOUND") {
                        addr = searchLocationSymtab(symtab, operand3);
                        objectCode = BinaryToHex(objectCode);
                        addr = addZeroesDisplacement(addr, E);
                    } else {
                        addr = addZeroesDisplacement(DecimalToHex(stoi(operand3)), E);
                        objectCode = BinaryToHex(objectCode);
                    }
                    // searchLocationSymtab(symtab, operand3);
                    objectCode = objectCode.append(addr);
                }

            } else if(size == "2") {
                objectCode = searchOPTAB(optab, instr);

                string op1, op2;
                op1 = searchRegisters(registers, operand.substr(0, 1));
                op2 = "0";

                if(operand.length() == 3) {
                    op2 = searchRegisters(registers, operand.substr(2, operand.length()));
                }

                objectCode = objectCode.append(op1);
                objectCode = objectCode.append(op2);
            } else {
                objectCode = searchOPTAB(optab, instr);
            }
            opcode[j] = objectCode;
        }
        k++; j++;
    }
        fin.close();
        i = 0;
        while(opcode[i] != "") {
            cout<<opcode[i]<<endl;
            i++;
        }
        cout<<baseCounter;
}


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

int main() {
    int LOCTRSize = 0, SYMTABSize = 0;
    getSize(&LOCTRSize, &SYMTABSize);

    int loc[LOCTRSize];
    string locationCounter[LOCTRSize];
    Symbol symtab[SYMTABSize];

    Instruction optab[59];
    OPTAB(optab);

    Register registers[9];
    REGISTER(registers);

    Pass1(optab, symtab, loc, locationCounter);


    // cout<<(HexToDecimal(searchLocationSymtab(symtab, "LENGTH")) - HexToDecimal("001059"));

    // cout<<LOCTRSize<<" "<<SYMTABSize<<endl;
    // int i = 0;
    // while(i < LOCTRSize) {
    //     cout<<locationCounter[i]<<endl;
    //     i++;
    // }
    int i = 0;
    while(i < SYMTABSize) {
        cout<<symtab[i].getLocation()<<" "<<symtab[i].getLabel()<<endl;
        i++;
    }
    // while(i < 9) {
    //     cout<<registers[i].getname()<<" "<<registers[i].getopcode()<<endl;
    //     i++;
    // }
    // i = 0;
    // while(i < 59) {
    //     cout<<optab[i].getopname()<<" "<<optab[i].getopcode()<<" "<<i<<endl;
    //     i++;
    // }
    // string disp = "#0", E = "0";
    // disp = addZeroesDisplacement(DecimalToHex(stoi(disp)), E);
    // cout<<disp;
    Pass2(optab, symtab, locationCounter, registers);
    // string B, P;
    // string tempDisp0 = searchLocationSymtab(symtab, "LENGTH2");
    // cout<<tempDisp0;
    // if(tempDisp0 != "NOT FOUND") {
    //     int tempDisp = HexToDecimal(searchLocationSymtab(symtab, "0")) - HexToDecimal("001056");
    //     if(tempDisp > 4095 || tempDisp < -4095) {
    //         B = "1";
    //         P = "0";
    //     } else {
    //         P = "1";
    //         B = "0";
    //     }
    // } else {
    //     cout<<"ERROR"<<endl;
    // }
    // cout<<"B = "<<B<<" P = "<<P<<endl;
    // cout<<stoi("4096") + 1;

    // cout<<asciiToHex("EOF");
    // Pass1(optab, symtab, loc, locationCounter);
    // Pass2(optab, symtab, locationCounter, registers);
    // cout<<addZeroes(DecimalToHex(stoi("4096")));
    // // cout<<BinaryToHex("11001110");
    // string N = "0", I = "1", X = "0", B = "0", P = "0", E = "1";
    // // //
    // string objectCode = searchOPTAB(optab, "LDT");
    // // // cout<<objectCode<<endl;
    // objectCode = HexToBinary(objectCode);
    // objectCode = objectCode.substr(24, 29);
    // objectCode = objectCode.substr(0, 6);
    // objectCode = objectCode.append(N);
    // objectCode = objectCode.append(X);
    // objectCode = objectCode.append(B);
    // objectCode = objectCode.append(P);
    // objectCode = objectCode.append(E);
    // // // cout<<objectCode;
    // // string disp = DecimalToHex(HexToDecimal("00001a") - HexToDecimal("000013"));
    // objectCode = BinaryToHex(objectCode);
    // string word = "#4096";
    // string disp = word.substr(1, word.length());
    // disp = (DecimalToHex(stoi(disp)));
    // disp = addZeroesDisplacement(disp, "1");
    // // if(disp.length() > 3) {
    //     // disp = disp.substr(5, disp.length());
    // // }
    // cout<<disp<<endl;
    // // // cout<<DecimalToHex(-10752);
    // // // cout<<HexToDecimal("000600") - HexToDecimal("003000");
    // objectCode = objectCode.append(disp);
    // cout<<objectCode<<endl;
    // objectCode = BinaryToHex(objectCode);
    // cout<<objectCode;
    // cout<<addZeroes(DecimalToHex(4096))<<endl;
    // string op;
    // cout<<addZeroesRegister(op);
    // cout<<disp;
    // cout<<"Heloo";
    // string opcode[100], baseCounter, objectCode, N, I, X, B, P, E;
    //
    // ifstream fin;
    // string line, word[3], size, s1;
    // fin.open("/home/rahul/Documents/P.R.O.J.E.C.T.S/SampleProgram.txt");
    // int i, j = 0, k = 0;
    // while(fin) {
    //     getline(fin, line);
    //     if(line == "") {
    //         break;
    //     }
    //     stringstream iss(line);
    //     i = 0;
    //     while (iss >> word[i]) {
    //         word[i];
    //         i++;
    //     }
    //
    //     string label, instr, operand;
    //     if(i == 3) {
    //         label = word[0];
    //         instr = word[1];
    //         operand = word[2];
    //     } else if(i == 2) {
    //         instr = word[0];
    //         operand = word[1];
    //     } else {
    //         instr = word[0];
    //     }
    //
    //    //  if(instr == "START" || instr == "RESB" || instr == "RESW" || instr == "BASE") {
    //    //     continue;
    //    // } else if(instr == "BYTE") {
    //    //      s1 = operand.substr(2, (operand.length() - 1));
    //    //      if(operand.at(0) == 'C') {
    //    //          opcode[j] = asciiToHex(s1);
    //    //      } else if(operand.at(0) == 'X') {
    //    //          opcode[j] = s1;
    //    //      }
    //    //  } else if(instr == "WORD") {
    //    //      // opcode[j] = addZeroes(DecimalToHex(stoi(operand)));
    //    //  } else {
    //    //      string size = searchFormatOPTAB(optab, instr);
    //    //      if(size == "3/4") {
    //    //          // objectCode = objectCode.substr(0, objectCode.length() - 3);
    //    //          if(operand.at(0) == '#') {
    //    //              N = "0"; I = "1";
    //    //          } else if(operand.at(0) == '@') {
    //    //              N = "1"; I = "0";
    //    //          } else {
    //    //              N = "1"; I = "1";
    //    //          }
    //    //
    //    //          if(operand.at(operand.length()-1) == 'X' && operand.at(operand.length()-2) == ',') {
    //    //              X = "1";
    //    //          } else {
    //    //              X = "0";
    //    //          }
    //    //
    //    //          //B, P Condition Flags
    //    //          B = "0"; P = "1";
    //    //
    //    //          if(instr.at(0) == '+') {
    //    //              E = "1";
    //    //              instr = instr.substr(1, instr.length());
    //    //          } else {
    //    //              E = "0";
    //    //          }
    //    //
    //    //          objectCode = searchOPTAB(optab, instr);
    //    //          objectCode = HexToBinary(objectCode);
    //    //          objectCode = objectCode.substr(24, 29);
    //    //          objectCode = objectCode.substr(0, 6);
    //    //
    //    //          //Appending flags to the intermediate object code in binary
    //    //          objectCode = objectCode.append(N);
    //    //          objectCode = objectCode.append(I);
    //    //          objectCode = objectCode.append(X);
    //    //          objectCode = objectCode.append(B);
    //    //          objectCode = objectCode.append(P);
    //    //          objectCode = objectCode.append(E);
    //    //
    //    //          if(E == "0") {
    //    //              string disp;
    //    //              if(N == "1" && (I == "1" || I == "0")) {
    //    //                  if(B == "0" && P == "1") {
    //    //                      disp = DecimalToHex(HexToDecimal(searchLocationSymtab(symtab, operand)) - HexToDecimal(locationCounter[k+1]));
    //    //                      objectCode = BinaryToHex(objectCode);
    //    //                      addZeroesDisplacement(disp, E);
    //    //                      if(disp.length() > 3) {
    //    //                          disp = disp.substr(5, disp.length());
    //    //                      }
    //    //                      objectCode = objectCode.append(disp);
    //    //                  } else if(B == "1" && P == "0") {
    //    //                      disp = DecimalToHex(HexToDecimal(searchLocationSymtab(symtab, operand)) - HexToDecimal(baseCounter));
    //    //                      objectCode = BinaryToHex(objectCode);
    //    //                      addZeroesDisplacement(disp, E);
    //    //                      objectCode = objectCode.append(disp);
    //    //                  } else {
    //    //                      disp = searchLocationSymtab(symtab, operand);
    //    //                      objectCode = BinaryToHex(objectCode);
    //    //                      addZeroesDisplacement(disp, E);
    //    //                      objectCode = objectCode.append(disp);
    //    //                  }
    //    //              } else if(N == "0" && I == "1") {
    //    //                  disp = operand.substr(1, operand.length());
    //    //                  // disp = addZeroesDisplacement(DecimalToHex(stoi(disp)), E);
    //    //                  objectCode = BinaryToHex(objectCode);
    //    //                  objectCode = objectCode.append(disp);
    //    //              }
    //    //          } else if(E == "1") {
    //    //              string addr = searchLocationSymtab(symtab, operand);
    //    //              objectCode = BinaryToHex(objectCode);
    //    //              addr = addZeroesDisplacement(addr, E);
    //    //              objectCode = objectCode.append(addr);
    //    //          }
    //    //      } else if(size == "2") {
    //    //          objectCode = searchOPTAB(optab, instr);
    //    //          objectCode = HexToBinary(objectCode);
    //    //          objectCode = objectCode.substr(24, objectCode.length());
    //    //
    //    //          string op1, op2;
    //    //          op1 = searchRegisters(registers, operand);
    //    //          addZeroesRegister(op1);
    //    //
    //    //          if(operand.length() == 3) {
    //    //              op2 = searchRegisters(registers, operand.substr(2, operand.length()));
    //    //          }
    //    //
    //    //          addZeroesRegister(op2);
    //    //          objectCode = objectCode.append(op1);
    //    //          objectCode = objectCode.append(op2);
    //    //      } else {
    //    //          objectCode = "4f0000";
    //    //      }
    //    //      opcode[j] = objectCode;
    //    //  }
    //     k++;
    //
    // }
    // fin.close();

    return 0;
}
