#include <iostream>
using namespace std;

int main()
{
	int dividend = 2; int divisor = 3;
	int truth = 1; int count = 16;
	
	
	if (dividend < 0){
		truth*=-1;
		dividend*=-1;
	}
	if (divisor < 0){
		truth*=-1;
		divisor*=-1;
	}
	int quotient = dividend;
	int remainder = 0;
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
	cout << "QUO: " << (quotient & 0xFFFF) << " REM: " << remainder <<endl;

	return 0;
}