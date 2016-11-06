#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>
#include "inst_mem.h"
#include "regfile.h"
#include "translate.h"
#include "instructs.h"
#include "pm_count.h"

#define REG_A(instrSet) ((instrSet & 0x01C0 ) >> 6) /*Bitmask to isolate Register A*/
#define REG_B(instrSet) ((instrSet & 0x0E00) >> 9) /*Bitmask to isolate Register B*/
#define SA(instrSet) ((instrSet & 0x0030) >> 4)
#define getLowInt(instrSet) (instrSet & 0x001F) /*Isolates the immediate shorteger*/
#define getFullInt(instrSet) (instrSet & 0x00FF) /*Isolates shorteger li and lui*/
using namespace std;


short half, full_short, temp_short;
void ALU_code_R(unsigned short);
void ALU_code_I(unsigned short);
void ALU_code_J(unsigned short);
void perform_code(unsigned short);


void perform_code(unsigned short instr_)
{
	unsigned short op_code = (instr_ & 0xF000) >> 12; /*Isolates opcode value*/
	
	if (op_code == 0){
		ALU_code_R(instr_); //Passes to the ALU to begin the calculation
	}
	else if ( op_code > 0 && op_code < 4)
	{
		ALU_code_J(instr_); //J type instruction
	}
	else
	{
		ALU_code_I(instr_); //I type instruction
	}
}
void ALU_code_R(unsigned short alu_inst){
	
	/*Through bit masking finds the function code*/
	unsigned short function_code = (alu_inst & 0x000F);
	switch(function_code){
		case inst_ADD:
		{
			unsigned short regA = REG_A(alu_inst);
			unsigned short regB = REG_B(alu_inst);
			registers[regB].value = registers[regB].value + registers[regA].value;
			break;
		}
		case inst_SUB:
		{
			unsigned short regA = REG_A(alu_inst);
			unsigned short regB = REG_B(alu_inst);
			registers[regB].value = registers[regB].value - registers[regA].value;
			cout << "SUB: " << pc_count << " " << registers[regB].value << endl;
			break;
		}
		case inst_OR:
		{
			unsigned short regA = REG_A(alu_inst);
			unsigned short regB = REG_B(alu_inst);
			registers[regB].value = registers[regB].value | registers[regA].value;
			break;
		}
		case inst_AND:
		{
			unsigned short regA = REG_A(alu_inst);
			unsigned short regB = REG_B(alu_inst);
			registers[regB].value = registers[regB].value & registers[regA].value;
			cout << "AND: " << pc_count << registers[regB].value << endl;
			break;
		}
		case inst_DIV:
		{
			//http://www.cs.utah.edu/~rajeev/cs3810/slides/3810-08.
			//http://www4.wittenberg.edu/academics/mathcomp/shelburne/comp255/notes/binarydivision.pdf
			unsigned short regA = REG_A(alu_inst);
			unsigned short regB = REG_B(alu_inst);
			short dividend = registers[regB].value;
			short divisor = registers[regA].value;
			short truth = 1; short count = 16;
	
			if (dividend < 0){
				truth*=-1;
				dividend*=-1;
			}
			if (divisor < 0){
				truth*=-1;
				divisor*=-1;
			}
			short quotient = dividend;
			short remainder = 0;
			while (count > 0)
			{
				if ((quotient & 0x8000) != 0)
				{
					remainder = (remainder << 1) + 1;
				}
				else
				{
					remainder<<=1;
				}
				quotient<<=1;
				if ((remainder - divisor) >= 0)
				{
					remainder-=divisor;
					quotient = quotient | 0x01;
				}
				count--;
			}
			registers[9].value = quotient & 0xFFFF; //quotient MFLo
			registers[10].value = remainder; //MFHI remained
			cout << "QUO: " << (quotient & 0xFFFF) << " REM: " << remainder <<endl;
			
			break;
		}
		case inst_MULT:
		{
			bool neg = false;
			unsigned short regA = REG_A(alu_inst);
			unsigned short regB = REG_B(alu_inst);
			short multiplicand = registers[regB].value;
			short multiplier = registers[regA].value;
			short truth = 1; short count = 16;
	
			if (multiplicand < 0){
				truth*=-1;
				multiplicand*=-1;
			}
			if (multiplier < 0){
				truth*=-1;
				multiplier*=-1;
			}
			short total = 0;
			while (count > 0)
			{
				if (multiplier & 0x01){
					total+=multiplicand;
				}
				multiplicand<<=1;
				multiplier>>=1;
				count--;
			}
			registers[9].value = truth * (total & 0xFFFF); //stores in mflo
			
			break;
		}
		case inst_MFLO:
		{
			unsigned short regB = REG_B(alu_inst);
			registers[regB].value = registers[9].value;
			cout << "MFLO: " << pc_count << " " << registers[regB].value << endl;
			break;
		}
		case inst_MFHI:
		{
			unsigned short regB = REG_B(alu_inst);
			registers[regB].value = registers[10].value;
			break;
		}
		case inst_SRL:
		{
			unsigned short sa = SA(alu_inst);
			unsigned short regA = REG_A(alu_inst);
			unsigned short regB = REG_B(alu_inst);
			registers[regB].value = registers[regA].value >> sa;
			break;
		}
		case inst_SLL:
		{
			unsigned short sa = SA(alu_inst);
			unsigned short regA = REG_A(alu_inst);
			unsigned short regB = REG_B(alu_inst);
			registers[regB].value = registers[regA].value << sa;
			break;
		}	
		case inst_XOR:
		{
			unsigned short regA = REG_A(alu_inst);
			unsigned short regB = REG_B(alu_inst);
			registers[regB].value = registers[regB].value ^ registers[regA].value;
			cout << "XOR: " << pc_count << " " << registers[regB].value << " REGA: " << registers[regA].value << endl; 
			break;
		}
	}
}

