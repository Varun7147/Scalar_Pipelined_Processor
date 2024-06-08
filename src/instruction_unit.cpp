// This is the intellectual property of Jupaka Varun Teja (CS22B050) and Ansh Meshram (CS22B051)
// CS2610 - Lab 8
// Scalar Pipeline Processor



#include <bits/stdc++.h>
using namespace std;

#include "Icache.cpp"

// this is the class for instruction buffer

class Instruction_Unit  {

public:
    Instruction_cache insc;
    int PC;
    vector <int> InsReg;
    
    Instruction_Unit() {
        PC =0;
        InsReg.resize(16,0);
        insc = Instruction_cache();
        insc.take_input();
        //insc.dump();
    }

    // this fetches the next instruction and store the value in the InsReg which
    // is the instruction register 
    void fetchin () {
        InsReg = insc.data_read(PC);
        PC +=2;
    }

};

