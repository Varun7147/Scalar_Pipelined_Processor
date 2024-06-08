// This is the intellectual property of Jupaka Varun Teja (CS22B050) and Ansh Meshram (CS22B051)
// CS2610 - Lab 8
// Scalar Pipeline Processor


#include <iostream>
#include <vector>
using namespace std;

// class to decode instruction
class CU {
    public:
        vector <int> CU_intruction;
        int operation;
        int rs1;
        int rs2;
        int target_register_address;
        int ishalt ;

        // creating CU object
        CU() {
            CU_intruction.resize(16,0);
            operation = 0;
            rs1 = 0;
            rs2 = 0;
            target_register_address = 0;
            ishalt =0;
        }


        int convert(vector<int>v){
            int i=1;
            int out=0;
            if(v[0]==0){
                for(int j=7;j>=0;j--){
                    out+=v[j]*i;
                    i*=2;
                }
                return out;
            }else{
                int f=7;
                while(v[f]==0){f--;}
                v[f]=0;
                for(int h=f+1;h<8;h++){
                    v[h]=1;
                }
                for(int j=0;j<8;j++){
                    if(v[j]==1)v[j]=0;
                    else v[j]=1;
                }
                for(int j=7;j>=0;j--){
                    out+=v[j]*i;
                    i*=2;
                }
                return (-1)*out;
            }
    }
        //  main decoding happens here
        void decode() {
            operation = 0;
            rs1 = 0;
            rs2 = 0;
            target_register_address = 0;

            int i=0;
            for (int r=0;r<4;++r) {
                operation <<= 1;
                operation += CU_intruction[i];
                ++i;
            }

            // handling decode based on the operation


            if (operation != 3 && operation !=7 && operation <=7 ) {
                for (int r=0;r<4;++r) {
                    target_register_address <<= 1;
                    target_register_address += CU_intruction[i];
                    ++i;
                }

                for (int r=0;r<4;++r) {
                    rs1 <<= 1;
                    rs1 += CU_intruction[i];
                    ++i;
                }
                for (int r=0;r<4;++r) {
                    rs2 <<= 1;
                    rs2 += CU_intruction[i];
                    ++i;
                }
            }

            if (operation == 10) {
                for (int r=0;r<4;++r) {
                    target_register_address <<= 1;
                    target_register_address += CU_intruction[i];
                    ++i;
                }

                for (int r=0;r<4;++r) {
                    rs1 <<= 1;
                    rs1 += CU_intruction[i];
                    ++i;
                }
                for (int r=0;r<4;++r) {
                    rs2 <<= 1;
                    rs2 += CU_intruction[i];
                    ++i;
                }
            }

            if (operation >= 8 && operation <=12 && operation != 10) {
                for (int r=0;r<4;++r) {
                    target_register_address <<= 1;
                    target_register_address += CU_intruction[i];
                    ++i;
                }

                for (int r=0;r<4;++r) {
                    rs1 <<= 1;
                    rs1 += CU_intruction[i];
                    ++i;
                }

                vector <int> bits(4,0);
                for (int r=0;r<4;++r) {
                    bits[r] = CU_intruction[i];
                    rs2 <<= 1;
                    rs2 += bits[r];
                    ++i;
                }
                if (bits[0] == 1) {
                    rs2 -= 16;
                }
            }

            if (operation == 3 ) {
                int temp =0;
                for (int r=0;r<4;++r) {
                    temp <<= 1;
                    temp += CU_intruction[i];
                    ++i;
                }
                rs1 = target_register_address = temp;
                rs2 = -1;
            }

            if (operation == 7) {
                for (int r=0;r<4;++r) {
                    target_register_address<<=1;
                    target_register_address+=CU_intruction[i];
                    ++i;
                }

                for (int r=0;r<4;++r) {
                    rs1 <<=1;
                    rs1 += CU_intruction[i];
                    ++i;
                }
                rs2 = -1;
            }


            if (operation == 13) {
                vector <int> bits(8,0);
                for (int r=0;r<8;++r) {
                    bits[r] = CU_intruction[i];
                    rs2 <<= 1;
                    rs2 += bits[r];
                    ++i;
                }
                if (bits[0] == 1) {
                    rs2 -= 256;
                }
            }

            if (operation == 14) {
                for (int r=0;r<4;++r) {
                    rs1 <<= 1;
                    rs1 += CU_intruction[i];
                    ++i;
                }
                vector <int> bits(8,0);
                for (int r=0;r<8;++r) {
                    bits[r] = CU_intruction[i];
                    rs2 <<= 1;
                    rs2 += bits[r];
                    ++i;
                }
                if (bits[0] == 1) {
                    rs2 -= 256;
                }
            }

            if (operation == 15) {
                ishalt=1;
            }


        }

};