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
	//                              'list' Class Definition
	//
	//          The list class is a doubled ended queue.  It is implimented
	//          using two expression data types representing the front and back
	//          portions of the queue.  
	//
	/********************************************************************************************/

	class list {

		let    _lead;
		let    _last;
		int_t  _size;

	public:

		list();
		list(const list& exp);
		list(let exp);
		virtual ~list();

		friend str_t           __type__(const list& self);
		friend bool_t            __is__(const list& self);
		friend real_t          __comp__(const list& self, const let& other);
		friend void             __str__(stream_t& out, const list& self);
		friend void            __repr__(stream_t& out, const list& self);

		friend int_t            __len__(const list& self);
		friend let             __lead__(const list& self);
		friend let             __last__(const list& self);
		friend let       __place_lead__(const list& self, const let& other);
		friend let       __shift_lead__(const list& self);
		friend let       __place_last__(const list& self, const let& other);
		friend let       __shift_last__(const list& self);
		friend let          __reverse__(const list& self);

	private:
		void  balance();
	};

	/********************************************************************************************/
	//
	//                               'list' Class Implimentation
	//
	/********************************************************************************************/

	list::list() : _lead(__node__()), _last(__node__()), _size(0) {
	}

	list::list(const list& exp) : _lead(exp._lead), _last(exp._last), _size(exp._size) {
	}

	list::list(let exp) : _lead(__node__()), _last(__node__()), _size(0) {
		
		while (exp.is()) {

			_lead = _lead.place_lead(pop_lead(exp));

			_size += 1;
		}

		balance();
	}

	list::~list() {
	}

	std::string __type__(const list& self) {
		return "list";
	}

	bool_t __is__(const list& self) {

		if (self._lead.is() || self._last.is()) {
			return true;
		}

		return false;
	}

	real_t __comp__(const list& self, const let& other) {

		const list* e = other.cast<list>();

		if (e) {

			if ((self._lead == e->_lead) && (self._last == e->_last)) {
				return 0.0;
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

		out << str(self._lead);

		if (self._last.is()) {

			if (self._lead.is()) {
				out << " ";
			}

			out << str(self._last.reverse());
		}

		out << "]";
	}

	void __repr__(stream_t& out, const list& self) {

		if (!__is__(self)) {
			out << "[]";
			return;
		}

		out << "[";

		out << repr(self._lead);

		if (self._last.is()) {

			if (self._lead.is()) {
				out << " ";
			}

			out << repr(self._last.reverse());
		}

		out << "]";
	}

	int_t __len__(const list& self) {
		return self._size;
	}

	let __lead__(const list& self) {

		if (!self._lead.is()) {
			return self._last.lead();
		}

		return self._lead.lead();
	}

	let __last__(const list& self) {

		if (!self._last.is()) {
			return self._lead.lead();
		}

		return self._last.lead();
	}

	let __place_lead__(const list& self, const let& other) {

		if (other.is_nothing()) {
			return self;
		}

		list e = self;

		e._lead = e._lead.place_lead(other);

		e._size += 1;

		if (!e._last.is()) {
			e.balance();
		}

		return e;
	}

	let __shift_lead__(const list& self) {

		if (__len__(self) == 0) {
			return self;
		}

		list e = self;

		if (!e._lead.is()) {

			if (e._last.shift_lead().is()) {
				// Balance if _last has more than one element.
				e.balance();
			}
			else {
				e._last = e._last.shift_lead();
				return e;
			}
		}

		e._lead = e._lead.shift_lead();

		if (!e._lead.is()) {

			if (e._last.shift_lead().is()) {
				e.balance();
			}
		}

		return e;
	}

	let __place_last__(const list& self, const let& other) {

		if (other.is_nothing()) {
			return self;
		}

		list e = self;

		e._last = e._last.place_lead(other);

		e._size += 1;

		if (!e._lead.is()) {
			e.balance();
		}

		return e;
	}

	let __shift_last__(const list& self) {

		if (__len__(self) == 0) {
			return self;
		}

		list e = self;

		if (!e._last.is()) {

			if (e._lead.shift_lead().is()) {
				// Balance if _last has more than one element.
				e.balance();
			}
			else {
				e._lead = e._lead.shift_lead();
				return e;
			}
		}

		e._last = e._last.shift_lead();

		if (!e._last.is()) {

			if (e._lead.shift_lead().is()) {
				e.balance();
			}
		}

		return e;
	}

	let __reverse__(const list& self) {

		if (__len__(self) < 2) {
			return self;
		}

		list e;

		e._lead = self._last;
		e._last = self._lead;
		e._size = self._size;

		return e;
	}

	void list::balance() {

		// print("lead = " + str(_lead) + " : last = " + str(_last));

		bool_t flip = _last.is() && !_lead.is();

		if (flip) {
			std::swap(_lead, _last);
		}

		int_t i = _size < 2 ? 1 : _size / 2;

		_lead = _lead.reverse();
		_last = _last.reverse();

		while (i-- > 0) {

			_last = _last.place_lead(_lead.lead());
			_lead = _lead.shift_lead();
		}

		_lead = _lead.reverse();
		_last = _last.reverse();

		if (flip) {
			std::swap(_lead, _last);
		}

		// print("lead = " + str(_lead) + " : last = " + str(_last));
	}
}