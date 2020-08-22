// Testbed.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>

#include "Oliver.h"
#include ".\parser\parser.h"

Olly::int_t main(Olly::int_t argc, char** argv) {


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

			// Olly::print("code = " + repr(code_exp));

			{
				Olly::evaluator olly;
				code_exp = olly.eval(code_exp);
			}

			Olly::print("code = " + repr(code_exp));
		}
	}
	catch (std::exception & e) {
		std::cerr << "Error during runtime: " << e.what() << std::endl;
	}

	return 0;
}