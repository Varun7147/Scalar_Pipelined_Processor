// This is the intellectual property of Jupaka Varun Teja (CS22B050) and Ansh Meshram (CS22B051)
// CS2610 - Lab 8
// Scalar Pipeline Processor


#include <bits/stdc++.h>
#include<fstream>
using namespace std;


// this represents the data cache

class Data_cache {
public:
    vector <vector <int> > cache ;

    Data_cache() {
        cache.resize(128,vector <int> (16,0));
        take_input();
    }
    
   
    
// taking input from the required files

     void take_input() {

  ifstream ifile("./input/DCache.txt");

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


    // reading data from the data cache

    vector <int> data_read (int address) {
        int row =0;
        int column =0;

        if (address%2==1) column =8;

        row = address/2;

        vector <int> data(8,0);
        
        for (int r=0;r<8;++r) {
            data[r] = cache[row][column];
            ++column;
        }

        return data;
    }

    // writing data to the data cache

    void data_write ( int address, vector <int>  data) {

        int row =0;
        int column =0;

        if (address%2==1) column =8;

        row = address/2;

        
        for (int r=0;r<8;++r) {
            cache[row][column] = data[r];
            ++column;
        }
        return;
    }

    // dump the contents of the data cache into the required files
    void dump () {

        ofstream ofile("./output/DCache.txt");
   
        for (auto p : cache) {
            int i=0;
            {
                vector <int> up(4,0);
                vector <int> down(4,0);
                
                for (int r=0;r<4;++r) {
                    up[r] = p[i];
                    ++i;
                }
                for (int r=0;r<4;++r) {
                    down[r] = p[i];
                    ++i;
                }

                int one = 0;
                for (int r=0;r<4;++r) {
                    one <<= 1;
                    one += up[r];
                }
                int two = 0;
                for (int r=0;r<4;++r) {
                    two <<= 1;
                    two += down[r];
                }

                char c1,c2;
                if (one >=10) {
                    c1 = char( (one-10) + 'a');
                }
                else {
                    c1 = char( one + '0');
                }

                if (two >= 10) {
                    c2 = char ( (two - 10) + 'a' );
                }
                else {
                    c2 = char( two + '0');
                }
                ofile << c1 << c2 << endl;
            }
            {
                vector <int> up(4,0);
                vector <int> down(4,0);
                
                for (int r=0;r<4;++r) {
                    up[r] = p[i];
                    ++i;
                }
                for (int r=0;r<4;++r) {
                    down[r] = p[i];
                    ++i;
                }

                int one = 0;
                for (int r=0;r<4;++r) {
                    one <<= 1;
                    one += up[r];
                }
                int two = 0;
                for (int r=0;r<4;++r) {
                    two <<= 1;
                    two += down[r];
                }

                char c1,c2;
                if (one >=10) {
                    c1 = char( (one-10) + 'a');
                }
                else {
                    c1 = char( one + '0');
                }

                if (two >= 10) {
                    c2 = char ( (two - 10) + 'a' );
                }
                else {
                    c2 = char( two + '0');
                }
                ofile << c1 << c2 << endl;
    
            }
        }

        ofile.close();
    }

     ~Data_cache() {
        cout << "destructor" << endl;
        cache.clear();
    }

   
};
