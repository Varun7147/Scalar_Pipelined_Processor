// This is the intellectual property of Jupaka Varun Teja (CS22B050) and Ansh Meshram (CS22B051)
// CS2610 - Lab 8
// Scalar Pipeline Processor




#include <iostream>
#include <vector>
#include<fstream>
using namespace std;
#include "CU.cpp"
#include <map>

// this is the register unit class it handles all the operation related to the register file
// which are register read and register write

class Register_Unit : public CU {

public :
    vector <vector <int> > RegisterFile;
    vector <int> instruction; 
    int op1;
    int op2;
    int rd_content;
    int rd;
    int target_reg_WB;
    int to_WB;
    vector <int> value_WB;
    map <int,int> used_regis;
    int isstall =0;
    int ishalt_main = 0;
    int is_branch=0;


    Register_Unit() : CU() {
        RegisterFile.resize(16,vector <int> (8,0));
        isstall =0;
        rd_content =-1;
        value_WB.resize(8,0);

        // taking input from the required file


        ifstream ifile("./input/RF.txt");

        for (int r=0;r<16;++r) {
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
                    RegisterFile[r][e] = bit[e];
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
                    RegisterFile[r][e+4] = bit[e];
                }
            }
        }
        ifile.close();
        RegisterFile[0] = vector <int> (8,0);
    }

    //convert vector into int 2's complement is taken care of
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

    // update stall this is use to update the positon of the used register in the pipeline 
    // this is needed to detect data stalls
    void update_stall () {
        if (used_regis.size() != 0) {
            for (auto &p :  used_regis ) {
                p.second++;
            }

            vector <int> todel;
            for (auto &p : used_regis) {
                if (p.second >=4) {
                    todel.push_back(p.first);
                }
            }

            for (auto &p : todel) {
                auto it = used_regis.find(p);
                if (it != used_regis.end()) {
                    used_regis.erase(it);
                }
            }
        }
    }

    // this function corresponds to the decode and read pipline stage
    // this also contain the logic for stalling , dectecting type of stall and halting
    void decode_and_read() {
         is_branch=0;
        isstall=0;

        CU::CU_intruction = instruction;
        CU::decode();

        rd = CU::target_register_address;
        if (operation < 11) {
            to_WB = 1;
 
        }
        else to_WB = 0;



        if (CU::ishalt == 1) {
            ishalt_main =1 ;
            is_branch=0;
            isstall=0;
            return;
        }
        if (CU::operation == 14) {
            if (used_regis.find(rs1) != used_regis.end()) {
                isstall =4-used_regis[rs1];
                return;
            }
            else op1 = convert(RegisterFile[rs1]);

            is_branch = 2;
            isstall = 2;
            op2 = rs2;
            return;
        }

        if (CU::operation == 10) {
            op1 = rs1;
            op2 = rs2;
            used_regis[CU::target_register_address]=1;
            is_branch=0;
            isstall=0;
            return;
        }

        if (CU::operation == 13) {
            is_branch = 2;
            isstall = 2;
            op1 = -1;
            op2 = rs2;
            return;
        }


        if (CU::operation == 12 || CU::operation == 11) {
            if (used_regis.find(rs1) != used_regis.end()) {
                    isstall =4-used_regis[rs1];
                    return;
                }
                else op1 = convert(RegisterFile[rs1]);
            op2 = rs2;
            if (used_regis.find(target_register_address) != used_regis.end()) {
                isstall =4-used_regis[target_register_address];
                return;
            }
            else rd_content = convert(RegisterFile[CU::target_register_address]);
            if (CU::operation == 11)  used_regis[CU::target_register_address]=1;

            return;
        }



        else {

                if (used_regis.find(rs1) != used_regis.end()) {
                    isstall =4-used_regis[rs1];
                    return;
                }
                else op1 = convert(RegisterFile[rs1]);

            if (CU::operation < 7 && CU::operation !=3) {

                    if (used_regis.find(rs2) != used_regis.end()) {
                        isstall =4-used_regis[rs2];
                        return;
                    }
                    else op2 = convert(RegisterFile[rs2]);
            }
            else {
                 op2 = rs2;
            }



            rd = CU::target_register_address;

            used_regis[CU::target_register_address]=1;
        }
    }


    vector<int> convert(int a){
        if(a>=0){
            int i=0;
            vector<int> out(8,0);
            for(int j=7;j>=0;j--){
                out[j]=a%2;
                a=int(a/2);
            }
            return out;
        }
        a*=-1;
        int i=0;
        vector<int> v(8,0);
        for(int j=7;j>=0;j--){
            v[j]=a%2;
            a=int(a/2);
        }
        for(int j=0;j<8;j++){
            if(v[j]==1)v[j]=0;
            else v[j]=1;
        }
        int f=7;
        while(v[f]==1){f--;}
        v[f]=1;
        for(int h=f+1;h<8;h++){
            v[h] = 0;
        }
        return v;
    }

    // this coresponds to the write_back stage of the pipeline

    void write_back () {
         RegisterFile[target_reg_WB] = value_WB;
    }

    // dummping the contents of the register file into the required file
    void dump () {
          ofstream ofile("./output/RF.txt");

        for (auto p : RegisterFile) {
            for (auto q :p ) {ofile << q << ' '; cout << q << ' '; }
            ofile << endl;
            cout << endl;
        }
        ofile.close();
    }
};