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
#include "expression.h"

namespace Olly {

	
	/********************************************************************************************/
	//
	//                              'lambda' Class Definition
	//
	//          The lambda class is a doubled ended queue.  It is implimented
	//          using two expression data types representing the front and back
	//          portions of the queue.  
	//
	/********************************************************************************************/

	typedef     std::map<str_type, let>    map_type;

	class lambda {

		let    _args;
		let    _body;

		map_type  _variables;

	public:

		lambda();
		lambda(const lambda& exp);
		lambda(let exp);
		lambda(let args, let body);
		virtual ~lambda();

		friend str_type           _type_(const lambda& self);
		friend bool_type            _is_(const lambda& self);
		friend real_type          _comp_(const lambda& self, const let& other);
		friend void                _str_(stream_type& out, const lambda& self);
		friend void               _repr_(stream_type& out, const lambda& self);

		friend size_type          _size_(const lambda& self);
		friend let                _lead_(const lambda& self);
		friend let                _last_(const lambda& self);

		void bind_scope(const map_type& scope);
		void bind_variable(let var, let val);

		map_type variables() const;

		void print_enclosure() const;
	};

	/********************************************************************************************/
	//
	//                               'lambda' Class Implimentation
	//
	/********************************************************************************************/

	lambda::lambda() : _args(expression()), _body(expression()), _variables() {
	}

	lambda::lambda(const lambda& exp) : _args(exp._args), _body(exp._body), _variables(exp._variables) {
	}

	lambda::lambda(let exp) : _args(), _body(), _variables() {

		const lambda* l = exp.cast<lambda>();

		if (l) {
			_args      = l->_args;
			_body      = l->_body;
			_variables = l->_variables;
		}
	}

	lambda::lambda(let args, let body) : _args(args), _body(body), _variables() {
	}

	lambda::~lambda() {
	}

	std::string _type_(const lambda& self) {
		return "lambda";
	}

	bool_type _is_(const lambda& self) {

		if (self._args.is() || self._body.is()) {
			return true;
		}

		return false;
	}

	real_type _comp_(const lambda& self, const let& other) {

		const lambda* e = other.cast<lambda>();

		if (e) {

			if ((self._args == e->_args) && (self._body == e->_body)) {
				return 0.0;
			}
		}

		return NOT_A_NUMBER;
	}

	void _str_(stream_type& out, const lambda& self) {

		if (!_is_(self)) {
			out << "lambda():;";
			return;
		}

		out << "lambda";

		out << str(self._args);

		out << ":";

		let body = self._body;

		while (body.is()) {
			out << str(pop_lead(body));
		}

		out << ";";
	}

	void _repr_(stream_type& out, const lambda& self) {

		if (!_is_(self)) {
			out << "lambda():;";
			return;
		}

		out << "lambda";

		out << repr(self._args);

		out << ": ";

		let body = self._body;

		while (body.is()) {
			out << repr(pop_lead(body)) << " ";
		}

		out << ";";
	}

	size_type _size_(const lambda& self) {
		return self._args.size();
	}

	let _lead_(const lambda& self) {

		if (!self._args.is()) {
			return nothing();
		}

		return self._args;
	}

	let _last_(const lambda& self) {

		if (!self._body.is()) {
			return nothing();
		}

		return self._body;
	}

	inline void Olly::lambda::bind_scope(const map_type& scope) {
		
		auto name_loc = scope.find("self");

		auto name = (name_loc == scope.cend()) ? "" : str(name_loc->second);

		for (auto itr : scope) {
			
			if (itr.first != name) {

				_variables[itr.first] = itr.second;
			}
		}
	}

	inline void lambda::bind_variable(let var, let val) {
		_variables[str(var)] = val;
	}

	inline map_type Olly::lambda::variables() const {
		return _variables;
	}

	inline void Olly::lambda::print_enclosure() const {

		for (auto itr : _variables) {
			std::cout << itr.first << " = " << str(itr.second) << std::endl;
		}
	}
}