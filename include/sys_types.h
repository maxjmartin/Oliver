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
// #include <codecvt>
#include <complex>
// #include <cstddef>
// #include <cstdio>
// #include <ctime>
// #include <chrono>
#include <deque>
#include <fstream>
// #include <functional>
#include <iomanip>
#include <iostream>
// #include <iterator>
// #include <limits>
// #include <list>
#include <locale>
#include <map>
// #include <memory>
#include <mutex>
// #include <random>
#include <regex>
// #include <stdexcept>
#include <sstream>
#include <string>
// #include <typeinfo>
// #include <type_traits>
// #include <utility>
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
#else
	using int_t = int;
#endif
#endif

#if __GNUC__
#if __x86_64__ || __ppc64__
	using int_t = long int;
#else
	using int_t = int;
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

	static inline str_t to_lower(str_t str);
	static inline str_t to_upper(str_t str);

	static inline void ltrim(str_t& s);
	static inline void rtrim(str_t& s);
	static inline void lrtrim(str_t& s);

	static inline str_t left_trim(str_t s);
	static inline str_t right_trim(str_t s);
	static inline str_t trim(str_t s);

	static inline str_t correct_ml_indentions(str_t& str);

	static str_t set_file_ext(str_t name, const str_t ext);

	static int_t get_op_prec(const str_t& op);

	static inline bool_t is_number(const str_t& word);

	static tokens_t split(str_t str, char delim);
	static tokens_t split(const str_t& str, str_t delim);

	/*
		A general std::string conversion function to a type.
		Objects must overload std::stringstream '>>' to work.
		TODO: Determine if this function needs supported to
		allow the removal of the stream '>>' operator from 
		the overall definition of the var data type.  
	*/
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

	inline str_t correct_ml_indentions(str_t& str) {
		/*
			Trim any new lines followed by ' ' chars to allow
			multi line text strings to be definable within a
			single vertical scope.
		*/

		str_t result;
		std::regex_replace(std::back_inserter(result), str.begin(), str.end(), STRING_INDENT, "\n");

		lrtrim(result);

		return result;
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

	static const std::map<str_t, int_t> OPERATORS = {

		{ "LAMBDA",			 1 },
		{ ":",				 2 },
		{ ";",				 3 },

		{ "PSTACK",			10 },
		{ "PCODE",			11 },
		{ "EMIT",			12 },

		{ "DEF",			21 },
		{ "CON",			22 },
		{ "GET",			23 },
		{ "def",			24 },

		{ "ADD",			31 },
		{ "SUB",			32 },

		/*
		{ "LEAD",        1 },
		{ "LAST",        2 },
		{ "L_SHIFT",     3 },
		{ "R_SHIFT",     4 },
		{ "PLACE",       5 },
		{ "PUSH",        6 },
		{ "STACK",       7 },
		{ "LEN",         8 },
		{ "REVERSE",     9 },
		{ "CLEAR",      10 },

		{ "const",      11 },
		{ "def",        12 },
		{ "lambda",     13 },
		{ "if",         14 },
		{ "else",       15 },
		{ "while",      16 },
		{ "do",         17 },
		{ "for",        18 },
		{ "DEF",        19 },
		{ "#",          20 },
		{ ".",          21 },
		{ "self",       22 },
		{ "EMIT",       23 },
		{ "print",      24 },
		{ "pstack",     25 },
		{ "pcode",      26 },
		{ "null",       27 },
		{ "now",        28 },
		{ "input",      29 },

		{ "bool",      31 },
		{ "date",      32 },
		{ "polar",     33 },
		{ "regex",     34 },
		{ "type",      35 },
		{ "rand",      36 },

		{ "term",      41 },
		{ "term?",     42 },       // A small (probably apocryphal) tribute to Douglas Adams.
		{ "weight",    43 },
		{ "weight?",   44 },

		{ "hash",      51 },
		{ "str",       52 },
		{ "rpr",       53 },
		{ "int",       54 },
		{ "real",      55 },
		{ "complex",   56 },

		{ "comp",      61 },
		{ "==",        62 },
		{ "!=",        63 },
		{ ">=",        64 },
		{ ">",         65 },
		{ "<=",        67 },
		{ "<",         68 },

		{ "is",        71 },
		{ "and",       72 },
		{ "or",        73 },
		{ "xor",       74 },
		{ "not",       75 },

		{ "&",         81 },
		{ "|",         82 },
		{ "^",         83 },
		{ "~",         84 },
		{ "<<",        85 },
		{ ">>",        86 },

		{ "lead",      91 },
		{ "last",      92 },
		{ "shift",     93 },
		{ "slide",     94 },
		{ "place",     95 },
		{ "push",      96 },

		{ "card",      101 },
		{ "len",       102 },
		{ "reverse",   103 },
		{ "find",      104 },
		{ "has",       105 },
		{ "slice",     106 },

		{ "ADD",       111 },
		{ "SUB",       112 },
		{ "MUL",       113 },
		{ "DIV",       114 },
		{ "MOD",       115 },
		{ "POW",       116 },
		{ "FDIV",      117 },
		{ "REM",       118 },
		{ "INC",       126 },
		{ "DEC",       127 },

		{ "abs",       121 },
		{ "ceil",      122 },
		{ "floor",     123 },
		{ "max",       124 },
		{ "min",       125 },
		{ "inc",       126 },
		{ "dec",       127 },

		{ "exp",       131 },
		{ "exp2",      132 },
		{ "log",       133 },
		{ "log10",     134 },
		{ "log2",      135 },
		{ "sqrt",      136 },
		{ "cbrt",      137 },

		{ "sin",       141 },
		{ "cos",       142 },
		{ "tan",       143 },
		{ "asin",      144 },
		{ "acos",      145 },
		{ "atan",      146 },

		{ "sinh",      151 },
		{ "cosh",      152 },
		{ "tanh",      153 },
		{ "asinh",     154 },
		{ "acosh",     155 },
		{ "atanh",     156 },

		{ "hypot",     161 },
		{ "atan2",     162 },
		{ "divmod",    163 },
		{ "phase",     164 },
		{ "norm",      165 },
		{ "conj",      166 },
		{ "proj",      167 },

		{ "finite?",   171 },
		{ "inf?",      172 },
		{ "nan?",      173 },
		{ "negative?", 174 },
		{ "positive?", 175 },
		{ "normal?",   176 },
		{ "complex?",  177 },

		{ "match",     181 },
		{ "search",    182 },
		{ "replace",   183 },
		{ "split",     184 },
		*/
	};

} // end Olly