#include <bits/stdc++.h>
#include <iostream>
#include <cstring>
#include <fstream>
#include "AssemblerDataStructures.h"
#include "functions.h"

using namespace std;

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
        k++;
    }
    fin.close();

}

void Pass2(Instruction optab[], Symbol symtab[], string locationCounter[], Register registers[], string opcode[], int SYMTABSize) {

    string baseCounter, objectCode, N, I, X, B, P, E;
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
        } else {
            continue;
        }
        cout<<"k = "<<k<<" j = "<<j<<label<<" "<<instr<<" "<<operand<<endl;
        if(instr == "START" || instr == "RESB" || instr == "RESW" || instr == "END") {
            k++;
           continue;
       } else if(instr == "BASE") {
           baseCounter = searchLocationSymtab(symtab, operand, SYMTABSize);
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

            if(size == "3/4") {

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

                // B, P Condition Flags
                string tempDisp0 = searchLocationSymtab(symtab, operand2, SYMTABSize);
                if(tempDisp0 != "NOT FOUND") {
                    int tempDisp = HexToDecimal(searchLocationSymtab(symtab, operand2, SYMTABSize)) - HexToDecimal(locationCounter[k+1]);
                    if(tempDisp > 4095 || tempDisp < -4095) {
                        B = "1";
                        P = "0";
                    } else {
                        P = "1";
                        B = "0";
                    }
                }

                if(operand.at(0) == '#') {
                    N = "0"; I = "1";
                    B = "0";
                    P = "0";
                    if(searchLocationSymtab(symtab, operand2, SYMTABSize) != "NOT FOUND") {
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
                } else {
                    E = "0";
                }

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

                if(E == "0") {
                    string disp;
                    if(N == "1") {
                        if(B == "0" && P == "1") {
                            disp = DecimalToHex(HexToDecimal(searchLocationSymtab(symtab, operand2, SYMTABSize)) - HexToDecimal(locationCounter[k+1]));
                            objectCode = BinaryToHex(objectCode);
                            disp = addZeroesDisplacement(disp, E);
                            if(disp.length() > 3) {
                                disp = disp.substr(5, disp.length());
                            }
                            objectCode = objectCode.append(disp);
                        } else if(B =="1" && P == "0"){
                            disp = DecimalToHex(HexToDecimal(searchLocationSymtab(symtab, operand2, SYMTABSize)) - HexToDecimal(baseCounter));
                            objectCode = BinaryToHex(objectCode);
                            disp = addZeroesDisplacement(disp, E);
                            objectCode = objectCode.append(disp);
                        }
                    } else if(N == "0" && I == "1") {
                        disp = operand2;
                        if(searchLocationSymtab(symtab, disp, SYMTABSize) != "NOT FOUND") {
                            disp = searchLocationSymtab(symtab, disp, SYMTABSize);
                        }
                        if(P == "1") {
                            disp = DecimalToHex(HexToDecimal(searchLocationSymtab(symtab, operand2, SYMTABSize)) - HexToDecimal(locationCounter[k+1]));
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
                    if(searchLocationSymtab(symtab, operand3, SYMTABSize) != "NOT FOUND") {
                        addr = searchLocationSymtab(symtab, operand3, SYMTABSize);
                        objectCode = BinaryToHex(objectCode);
                        addr = addZeroesDisplacement(addr, E);
                    } else {
                        addr = addZeroesDisplacement(DecimalToHex(stoi(operand3)), E);
                        objectCode = BinaryToHex(objectCode);
                    }
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
}

int main() {
    int LOCTRSize = 0, SYMTABSize = 0;
    getSize(&LOCTRSize, &SYMTABSize);

    int loc[LOCTRSize];
    string locationCounter[LOCTRSize], opcode[100];
    Symbol symtab[SYMTABSize];

    Instruction optab[59];
    OPTAB(optab);

    Register registers[9];
    REGISTER(registers);

    Pass1(optab, symtab, loc, locationCounter);

    Pass2(optab, symtab, locationCounter, registers, opcode, SYMTABSize);

    int i = 0;
    while(opcode[i] != "") {
        cout<<opcode[i]<<endl;
        i++;
    }
    return 0;
}
