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
#include "expression.h"

namespace Olly {

	/********************************************************************************************/
	//
	//                              'set' Class Definition
	//
	//        The set class is design to hold variables which are to be
	//        evaluated by the Oliver interpreter.  It is almost identical to the
	//        set class, but supports less operator overloads, and allows for a
	//        clear distinction to be made between a set to hold data and a set
	//        is always evaluated to the stack.  
	//
	/********************************************************************************************/

	bool_t is_ordered_pair(var& other);

	bool_t is_ordered_pair(var& other) {

		if (other.type() == "expression") {

			if (other.atomic() || other.shift().atomic()) {
				return true;
			}
		}

		return false;
	}

	class set {

		var _set;

		static std::string _tab;

	public:

		set();
		set(const set& s);
		set(var obj);
		virtual ~set();

		friend stream_t& operator >> (stream_t& stream, set& self);

		friend str_t           __type__(const set& self);
		friend bool_t            __is__(const set& self);
		friend real_t          __comp__(const set& self, var other);
		friend void             __str__(stream_t& out, const set& self);
		friend void            __repr__(stream_t& out, const set& self);

		friend int_t            __len__(const set& self);
		friend var             __lead__(const set& self);
		friend var            __place__(const set& self, var other);
		friend var            __shift__(const set& self);
		friend var          __reverse__(const set& self);

		friend var            __slice__(const set& self, var start, var stop, var skip);
		friend var              __set__(const set& self, var val, var start, var stop, var skip);

		friend bool_t        __atomic__(const set& self);


	};

	std::string set::_tab = "\n";

	set::set() : _set(expression()) {
	}

	set::set(const set& s) : _set(s._set) {
	}

	set::set(var object) : _set(expression()) {

		_set = _set.place(object);
	}

	set::~set() {
	}

	stream_t& operator >> (stream_t& stream, set& self) {

		self = set();

		return stream;
	}

	std::string __type__(const set& self) {
		return "set";
	}

	bool_t __is__(const set& self) {
		return self._set.is();
	}

	real_t __comp__(const set& self, var other) {
		/*
		This can probably be removed.  Because an set
		always evaluates to the stack, there would never be
		a change to actually perform the comparison
		between two sets.

		However this may be useful under other circumstances.
		*/

		const set* e = other.cast<set>();

		if (e) {
			return self._set.comp(e->_set);
		}

		return NOT_A_NUMBER;
	}

	void __str__(stream_t& out, const set& self) {

		if (!__is__(self)) {
			out << "{}";
			return;
		}

		out << "{";

		self._tab += "  ";

		out << self._tab << str(self._set.lead());

		var next = self._set.shift();

		while (next.is()) {

			out << self._tab << str(next.lead());

			next = next.shift();
		}

		self._tab.pop_back();
		self._tab.pop_back();

		out << self._tab << "}";
	}

	void __repr__(stream_t& out, const set& self) {

		if (!__is__(self)) {
			out << "{}";
			return;
		}

		out << "{";

		out << repr(self._set.lead());

		var next = self._set.shift();

		while (next.is()) {

			out << repr(next.lead());

			next = next.shift();
		}

		out << "}";
	}

	int_t __len__(const set& self) {
		return self._set.len();
	}

	var __lead__(const set& self) {

		set s;

		s._set = self._set.lead();

		return s;
	}

	var __place__(const set& self, var other) {

		if (other.nothing()) {
			return self;
		}

		set s;

		if (is_ordered_pair(other)) {

			s._set = self._set.place(other);
		}
		else {
			s._set = self._set.place(expression(other));
		}
		
		return s;
	}

	var __shift__(const set& self) {

		set s;

		s._set = self._set.shift();

		return s;
	}

	var __reverse__(const set& self) {

		set s;

		s._set = self._set.reverse();

		return s;
	}

	bool_t __atomic__(const set& self) {

		return self._set.atomic();
	}

	var __slice__(const set& self, var start, var stop, var skip) {

		try {


		if (!start.nothing()) {

			var s = self._set;

			while (!s.atomic()) {

				std::cout << "type match = " << (s.lead().lead().comp(start)) << std::endl;
				// std::cout << (s.lead().lead().comp(start) == 0) << std::endl;
				// if (s.lead().lead().comp(start) == 0) {
					print(s.lead().shift().lead());
				// }				

				s = s.shift();
			}

			std::cout << "type match = " << (s.lead().lead().type() == start.type()) << std::endl;
			// std::cout << (s.lead().lead().comp(start) == 0) << std::endl;
			// if (s.lead().lead().comp(start) == 0) {
				print(s.lead().shift().lead());
			// }	
		}

		}
		catch (const std::exception & e) { // caught by reference to base
			std::cout << " a standard exception was caught, with message '"
				<< e.what() << "'\n";
		}

		return null();
	}

	var __set__(const set& self, var val, var start, var stop, var skip) {
		return null();
	}

} // end
