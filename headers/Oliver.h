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
#include "eval_support_functions.h"

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

			value = eval_expression(env, value).lead();

			let key = pop_lead(keys);

			env.variables = set_variable(env.variables, key, value);
		}

		return eval(env, self._body);
	}

	/********************************************************************************************/
	//
	//                                'Oliver' Function Implimentation
	//
	/********************************************************************************************/

	inline let eval(let& exp) {
		return trampoline(__eval_expression__(environment(ITERATION_LIMIT), exp));
	}

	inline lambda_t eval(environment env, let exp) {
		return lambda_t(closure_t(__eval_expression__, env, exp), null(), false);
	}

	inline let eval_expression(environment& env, let& exp) {
		/*
			The eval_expression function defines a completly
			new trampoline stack.  In practice this should
			typically only be invoked a few times preventing
			a segmentation fault.  But in reality we want to
			protect against that by enforcing an interation
			limit against internal invocations of this function.  
		*/

		env.iter_limit -= 1;

		let result = (env.iter_limit > 0) ? trampoline(__eval_expression__(env, exp)) : null();

		env.iter_limit += 1;

		return result;
	}

	lambda_t __eval_expression__(environment env, let exp) {

		if (env.iter_limit <= 0) {
			return result(null());
		}

		if (expression_is_empty(exp)) {
			/*
				If there are no argumenta within the expression return the stack.
				Passing a null lambda function, the stack and setting the runtime to true.
			*/
			return result(env.return_stack);
		}

		let a = exp.expression() ? pop_lead(exp) : exp;

		while (a.type() == "symbol") {
			a = get_symbol(env, a);
		}

		if (a.type() == "expression") {
			return eval(env, a);
		}

		else if (a.type() == "function") {
			return a.eval(env, exp);
		}

		else if (a.type() != "op_call") {
			env.return_stack = env.return_stack.place(a);
		}

		else {

			switch (a.integer()) {

			case PRINT_OP:
				PRINT(env, exp);
				break;

			case print_OP:
			{
				let args = expression(pop_lead(exp));
				args = eval_expression(env, args).reverse();
				print(env, args);
			}	break;

			case REPR_OP:
				REPR(env, exp);
				break;

			case repr_OP:
			{
				let args = expression(pop_lead(exp));
				args = eval_expression(env, args).reverse();
				repr(env, args);
			}	break;

			case LEAD_OP:
			{
				let l = pop_lead(env.return_stack);

				env.return_stack = env.return_stack.place(l.lead());
			}	break;

			case SHIFT_OP:
			{
				let l = pop_lead(env.return_stack);

				env.return_stack = env.return_stack.place(l.shift());
			}	break;

			case PLACE_OP:
			{
				let l = pop_lead(env.return_stack);
				let x = pop_lead(env.return_stack);

				l = l.place(x);

				env.return_stack = env.return_stack.place(l);
			}	break;

			case lead_OP:
			{
				let l = pop_lead(exp);

				exp = exp.place(op_call(LEAD_OP));
				exp = exp.place(l);
			}	break;

			case shift_OP:
			{
				let l = pop_lead(exp);

				exp = exp.place(op_call(SHIFT_OP));
				exp = exp.place(l);
			}	break;

			case place_OP:
			{
				let x = pop_lead(exp);
				let l = pop_lead(exp);

				exp = exp.place(op_call(PLACE_OP));
				exp = exp.place(l);
				exp = exp.place(x);
			}	break;

			case let_OP:
			{
				let   key   = pop_lead(exp);
				str_t op    = str(pop_lead(exp));
				let   value = pop_lead(exp);

				if (op == "=") {
					env.variables = set_variable(env.variables, key, value);
				}
			}	break;

			case const_OP:	
			{
				let   key   = pop_lead(exp);
				str_t op    = str(pop_lead(exp));
				let   value = pop_lead(exp);

				if (op == "=") {
					env.constants = set_constant(env.constants, key, value);
				}
			}	break;

			case EQ_OP: 
			{
				let y = pop_lead(env.return_stack);
				let x = pop_lead(env.return_stack);

				env.return_stack = env.return_stack.place(boolean(x == y));
			}	break;

			case NE_OP:
			{
				let y = pop_lead(env.return_stack);
				let x = pop_lead(env.return_stack);

				env.return_stack = env.return_stack.place(boolean(x != y));
			}	break;

			case LT_OP: 
			{
				let y = pop_lead(env.return_stack);
				let x = pop_lead(env.return_stack);

				env.return_stack = env.return_stack.place(boolean(x < y));
			}	break;

			case LE_OP: 
			{
				let y = pop_lead(env.return_stack);
				let x = pop_lead(env.return_stack);

				env.return_stack = env.return_stack.place(boolean(x <= y));
			}	break;

			case GT_OP: 
			{
				let y = pop_lead(env.return_stack);
				let x = pop_lead(env.return_stack);

				env.return_stack = env.return_stack.place(boolean(x > y));
			}	break;

			case GE_OP: 
			{
				let y = pop_lead(env.return_stack);
				let x = pop_lead(env.return_stack);

				env.return_stack = env.return_stack.place(boolean(x >= y));
			}	break;

			case IN_EQ_OP:
			{
				let x = pop_lead(exp);

				exp = exp.place(op_call(EQ_OP));
				exp = exp.place(x);
			}	break;

			case IN_NE_OP: 
			{
				let x = pop_lead(exp);

				exp = exp.place(op_call(NE_OP));
				exp = exp.place(x);
			}	break;

			case IN_LT_OP:
			{
				let x = pop_lead(exp);

				exp = exp.place(op_call(LT_OP));
				exp = exp.place(x);
			}	break;

			case IN_LE_OP:
			{
				let x = pop_lead(exp);

				exp = exp.place(op_call(LE_OP));
				exp = exp.place(x);
			}	break;

			case IN_GT_OP: 
			{
				let x = pop_lead(exp);

				exp = exp.place(op_call(GT_OP));
				exp = exp.place(x);
			}	break;

			case IN_GE_OP:
			{
				let x = pop_lead(exp);

				exp = exp.place(op_call(GE_OP));
				exp = exp.place(x);
			}	break;

			case IS_TRUE_OP:
			{
				let truth = pop_lead(env.return_stack);

				env.return_stack = env.return_stack.place(boolean(truth.is()));
			} break;

			case IF_TRUE_OP:
			{
				let truth = pop_lead(env.return_stack);

				let then = pop_lead(exp);

				if (truth.is()) {
					exp = exp.place(then);
				}
			} break;

			case AND_OP:
			{
				let y = pop_lead(env.return_stack);
				let x = pop_lead(env.return_stack);

				env.return_stack = env.return_stack.place(boolean(x.is() && y.is()));
			}	break;

			case OR_OP:
			{
				let y = pop_lead(env.return_stack);
				let x = pop_lead(env.return_stack);

				env.return_stack = env.return_stack.place(boolean(x.is() || y.is()));
			}	break;

			case NOT_OP:
			{
				let x = pop_lead(env.return_stack);

				env.return_stack = env.return_stack.place(boolean(!x.is()));
			}	break;

			case XOR_OP:
			{
				let y = pop_lead(env.return_stack);
				let x = pop_lead(env.return_stack);

				int_t truth = static_cast<int_t>(x.is()) ^ static_cast<int_t>(y.is());

				env.return_stack = env.return_stack.place(boolean(truth));
			}	break;

			case if_OP:
			{
				let conditions = pop_lead(exp);

				while (conditions.is()) {

					let condition = pop_lead(conditions);

					let truth = eval_expression(env, condition);

					if (truth.lead().is()) {
						exp = exp.place(conditions.lead());
						conditions = expression();
					}
					else {
						conditions = conditions.shift();
					}
				}
			}	break;

			case and_OP:
			{
				let x = pop_lead(exp);

				exp = exp.place(op_call(AND_OP));
				exp = exp.place(x);
			}	break;

			case or_OP:
			{
				let x = pop_lead(exp);

				exp = exp.place(op_call(OR_OP));
				exp = exp.place(x);
			}	break;

			case not_OP:
			{
				let x = pop_lead(exp);

				exp = exp.place(op_call(NOT_OP));
				exp = exp.place(x);
			}	break;

			case xor_OP:
			{
				let x = pop_lead(exp);

				exp = exp.place(op_call(XOR_OP));
				exp = exp.place(x);
			}	break;

			case ADD_OP:
			{
				let y = pop_lead(env.return_stack);
				let x = pop_lead(env.return_stack);

				env.return_stack = env.return_stack.place(x + y);
			}	break;

			case SUB_OP:
			{
				let y = pop_lead(env.return_stack);
				let x = pop_lead(env.return_stack);

				env.return_stack = env.return_stack.place(x - y);
			}	break;

			case MUL_OP:
			{
				let y = pop_lead(env.return_stack);
				let x = pop_lead(env.return_stack);

				env.return_stack = env.return_stack.place(x * y);
			}	break;

			case DIV_OP:
			{
				let y = pop_lead(env.return_stack);
				let x = pop_lead(env.return_stack);

				env.return_stack = env.return_stack.place(x / y);
			}	break;

			case MOD_OP:
			{
				let y = pop_lead(env.return_stack);
				let x = pop_lead(env.return_stack);

				env.return_stack = env.return_stack.place(x % y);
			}	break;

			case FDIV_OP:
			{
				let y = pop_lead(env.return_stack);
				let x = pop_lead(env.return_stack);

				env.return_stack = env.return_stack.place(x.f_div(y));
			}	break;

			case add_OP:
			{
				let x = pop_lead(exp);

				exp = exp.place(op_call(ADD_OP));
				exp = exp.place(x);
			}	break;

			case sub_OP:
			{
				let x = pop_lead(exp);

				exp = exp.place(op_call(SUB_OP));
				exp = exp.place(x);
			}	break;

			case mul_OP:
			{
				let x = pop_lead(exp);

				exp = exp.place(op_call(MUL_OP));
				exp = exp.place(x);
			}	break;

			case div_OP:
			{
				let x = pop_lead(exp);

				exp = exp.place(op_call(DIV_OP));
				exp = exp.place(x);
			}	break;

			case mod_OP:
			{
				let x = pop_lead(exp);

				exp = exp.place(op_call(MOD_OP));
				exp = exp.place(x);
			}	break;

			case fdiv_OP:
			{
				let x = pop_lead(exp);

				exp = exp.place(op_call(FDIV_OP));
				exp = exp.place(x);
			}	break;

			case 310: // STACK
			{
				print("STACK = " + str(env.return_stack));
			}	break;

			case 320: // CODE
			{
				print("CODE = " + str(exp));
			}	break;

			case 330: // VARS
			{
				print("VARS = " + str(env.variables));
			}	break;

			case 340: // CONSTS
			{
				print("CONSTS = " + str(env.constants));
			}	break;

			case 350: // CLEAR
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

		return eval(env, exp);
	}



} // end