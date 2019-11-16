#pragma once

/********************************************************************************************/
//
//			Copyright 2019 Max J. Martin
//
//			This file is part of Oliver.
//			
//			Oliver is free software : you can redistribute it and / or modify
//			it under the terms of the GNU General Public License as published by
//			the Free Software Foundation, either version 3 of the License, or
//			(at your option) any later version.
//			
//			Oliver is distributed in the hope that it will be useful,
//			but WITHOUT ANY WARRANTY; without even the implied warranty of
//			MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//			GNU General Public License for more details.
//			
//			You should have received a copy of the GNU General Public License
//			along with Oliver.If not, see < https://www.gnu.org/licenses/>.
//			
/********************************************************************************************/

#include <algorithm>
#include <cctype>
#include <complex>
#include <deque>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <locale>
#include <map>
#include <mutex>
#include <regex>
#include <sstream>
#include <string>
#include <vector>

#ifdef _MSC_VER
#include "Windows.h"
#endif

/*  Place holder for porting over to another operating system.
#ifdef _MSC_VER

#else

#endif
*/

namespace Olly {

#if _WIN32 || _WIN64
#if _WIN64
	using int_t = long int;
	static const int ITERATION_LIMIT = 64;
#else
	using int_t = int;
	static const int ITERATION_LIMIT = 32;
#endif
#endif

#if __GNUC__
#if __x86_64__ || __ppc64__
	using int_t = long int;
	static const int ITERATION_LIMIT = 64;
#else
	using int_t = int;
	static const int ITERATION_LIMIT = 32;
#endif
#endif

	class var;

	typedef		bool						bool_t;
	typedef		char						char_t;
	typedef		std::fstream				file_t;
	typedef		long double					real_t;
	typedef		std::complex<real_t>		num_t;
	typedef		std::recursive_mutex		mutex_t;
	typedef		std::regex					regex_t;
	typedef		std::string					str_t;
	typedef		std::stringstream			stream_t;
	typedef		std::vector<str_t>			tokens_t;
	typedef		std::map<str_t, var>		map_t;
	typedef		std::vector<map_t>			env_t;
	typedef		std::vector<var>			exp_t;


	static const regex_t	INTEGER_REGEX("(\\+|-)?[[:digit:]]+");
	static const regex_t       REAL_REGEX("((\\+|-)?[[:digit:]]+)(\\.(([[:digit:]]+)?))?((e|E)((\\+|-)?)[[:digit:]]+)?", std::regex_constants::ECMAScript | std::regex_constants::optimize);
	static const regex_t    STRING_INDENT("\\n\\s*");

	static const real_t		NOT_A_NUMBER = std::numeric_limits<real_t>::quiet_NaN();

	static const			std::hash<str_t> DEFAULT_HASH_FUNCTION;

	// static const str_t		     BASE_OPS("=+-*/&|^");
	static const str_t		  ESCAPE_CHAR(" \t\r\n\a\f\v\b");

	static bool_t escape_char(char c);

	inline str_t to_lower(str_t str);
	inline str_t to_upper(str_t str);

	inline void ltrim(str_t& s);
	inline void rtrim(str_t& s);
	inline void lrtrim(str_t& s);

	inline str_t left_trim(str_t s);
	inline str_t right_trim(str_t s);
	inline str_t trim(str_t s);

	static str_t set_file_ext(str_t name, const str_t ext);

	static int_t get_op_prec(const str_t& op);

	inline bool_t is_number(const str_t& word);

	static tokens_t split(str_t str, char delim);
	static tokens_t split(const str_t& str, str_t delim);

	template<typename T> T to(std::string str);                      


	bool_t escape_char(char c) {

		if (c < 32) {
			return true;
		}

		if (::isspace(c)) {
			return true;
		}

		return false;
	}

	inline str_t to_lower(str_t str) {

		std::transform(str.begin(), str.end(), str.begin(), ::tolower);

		return str;
	}

	inline str_t to_upper(str_t str) {

		std::transform(str.begin(), str.end(), str.begin(), ::toupper);

		return str;
	}

