// Oliver.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>

#include "Oliver.h"
#include ".\parser\parser.h"

int main(Olly::int_t argc, char** argv) {

	try {
		if (argc == 1) {
			return 0;
		}

		if (argc == 2) {

			Olly::let code_exp = Olly::expression();

			{
				Olly::parser lex(argv[1]);
				code_exp = lex.parse();
			}

			Olly::print("input code = " + repr(code_exp));

			{
				Olly::evaluator olly;
				code_exp = olly.eval(code_exp);
			}

			Olly::print("result code = " + repr(code_exp));
		}
	}
	catch (std::exception& e) {
		std::cerr << "Error during runtime: " << e.what() << std::endl;
	}

	return 0;
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
