#include <bits/stdc++.h>
#include <iostream>
#include <cstring>
#include <fstream>
#include "AssemblerDataStructures.h"
#include "functions.h"

using namespace std;

void Pass1(string File_Path, Instruction optab[], Symbol symtab[], Block block[], int BlockSize, string *programName, string *programLength) {

    ifstream fin;
    ofstream fout;

    fout.open("SampleProgramINTERMEDIATE.txt");

    string line, word[3], size;
    fin.open(File_Path);

    int i, j = 0, k = 0, z = 0, index = 0, b = 0, blockIndex = 0, previous, current, w[BlockSize], temp[BlockSize];
    for(i = 0; i < BlockSize; i++) {
        w[i] = 0;
        temp[i] = 0;
    }

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
        } else {
            continue;
        }

        //WRITING TO INTERMEDIATE FILE
        if(instr != "LTORG") {
            fout<<line<<endl;
        }

        if(instr == "START") {
            block[0].setAddress(addZeroes(operand));
            *programName = label;
            blockIndex = 0;
            previous = temp[blockIndex];
            current = 0;
        } else if(instr == "BASE") {
            continue;
        }

         else if(instr == "USE") {
            temp[blockIndex] = current;
            blockIndex = searchNumberBlock(block, BlockSize, operand);
            previous = temp[blockIndex];
            current = 0;
            continue;
        } else if(instr == "WORD") {
            current = previous + 3;
        } else if(instr == "RESW") {
            current = previous + 3*(HexToDecimal((operand)));
        } else if(instr == "RESB") {
            current = previous + (stoi(operand));
        } else if(instr == "BYTE") {
            if(operand == "X'05'" || operand == "X'F1'") {
                current = previous + 1;
            } else {
                current = previous + (operand.length() - 3);
            }
         } else if(instr == "END") {
            block[blockIndex].getLoctr()[w[blockIndex]] = addZeroes(DecimalToHex(previous));
            w[blockIndex]++;
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
                current = previous + 4;
            } else if(size == "3/4") {
                current = previous + 3;
            } else if(size == "2") {
                current = previous + 2;
            } else {
                current = previous + 1;
            }
        }

        block[blockIndex].getLoctr()[w[blockIndex]] = addZeroes(DecimalToHex(previous));
        w[blockIndex]++;

        if(label != "") {
            if(blockIndex == 0) {
                symtab[j].set(label, block[blockIndex].getLoctr()[w[blockIndex]-1]);
                j++;
            } else {
                symtab[j].set(label, to_string(blockIndex) + "+" + block[blockIndex].getLoctr()[w[blockIndex]-1]);
                j++;
            }

        }

        previous = current;
    }

    //ACCOUNTING FOR THE LAST BLOCK'S ADDRESSE BIAS
    temp[blockIndex] = current;

    fin.close();
    fout.close();

    //ASSIGNING BLOCK ADDRESSES
    for(int a = 1; a < BlockSize; a++) {
        block[a].setAddress(addZeroes(DecimalToHex(temp[a-1] + HexToDecimal(block[a-1].getAddress()))));
    }

    *programLength = addZeroes(DecimalToHex(temp[BlockSize-1] + HexToDecimal(block[BlockSize-2].getAddress())));


    //UPDATE SYMTAB
    for(int t = 0; t < j; t++) {
        size_t found = symtab[t].getLocation().find("+");
        if(found != string::npos) {
            string s1 = symtab[t].getLocation().substr(0, found);
            string s2 = symtab[t].getLocation().substr(found+1, symtab[t].getLocation().length());

            s2 = addZeroes(DecimalToHex(HexToDecimal(s2) + HexToDecimal(block[stoi(s1)].getAddress())));

            symtab[t].set(symtab[t].getLabel(), s2);
        }
    }


}

typedef list< pair<string, string> > objcType;

