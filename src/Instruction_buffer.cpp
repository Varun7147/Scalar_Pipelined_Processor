// This is the intellectual property of Jupaka Varun Teja (CS22B050) and Ansh Meshram (CS22B051)
// CS2610 - Lab 8
// Scalar Pipeline Processor



#include <iostream>
#include <vector>
#include <map>
using namespace std;


// this stimulates the instruction buffer of the processor

class Instruction_Buffer {
public:
    int valid;
    vector <int> instruction;

    Instruction_Buffer() {
        valid = 0;
        instruction.resize(16,0);
        
    }

        // for debugging purpose
      void print () {
            cout << "Instruction-buffer" << endl;
            cout << "valid" << ' ' << valid<<endl;
            cout << "instruction " ;
            for (auto p : instruction ) cout << p << ' ' ;
            cout << endl;
        }
};
