// Oliver_CMD.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>

#include "Oliver.h"

Olly::int_t main(Olly::int_t argc, char** argv) {

	try {

		Olly::var a = Olly::expression();
		a = a.place(Olly::number(10));
		a = a.place(Olly::number(9));
		a = a.place(Olly::number(8));
		a = a.place(Olly::number(7));
		a = a.place(Olly::number(6));
		a = a.place(Olly::number(5));
		a = a.place(Olly::number(4));
		a = a.place(Olly::number(3));
		a = a.place(Olly::number(2));
		a = a.place(Olly::number(1));

		Olly::var b = a;
		b = b.place(a);

		Olly::print(a);
		Olly::print(b);

		std::cout << std::boolalpha << (a.eq(b)) << std::endl;
		Olly::print();

		print(b);
		print(Olly::Oliver(b));
		
	} 
	catch (const std::exception & e) { // caught by reference to base
	std::cout << " a standard exception was caught, with message '"
		<< e.what() << "'\n";
	}

 /*
	if (argc == 1) {

		return 0;
	}

	if (argc == 2) {

		// expression code_exp;

		{
		//	Olly::parser lex(argv[1]);

		//	lex.parse();
		//	lex.convert_to_postfix();

		//	lex.compile();

		//	lex.write_to_file();

		//	code_exp = lex.compile();
		}

	//	Oliver olly;

	//	olly.init_env();

	//	olly.init_code(code_exp);

	//	olly.eval();
	}
*/

	return 0;
}

