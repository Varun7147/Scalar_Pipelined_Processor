// This is the intellectual property of Jupaka Varun Teja (CS22B050) and Ansh Meshram (CS22B051)
// CS2610 - Lab 8
// Scalar Pipeline Processor




#include <bits/stdc++.h>
#include "Dcache.cpp"

using namespace std;

// this class stimulates the memory unit 

class Memory_Unit {
    public:
        vector <int> ALU_output;
		vector <int> LMD ;
        int rd;
		int operation;
        int to_write =0;
        Data_cache *dc;
        int store_value;

    Memory_Unit() {
        ALU_output.resize(8);
        LMD.resize(8);
        to_write =0;
        dc = new Data_cache();
        store_value =0;
    }

    // taking in nessecary information from the ALU buffer
    void Mem_Unit_takein(vector<int>alu_out, int oper, int Rd,int sv){
        ALU_output = alu_out;
        operation = oper;
        rd = Rd;
        store_value = sv;
    }


    // doing load or store operation based on the value of operation
    void Mem_operation(){
        to_write=0;
        if(operation == 11){
            LMD = dc->data_read(convert(ALU_output));
            to_write = 1;
        }
        else if(operation == 12){
            dc->data_write(convert(ALU_output), convert(store_value));
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

    int convert(vector<int>v){
        int i=1;
        int out=0;
        if(v[0]==0){
            for(int j=7;j>=0;j--){
                out+=v[j]*i;
                i*=2;
            }
            return out;
        }
        else{
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
};