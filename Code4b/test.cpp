#include <cmath>
#include <fstream>
#include <vector>
#include <iostream>
#include <arpa/inet.h>

#include "GreyScale.h"
#include "unit.h"
int readBit(std::istream& s, char& byte, int& bitPos) {
    if(bitPos == 7) {
        byte = s.get();
    }
    int bitVal = byte & (1 << bitPos);
    bitVal = bitVal >> bitPos;
    bitPos --;
    bitPos = bitPos == -1 ? 7 : bitPos;
    
    return bitVal;
}

// 7 -> 0
int writeBit(std::ostream& o, char& byte, int bitVal, int& bitPos) {
    byte = byte | (bitVal<< bitPos);
    if(bitPos == 0) {
        o << byte;
        byte = 0b00000000; //reset
    }
    bitPos --;
    bitPos = bitPos == -1 ? 7 : bitPos;
    return 0;
}

int writeBitFlush(std::ostream& o, char& byte, int bitVal, int& bitPos) {
    writeBit(o, byte, bitVal, bitPos);
    //zero padding
    while(bitPos != 7) {
        writeBit(o, byte, 0, bitPos);
    }
    return 0;
}

int main () {
    //     std::cout<< "bp " <<bitPos<<" " <<  (int)byte << std::endl;
    // for(int i=7; i>=0; i--) {
    //     int bitVal = byte & (1 << i);
    //     bitVal = bitVal >> i;
    //     std::cout<<  bitVal;
    // }
    // std::cout <<std::endl;

    // std::ofstream file("record.txt");
    // u_int32_t num1 =66;
    // std::cout<<  num1 <<std::endl;
    // num1 = htonl(num1);
    // std::cout<<  num1 <<std::endl;
    // file.close();
    // std::ifstream file2("record.txt");
    // file2 >> num1;
    // std::cout<<  num1 <<std::endl;
    // file2.close();
    // num1 = ntohl(num1);
    // std::cout<< num1 <<std::endl;
    char byte = 0b00010111;
    for(int i=0; i<=7; i++) {
        std::cout<< ((byte >> (7-i)) & 1);

    }
    std::cout <<std::endl;




    return 0;
}
