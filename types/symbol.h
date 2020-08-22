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

#include <map>
#include "..\let.h"
#include ".\support\op_codes.h"

namespace Olly {

	/********************************************************************************************/
	//
	//                                'op_call' Class Definition
	//
	//        The op_call class encapsulates system calls to functions within the
	//        int_terpreter itself.  
	//
	/********************************************************************************************/


	class op_call {

		OP_CODE _value;


	public:

		op_call();
		op_call(const op_call& obj);
		op_call(OP_CODE val);
		op_call(str_t str);
		virtual ~op_call();

		friend  stream_t& operator >> (stream_t& stream, op_call& self);

		friend bool           __is__(const op_call& self);
		friend str_t        __type__(const op_call& self);
		friend real_t       __comp__(const op_call& self, const let& other);
		friend void          __str__(stream_t& out, const op_call& self);
		friend void         __repr__(stream_t& out, const op_call& self);
		friend OP_CODE   __op_code__(const op_call& self);
	};


	op_call::op_call() : _value() {
	}

	op_call::op_call(const op_call& obj) : _value(obj._value) {
	}

	op_call::op_call(OP_CODE val) : _value(val) {
	}

	op_call::op_call(str_t str) : _value() {

		auto it = OPERATORS.find(str);

		if (it != OPERATORS.end()) {

			_value = it->second;
		}
	}

	op_call::~op_call() {
	}

	stream_t& operator >> (stream_t& stream, op_call& self) {

		self = op_call(stream.str());

		return stream;
	}

	bool __is__(const op_call& self) {
		return self._value != OP_CODE::NOTHING_OP;
	}

	str_t __type__(const op_call& self) {
		return "op_call";
	}

	real_t __comp__(const op_call& self, const let& other) {

		const op_call* s = other.cast<op_call>();

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

	void __str__(stream_t& out, const op_call& self) {

		for (auto it = OPERATORS.cbegin(); it != OPERATORS.cend(); ++it) {

			if (it->second == self._value) {

				out << it->first;
				return;
			}
		}

		out << "unknown_operator";
	}

	void __repr__(stream_t& out, const op_call& self) {
		__str__(out, self);
	}

	OP_CODE __op_code__(const op_call& self) {

		return self._value;
	}




	/********************************************************************************************/
	//
	//                                'symbol' Class Definition
	//
	//        The symbol class defines a letiable instance which exists within
	//        letiable environment of the program.  
	//
	/********************************************************************************************/


	class symbol {

		str_t _value;


	public:

		symbol();
		symbol(const symbol& obj);
		symbol(str_t str);
		virtual ~symbol();

		friend  stream_t& operator >> (stream_t& stream, symbol& self);

		friend bool           __is__(const symbol& self);
		friend str_t        __type__(const symbol& self);
		friend real_t       __comp__(const symbol& self, const let& other);
		friend void          __str__(stream_t& out, const symbol& self);
		friend void         __repr__(stream_t& out, const symbol& self);

		friend str_t        __help__(const symbol& self);
	};


	symbol::symbol() : _value("") {
	}

	symbol::symbol(const symbol& obj) : _value(obj._value) {
	}

	symbol::symbol(str_t str) : _value(str) {
	}

	symbol::~symbol() {
	}

	stream_t& operator >> (stream_t& stream, symbol& self) {

		self = symbol(stream.str());

		return stream;
	}

	bool __is__(const symbol& self) {
		return !self._value.empty();
	}

	str_t __type__(const symbol& self) {
		return "symbol";
	}

	real_t __comp__(const symbol& self, const let& other) {

		const symbol* s = other.cast<symbol>();

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

	void __str__(stream_t& out, const symbol& self) {
		out << self._value;
	}

	void __repr__(stream_t& out, const symbol& self) {
		__str__(out, self);
	}

	str_t __help__(const symbol& self) {
		return "symbol";
	}

} // end