void ALU_code_I(unsigned short alu_inst){
	unsigned short function_code = (alu_inst & 0xF000) >> 12;
	switch (function_code){
		case inst_BEQ:
		{
			cout << "BEQ: " << pc_count << endl;
			half = 0;
			if (alu_inst & 0x0020){
				half = (0xFFC0 | (alu_inst & 0x003F));
			}
			else 
			{
				half = getLowInt(alu_inst);
			}
			unsigned short regA = REG_A(alu_inst);
			unsigned short regB = REG_B(alu_inst);
			if ((registers[regB].value ^ registers[regA].value) == 0)
			{
				pc_count+=(half-2);
			}
			break;
		}
		case inst_BNE:
		{
			cout << "BNE: " << pc_count << endl;
			
			half = 0;
			if (alu_inst & 0x0020){
				half = (0xFFC0 | (alu_inst & 0x003F));
			}
			else 
			{
				half = getLowInt(alu_inst);
			}
			unsigned short regA = REG_A(alu_inst);
			unsigned short regB = REG_B(alu_inst);
			if ((registers[regB].value ^ registers[regA].value) != 0)
			{
				pc_count+=(half - 2);
				//cout << "HALF: " << half << endl;
				//cout << "count: " << pc_count << endl;
			}
			
			cout << "BNE " << registers[regB].value << " " << registers[regA].value <<endl;
			break;
		}
		case inst_ORI:
		{
			cout << "ORI: " << pc_count << endl;
			half = 0;
			if (alu_inst & 0x0020){
				half = (0xFFC0 | (alu_inst & 0x002F));
			}
			else 
			{
				half = getLowInt(alu_inst);
			}
			unsigned short regA = REG_A(alu_inst);
			unsigned short regB = REG_B(alu_inst);
			registers[regB].value = registers[regA].value | half;
			break;
		}
		case inst_ANDI:
		{
			cout << "ANDI: " << pc_count << endl;
			half = 0;
			if (alu_inst & 0x0020){
				half = (0xFFC0 | (alu_inst & 0x002F));
			}
			else 
			{
				half = getLowInt(alu_inst);
			}
			unsigned short regA = REG_A(alu_inst);
			unsigned short regB = REG_B(alu_inst);
			registers[regB].value = registers[regA].value & half;
			break;
		}
		case inst_ADDI:
		{
			//cout << "ADDI: " << pc_count << endl;
			half = 0;
			
			if (alu_inst & 0x0020){
				half = (0xFFC0 | (alu_inst & 0x003F));
				cout << "HALF1: " << half << endl;
			}
			else 
			{
				half = getLowInt(alu_inst);
				cout << "HALF2: " << half << endl;
			}
			unsigned short regA = REG_A(alu_inst);
			unsigned short regB = REG_B(alu_inst);
			registers[regB].value = registers[regA].value + half;
			//cout << "ADDI: " << alu_inst << " " << registers[regB].value << " " << (alu_inst & 0x0020) << endl;
			break;
		}
		case inst_SUBI:
		{
			
			half = 0;
			if (alu_inst & 0x0020){
				half = (0xFFC0 | (alu_inst & 0x002F));
			}
			else 
			{
				half = getLowInt(alu_inst);
			}
			unsigned short regA = REG_A(alu_inst);
			unsigned short regB = REG_B(alu_inst);
			registers[regB].value = registers[regA].value - half;
			cout << "SUBI HALF: " << half << endl;
			cout << "SUBI: " << registers[regB].value << " " << pc_count << endl;
			break;
		}
		case inst_LUI:
		{
			
			full_short = 0;
			if (alu_inst & 0x0100){
				full_short = (0xFE00 | (alu_inst & 0x01FF));
			}
			else 
			{
				full_short = getFullInt(alu_inst);
			}
			unsigned short regB = REG_B(alu_inst);
			registers[regB].value = full_short << 8;
			
			cout << "LUI: " << pc_count << " " << registers[regB].value << endl;
			break;  
		}
		case inst_LW:
		{
			unsigned short regA = REG_A(alu_inst);
			unsigned short regB = REG_B(alu_inst);
			get_instruc(&temp_short, registers[regA].value);
			registers[regB].value = temp_short;
			break;
		}
		case inst_SW:
		{
			unsigned short regA = REG_A(alu_inst);
			unsigned short regB = REG_B(alu_inst);
			set_instruct(registers[regB].value, registers[regA].value);			
			break;
		}
		case inst_LI:
		{
			
			full_short = 0;
			if (alu_inst & 0x0100){
				full_short = (0xFE00 | (alu_inst & 0x01FF));
			}
			else 
			{
				full_short = getFullInt(alu_inst);
			}
			unsigned short regB = REG_B(alu_inst);
			registers[regB].value = full_short;
			
			cout << "LI: " << pc_count << " " << registers[regB].value << endl;
			break;  
		}
		case inst_MOV:
		{
			cout << "MOV: " << pc_count << endl;
			unsigned short regA = REG_A(alu_inst);
			unsigned short regB = REG_B(alu_inst);
			registers[regB].value = registers[regA].value;
			cout << "IN MEM: " << memory_array[14] << " AND " <<  memory_array[16] << endl;
			break;
		}
		case inst_LA:
		//
		{	
			cout << "LA: " << pc_count << endl;
			unsigned short regB = REG_B(alu_inst);
			registers[regB].value = alu_inst & 0x1FF; //stores the memory address
		}
	}
}

void ALU_code_J(unsigned short alu_inst){
	unsigned short function_code = (alu_inst & 0xF000) >> 12;
	switch (function_code){
		case inst_J:
		{
			cout << "J: " << pc_count << endl;
			unsigned short j_target = (alu_inst & 0x0FFF);
			pc_count = j_target - 2;
			break;
		}
		case inst_JAL:
		{
			cout << "JAL: " << pc_count << endl;
			unsigned short j_target = (alu_inst & 0x01FF);
			registers[2].value = pc_count; //stores current location, because pc + 2
			 
			pc_count = j_target - 2;
			break;
		}
		case inst_JR:
		{
			cout << "JR: " << pc_count << " reg return: " << registers[2].value << endl;
			pc_count = registers[2].value;
			break;
		}
	}
}
