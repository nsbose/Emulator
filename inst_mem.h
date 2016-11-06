#ifndef INST_MEM
#define INST_MEM
#include <map>

typedef struct{
	short addressVal;
	short instruction_set;
}inst_m;


typedef short marker;
extern short pc_count; /*Program counter*/
extern short memory_array[65289]; /*Holds the memory value*/
extern char arraynum;
extern char *numbers;
extern bool check;
extern short buffer;
extern short count_buf;
typedef std::map<marker, inst_m> inst_; /*Stores the instructions*/
extern inst_ instructions; 

void get_instruc(short *reg_value, unsigned short address_begin); /*Reads from memory*/
void set_instruct(short reg_value, unsigned short address_begin); /*Writes to memory*/
void initialize_memory();

#endif 