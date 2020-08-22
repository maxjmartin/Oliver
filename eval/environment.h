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
#include <vector>

#include "..\let.h"

namespace Olly {

	/********************************************************************************************/
	//
	//                                'environement' Class Definition
	//
	//			The environment class defines a data space of type to be passes between
	//          function calls of a trampolined process.  
	//
	/********************************************************************************************/
	/*
	class environment {

		typedef     std::map<str_t, let>  map_t;
		typedef     std::vector<let>	  stack_t;
		typedef     std::vector<map_t>	  closure_t;

		closure_t     _constants;
		closure_t     _variables;
		stack_t       _stack;
		stack_t       _code;

	public:
		environment();
		environment(environment& env);

		static int_t count;

		stack_t& code();
		stack_t& stack();

		let get(let& a) const;

		void set_constant(let& a, let& b);
		void set_variable(let& a, let& b);

		void return_stack_forward();

		void define_closure();
		void delete_closure();

	private:
		let get(str_t& a) const;

		void set_constant(str_t& a, let& b);
		void set_variable(str_t& a, let& b);
	};
	*/
	/********************************************************************************************/
	//
	//                              'environment' Class Implimentation
	//
	/********************************************************************************************/
	/*
	int_t environment::count = 0;

	environment::environment() : _constants(), _variables(), _stack(), _code() {
	}

	environment::environment(environment& env) : _constants(env._constants), _variables(env._variables), _stack(env._stack), _code(env._code) {
	}

	environment::stack_t& Olly::environment::code() {
		return _code;
	}

	environment::stack_t& Olly::environment::stack() {
		return _stack;
	}

	let environment::get(let& a) const {

		str_t i = str(a);

		return get(i);
	}

	void environment::set_constant(let& a, let& b) {

		str_t i = str(a);

		set_constant(i, b);
	}

	void environment::set_variable(let& a, let& b) {

		str_t i = str(a);

		set_variable(i, b);
	}

	void environment::return_stack_forward() {
	}

	void environment::define_closure() {

		_constants.push_back(map_t());
		_variables.push_back(map_t());
	}

	void environment::delete_closure() {

		if (!_constants.empty()) {
			_constants.pop_back();
		}

		if (!_variables.empty()) {
			_variables.pop_back();
		}
	}

	void environment::set_constant(str_t& a, let& b) {

		auto c_itr = _constants.back().find(a);

		if (c_itr != _constants.back().end()) {
			return;
		}

		_constants.back()[a] = b;
	}

	void environment::set_variable(str_t& a, let& b) {

		auto c_itr = _constants.back().find(a);

		if (c_itr != _constants.back().end()) {
			return;
		}

		auto v_itr = _variables.back().find(a);

		if (v_itr != _variables.back().end()) {
			v_itr->second = b;
			return;
		}

		_variables.back()[a] = b;
	}

	let environment::get(str_t& a) const {

		for (auto i = _variables.crbegin(); i != _variables.crend(); ++i) {

			auto v_itr = i->find(a);

			if (v_itr != i->end()) {
				return v_itr->second;
			}
		}

		for (auto i = _constants.crbegin(); i != _constants.crend(); ++i) {

			auto c_itr = i->find(a);

			if (c_itr != i->end()) {
				return c_itr->second;
			}
		}

		return nothing();
	}
	*/
} // end