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
	//                                The Oliver Interpreter Runtime
	//
	//          The Oliver Interpreter runtime is defined below.  It is design to run
	//			as a stack inpterpreter evaluation of the code expression passed to it.
	// 
	//
	/********************************************************************************************/

	/********************************************************************************************/
	//
	//                                'eval' Function Definitions
	//
	/********************************************************************************************/

	inline let         eval  (let exp);
	inline lambda_t    eval  (environment env, let exp);
	       lambda_t  __eval__(environment env, let exp);

	/********************************************************************************************/
	//
	//                              'function' Class Definition
	//
	//
	/********************************************************************************************/

	class function {

		environment _env;
		let         _args;
		let         _body;

	public:

		function();
		function(const function& exp);
		function(let args, let body);
		virtual ~function();

		friend str_t           __type__(const function& self);
		friend bool_t            __is__(const function& self);
		friend real_t          __comp__(const function& self, const let& other);
		friend void             __str__(stream_t& out, const function& self);
		friend void            __repr__(stream_t& out, const function& self);

		friend lambda_t          __eval__(const function& self, environment env, let& exp);
	};

	function::function() : _env(), _args(expression()), _body(expression()) {
	}

	function::function(const function& exp) : _env(exp._env), _args(exp._args), _body(exp._body) {
	}

	function::function(let args, let body) : _env(), _args(args), _body(body) {
	}

	function::~function() {
	}

	std::string __type__(const function& self) {
		return "function";
	}

	bool_t __is__(const function& self) {
		return self._args.is() || self._body.is();
	}

	real_t __comp__(const function& self, const let& other) {
		/*
			This can probably be removed.  Because an function
			always evaluates to the stack, there would never be
			a change to actually perform the comparison
			between two functions.

			However this may be useful under other circumstances.
		*/

		const function* e = other.cast<function>();

		if (e) {

			if (self._args == e->_args && self._body == e->_body) {
				return 0.0;
			}
		}

		return NOT_A_NUMBER;
	}

	void __str__(stream_t& out, const function& self) {
		out << "F" << str(self._args) << str(self._body);
	}

	void __repr__(stream_t& out, const function& self) {
		out << "F" << repr(self._args) << repr(self._body);
	}

	lambda_t __eval__(const function& self, environment env, let& exp) {
		let body = self._body;
		environment e = env;
		return lambda_t(closure_t(__eval__, e, body), null(), false);
	}

	/********************************************************************************************/
	//
	//                                'Oliver' Function Implimentation
	//
	/********************************************************************************************/

	inline let eval(let exp) {
		return trampoline(__eval__(environment(), exp));
	}

	inline lambda_t eval(environment env, let exp) {
		return lambda_t(closure_t(__eval__, env, exp), null(), false);
	}

	lambda_t __eval__(environment env, let exp) {

		if (expression_is_empty(exp)) {
			/*
				If there are no argumenta within the expression return the stack.
				Passing a null lambda function, the stack and setting the runtime to true.
			*/
			return result(env.op_stack);
		}

		let a = pop_lead(exp);

		str_t t_name = a.type();

		env.op_stack = env.op_stack.place(a);

		print(env.op_stack);


		return eval(env, exp);
	}

} // end