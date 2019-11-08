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
#include "eval_functions.h"

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

	inline let         eval(let& exp);
	inline lambda_t    eval(environment env, let exp);

	inline let         eval_expression  (environment& env, let& exp);
	       lambda_t  __eval_expression__(environment  env, let  exp);

	inline let         eval_function(let& func, environment& env, let& exp);

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

		friend lambda_t        __eval__(const function& self, environment env, let& exp);
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
		out << "function" << str(self._args) << str(self._body);
	}

	void __repr__(stream_t& out, const function& self) {
		out << "function" << repr(self._args) << repr(self._body);
	}

	lambda_t __eval__(const function& self, environment env, let& exp) {

		let keys = self._args; 

		while (keys.is()) {

			let value = pop_lead(exp);

			env.variables = set_variable(env.variables, pop_lead(keys), value);
		}

		return eval(env, self._body);
	}

	/********************************************************************************************/
	//
	//                                'Oliver' Function Implimentation
	//
	/********************************************************************************************/

	inline let eval(let& exp) {
		return trampoline(__eval_expression__(environment(), exp));
	}

	inline lambda_t eval(environment env, let exp) {
		return lambda_t(closure_t(__eval_expression__, env, exp), null(), false);
	}

	inline let eval_function(let& func, environment& env, let& exp) {
		return trampoline(func.eval(env, exp));
	}

	inline let eval_expression(environment& env, let& exp) {
		return trampoline(__eval_expression__(env, exp));
	}

	lambda_t __eval_expression__(environment env, let exp) {

		if (expression_is_empty(exp)) {
			/*
				If there are no argumenta within the expression return the stack.
				Passing a null lambda function, the stack and setting the runtime to true.
			*/
			return result(env.return_stack);
		}

		let a = pop_lead(exp);

		str_t t_name = a.type();

		while (t_name == "symbol") {
			a = get_symbol(env, a);
			t_name = a.type();
		}

		if (t_name == "expression") {

			env.return_stack = eval_expression(env, a);
		}

		else if (t_name == "function") {
			env.return_stack = eval_function(a, env, exp);
		}

		else if (t_name != "op_call") {
			env.return_stack = env.return_stack.place(a);
		}

		else {

			switch (a.integer()) {

			case 1:	// lead
			{
				env.return_stack = env.return_stack.place(pop_lead(exp).lead());
			}	break;

			case 2:	// shift
			{
				env.return_stack = env.return_stack.place(pop_lead(exp).shift());
			}	break;

			case 3:	// place
			{
				let x = pop_lead(exp);
				env.return_stack = env.return_stack.place(pop_lead(exp).place(x));
			}	break;

			case 4:	// is?
			{
				env.return_stack = env.return_stack.place(boolean(pop_lead(exp).is()));
			}	break;

			case 5:	// let
			{
				let   key = pop_lead(exp);
				str_t op = str(pop_lead(exp));
				let   value = pop_lead(exp);

				if (op == "=") {
					env.variables = set_variable(env.variables, key, value);
				}
			}	break;

			case 6:	// const
			{
				let   key = pop_lead(exp);
				str_t op = str(pop_lead(exp));
				let   value = pop_lead(exp);

				if (op == "=") {
					env.constants= set_constant(env.constants, key, value);
				}
			}	break;

			case 7:	// if
			{
				let   key = pop_lead(exp);
				str_t op = str(pop_lead(exp));
				let   value = pop_lead(exp);

				if (op == "=") {
					env.constants = set_constant(env.constants, key, value);
				}
			}	break;

			case 8:	// print
			{
				let args = expression(pop_lead(exp));

				args = eval_expression(env, args).reverse();
				
				stream_t stream;

				stream << std::boolalpha;

				while (args.is()) {
					pop_lead(args).str(stream);
				}

				std::cout << stream.str();
			}	break;

			case 11: // EQ (Equility)
			{
				let y = pop_lead(env.return_stack);
				let x = pop_lead(env.return_stack);
				env.return_stack = env.return_stack.place(boolean(x == y));
			}	break;

			case 12: // NE (Postfix Non Equility)
			{
				let y = pop_lead(env.return_stack);
				let x = pop_lead(env.return_stack);
				env.return_stack = env.return_stack.place(boolean(x != y));
			}	break;

			case 13: // LT (Postfix Less Than)
			{
				let y = pop_lead(env.return_stack);
				let x = pop_lead(env.return_stack);
				env.return_stack = env.return_stack.place(boolean(x < y));
			}	break;

			case 14: // LE (Postfix Less Than Equal)
			{
				let y = pop_lead(env.return_stack);
				let x = pop_lead(env.return_stack);
				env.return_stack = env.return_stack.place(boolean(x <= y));
			}	break;

			case 15: // GT (Postfix Greater Than)
			{
				let y = pop_lead(env.return_stack);
				let x = pop_lead(env.return_stack);
				env.return_stack = env.return_stack.place(boolean(x > y));
			}	break;

			case 16: // GE (Postfix Greater Than Equal)
			{
				let y = pop_lead(env.return_stack);
				let x = pop_lead(env.return_stack);
				env.return_stack = env.return_stack.place(boolean(x >= y));
			}	break;

			case 21: // = (Prefix Equility)
			{
				let x = pop_lead(exp);
				exp = exp.place(op_call("EQ"));
				exp = exp.place(x);
			}	break;

			case 22: // != (Prefix Non Equility)
			{
				let x = pop_lead(exp);
				exp = exp.place(op_call("NE"));
				exp = exp.place(x);
			}	break;

			case 23: // < (Prefix Less Than)
			{
				let x = pop_lead(exp);
				exp = exp.place(op_call("LT"));
				exp = exp.place(x);
			}	break;

			case 24: // <= (Prefix Less Than Equal)
			{
				let x = pop_lead(exp);
				exp = exp.place(op_call("LE"));
				exp = exp.place(x);
			}	break;

			case 25: // > (Prefix Greater Than)
			{
				let x = pop_lead(exp);
				exp = exp.place(op_call("GT"));
				exp = exp.place(x);
			}	break;

			case 26: // >= (Prefix Greater Than Equal)
			{
				let x = pop_lead(exp);
				exp = exp.place(op_call("GE"));
				exp = exp.place(x);
			}	break;

			case 31: // STACK
			{
				print("STACK = " + str(env.return_stack));
			}	break;

			case 32: // CODE
			{
				print("CODE = " + str(exp));
			}	break;

			case 33: // VARS
			{
				print("VARS = " + str(env.variables));
			}	break;

			case 34: // CONSTS
			{
				print("CONSTS = " + str(env.constants));
			}	break;

			case 35: // CLEAR
			{
				let op = pop_lead(exp);

				if (op.type() == "op_call") {

					switch (op.integer()) {

					case 31:
						env.return_stack = expression();
						break;

					case 32:
						exp = expression();
						break;

					case 33:
						env.variables = expression();
						break;

					case 34:
						env.constants = expression();
						break;

					default:
						break;
					}
				}
			}	break;

			case 40000:	// TYPE
			{
			}
			break;

			case 50000:	// FIRST
			{
			}
			break;

			case 60000:	// SECOND
			{
			}
			break;

			case 1500000:	// CLEAR = Clear the local runtime variables
			{
			}
			break;

			case 210000:	// POP = Remove the top element of the stack.
			{
			}
			break;

			case 2200000:	// DUP = Duplicate the top element of the stack.
			{
			}
			break;

			case 230000:	// SWAP = Change the order of the top two elements of the stack.
			{
			}
			break;

			case 250000:	// BUFF = Buffer in reverse order an integer number of stack elements.
			{
			}
			break;

			case 240000:	// ROLL = Roll or Rotate all elements of the stack.  
			{			// Takea a single numerical integer argument.  
						// arg < 0  Roll top elements to the bottom of the stack.  
						// arg > 0  Roll bottom elements to the top of the stack.
			}
			break;


			default:
				break;
			}
		}

		/*
		while (t_name == "symbol") {


		}

		if (t_name == "function") {

			var args = a.lead();
			var body = a.shift();

			// print(args);
			// print(body);

			var closure = expression();

			while (args.is()) {
				var x = pop_lead(args);
				var y = pop_lead(expr);
				var p = expression(x, y);
				closure = closure.place(p);
			}

			// print(closure);

			var new_envr = envr;

			while (closure.is()) {
				new_envr = new_envr.place(pop_lead(closure));
			}

			auto func = Lambda(Closure(__Oliver__, body, stack, new_envr), null(), false);

			while (func.not_done()) {
				func = func.lambda();
			}

			a = func.range();

			t_name = a.type();
		}

		
		*/

		return eval(env, exp);
	}



} // end