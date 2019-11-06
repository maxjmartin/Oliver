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

#include "let.h"

namespace Olly {


	/********************************************************************************************/
	//
	//                               'string' Class Definition
	//
	//        The string class provides a wrapper around normal C++ strings.  Why
	//        re-invent the wheel?
	//
	/********************************************************************************************/


	class string {

		str_t _value;

	public:

		string();
		string(const string& obj);
		string(str_t str);
		string(const char& c);
		virtual ~string();

		friend  stream_t& operator >> (stream_t& stream, string& self);

		friend str_t       __type__(const string& self);
		friend bool          __is__(const string& self);
		friend real_t      __comp__(const string& self, const let& other);
		friend void         __str__(stream_t& out, const string& self);
		friend void        __repr__(stream_t& out, const string& self);

		friend int_t        __len__(const string& self);
		friend let         __lead__(const string& self);
		friend let        __place__(const string& self, const let& other);
		friend let        __shift__(const string& self);
		friend let      __reverse__(const string& self);

		friend bool_t  __iterable__(const string& self);
	};


	string::string() : _value("") {
	}

	string::string(const string& obj) : _value(obj._value) {
	}

	string::string(str_t str) : _value(str) {
	}

	string::string(const char& c) : _value(str(c)) {
	}

	string::~string() {
	}

	stream_t& operator >> (stream_t& stream, string& self) {

		self = string(stream.str());

		return stream;
	}

	str_t __type__(const string& self) {
		return "string";
	}

	bool __is__(const string& self) {
		return !self._value.empty();
	}

	real_t __comp__(const string& self, const let& other) {

		const string* s = other.cast<string>();

		if (s) {

			if (self._value > s->_value) {
				return 1.0;
			}

			if (self._value < s->_value) {
				return -1.0;
			}

			return 0.0;
		}
		return NOT_A_NUMBER;
	}

	void __str__(stream_t& out, const string& self) {
		out << self._value;
	}

	void __repr__(stream_t& out, const string& self) {
		out << "\"";
		__str__(out, self);
		out << "\"";
	}

	int_t __len__(const string& self) {
		return (int_t)self._value.size();
	}

	let __lead__(const string& self) {

		return string(self._value.front());
	}

	let __place__(const string& self, const let& other) {

		const string* s = other.cast<string>();

		if (s) {

			string r;

			r._value = s->_value + self._value;

			return r;
		}

		return null();
	}

	let __shift__(const string& self) {

		if (self._value.empty()) {
			return self;
		}

		string t = self;
		t._value.erase(0, 1);

		return t;
	}

	let __reverse__(const string& self) {

		string l = self;

		std::reverse(l._value.begin(), l._value.end());

		return l;
	}

	bool_t __iterable__(const string& self) {
		return true;
	}

} // end