	inline void ltrim(str_t& s) {
		s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](int ch) {
			return !std::isspace(ch);
			}));
	}

	inline void rtrim(str_t& s) {
		s.erase(std::find_if(s.rbegin(), s.rend(), [](int ch) {
			return !std::isspace(ch);
			}).base(), s.end());
	}

	inline void lrtrim(str_t& s) {
		ltrim(s);
		rtrim(s);
	}

	inline str_t left_trim(str_t s) {
		ltrim(s);
		return s;
	}

	inline str_t right_trim(str_t s) {
		rtrim(s);
		return s;
	}

	inline str_t trim(str_t s) {
		lrtrim(s);
		return s;
	}

	str_t set_file_ext(str_t name, const str_t ext) {


		auto i = name.find_last_of(".");

		if (i < name.size()) {

			name.erase(i, name.size() - i);
		}

		name += ".oll";

		return name;
	}

	int_t get_op_prec(const str_t& op) {

		if (op == "*") {
			return 3;
		}
		else if (op == "/") {
			return 3;
		}
		else if (op == "+") {
			return 2;
		}
		else if (op == "-") {
			return 2;
		}

		return 0;
	}

	inline bool_t is_number(const str_t& word) {
		return std::regex_match(word, REAL_REGEX);
	}

	tokens_t split(str_t str, char delim) {

		std::stringstream stream;
		stream.str(str);

		tokens_t tokens;

		while (std::getline(stream, str, delim)) {

			tokens.push_back(str);
		}

		return tokens;
	}

	tokens_t split(const str_t& str, str_t delim) {

		if (delim == "") {
			return split(str, ESCAPE_CHAR);
		}

		tokens_t tokens, buffer, temp;

		char d;

		d = delim.back();
		delim.pop_back();

		tokens = split(str, d);

		while (delim.size()) {

			d = delim.back();
			delim.pop_back();

			buffer = tokens;
			tokens.clear();

			for (int_t i = 0, stop = buffer.size(); i < stop; i += 1) {

				temp = split(buffer.at(i), d);

				for (int_t j = 0, stop = temp.size(); j < stop; j += 1) {

					tokens.push_back(temp.at(j));
				}

				temp.clear();
			}
		}

		return tokens;
	}

	template<typename T> T to(std::string str) {

		T n;

		std::stringstream stream;

		try {
			stream << trim(str);
			stream >> n;
		}
		catch (...) {
			n = T();
		}

		return n;
	}

	/********************************************************************************************/
	//
	//			                       Interpreter Operator Map
	//			
	/********************************************************************************************/

	static const enum OP_CODES {
		NONE_OP = 0,
		PRINT_OP, print_OP, REPR_OP, repr_OP,
		LEAD_OP, SHIFT_OP, PLACE_OP, lead_OP, shift_OP, place_OP, let_OP, const_OP,
		EQ_OP, NE_OP, LT_OP, LE_OP, GT_OP, GE_OP,
		IN_EQ_OP, IN_NE_OP, IN_LT_OP, IN_LE_OP, IN_GT_OP, IN_GE_OP,
		IS_TRUE_OP, IF_TRUE_OP, AND_OP, OR_OP, XOR_OP, NOT_OP, 
		if_OP, and_OP, or_OP, not_OP, xor_OP,
		ADD_OP, SUB_OP, MUL_OP, DIV_OP, MOD_OP, FDIV_OP,
		add_OP, sub_OP, mul_OP, div_OP, mod_OP, fdiv_OP
	};

	static const std::map<str_t, int_t> OPERATORS = {

		{ "LEAD",		 LEAD_OP },
		{ "SHIFT",		SHIFT_OP },
		{ "PLACE",		PLACE_OP },
		{ "lead",		 lead_OP },
		{ "shift",		shift_OP },
		{ "place",		place_OP },
		{ "let",		  let_OP },
		{ "const",		const_OP },

		{ "EQ",			EQ_OP },
		{ "NE",			NE_OP },
		{ "LT",			LT_OP },
		{ "LE",			LE_OP },
		{ "GT",			GT_OP },
		{ "GE",			GE_OP },

		{ "=",			IN_EQ_OP },
		{ "!=",			IN_NE_OP },
		{ "<",			IN_LT_OP },
		{ "<=",			IN_LE_OP },
		{ ">",			IN_GT_OP },
		{ ">=",			IN_GE_OP },

		{ "?",			IS_TRUE_OP },
		{ "THEN",		IF_TRUE_OP },
		{ "AND",		    AND_OP },
		{ "OR",			     OR_OP },
		{ "NOT",		    NOT_OP },
		{ "XOR",		    XOR_OP },

		{ "if",			 if_OP },
		{ "and",		and_OP },
		{ "or",			 or_OP },
		{ "not",		not_OP },
		{ "xor",		xor_OP },

		{ "ADD",		ADD_OP },
		{ "SUB",		SUB_OP },
		{ "MUL",		MUL_OP },
		{ "DIV",		DIV_OP },
		{ "MOD",		MOD_OP },
		{ "DDIV",	   FDIV_OP },

		{ "+",			add_OP },
		{ "-",			sub_OP },
		{ "*",			mul_OP },
		{ "/",			div_OP },
		{ "mod",		mod_OP },
		{ "fdiv",	   fdiv_OP },

		{ "PRINT",	    PRINT_OP },
		{ "print",	    print_OP },
		{ "REPR",	     REPR_OP },
		{ "repr",	     repr_OP },

		{ "STACK",			310 },
		{ "CODE",			320 },
		{ "VARS",			330 },
		{ "CONSTS",			340 },
		{ "CLEAR",			350 },

		{ "first",			 10000 },
		{ "second",		     40000 },
		{ "FIRST",			 50000 },
		{ "SECOND",			 60000 },

		{ "POP",			210000 },
		{ "DUP",			220000 },
		{ "SWAP",			230000 },
		{ "ROLL",			240000 },
		{ "BUFF",			250000 }
	};

} // end Olly