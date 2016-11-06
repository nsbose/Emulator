#include <iostream>
#include <cstring>
#include <string>
#include <cstdio>
#include <cstdlib>
#include "inst_mem.h"
#include "translate.h"
#include "pm_count.h"

using namespace std;

void translateFile(const char*);
void startprogram();

int main(){
	
	char multiplier[] = {'m','u','l','t','i','p','l','i','e','r',':',' '};
	char multiplicand[] = {'m','u','l','t','i','p','l','i','c','a','n','d',':',' '};
	
	int i_multiplier = sizeof(multiplier)/sizeof(char);
	int i_multiplicand = sizeof(multiplicand)/sizeof(char);
	
	
	for (int i = 0,j = 40; i < i_multiplier;i++,j+=2)
	{
		memory_array[j] = multiplier[i];
		//cout << j << endl;
	}
	
	for (int i = 0, j = 68; i < i_multiplicand; i++,j+=2)
	{
		memory_array[j] = multiplicand[i];
	}
	
	
	
	string inputFile;
	//pass_instruc();
	cout << "Enter the name of the MIF file: ";
	cin >> inputFile;

	inputFile = inputFile + ".mif";
	translateFile(inputFile.c_str());
	cout << "done" << endl;
	startprogram();
	//cout << pc_count << endl;


	return 0;
}

void translateFile(const char* c)
{
	FILE *fp;
	fp = fopen(c,"r");
	int x,y;
	int count = 0;
	int countB = 0;
	char line[50];

	if (fp == NULL){
		printf("Error, cannot open!\n");
	}
	else
	{
		while (fgets(line, sizeof line, fp)){
			countB++;
		}
		fclose(fp);
	}
	countB = countB - 1;
	fp = fopen(c,"r");
	while (fgets(line, sizeof line, fp)){

		if ((fscanf(fp, "%x : %x", &x, &y)) && ((count > 4) && (count < countB - 1))){
			//printf("%04x,%04x\n", x,y);
			instructions[x].instruction_set = y;
			instructions[x].addressVal = x;
		}
		count++;
	}
	fclose(fp);
}

void startprogram()
{
	short i;
	pc_count = 0;
	while (pc_count != (instructions.size() * 2)) {
		perform_code(instructions[pc_count].instruction_set);
		pc_count+=2;
	}
}