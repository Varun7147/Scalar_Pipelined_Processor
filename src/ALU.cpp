// This is the intellectual property of Jupaka Varun Teja (CS22B050) and Ansh Meshram (CS22B051)
// CS2610 - Lab 8
// Scalar Pipeline Processor



#include <bits/stdc++.h>

using namespace std;


// class for the ALU 
class ALU{
    public:

    // basic signals to and information needed by the ALU 
    int op1;
    int op2;
    bool yesbranch = false;
    int branch_address;
    int operation;
    int PC;
    vector<int>ALUOUT;

    // createing ALU class
    ALU(){
        ALUOUT.resize(8,0);
    }

    // debugging
    void ALU_print(){
        cout<<"OP1 : "<<op1<<endl;
        cout<<endl<<"OP2 : "<<op2<<endl;
        cout<<"Operation : "<<operation<<endl;
        cout<<"Branch_add : "<<branch_address<<endl;
        cout<<endl<<"PC : "<<PC<<endl;
        return;
    }

    // taking in the nessasary information from the decode and read buffer 
    void ALU_takein(int v1, int v2, int oper, int pc){
        op1 = v1;
        op2 = v2;
        PC = pc;
        operation = oper;
        return;
    }

    // convert vector into integer 2's complement is taken care of
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

    // convert decimal into binary , negative numbers are taken care of
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

    // main ALU operation
    void ALUout(){
        yesbranch = false;
        int a = (op1);
        int b = (op2);
        vector<int> op1v(8,0);
        vector<int> op2v(8,0);
        op1v = convert(op1);
        op2v = convert(op2);
        int pc = PC;

        // executing operation based on operation number and in accordance to the CU unit

        if(operation == 0 || operation == 11 || operation == 12){
            ALUOUT = convert(a+b);
            return;
        }
        else if(operation == 1){
            ALUOUT = convert(a-b);
            return;
        }
        else if(operation == 2){
            ALUOUT = convert(a*b);
            return;
        }
        else if(operation == 3){
            ALUOUT = convert(a+1);
            return;
        }
        vector<int> out(8,0);
        if(operation == 4){
            for(int i=0;i<8;i++){
                out[i]=op1v[i]&op2v[i];
            }
            ALUOUT = (out);
            return;
        }
        else if(operation == 5){
            for(int i=0;i<8;i++){
                out[i]=op1v[i]|op2v[i];
            }
            ALUOUT = (out);
            return;
        }
        else if(operation == 6){
            for(int i=0;i<8;i++){
                out[i]=op1v[i]^op2v[i];
            }
            ALUOUT = (out);
            return;
        }
        else if(operation == 7){
            for(int j=0;j<8;j++){
                if(op1v[j]==1)out[j]=0;
                else out[j]=1;
            }
            ALUOUT = out;
            return;
        }
        else if (operation == 8){
            if(b>=8){
                ALUOUT = out;
                return;
            }out=op1v;
            for(int i=0;i<b;i++){
                for(int j=0;j<7;j++){
                    out[j]=out[j+1];
                }out[7]=0;
            }
            ALUOUT = out;
            return;
        }
        else if(operation == 9){
            if(b>=8){
                ALUOUT = out;
                return;
            }
            out=op1v;
            for(int i=0;i<b;i++){
                for(int j=7;j>0;j--){
                    out[j]=out[j-1];
                }out[0]=0;
            }   
            ALUOUT = out;
            return;
        }
        else if (operation == 10) {
            for (int r=0;r<4;++r) {
                op2v[r] = op1v[r+4];
            }
            ALUOUT = op2v;
        }
        else if(operation == 13){
            yesbranch = true;
            b*=2;
            branch_address = (b+pc);
            ALUOUT = convert(b+pc);
            return;
        }
        else if(operation == 14){
            if(a==0){
                yesbranch = true;
                b*=2;
                branch_address = (b+pc);
            }
            else {
                branch_address = pc;
            }
            ALUOUT = convert(b+pc);
            return;
        }
        return;
    }

};

