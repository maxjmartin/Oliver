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
	//                              'text_reader' class definition
	//
	//        The text_reader class opens a file and then passes each individual character
	//        to the parser for evaluation.  If a valid file is not opened then the string
	//        itself is iterated over for reading.  
	//
	/********************************************************************************************/

	class text_reader {

		char_t      _c;
		file_t		_input;
		bool_t      _file;
		stream_t    _stream;
		mutex_t		_mutex;
		bool_t		_locked;

	public:

		text_reader(const str_t& inp);
		virtual ~text_reader();

		char_t next();
		char_t peek();

		str_t get_line();

		bool_t is();

		bool_t is_file();

	private:
		text_reader();
		text_reader(const text_reader& obj) = delete;
	};

	/********************************************************************************************/
	//
	//                               'text_reader' method definition
	//
	/********************************************************************************************/

	text_reader::text_reader() : _c('\0'), _input(""), _file(false), _stream(), _mutex(), _locked(false) {
	}

	text_reader::text_reader(const str_t& input_code) : _c('\0'), _input(input_code, file_t::in), _file(true), _stream(), _mutex(), _locked(_mutex.try_lock()) {
		/*
			Open a file 'file_name' and retrieve its first character
			Setting the value of the character to '_c'.  So long as
			the file is not empty.
		*/


		if (!_input.eof() && _input.good()) {

			if (!_locked) {
				return;
			}

			_c = _input.get();
		}
		else {
			/*
				Not a file, so send the text to a stream to be
				treated just like a file.  
			*/
			_file = false;
			_stream << input_code;

			_c = _stream.get();
		}
	}

	text_reader::~text_reader() {
		_input.close();

		if (_locked) {
			_mutex.unlock();
		}
	}

	char_t text_reader::next() {
		/*
			As long as the file is not at the end of the file,
			return the current value of '_c' and set the next
			value of '_c'.  Return a null character if the
			file is eof.
		*/

		if (is()) {

			char_t t = _c;

			if (_file) {
				_input >> std::noskipws >> _c;
			}
			else {
				_stream >> std::noskipws >> _c;
			}

			return t;
		}

		return '\0';
	}

	char_t text_reader::peek() {
		/*
			Return the current value of '_c'.
		*/
		return _c;
	}

	bool_t text_reader::is() {
		/*
			Return true if the file is not eof
			and in good condition.
		*/

		if (_file) {
			return (!_input.eof() && _input.good());
		}
		return (!_stream.eof() && _stream.good());
	}

	bool_t text_reader::is_file() {
		/*
			Return true if the file is not eof
			and in good condition.
		*/

		return _file;
	}

	str_t text_reader::get_line() {

		if (_file && !_input.eof() && _input.good()) {

			str_t line;

			std::getline(_input, line);

			lrtrim(line);

			if (line.empty()) {
				return "";
			}

			return line;
		}

		return "";
	}

} // end Olly