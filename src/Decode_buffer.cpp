// This is the intellectual property of Jupaka Varun Teja (CS22B050) and Ansh Meshram (CS22B051)
// CS2610 - Lab 8
// Scalar Pipeline Processor


#include <iostream>
#include <vector>
#include <map>
using namespace std;

// this is the decode and read buffer

class Decode_and_Read_Buffer {
public:

    int valid;
    int op1;
    int op2;
    int operation;
    int PC;
    int to_write;
    int write_where;
    int value_to_be_stored;

    Decode_and_Read_Buffer() {
        op1=0;
        op2=0;
        operation = -1;
        PC = -1;
        to_write = 0;
        write_where = -1;
        valid = 0;
        value_to_be_stored=0;
    }

    // for debugging purpose
      void print () {
            cout << "Decode-buffer" << endl;
            cout << "valid" << ' ' << valid << endl;
            cout << "op1" << ' ' << op1 << endl;
            cout << "op2 " << op2 << endl;
            cout << "operation " << operation << endl;
            cout << "PC " << PC << endl;
            cout << "to_write " << to_write << endl;
            cout << "write_where " << write_where << endl;
            cout << "value to be stored " << value_to_be_stored << endl;
        }
    
};