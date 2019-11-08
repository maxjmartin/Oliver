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
	//                                  'list' Class Definition
	//
	//          The list class is implimented using a linked list expression.  This has
	//          been chosen over a std::vector becuase of the cost of recreating the 
	//          vector in an immutable environment.  
	//
	/********************************************************************************************/

	class list {

		let    _list;
		int_t  _len;

	public:

		list();
		list(const list& exp);
		list(let x);
		list(let x, let y);
		virtual ~list();

		friend str_t           __type__(const list& self);
		friend bool_t            __is__(const list& self);
		friend real_t          __comp__(const list& self, const let& other);
		friend void             __str__(stream_t& out, const list& self);
		friend void            __repr__(stream_t& out, const list& self);

		friend int_t            __len__(const list& self);
		friend let             __lead__(const list& self);
		friend let            __place__(const list& self, const let& other);
		friend let            __shift__(const list& self);
		friend let          __reverse__(const list& self);
	};

	/********************************************************************************************/
	//
	//                               'list' Class Implimentation
	//
	/********************************************************************************************/

	list::list() : _list(node()), _len(0) {
	}

	list::list(const list& exp) : _list(exp._list), _len(exp._len) {
	}

	list::list(let x) : _list(node(x)), _len(0) {
		if (_list.is()) {
			_len = 1;
		}
	}

	list::list(let x, let y) : _list(node()), _len(0) {
		_list = _list.place(y);
		_list = _list.place(x);

		_len = _list.len();
	}

	list::~list() {
	}
	
	std::string __type__(const list& self) {
		return "list";
	}

	bool_t __is__(const list& self) {

		if (self._len) {
			return true;
		}

		return false;
	}

	real_t __comp__(const list& self, const let& other) {
		/*
			This can probably be removed.  Because an list
			always evaluates to the stack, there would never be
			a change to actually perform the comparison
			between two lists.

			However this may be useful under other circumstances.
		*/

		const list* e = other.cast<list >();

		if (e) {

			if (self._len == e->_len) {
				return (self._list == e->_list);
			}
		}

		return NOT_A_NUMBER;
	}

	void __str__(stream_t& out, const list& self) {

		if (!__is__(self)) {
			out << "[]";
			return;
		}

		out << "[";

		out << str(self._list);

		out << "]";
	}

	void __repr__(stream_t& out, const list& self) {

		if (!__is__(self)) {
			out << "[]";
			return;
		}

		out << "[";

		out << repr(self._list);

		out << "]";
	}
	
	int_t __len__(const list& self) {

		return self._len;
	}
	
	let __lead__(const list& self) {

		return self._list.lead();
	}

	let __place__(const list& self, const let& other) {

		if (other.nothing()) {
			return self;
		}

		list e = self;

		e._list = e._list.place(other);
		e._len += 1;

		return e;
	}

	let __shift__(const list& self) {

		if (self._len == 0) {
			return null();
		}

		list e = self;

		e._list = e._list.shift();
		e._len -= 1;

		return e;
	}

	let __reverse__(const list& self) {

		if (self._len < 2) {
			return self;
		}

		list e = self;

		e._list = e._list.reverse();

		return e;
	}
}