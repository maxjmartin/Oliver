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

#include <vector>

#include "expression.h"
#include "number.h"

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

	public:

		typedef     std::vector<let>	  list_type;

		list();
		list(const list& exp);
		list(let exp);
		virtual ~list();

		friend str_type           _type_(const list& self);
		friend bool_type            _is_(const list& self);
		friend real_type          _comp_(const list& self, const let& other);
		friend void                _str_(stream_type& out, const list& self);
		friend void               _repr_(stream_type& out, const list& self);

		friend size_type          _size_(const list& self);
		friend let                _lead_(const list& self);
		friend let                _last_(const list& self);
		friend let          _place_lead_(const list& self, const let& other);
		friend let          _shift_lead_(const list& self);
		friend let          _place_last_(const list& self, const let& other);
		friend let          _shift_last_(const list& self);
		friend let             _reverse_(const list& self);

		friend let                 _get_(const list& self, const let& key);
		friend let                 _set_(const list& self, const let& key, const let& val);
		friend let                 _itr_(const list& self);

		friend bool_type     _interable_(const list& self);

		const list_type& get_list() const;

		let begin();

	private:
		list_type    _list;
	};

	/********************************************************************************************/
	//
	//                               'list' Class Implimentation
	//
	/********************************************************************************************/

	list::list() : _list() {
	}

	list::list(const list& exp) : _list(exp._list) {
	}

	list::list(let exp) : _list() {
		
		_list.reserve(exp.size());

		while (exp.is()) {

			_list.emplace_back(pop_lead(exp));
		}
	}

	list::~list() {
	}

	std::string _type_(const list& self) {
		return "list";
	}

	bool_type _is_(const list& self) {

		if (_size_(self) > 0) {
			return true;
		}

		return false;
	}

	real_type _comp_(const list& self, const let& other) {

		const list* e = other.cast<list>();

		if (e) {

			if (self._list == e->_list) {
				return 0.0;
			}
		}

		return NOT_A_NUMBER;
	}

	void _str_(stream_type& out, const list& self) {

		if (self._list.empty()) {
			out << "[]";
			return;
		}

		out << "[";

		for (auto i = self._list.cbegin(); i != self._list.cend(); ++i) {

			i->str(out);
			out << " ";
		}

		out.seekp(-1, out.cur);

		out << "]";
	}

	void _repr_(stream_type& out, const list& self) {

		if (self._list.empty()) {
			out << "[]";
			return;
		}

		out << "[";

		for (auto i = self._list.cbegin(); i != self._list.cend(); ++i) {

			i->repr(out);
			out << " ";
		}

		out.seekp(-1, out.cur);

		out << "]";
	}

	size_type _size_(const list& self) {
		return self._list.size();
	}

	let _lead_(const list& self) {

		if (self._list.empty()) {
			return nothing();
		}

		return self._list.front();
	}

	let _last_(const list& self) {

		if (self._list.empty()) {
			return nothing();
		}

		return self._list.back();
	}

	let _place_lead_(const list& self, const let& other) {

		if (other.is_nothing()) {
			return self;
		}

		list e = self;

		e._list.emplace(e._list.begin(), other);

		return e;
	}

	let _shift_lead_(const list& self) {

		if (_size_(self) == 0) {
			return self;
		}

		list e = self;

		e._list.erase(e._list.begin());

		return e;
	}

	let _place_last_(const list& self, const let& other) {

		if (other.is_nothing()) {
			return self;
		}

		list e = self;

		e._list.emplace_back(other);

		return e;
	}

	let _shift_last_(const list& self) {

		if (_size_(self) == 0) {
			return self;
		}

		list e = self;

		e._list.pop_back();

		return e;
	}

	let _reverse_(const list& self) {

		if (_size_(self) < 2) {
			return self;
		}

		list e;

		std::reverse(e._list.begin(), e._list.end());

		return e;
	}

	let _get_(const list& self, const let& key) {

		const number* n = key.cast<number>();

		if (n) {

			auto i = n->integer();

			if (i == 0 || std::abs(i) > _size_(self)) {
				return nothing();
			}

			list l = self;

			bool_type rev = i < 0 ? true : false;

			if (rev) {
				int_type len = static_cast<int_type>(l._list.size());
				i = len + i;
			}
			else {
				i -= 1;
			}

			return l._list[i];
		}


		return nothing();
	}

	let _set_(const list& self, const let& key, const let& val) {

		const number* n = key.cast<number>();

		if (n) {

			auto i = n->integer();

			if (i == 0 || std::abs(i) > _size_(self)) {
				return nothing();
			}

			list l = self;

			bool_type rev = i < 0 ? true : false;

			if (rev) {
				int_type len = static_cast<int_type>(l._list.size());
				i = len + i;
			}
			else {
				i -= 1;
			}

			l._list[i] = val;

			return l;
		}


		return nothing();
	}

	let _itr_(const list& self) { 

		auto i = self._list.begin();

		return nothing();
	}

	bool_type _interable_(const list& self) {
		return true;
	}

	inline const Olly::list::list_type& Olly::list::get_list() const {
		return _list;
	}
}