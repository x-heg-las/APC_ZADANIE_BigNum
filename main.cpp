
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
			
		


			int dd = rand() % 100000;
			BigNum d{ dd };

			int ee = rand() % 100000;
			BigNum e{ "das"};
			
			BigNum q = e + e;
			BigNum y = -q;
			
			

			BigNum t = e + d ;
			int64_t k =ee + dd;

			

			assert((e > d) == (ee > dd));
			assert((e < d) == (ee < dd));
			assert((e == d) == (ee == dd));
			assert((e != d) == (ee != dd));
			assert((e < d) == (ee < dd));
			assert((e <= d) == (ee <= dd));
			assert((e >= d) == (ee >= dd));

			std::cout << "###############BIGNUM :" << t << std::endl;
			std::cout << "###############INT :" << k << std::endl;
			std::stringstream moje;
			moje << t;
			std::stringstream ine;
			ine << k;
			std::string mmoje = moje.str();
			std::string iine = ine.str();

			assert(!mmoje.starts_with("0") );

			assert(mmoje == iine);
		}
	}
	catch (std::exception& err) {
		std::cout << err.what();
	}
	
	return 0;
}
