 #include <iostream>
#include <cstdio>
#include "inst_mem.h"
#include "pm_count.h"
#include <string>
using namespace std;
inst_ instructions; 
short pc_count = 0;
short memory_array[65289];
char arraynum; //stores the user input of characters
char *numbers;
short buffer = 0;
short count_buf;
short high_byte, low_byte;
bool check = true;

void initialize_memory(){
	//initialize the rom data sets
	
	
}

void get_instruc(short *reg_value, unsigned short address_begin){
	char i;
	if (address_begin == 0xFF00)
	{	
		if (memory_array[0xFF00] & 0x02){ //if write flag is on
			*reg_value = 2;
			cout << "BITS ON!: " << endl;
			cout << "FLAG2: " << memory_array[0xFF00] << endl;
		}
		else
		{
			if (check)
			{
				 //flush buffer
				getchar();
				check = false;
			}
			
			scanf("%c", &arraynum);
			memory_array[0xFF04] = arraynum; //stores as ascii
			cout << "ARRAY WHAT:########################  " << arraynum << " #### " << memory_array[0xFF04] <<  endl;
			memory_array[0xFF05] = 0;			
			memory_array[0xFF00] =  (memory_array[0xFF00] & 0) |  0x1; //sets flag to 1
			*reg_value = memory_array[0xFF00];
			if (arraynum == '\n')
			{
				//check = true;
			}
		}
	}
	else 
	{
		//Passes value shorto memory
		low_byte = memory_array[address_begin];
		high_byte = memory_array[address_begin + 1];
		*reg_value = high_byte | low_byte;
		if (address_begin == 0xFF04) //loads value from 0xff04 to register, we clear flag
		{
			memory_array[0xFF00] = 0; //reset flag to 0
		}
		cout << "LW: " << pc_count << " " << *reg_value << endl;
	}
}

void set_instruct(short reg_value, unsigned short address_begin){
	if (address_begin == 0xFF00){ //outputting the character to terminal
		char temp = reg_value;
		printf("LOOK!!!!!!!!!!!!!!!!!!!!!!!!!!! %c",temp);
		memory_array[0xFF00] = memory_array[0xFF00] & 0x0; //reset the flag bit 2
		//cout << "FLAG: " << memory_array[0xFF00] << endl;
	}
	if (reg_value & 0x8000){
		high_byte = (reg_value & 0xFF00);
	}
	else
	{
		high_byte = (reg_value & 0xFF00);
	}
	low_byte = (reg_value & 0x00FF);
	//Stores in memory in little endian order
	if ((address_begin != 0xFF00) && (address_begin != 0xFF04)){
		memory_array[address_begin] = low_byte;
		memory_array[address_begin + 1] = high_byte;
	}
	else if (address_begin == 0xFF04){
		memory_array[address_begin] = reg_value;
	}
	
	if (address_begin == 0xFF04){
		memory_array[0xFF00] =  0x2; //setting the value of bit 2 
	}
	cout << "SW: " << reg_value << " " << address_begin <<  endl;
}