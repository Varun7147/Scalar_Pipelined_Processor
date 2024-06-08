# Instructions to run and execute the program :
#### For compiling the program : 
 
Open the terminal, proceed to navigate to the directory containing the program main.cpp, and execute the subsequent command.  This compiles the program. 
```
g++ main.cpp ./src/*.cpp
```
#### For executing the program :
Upon successfully compiling the program using the preceding command, proceed to execute the subsequent command. This action will generate the files **DCache.txt** and **Output.txt** within the designated output directory.
```
./a.out
```
___
#### Processor Design Credits:


##### Designed by: 

✨ [Ansh Meshram (CS22B052)]
 ✨ [ Varun Teja (CS22B050)]
---



# Report : 


# Scalar Pipelined Processor

The Scalar Pipelined Processor is designed by employing five primary units, which are as follows:

 - Instruction_Unit
 - Register_Unit
 - ALU
 - Memory_Unit
 - Processor
 
## Overview
The implementation of the Processor in C++ utilizes classes, with each of the five units mentioned implemented as separate classes. The Processor class inherits from the following classes: Instruction_Unit, Register_Unit, ALU, and Memory_Unit. Within the Processor class, various buffer objects essential for pipelining the processor are encapsulated, with each implemented as a separate class. These include Memory_Buffer (memb), ALU_Buffer (alub), Decode_and_Read_Buffer (drb), and Instruction_Buffer (ib).

## Implementation of different units

### 1. Instruction_Unit:
 The Instrution_Unit consists of
  - InsReg : This is the Instruction register (of 2 bytes) which stores the instruction.
  - PC : This is the Program counter which points to the address of instruction to be excuted in the instruction cache.
  - insc : An object of the class Instruction_cache, created to access its members and execute memory operations.
 

 ####  < Instruction_cache :
 - The Instruction_cache class stores the contents of Icache.txt in 128 lines, with each line containing 2 bytes of data, achieved through the **take_input()** method of this class.
- The **data_read()** method accepts the Program Counter (PC) as an argument and retrieves the data referenced by the PC in the instruction cache.

 The **fetchin()** method of Instruction_Unit invokes the **data_read()** method of Instruction_cache, allowing the next instruction to be executed to be stored in the Instruction Register (InsReg). Subsequently, the Program Counter (PC) is incremented by 2.

### 2. Register_Unit:
 The Register_Unit consists of
  - RegisterFile : Stores the contents of all the 16 registers.
  - to_WB : This is set to one when there is a requirement to perform a write-back operation to the RegisterFile (specifically, to the register target_reg_WB) after executing the instruction. 
  - used_regis : This is a C++ map (utilizing the Standard Template Library) containing recently utilized registers to which data must be written back. Upon completing the write-back operation to the register, it is subsequently removed from the "used_regis" map.

The Register_Unit class is the child class, publically inherited from the class CU (Control Unit)
The Register_Unit reads and decodes the data in the RF.txt and stores it in RegisterFile as vectors of size 8 (1 byte). 
> Note: The contents of register zero will be initialized to zero at the beginning of the program, regardless of the contents specified in the input RF.txt file.

 #####  < CU (Control Unit) :
The Control Unit is responsible for decoding the instruction and categorizing it into operation, rs1, rs2, and rd (target_register_address). Additionally, the "ishalt" flag is set to 1 when a Halt instruction is decoded.
 
The **decode_and_read()** method of Register_Unit invokes the **decode()** method of the CU class which decodes the instruction present in CU_intruction and sets the corresponding values of rs1, rs2, rd, operation. Additionally, it sets the values of op1, op2 which are to be passed onto ALU based on the instruction. 
The **update_stall()** method manages the contents of the used_regis map by adding the decoded destination registers to the map and removing them after the data has been written back to them following 3 clock cycles. If there is a necessity to stall, it sets the "isstall" flag to one.
 > Note: By examining the used_regis map and the registers (rs1, rs2, rd) decoded in the current clock cycle, the isstall flag is set to either one or zero. This mechanism aids in detecting and managing Data stalls effectively.

### 3. ALU:
The ALU comprises the following components:
- op1, op2 : These represent the contents of registers rs1 and rs2, respectively, on which the ALU needs to perform operations.
- operation : This holds the encoded value representing the operation to be executed by the ALU.
- ALUOUT : After performing the required operation, the result is stored in ALUOUT in binary format.
- branch_address : This variable stores the address to which the Program Counter (PC) must be set in the event of a branch, which is determined by the boolean value yesbranch.

The **convert()** method in the ALU class facilitates the conversion of a decimal number to an 8-bit binary representation and vice versa. Additionally, it manages the conversion to 2's complement form when handling negative integers.
The ALUout() method executes the necessary operations on operands op1 and op2 based on the specified operation. Below are the values of "operation", op1", and "op2" corresponding to each function performed by the ALU:


| operation | Operation | op1 | op2 |
| ------ | ------ | ------ | ------ |
| 0 | ADD | rs1 | rs2 |
| 1 | SUB | rs1 | rs2 |
| 2 | MUL | rs1 | rs2 |
| 3 | INC | r | -1 |
| 4 | AND | rs1 | rs2 |
| 5 | OR | rs1 | rs2 |
| 6 | XOR | rs1 | rs2 |
| 7 | NOT | rs | -1 |
| 8 | SLLI | rs1 | imm(4) |
| 9 | SRLI | rs1 | imm(4) |
| 10 | LI | imm[7:4] | imm[3:0] |
| 11 | LD | rs1 | imm(4) |
| 12 | ST | rs1 | imm(4) |
| 13 | JMP | -1 | L1 |
| 14 | BEQZ | rs | L1 |


