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

#include "..\let.h"

namespace Olly {


	/********************************************************************************************/
	//
	//                               'error' Class Definition
	//
	//        The error class provides a wrapper around normal C++ errors.  Why
	//        re-invent the wheel?
	//
	/********************************************************************************************/


	class error {

		str_t _value;

	public:

		error();
		error(const error& obj);
		error(str_t val);
		virtual ~error();

		friend str_t       __type__(const error& self);
		friend bool          __is__(const error& self);
		friend real_t      __comp__(const error& self, const let& other);
		friend void         __str__(stream_t& out, const error& self);
		friend void        __repr__(stream_t& out, const error& self);
	};


	error::error() : _value("") {
	}

	error::error(const error& obj) : _value(obj._value) {
	}

	error::error(str_t val) :  _value(val) {
	}

	error::~error() {
	}

	str_t __type__(const error& self) {
		return "ERROR";
	}

	bool __is__(const error& self) {
		return !self._value.empty();
	}

	real_t __comp__(const error& self, const let& other) {

		const error* s = other.cast<error>();

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

	void __str__(stream_t& out, const error& self) {
		out << self._value;
	}

	void __repr__(stream_t& out, const error& self) {
		__str__(out, self);
		out << "ERROR";
	}
	
} // end
