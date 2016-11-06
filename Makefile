all: emulator

emulator: regfile.cpp regfile.h translate.cpp translate.h pm_count.h inst_mem.h inst_mem.cpp loadprog.cpp 
	g++ -g -o emulator regfile.cpp translate.cpp inst_mem.cpp loadprog.cpp -lfl

clean:
	rm emulator