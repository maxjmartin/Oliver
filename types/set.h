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
#include "..\types\symbol.h"

namespace Olly {

	/********************************************************************************************/
	//
	//                              'set' Class Definition
	//
	//          The set class is a doubled ended queue.  It is implimented
	//          using two expression data types representing the front and back
	//          portions of the queue.  
	//
	/********************************************************************************************/

	class set {

		let    _set;
		str_t  _name;

	public:

		set();
		set(const set& exp);
		set(let exp);
		set(let x, let y);
		virtual ~set();

		friend str_t           __type__(const set& self);
		friend bool_t            __is__(const set& self);
		friend real_t          __comp__(const set& self, const let& other);
		friend void             __str__(stream_t& out, const set& self);
		friend void            __repr__(stream_t& out, const set& self);

		friend int_t            __len__(const set& self);
		friend let              __get__(const set& self, const let& key);
		friend let              __set__(const set& self, const let& key, const let& val);
	};

	/********************************************************************************************/
	//
	//                               'set' Class Implimentation
	//
	/********************************************************************************************/

	set::set() : _set(expression()), _name("\"set\"") {
	}

	set::set(const set& obj) : _set(obj._set), _name(obj._name) {
	}

	set::set(let exp) : _set(expression()), _name("\"set\"") {

		while (exp.is()) {

			let var = pop_lead(exp);
			let val = pop_lead(exp);

			if (val.type() == "op_call" && val.op_code() == OP_CODE::NOTHING_OP) {
				val = pop_lead(exp);

				if (to_upper(str(var)) == "NAME") {
					_name = repr(val);
				}
				else {
					let pair = expression(var, val);

					_set = _set.place_last(pair);
				}
			}
			else {
				exp = exp.place_lead(val);
				_set = _set.place_last(expression(var));
			}
		}
	}

	set::set(let x, let y) : _set(expression(x, y)), _name("\"set\"") {
	}

	set::~set() {
	}

	std::string __type__(const set& self) {
		return self._name;
	}

	bool_t __is__(const set& self) {
		return self._set.is();
	}

	real_t __comp__(const set& self, const let& other) {

		const set* e = other.cast<set>();

		if (e) {

			if (self._name == e->_name) {

				if (self._set == e->_set) {
					return 0.0;
				}
			}
		}

		return NOT_A_NUMBER;
	}

	void __str__(stream_t& out, const set& self) {

		out << "{";

		let exp = self._set;
		int_t limit = self._set.len();

		if (limit) {
			limit -= 1;
		}

		if (self._name != "\"set\"") {
			out << self._name;

			if (limit) {
				out << " ";
			}
		}

		while (exp.is()) {

			let pair = pop_lead(exp);

			if (pair.len() > 1) {
				out << str(pair.lead()) << "=" << str(pair.last());
			}
			else {
				out << str(pair.lead());
			}

			if (limit) {
				limit -= 1;
				out << " ";
			}
		}

		out << "}";
	}

	void __repr__(stream_t& out, const set& self) {

		out << "{";

		let exp = self._set;
		int_t limit = self._set.len();

		if (limit) {
			limit -= 1;
		}

		if (self._name != "set") {
			out << "name=" << self._name;

			if (limit) {
				out << " ";
			}
		}

		while (exp.is()) {

			let pair = pop_lead(exp);

			if (pair.len() > 1) {
				out << repr(first(pair)) << "=" << repr(second(pair));
			}
			else {
				out << repr(pair.lead());
			}

			if (limit) {
				limit -= 1;
				out << " ";
			}
		}

		out << "}";
	}

	int_t __len__(const set& self) {

		return self._set.len();
	}

	let __get__(const set& self, const let& key) {

		let exp = self;

		while (exp.is()) {

			let pair = pop_lead(exp);

			if (first(pair) == key) {
				return second(pair);
			}
		}

		return nothing();
	}

	let __set__(const set& self, const let& key, const let& val) {

		set new_set;

		if (to_upper(str(key)) == "NAME") {
			new_set._name = repr(val.lead());
			return new_set;
		}

		let exp = self._set;
		bool_t set = false;

		while (exp.is()) {

			let pair = pop_lead(exp);

			if (not set && first(pair) == key) {
				set = true;
				pair = expression(first(pair), val);
			}

			new_set._set = new_set._set.place_last(pair);
		}

		if (not set) {

			new_set._set = new_set._set.place_last(expression(key, val));
		}

		return new_set;
	}

}