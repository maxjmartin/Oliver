#pragma once

/********************************************************************************************/
//
//			Copyright 2019 Max J. Martin
//
//		  	This file is part of Oliver.
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

#include "..\let.h";

namespace Olly {

	/********************************************************************************************/
	//
	//                              'token_reader' class definition
	//
	//        The token_reader class simply manages iteration over a series of text tokens.
	//
	/********************************************************************************************/

	class token_reader {

	public:

		token_reader(const tokens_t& input_code);
		virtual ~token_reader();

		str_t next();
		str_t peek();

		bool_t is();

	private:

		const tokens_t& _code;
		tokens_t::const_iterator	_i;

		token_reader();
		token_reader(const token_reader& obj) = delete;
	};

	/********************************************************************************************/
	//
	//                               'token_reader' method definition
	//
	/********************************************************************************************/

	token_reader::token_reader() : _code(tokens_t()), _i() {
	}

	token_reader::token_reader(const tokens_t& input_code) : _code(input_code), _i() {

		if (!_code.empty()) {
			_i = _code.cbegin();
		}
	}

	token_reader::~token_reader() {
	}

	str_t token_reader::next() {

		if (is()) {

			str_t t = *_i;

			++_i;

			return t;
		}

		return "";
	}

	str_t token_reader::peek() {
		/*
			Return the current value of '_c'.
		*/
		return *_i;
	}

	bool_t token_reader::is() {
		/*
			Return true if the file is not eof
			and in good condition.
		*/
		return (_i != _code.cend());
	}

} // end Olly