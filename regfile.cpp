#include <iostream>
#include "regfile.h"

using namespace std;

regs registers;
void register_initialize()
{
	/*Initializes the register files to be used*/
	registers[0].name = "$0";
	registers[1].name = "$a0";
	registers[2].name = "$ra";
	registers[3].name = "$t0";
	registers[4].name = "$t1";
	registers[5].name = "$t2";
	registers[6].name = "$t3";
	registers[7].name = "$t4";
	registers[8].name = "HI";
	registers[9].name = "LO";
	registers[10].name = "IR";
	
	
	registers[0].value = 0;
	registers[1].value = 0;
	registers[2].value = 0;
	registers[3].value = 0;
	registers[4].value = 0;
	registers[5].value = 0;
	registers[6].value = 0;
	registers[7].value = 0;
	registers[8].value = 0;
	registers[9].value = 0;
	registers[10].value = 0;
	
}