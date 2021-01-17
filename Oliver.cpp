// Oliver.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>

#include "Oliver.h"
#include ".\parser\parser.h"

int main(Olly::int_type argc, char** argv) {

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

			// Olly::print("result code = " + repr(code_exp));
		}
	}
	catch (std::exception& e) {
		std::cerr << "Error during runtime: " << e.what() << std::endl;
	}

	return 0;
}