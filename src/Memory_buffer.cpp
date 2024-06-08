// This is the intellectual property of Jupaka Varun Teja (CS22B050) and Ansh Meshram (CS22B051)
// CS2610 - Lab 8
// Scalar Pipeline Processor




#include <iostream>
#include <vector>
using namespace std;

// this is memory buffer class

class Memory_Buffer {
public:
    int valid ;
    int rd;
    vector <int> ALU_output;


    Memory_Buffer () {
        valid =0;
        rd = -1;
        ALU_output.resize(8,0);
    }

    // for debugging purpose
    void print () {
            cout << "memory-buffer" << endl;
            cout << "valid" << ' ' << valid << endl;
            cout << "rd " << rd << endl;
            cout << "ALU_output " ;
            for (auto p  : ALU_output) cout << p << ' ';
            cout << endl;
        }
};