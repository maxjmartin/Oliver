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

	inline let         eval_expression  (environment env, let& exp);
	       lambda_t  __eval_expression__(environment env, let  exp);

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

		friend lambda_t        __eval__(const function& self, environment& env, let& exp);
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

	lambda_t __eval__(const function& self, environment& env, let& exp) {

		let keys = self._args; 

		while (keys.is()) {

			let value = pop_lead(exp);

			value = eval_expression(env, value);

			let key = pop_lead(keys);

			set_variable(env, key, value);
		}

		if (!env.func_queue.is()) {
			env.func_queue = env.func_queue.place(env.variables);
			env.func_queue = env.func_queue.place(env.constants);
			env.func_queue = env.func_queue.place(exp);
			exp = expression();
		}

		return eval(env, self._body);
	}

	/********************************************************************************************/
	//
	//                                'Oliver' Function Implimentation
	//
	/********************************************************************************************/

	inline let eval(let& exp) {
		environment env;

		let res;

		do {
			res = trampoline(__eval_expression__(env, exp));

			env.stack = first(res);

			res = second(res);

			exp = pop_lead(res);

			env.constants = pop_lead(res);
			env.variables = pop_lead(res);
			
		} while (exp.is());

		return res;
	}

	inline lambda_t eval(environment env, let exp) {
		return lambda_t(closure_t(__eval_expression__, env, exp), null(), false);
	}

	inline let eval_expression(environment env, let& exp) {
		/*
			The eval_expression function defines a completly
			new trampoline stack.  In practice this should
			typically only be invoked a few times preventing
			a segmentation fault.  But in reality we want to
			protect against that by enforcing an interation
			limit against internal invocations of this function.  

			We also need to provide a new environment stack.
			Else the current run time stack will be included
			within the evaluated result.  
		*/

		env.stack = expression();

		let result = trampoline(__eval_expression__(env, exp)).lead();

		return result;
	}

	lambda_t __eval_expression__(environment env, let exp) {
		
		if (!exp.expression()) {
			return eval(env, expression(exp));
		}

		if (expression_is_empty(exp)) {
			/*
				If there are no argumenta within the expression return the stack.
				Passing a null lambda function, the stack and setting the runtime to true.
			*/
			if (env.exp_queue.is()) {
				return eval(env, pop_lead(env.exp_queue));
			}

			let res = expression();
			res = res.place(env.func_queue);
			res = res.place(env.stack);
			
			return result(res);
		}

		let a = pop_lead(exp);

		while (a.symbol()) {
			a = get_symbol(env, a);
		}

		if (a.expression()) {
			if (exp.is()) {
				env.exp_queue = env.exp_queue.place(exp);
			}
			return eval(env, a);
		}

		else if (a.type() == "function") {
			return a.eval(env, exp);
		}

		else if (!a.op_call()) {
			env.stack = env.stack.place(a);
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
				let l = pop_lead(env.stack);

				env.stack = env.stack.place(l.lead());
			}	break;

			case SHIFT_OP:
			{
				let l = pop_lead(env.stack);

				env.stack = env.stack.place(l.shift());
			}	break;

			case PLACE_OP:
			{
				let l = pop_lead(env.stack);
				let x = pop_lead(env.stack);

				l = l.place(x);

				env.stack = env.stack.place(l);
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
				let var = pop_lead(exp);
				env.stack = env.stack.place(var);
			}	break;

			case const_OP:
			{
				let var = pop_lead(exp);
				let op  = pop_lead(exp);
				let val = pop_lead(exp);

				if (op.op_call() && op.integer() == IN_EQ_OP) {
					set_constant(env, var, val);
				}
				else if (val.op_call() && val.integer() == EQ_OP) {
					set_constant(env, var, op);
				}
			}	break;

			case EQ_OP: 
			{
				let y = pop_lead(env.stack);
				let x = pop_lead(env.stack);

				if (x.symbol()) {
					set_variable(env, x, y);
				}
				else if (x.expression()) {
					let keys = x;
					let values = y;

					while (keys.is()) {
						x = pop_lead(keys);
						y = pop_lead(values);
						if (x.symbol()) {
							set_variable(env, x, y);
						}
					}
				}
				else {
					env.stack = env.stack.place(boolean(x == y));
				}
			}	break;

			case NE_OP:
			{
				let y = pop_lead(env.stack);
				let x = pop_lead(env.stack);

				env.stack = env.stack.place(boolean(x != y));
			}	break;

			case LT_OP: 
			{
				let y = pop_lead(env.stack);
				let x = pop_lead(env.stack);

				env.stack = env.stack.place(boolean(x < y));
			}	break;

			case LE_OP: 
			{
				let y = pop_lead(env.stack);
				let x = pop_lead(env.stack);

				env.stack = env.stack.place(boolean(x <= y));
			}	break;

			case GT_OP: 
			{
				let y = pop_lead(env.stack);
				let x = pop_lead(env.stack);

				env.stack = env.stack.place(boolean(x > y));
			}	break;

			case GE_OP: 
			{
				let y = pop_lead(env.stack);
				let x = pop_lead(env.stack);

				env.stack = env.stack.place(boolean(x >= y));
			}	break;

			case IN_EQ_OP:
			{
				let x = pop_lead(exp);

				exp = exp.place(op_call(EQ_OP));
				exp = exp.place(x);

				let l = env.stack.lead();

				if (l.expression() || l.symbol()) {
					exp = exp.place(op_call(let_OP));
				}
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
				let truth = pop_lead(env.stack);

				env.stack = env.stack.place(boolean(truth.is()));
			} break;

			case IF_TRUE_OP:
			{
				let truth = pop_lead(env.stack);

				let then = pop_lead(exp);

				if (truth.is()) {
					exp = exp.place(then);
				}
			} break;

			case AND_OP:
			{
				let y = pop_lead(env.stack);
				let x = pop_lead(env.stack);

				env.stack = env.stack.place(boolean(x.is() && y.is()));
			}	break;

			case OR_OP:
			{
				let y = pop_lead(env.stack);
				let x = pop_lead(env.stack);

				env.stack = env.stack.place(boolean(x.is() || y.is()));
			}	break;

			case NOT_OP:
			{
				let x = pop_lead(env.stack);

				env.stack = env.stack.place(boolean(!x.is()));
			}	break;

			case XOR_OP:
			{
				let y = pop_lead(env.stack);
				let x = pop_lead(env.stack);

				int_t truth = static_cast<int_t>(x.is()) ^ static_cast<int_t>(y.is());

				env.stack = env.stack.place(boolean(truth));
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
				let y = pop_lead(env.stack);
				let x = pop_lead(env.stack);

				env.stack = env.stack.place(x + y);
			}	break;

			case SUB_OP:
			{
				let y = pop_lead(env.stack);
				let x = pop_lead(env.stack);

				env.stack = env.stack.place(x - y);
			}	break;

			case MUL_OP:
			{
				let y = pop_lead(env.stack);
				let x = pop_lead(env.stack);

				env.stack = env.stack.place(x * y);
			}	break;

			case DIV_OP:
			{
				let y = pop_lead(env.stack);
				let x = pop_lead(env.stack);

				env.stack = env.stack.place(x / y);
			}	break;

			case MOD_OP:
			{
				let y = pop_lead(env.stack);
				let x = pop_lead(env.stack);

				env.stack = env.stack.place(x % y);
			}	break;

			case FDIV_OP:
			{
				let y = pop_lead(env.stack);
				let x = pop_lead(env.stack);

				env.stack = env.stack.place(x.f_div(y));
			}	break;

			case REM_OP:
			{
				let y = pop_lead(env.stack);
				let x = pop_lead(env.stack);

				env.stack = env.stack.place(x.rem(y));
			}	break;

			case POW_OP:
			{
				let y = pop_lead(env.stack);
				let x = pop_lead(env.stack);

				env.stack = env.stack.place(x.pow(y));
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

			case rem_OP:
			{
				let x = pop_lead(exp);

				exp = exp.place(op_call(REM_OP));
				exp = exp.place(x);
			}	break;

			case pow_OP:
			{
				let x = pop_lead(exp);

				exp = exp.place(op_call(POW_OP));
				exp = exp.place(x);
			}	break;

			case GET_OP:
			{
				let y = pop_lead(env.stack);
				let x = pop_lead(env.stack);

				env.stack = env.stack.place(x.get(y));
			}	break;

			case SET_OP:
			{
				let z = pop_lead(env.stack);
				let y = pop_lead(env.stack);
				let x = pop_lead(env.stack);

				env.stack = env.stack.place(x.set(y, z));
			}	break;

			case get_OP:
			{
				let x = pop_lead(exp);

				exp = exp.place(op_call(GET_OP));
				exp = exp.place(x);
			}	break;

			case set_OP:
			{
				let x = pop_lead(exp);
				let y = pop_lead(exp);

				exp = exp.place(op_call(SET_OP));
				exp = exp.place(y);
				exp = exp.place(x);
			}	break;

			case 310: // STACK
			{
				print("STACK = " + str(env.stack));
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

				if (op.op_call()) {

					switch (op.integer()) {

					case 31:
						env.stack = expression();
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