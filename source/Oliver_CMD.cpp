// Oliver_CMD.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>

#include "Oliver.h"

Olly::int_t main(Olly::int_t argc, char** argv) {

	if (argc == 1) {
		return 0;
	}

	if (argc == 2) {

		Olly::var code_exp = Olly::expression();

		{
		Olly::parser lex(argv[1]);

		lex.parse();
		// lex.convert_to_postfix();

		code_exp = lex.compile();

		lex.write_to_file();
		}

		code_exp = Olly::Oliver(code_exp);

		Olly::print(code_exp);

	}

	return 0;
}