objcType Pass2(Instruction optab[], Symbol symtab[], Block block[], int BlockSize, Register registers[], int SYMTABSize) {

    list <pair<string, string>> objc;

    string baseCounter, objectCode, N, I, X, B, P, E;
    ifstream fin;
    string line, word[3], size, s1, Location;
    fin.open("SampleProgramINTERMEDIATE.txt");
    int i, j = 0, blockIndex = 0, w[BlockSize];
    for(i = 0; i < BlockSize; i++) {
        w[i] = 0;
    }

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
            operand = "DEFAULT";
        } else {
            continue;
        }

        if(instr == "START" || instr == "RESB" || instr == "RESW" || instr == "END") {
            w[blockIndex]++;
           continue;
       } else if(instr == "USE") {
           blockIndex = searchNumberBlock(block, BlockSize, operand);
           Location = addZeroes(DecimalToHex(HexToDecimal(block[blockIndex].getLoctr()[w[blockIndex]]) + HexToDecimal(block[blockIndex].getAddress())));
           objc.push_back(make_pair("!", Location));
           j++;
           continue;
       } else if(instr == "BASE") {
           baseCounter = searchLocationSymtab(symtab, operand, SYMTABSize);
           continue;
       } else if(instr == "BYTE" || instr == "*") {
            s1 = operand.substr(2, (operand.length() - 3));
            if(operand.at(0) == 'C') {
                Location = addZeroes(DecimalToHex(HexToDecimal(block[blockIndex].getLoctr()[w[blockIndex]]) + HexToDecimal(block[blockIndex].getAddress())));
                objc.push_back(make_pair(asciiToHex(s1), Location));

            } else if(operand.at(0) == 'X') {
                Location = addZeroes(DecimalToHex(HexToDecimal(block[blockIndex].getLoctr()[w[blockIndex]]) + HexToDecimal(block[blockIndex].getAddress())));
                objc.push_back(make_pair(s1, Location));
            }
        } else if(instr == "WORD") {
            Location = addZeroes(DecimalToHex(HexToDecimal(block[blockIndex].getLoctr()[w[blockIndex]]) + HexToDecimal(block[blockIndex].getAddress())));
            objc.push_back(make_pair(addZeroes(DecimalToHex(stoi(operand))), Location));

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
                    Location = addZeroes(DecimalToHex(HexToDecimal(block[blockIndex].getLoctr()[w[blockIndex]]) + HexToDecimal(block[blockIndex].getAddress())));
                    objc.push_back(make_pair(objectCode, Location));


                    w[blockIndex]++; j++;
                    continue;
                }


                //OPERAND CORRECTION
                string operand2 = operand;
                if(operand.at(0) == '#' || operand.at(0) == '@') {
                    operand2 = operand.substr(1, operand.length());
                }

                if(operand2.at(operand2.length()-1) == 'X' && operand2.at(operand2.length()-2) == ',') {
                    X = "1";
                    operand2 = operand2.substr(0, operand2.length()-2);
                } else {
                    X = "0";
                }

                // B, P Condition Flags
                string tempDisp0 = searchLocationSymtab(symtab, operand2, SYMTABSize);
                if(tempDisp0 != "NOT FOUND") {
                    int tempDisp = HexToDecimal(searchLocationSymtab(symtab, operand2, SYMTABSize)) - HexToDecimal(block[blockIndex].getLoctr()[w[blockIndex]+1]);
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

                if(operand2.at(0) == '=') {
                    P = "1";
                    B = "0";
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
                            disp = DecimalToHex(HexToDecimal(searchLocationSymtab(symtab, operand2, SYMTABSize)) - HexToDecimal(block[blockIndex].getLoctr()[w[blockIndex]+1]));
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
                            disp = DecimalToHex(HexToDecimal(searchLocationSymtab(symtab, operand2, SYMTABSize)) - HexToDecimal(block[blockIndex].getLoctr()[w[blockIndex]+1]));
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

            Location = addZeroes(DecimalToHex(HexToDecimal(block[blockIndex].getLoctr()[w[blockIndex]]) + HexToDecimal(block[blockIndex].getAddress())));
            objc.push_back(make_pair(objectCode, Location));

        }
        w[blockIndex]++; j++;
    }
        fin.close();

    return objc;

}

void writeObjectProgram(objcType objc, string programName, string programLength, string programAddress) {
    ofstream fout;
    fout.open("ObjectProgram.txt");

    fout<<"H"<<"-"<<spaces(programName)<<"-"<<programAddress<<"-"<<programLength<<endl;

    objcType::iterator iter = objc.begin(), previous = objc.begin();

    int codeLength = 0, flag = 0;
    string code, loc, codeLine;
    for(; iter != objc.end(); previous=iter, ++iter) {
       code = (*iter).first;
       if(code != "!") {
           if(code.length() == 9) {
               code = code.substr(0, 3) + code.substr(4, 9);
           }

           if(codeLength == 0) {
               if(iter != objc.begin()) {
                   codeLength += ((*previous).first).length()/2;
               }
               loc = (*previous).second;
               fout<<"T"<<"-";
           }
           codeLength += code.length()/2;

           if(codeLength <= 30) {
               if(iter == objc.begin()) {
                   codeLine.append(code);
               } else {
                   codeLine.append("-" + code);
               }
           } else {

               fout<<loc<<"-"<<Zeroes(DecimalToHex(codeLength-(code.length()/2)))<<"-"<<codeLine<<endl;
               codeLength = 0;
               codeLine = code;
           }
       } else {
           do {
               iter++;
               code = (*iter).first;
           } while(code == "!");

           fout<<loc<<"-"<<Zeroes(DecimalToHex(codeLength))<<"-"<<codeLine<<endl;
           codeLength = 0;
           codeLine = code;
           loc = (*iter).second;

           objcType::iterator iter2 = iter;
           iter2++;
           string code2 = (*iter2).first;
           if(code2 == "!" || code2 == "") {
               fout<<"T"<<"-";
               codeLength = code.length()/2;
          }
       }
    }

    fout<<loc<<"-"<<Zeroes(DecimalToHex(codeLength))<<"-"<<codeLine<<endl;

    fout<<"E"<<"-"<<programAddress;
}

int main() {
    string File_Path = "SampleProgram1.txt";

    int SYMTABSize = 0, litTabSize = 0, BlockSize = 1;
    getSize(File_Path, &SYMTABSize, &litTabSize, &BlockSize);

    Symbol symtab[SYMTABSize];

    Block block[BlockSize];
    int* blockLoctrSize = BLOCK(File_Path, block, BlockSize);

    Instruction optab[59];
    OPTAB(optab);

    Register registers[9];
    REGISTER(registers);

    string programName, programLength;
    Pass1(File_Path, optab, symtab, block, BlockSize, &programName, &programLength);

    string programAddress = block[0].getAddress();

    objcType objc;

    objc = Pass2(optab, symtab, block, BlockSize, registers, SYMTABSize);

    writeObjectProgram(objc, programName, programLength, programAddress);

    return 0;
}
