// This is the intellectual property of Jupaka Varun Teja (CS22B050) and Ansh Meshram (CS22B051)
// CS2610 - Lab 8
// Scalar Pipeline Processor



#include <iostream>
#include <vector>
using namespace std;

// this is the ALU buffer class

class ALU_Buffer {
    public:
        // valid to implement stalling
        int valid; 
        // output of the ALU
        vector <int> ALU_output;

        // basic information

        int operation ;
        int rd;
        int to_write_WB;
        vector <int> write_value_WB;
        int store_value;

        // creating the ALU class
        ALU_Buffer() {
            valid =0;
            ALU_output.resize(8,0);
            operation =0;
            rd =0;
            to_write_WB = 0;
            write_value_WB.resize(8,0);
            store_value=0;
        }
        // printing the ALU class for debugging purpose
        void print () {
            cout << "ALU-buffer" << endl;
            cout << "valid" << ' ' << valid << endl;
            cout << "ALU_Output" << ' ';
            for (auto p  : ALU_output) cout << p << ' '; 
            cout << endl;
            cout << "operation" << ' ' << operation << endl;
            cout << "rd" << ' ' << rd << endl;
            cout << "to_write_WB" << ' ' << to_write_WB << endl;
            cout << "write_value_WB" << ' ';
            for (auto p : write_value_WB) cout << p << ' ' ;  
            cout << endl;
            cout << "value to be stored " << store_value << endl;
        }
        
};
