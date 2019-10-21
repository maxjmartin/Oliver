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

#include "sys_types.h"

namespace Olly {

	/********************************************************************************************/
	//
	//                              'code_reader' class definition
	//
	//        The code_reader class opens a file and then passes each individual character
	//        to the parser for evaluation.  If a valid file is not opened then the string
	//        itself is parsed as an object of code to evaluate.  
	//
	/********************************************************************************************/

	class code_reader {

		const tokens_t&             _code;
		tokens_t::const_iterator	_i;

	public:

		code_reader(const tokens_t& input_code);
		virtual ~code_reader();

		str_t next();
		str_t peek();

		bool_t is();

	private:
		code_reader();
		code_reader(const code_reader& obj) = delete;
	};

	/********************************************************************************************/
	//
	//                               'code_reader' method definition
	//
	/********************************************************************************************/

	code_reader::code_reader() : _code(tokens_t()), _i() {
	}

	code_reader::code_reader(const tokens_t& input_code) : _code(input_code), _i() {

		if (!_code.empty()) {
			_i = _code.cbegin();
		}
	}

	code_reader::~code_reader() {
	}

	str_t code_reader::next() {
		/*
			As long as the file is not at the end of the file,
			return the current value of '_c' and set the next
			value of '_c'.  Return a null character if the
			file is eof.
		*/

		if (is()) {

			str_t t = *_i;

			++_i;

			return t;
		}

		return "";
	}

	str_t code_reader::peek() {
		/*
			Return the current value of '_c'.
		*/
		return *_i;
	}

	bool_t code_reader::is() {
		/*
			Return true if the file is not eof
			and in good condition.
		*/
		return (_i != _code.cend());
	}

} // end Olly