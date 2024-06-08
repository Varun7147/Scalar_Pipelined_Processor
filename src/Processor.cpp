// This is the intellectual property of Jupaka Varun Teja (CS22B050) and Ansh Meshram (CS22B051)
// CS2610 - Lab 8
// Scalar Pipeline Processor



#include <iostream>
#include <vector>
#include <map>
#include <bits/stdc++.h>
#include <fstream>

#include "instruction_unit.cpp"
#include "Register_Unit.cpp"
#include "Memory_Unit.cpp"
#include "ALU.cpp"
#include "Memory_buffer.cpp"
#include "ALU_buffer.cpp"
#include "Decode_buffer.cpp"
#include "Instruction_buffer.cpp"

using namespace std;


// this is the most important class of all as it basically controls everything
// for more information read Report in README.md

class Processor : public Instruction_Unit, Register_Unit,Memory_Unit,ALU{
public:
    int clock =0;
    int num_arith =0;
    int num_logical =0;
    int num_shift =0;
    int num_memory=0;
    int num_control=0;
    int num_halt = 0;
    int cycle_per_ins = 0;
    int data_stall =0;
    int control_stall =0;
    int istall=0;
    int ishalt=0;
    int num_li=0;
    int isbranch=0;
    int ending=4;

    Memory_Buffer memb;
    ALU_Buffer alub;
    Decode_and_Read_Buffer drb;
    Instruction_Buffer ib;
    
    Processor () : Instruction_Unit() , Register_Unit() , Memory_Unit() , ALU() {
        memb = Memory_Buffer();
        alub = ALU_Buffer();
        drb = Decode_and_Read_Buffer();
        ib = Instruction_Buffer();
    }



    // this is the most important function of this class 

    void cycle () {
        
        // write back into the register file if the valid bit is 1

        if (memb.valid == 1) {
            Register_Unit::target_reg_WB = memb.rd;
            Register_Unit::value_WB = memb.ALU_output;
            Register_Unit::write_back();
            memb.valid =0;
            
        }

       // we need to update the register status in the update_stall method for each clock cycle 
        
        Register_Unit::update_stall();
       
        // execute memory unit  the ALU if the valid bit is 1 and update the mem register buffers

        if (alub.valid == 1) {
            Memory_Unit::Mem_Unit_takein(alub.ALU_output,alub.operation,alub.rd,alub.store_value);
            Memory_Unit::Mem_operation();
            if (alub.to_write_WB == 0 && Memory_Unit::to_write == 1) {
                memb.valid = 1;
                memb.rd= Memory_Unit::rd;
                memb.ALU_output = Memory_Unit::LMD;
            }
            else {
                if (alub.to_write_WB == 2) {
                    memb.valid = 1;
                    memb.rd= alub.rd;
                    memb.ALU_output = alub.write_value_WB;
                }
                else memb.valid = 0;
            }
            alub.valid = 0;      
        }


        // execute ALU if the valid bit is 1 and update the ALU buffer

        if (drb.valid == 1) {
            ALU::ALU_takein(drb.op1,drb.op2,drb.operation,drb.PC);
            ALU::ALUout();

            if (ALU::yesbranch) {
               Instruction_Unit::PC = ALU::branch_address;
            }
            else {
                alub.ALU_output = ALU::ALUOUT;
                alub.operation = ALU::operation;
                alub.rd = drb.write_where;
                if (drb.to_write == 1) alub.to_write_WB= drb.to_write+1;
                else alub.to_write_WB =0;
                alub.write_value_WB=ALU::ALUOUT;
                alub.valid = 1;
            }
            alub.store_value = drb.value_to_be_stored;
            drb.valid = 0;
        }

        if (ib.valid == 1) {
            Register_Unit::instruction = ib.instruction;
            Register_Unit::decode_and_read();

            if (Register_Unit::isstall != 0) {
                isstall = Register_Unit::isstall;
                if (Register_Unit::is_branch == 2) {
                    isbranch = Register_Unit::is_branch;
                    control_stall += isstall;
                    drb.valid = 1;
                    drb.op1 = Register_Unit::op1;
                    drb.op2 = Register_Unit::op2;
                    drb.operation = Register_Unit::CU::operation;
                    drb.PC = Instruction_Unit::PC;
                    drb.to_write = Register_Unit::to_WB;
                    drb.write_where = Register_Unit::rd;
                    drb.value_to_be_stored = Register_Unit::rd_content;

                        if ( Register_Unit::operation <=3 ) ++num_arith;
                        else if (Register_Unit::operation <=7 ) ++num_logical;
                        else if (Register_Unit::operation <=9 ) ++num_shift;
                        else if (Register_Unit::operation == 10) ++num_li;
                        else if (Register_Unit::operation <=12 ) ++num_memory;
                        else if (Register_Unit::operation <=14) ++num_control;
                        else if (Register_Unit::operation ==15) ++num_halt;
                }
                else {
                    data_stall+=isstall;
                
                }
            }
            else if (Register_Unit::ishalt_main == 1){
                ishalt = Register_Unit::ishalt_main;
            }
            else {
                drb.valid = 1;
                drb.op1 = Register_Unit::op1;
                drb.op2 = Register_Unit::op2;
                drb.operation = Register_Unit::CU::operation;
                drb.PC = Instruction_Unit::PC-2;
                drb.to_write = Register_Unit::to_WB;
                drb.write_where = Register_Unit::rd;
                drb.value_to_be_stored = Register_Unit::rd_content;

                        if ( Register_Unit::operation <=3 ) ++num_arith;
                        else if (Register_Unit::operation <=7 ) ++num_logical;
                        else if (Register_Unit::operation <=9 ) ++num_shift;
                        else if (Register_Unit::operation == 10) ++num_li;
                        else if (Register_Unit::operation <=12 ) ++num_memory;
                        else if (Register_Unit::operation <=14) ++num_control;
                        else if (Register_Unit::operation ==15) ++num_halt;
            }
            ib.valid =0;
        }



        // fetches instruction only if there is no stall or halt

        if (isstall ==0 && ishalt==0) {; 
            Instruction_Unit::fetchin();
            ib.valid =1;
            ib.instruction = Instruction_Unit::InsReg;
        }
        else {
            // mechanism to get out of stall
            if (isbranch ==0 ) {
                isstall -=1 ;
                if (isstall == 0) {
                    ib.valid=1;
                }
            }
            else {
                isbranch -=1;
                isstall -=1 ;   
            }

        }

        if (ishalt == 0) ++clock;

}

    void dump () {
         Memory_Unit::dc->dump();
        //Register_Unit::dump();
    }


};

