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

#include "var.h"

namespace Olly {

	/********************************************************************************************/
	//
	//                              'expression' Class Definition
	//
	//        The expression class is design to hold variables which are to be
	//        evaluated by the Oliver interpreter.  It is implimented using Lisp
	//		  inspired data nodes.  At one time this had been implimented with
	//        C++ std::vectors.  But given the imuttable nature of the interpreter
	//        this resulted in a signifigant slow down during run time.  
	//
	/********************************************************************************************/

	class expression {

		var _data;
		var _next;

	public:

		expression();
		expression(const expression& exp);
		expression(var obj);
		virtual ~expression();

		friend stream_t& operator >> (stream_t& stream, expression& self);

		friend str_t           __type__(const expression& self);
		friend bool_t            __is__(const expression& self);
		friend real_t          __comp__(const expression& self, var other);
		friend void             __str__(stream_t& out, const expression& self);
		friend void            __repr__(stream_t& out, const expression& self);

		friend int_t            __len__(const expression& self);
		friend var             __lead__(const expression& self);
		friend var            __place__(const expression& self, var other);
		friend var            __shift__(const expression& self);
		friend var          __reverse__(const expression& self);
	};

	expression::expression() : _data(), _next() {
	}

	expression::expression(const expression& exp) : _data(exp._data), _next(exp._next) {
	}

	expression::expression(var object) : _data(object), _next() {
	}

	expression::~expression() {
	}

	stream_t& operator >> (stream_t& stream, expression& self) {

		self = expression();

		return stream;
	}

	std::string __type__(const expression& self) {
		return "expression";
	}

	bool_t __is__(const expression& self) {

		if (self._data.nothing()) {
			return false;
		}

		return true;
	}

	real_t __comp__(const expression& self, var other) {
		/*
			This can probably be removed.  Because an expression
			always evaluates to the stack, there would never be
			a change to actually perform the comparison
			between two expressions.

			However this may be useful under other circumstances.
		*/

		const expression* e = other.cast<expression>();

		if (e) {

			var a = self;
			var b = *e;

			while (a.is() && b.is()) {

				if (a.lead() != b.lead()) {
					return NOT_A_NUMBER;
				}

				a = a.shift();
				b = b.shift();
			}

			if (!a.is() && !b.is()) {
				return 0.0;
			}
		}

		return NOT_A_NUMBER;
	}

	void __str__(stream_t& out, const expression& self) {

		if (!__is__(self)) {
			out << "()";
			return;
		}

		out << "(" << str(self._data);

		var next = self._next;

		while (next.is()) {

			out << " " << str(next.lead());

			next = next.shift();
		}

		out << ")";
	}

	void __repr__(stream_t& out, const expression& self) {

		if (!__is__(self)) {
			out << "()";
			return;
		}

		out << "(" << repr(self._data);

		var next = self._next;

		while (next.is()) {

			out << " " << repr(next.lead());

			next = next.shift();
		}

		out << ")";
	}

	int_t __len__(const expression& self) {

		if (!__is__(self)) {
			return 0;
		}

		int_t size = 1;

		var next = self._next;

		while (next.is()) {

			size += 1;

			next = next.shift();
		}

		return size;
	}

	var __lead__(const expression& self) {

		return self._data;
	}

	var __place__(const expression& self, var other) {

		if (other.nothing()) {
			return self;
		}

		expression a(other);

		if (__is__(self)) {

			a._next = self;
		}

		return a;
	}

	var __shift__(const expression& self) {
		/*  I don't think I actually need this check.  
		if (self._next.nothing()) {

			return null();
		}
		*/
		return self._next;
	}

	var __reverse__(const expression& self) {

		if (__atomic__(self)) {

			return self;
		}

		var a = expression();

		var next = self;

		while (next.is()) {

			a = a.place(next.lead());

			next = next.shift();
		}

		return a;
	}

} // end
