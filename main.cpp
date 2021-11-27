
#include <iostream>
#include "bignum.h"

//int main(int argc, char* argv[])
int main()
{
	try
	{
		std::string number = "123456789123456789123456789";
		BigNum aanumber{ number };

		BigNum a;
		BigNum b{"22"};
		BigNum c{"-22"};
		BigNum d{"128"};
		BigNum e{"-1"};

		BigNum t = c * e;
	}
	catch (std::exception& err) {
		std::cout << err.what();
	}
	
	return 0;
}
