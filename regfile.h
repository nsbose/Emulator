#ifndef REGFILE
#define REGFILE

#include <map>
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <string>
#include <malloc.h>

typedef struct{
	std::string name;
	short value;
} reg;
typedef short reg_addrs;

typedef std::map<reg_addrs, reg> regs;

extern regs registers;

//extern reg regis_files[8];

void register_initialize();
#endif