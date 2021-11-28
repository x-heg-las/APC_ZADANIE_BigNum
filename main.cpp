
#include <iostream>
#include "bignum.h"
#include <cassert>

//int main(int argc, char* argv[])
int main()
{
	srand(static_cast<unsigned int>(time(0)));
	try
	{

		for (int i = 0; i < 10000; i++) {
			
	/*		BigNum a{-3};
			int aa = -3;
			int bb = rand();
			BigNum b{ bb };

			int cc = rand();
			BigNum c{ cc };*/

			int dd = rand();
			BigNum d{ dd };

			int ee = rand();
			BigNum e{ ee };

	


			BigNum t = d+e;
			int64_t k = dd + ee;

			std::cout << "###############BIGNUM :" << t << std::endl;
			std::cout << "###############INT :" << k << std::endl;
			std::stringstream moje;
			moje << t;
			std::stringstream ine;
			ine << k;
			std::string mmoje = moje.str();
			std::string iine = ine.str();


			assert(mmoje == iine);
		}
	}
	catch (std::exception& err) {
		std::cout << err.what();
	}
	
	return 0;
}