> Note: Incase of JMP and BEQZ instructions, the Processor is stalled till the ALU calculates the address (PC + L1) to which the Program Counter (PC) needs has to be set. If the branch condition is not satisfied, the ALU returns the current PC unchanged. 


### 4. Memory_Unit:
The Memory_Unit comprises the following components:
- ALU_output : The address calculated by the ALU, indicating where the data needs to be stored or loaded from.
- LMD : If the instruction is a load operation, the data is retrieved from the data cache and stored in the LMD (Load Memory Data) register.
- dc : An object of the Data_cache class, created to access its members and execute memory operations.

 ####  < Data_cache :
The Data_cache class serves as the repository for the contents of the data cache throughout the program's execution. Its **take_input()** method facilitates the opening and reading of the file "DCache.txt", subsequently storing its contents in the cache. This cache is implemented as a vector consisting of 128 lines, each comprising 2 bytes.
The **data_read()** and **data_write()** methods of the Data_cache class bear the responsibility of loading data from and storing data into the cache, respectively.
 
The **Mem_operation()** method of the Memory_Unit class calls either the **data_read()** or **data_write()** method based on the operation. In the case of a load operation, the data read from the cache is stored in the LMD (Load Memory Data) register, and the "to_write" flag is set to one.

## THE INTEGRATION OF PROCESSOR: 
The Processor class encapsulates instances of various buffer objects, each implemented as a separate class. These include the Memory_Buffer (memb), ALU_Buffer (alub), Decode_and_Read_Buffer (drb), and Instruction_Buffer (ib).
Each buffer includes a validity flag. The corresponding units operate exclusively when the validity flag is set to one.
 #### <  Instruction_Buffer  :
 Instruction_Buffer includes the following components:
- valid : This indicator is configured to a value of one when the buffer is operational.
- instruction : This variable stores the instruction (2 bytes) to be executed.
 #### < Decode_and_Read_Buffer  : 
Decode_and_Read_Buffer includes the following components:
- valid: This indicator is configured to a value of one when the buffer is operational.
- op1, op2, operation: These variables represent the operands and operation decoded by the Register_Unit.
- value_to_be_stored: This variable holds the data that needs to be stored back in the Data_Cache incase of a store instruction. 
- to_write: This flag is set to one when the operation requires data to be written into a register.

 #### <  ALU_Buffer  :
ALU_Buffer comprises the following elements:
- valid: This indicator is configured to a value of one when the buffer is operational.
- ALU_output: This variable holds the output produced by the ALU.
- write_value_WB: This variable contains the data that needs to be written back into the register.

 #### <  Memory_Buffer  : 
 
Memory_Buffer is composed of the following elements:
- valid: This indicator is configured to a value of one when the buffer is operational.
- rd: This variable represents the destination register to which data is transferred from memory (Dcache).
- ALU_output: This variable holds the data to be written to the destination register (rd).

### The Processor : 
The Processor class includes several variables that store counts of different types of instructions, clock cycles, stalls, branches, and other relevant information.
The implementation of clock cycle is managed by the **cycle()** method of the Processor. 
  The order of implementation in one clock cycle follows the sequence: WB -> MEM -> ALU -> Ins_Decode -> Ins_Fetch, contingent upon the valid flag status of each buffer.
 
> The complete instruction data will be transferred to different components of the processor in the pipeline in the following sequence:
Instruction_Unit -> Instruction_Buffer (ib) -> Register_Unit (ID) -> Decode_and_Read_Buffer (drb) -> ALU -> ALU_Buffer (alub) -> Memory_Unit -> Memory_Buffer (memb) -> Register_Unit (WB).

Note that the Register_Unit is responsible for executing two primary functions, decoding of instructions through the CU (Control_Unit) and the subsequent writing back of data to the Register_File.
 #### < Handling of Data Hazards :
Data hazards are detected by the Register_Unit through the use of the "used_regis" map.
 > Whenever an instruction necessitates data to be written to a register (rd), the used_regis[rd] flag is set to one. In subsequent clock cycles, the used_regis[rd] flag will be incremented by one. By the time the data has been successfully written back to register rd, the used_regis[rd] flag will reach a value of four, at which point it will be removed from the used_regis map.
 
In subsequent instructions, if the data present in a register (rs1) is required for an operation and rs1 is found in the used_regis map, it indicates that the processor needs to stall for "4 - used_regis[rs1]" clock cycles.

  #### < Handling of Control Hazards :
A Control Hazard is identified when the Register_Unit decodes either a JMP or a BEQZ instruction. This occurs because in both cases, the ALU needs to execute operations to determine the next instruction to be fetched (i.e., the value to be loaded into the Program Counter).
> Indeed, since in the instructions JMP L1 and BEQZ rs, L1, the 8-bit signed immediate value serves as a relative offset to the current Program Counter (PC), the processor must be stalled for two clock cycles regardless of other factors, when encountering these instructions.




### 
###
###
###








