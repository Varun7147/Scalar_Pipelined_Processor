// This is the intellectual property of Jupaka Varun Teja (CS22B050) and Ansh Meshram (CS22B051)
// CS2610 - Lab 8
// Scalar Pipeline Processor



#include <bits/stdc++.h>
#include<fstream>
using namespace std;

// this class represent the Instruction cache of the processor
class Instruction_cache {
public:
    vector <vector <int> > cache ;

    Instruction_cache() {
        cache.resize(128,vector <int> (16,0));
    }
    

    void take_input() {

    ifstream ifile("./input/ICache.txt");

        for (int r=0;r<256;++r) {
            int rem = r%2;
            int row = r/2;
            if (rem == 1) rem =8;

            {
                char c;
                ifile>>c;
                int temp =0;
                if (c >='a') {
                    temp = int(c-'a') + 10;
                }
                else temp = int(c-'0');
                vector <int> bit (4,0);
                int index = 3;
                while (index>=0) {
                    if (temp&1) bit[index]=1;
                    temp>>=1;
                    --index;
                }

                for (int e=0;e<4;++e) {
                    cache[row][rem] = bit[e];
                    ++rem;
                }
            }

            {
                char c;
                ifile>>c;
                int temp =0;
                if (c >='a') {
                    temp = int(c-'a') + 10;
                }
                else temp = int(c-'0');
                vector <int> bit (4,0);
                int index = 3;
                while (index>=0) {
                    if (temp&1) bit[index]=1;
                    temp>>=1;
                    --index;
                }

                for (int e=0;e<4;++e) {
                    cache[row][rem] = bit[e];
                    ++rem;
                }
            }


        }
        ifile.close();
    }

    // to read data from the instruction cache

    vector <int> data_read (int PC) {
        int row =0;
        int column =0;

        if (PC%2==1) column =8;

        row = PC/2;

        vector <int> data(16,0);
        
        for (int r=0;r<16;++r) {
            data[r] = cache[row][column];
            ++column;
        }

        return data;
    }

    void dump () {
        for (auto p : cache) {
            for (auto q :p ) cout << q << ' ';
            cout << endl;
        }
    }
};
