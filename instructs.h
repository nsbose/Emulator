#ifndef INSTRUCTIONS
#define INSTRUCTIONS



/*R type instructions*/
#define inst_ADD 	0x01
#define inst_SUB 	0x02
#define inst_OR 	0x03
#define inst_AND 	0x04
#define inst_DIV 	0x05
#define inst_MULT 	0x06
#define inst_MFLO 	0x07
#define inst_MFHI 	0x08
#define inst_SRL 	0x09
#define inst_SLL 	0x0A
#define inst_XOR 	0x0B
#define inst_SRA 	0x0C

/*J type instructions*/
#define inst_J		0x01
#define inst_JAL	0x02
#define inst_JR		0x03

/*I type insturctions*/
#define inst_BEQ	0x04
#define inst_BNE	0x05
#define inst_ORI	0x06
#define inst_ANDI	0x07
#define inst_ADDI	0x08
#define inst_SUBI	0x09
#define inst_LA		0x0A
#define inst_LUI	0x0B
#define inst_LW		0x0C
#define inst_SW		0x0D
#define inst_LI		0x0E
#define inst_MOV	0x0F



#endif