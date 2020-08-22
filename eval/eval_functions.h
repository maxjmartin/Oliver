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
#include "..\types\boolean.h"
#include "..\types\error.h"
#include "..\types\number.h"
#include "..\types\string.h"
#include "..\types\symbol.h"
#include "Oliver.h"

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
	//                                  Function Definitions
	//
	/********************************************************************************************/

	let append_buffer(let buffer, let list);

	bool get_symbol(let variables, let key, let& value);

	let set_variable(let variables, let key, let value);
	let set_constant(let constants, let key, let value);

/*
	void LEAD(environment& env, let& exp);
	void SHIFT(environment& env, let& exp);
	void PLACE(environment& env, let& exp);

	void lead(environment& env, let& exp);
	void shift(environment& env, let& exp);
	void place(environment& env, let& exp);

	void set_let(environment& env, let& exp);
	void set_const(environment& env, let& exp);

	void PRINT(environment& env, let& exp);
	void print(environment& env, let& exp);

	void EQUAL(environment& env, let& exp);
	void NOT_EQUAL(environment& env, let& exp);
	void LESS_THAN(environment& env, let& exp);
	void LESS_THAN_EQUAL(environment& env, let& exp);
	void GREATER_THAN(environment& env, let& exp);
	void GREATER_THAN_EQUAL(environment& env, let& exp);

	void _EQUAL_(environment& env, let& exp);
	void _NOT_EQUAL_(environment& env, let& exp);
	void _LESS_THAN_(environment& env, let& exp);
	void _LESS_THAN_EQUAL_(environment& env, let& exp);
	void _GREATER_THAN_(environment& env, let& exp);
	void _GREATER_THAN_EQUAL_(environment& env, let& exp);

	void equal(environment& env, let& exp);
	void not_equal(environment& env, let& exp);
	void less_than(environment& env, let& exp);
	void less_than_equal(environment& env, let& exp);
	void greater_than(environment& env, let& exp);
	void greater_than_equal(environment& env, let& exp);

	void IS_TRUE(environment& env, let& exp);
	void IF_TRUE(environment& env, let& exp);
	void LOGICAL_AND(environment& env, let& exp);
	void LOGICAL_OR (environment& env, let& exp);
	void LOGICAL_NOT(environment& env, let& exp);
	void LOGICAL_XOR(environment& env, let& exp);
*/
	/********************************************************************************************/
	//
	//                                 Function Implimentations
	//
	/********************************************************************************************/

	let append_buffer(let buffer, let list) {

		while (buffer.is()) {
			list = list.place_lead(pop_lead(buffer));
		}

		return list;
	}

	bool get_symbol(let vars, let key, let& value) {

		while (vars.is()) {

			let pair = pop_lead(vars);

			if (first(pair) == key) {
				value = second(pair);
				return true;
			}
		}

		return false;
	}

	let set_variable(let variables, let key, let value) {
		
		let vars   = variables;
		let buffer = expression();

		while (vars.is()) {

			let pair = pop_lead(vars);

			if (first(pair) == key) {
				
				// pair = expression(key, value);

				vars = vars.place_lead(pair);

				return append_buffer(buffer, vars);
			}

			buffer = buffer.place_lead(pair);
		}

		// variables = variables.place_lead(expression(key, value));

		return variables;
	}

	let set_constant(let constants, let key, let value) {

		let vars = constants;

		while (vars.is()) {

			let pair = pop_lead(vars);

			if (first(pair) == key) {

				return constants;
			}
		}

		// constants = constants.place_lead(expression(key, value));

		return constants;
	}

/*
	void SHIFT(environment& env, let& exp) {
		
		let l = pop_lead(env.return_stack);

		env.return_stack = env.return_stack.place(l.shift());
	}

	void PLACE(environment& env, let& exp) {

		let l = pop_lead(env.return_stack);
		let x = pop_lead(env.return_stack);

		env.return_stack = env.return_stack.place(l.place(x));
	}

	void lead(environment& env, let& exp) {

		let l = pop_lead(exp);

		env.return_stack = env.return_stack.place(l.lead());
	}

	void shift(environment& env, let& exp) {

		let l = pop_lead(exp);

		env.return_stack = env.return_stack.place(l.shift());
	}

	void place(environment& env, let& exp) {

		let x = pop_lead(exp);
		let l = pop_lead(exp);

		env.return_stack = env.return_stack.place(l.place(x));
	}

	void set_let(environment& env, let& exp) {

		let   key = pop_lead(exp);
		str_t op = str(pop_lead(exp));
		let   value = pop_lead(exp);

		if (op == "=") {
			env.variables = set_variable(env.variables, key, value);
		}
	}

	void set_const(environment& env, let& exp) {

		let   key = pop_lead(exp);
		str_t op = str(pop_lead(exp));
		let   value = pop_lead(exp);

		if (op == "=") {
			env.constants = set_constant(env.constants, key, value);
		}
	}

	void PRINT(environment& env, let& exp) {

		let arg = pop_lead(env.return_stack);

		stream_t stream;

		stream << std::boolalpha;

		arg.str(stream);

		std::cout << stream.str();
	}

	void print(environment& env, let& exp) {

		stream_t stream;

		stream << std::boolalpha;

		while (exp.is()) {
			pop_lead(exp).str(stream);
		}

		std::cout << stream.str() << std::endl;
	}

	void EQUAL(environment& env, let& exp) {

		let y = pop_lead(env.return_stack);
		let x = pop_lead(env.return_stack);

		env.return_stack = env.return_stack.place(boolean(x == y));
	}

	void NOT_EQUAL(environment& env, let& exp) {

		let y = pop_lead(env.return_stack);
		let x = pop_lead(env.return_stack);

		env.return_stack = env.return_stack.place(boolean(x != y));
	}

	void LESS_THAN(environment& env, let& exp) {

		let y = pop_lead(env.return_stack);
		let x = pop_lead(env.return_stack);

		env.return_stack = env.return_stack.place(boolean(x < y));
	}

	void LESS_THAN_EQUAL(environment& env, let& exp) {

		let y = pop_lead(env.return_stack);
		let x = pop_lead(env.return_stack);

		env.return_stack = env.return_stack.place(boolean(x <= y));
	}

	void GREATER_THAN(environment& env, let& exp) {

		let y = pop_lead(env.return_stack);
		let x = pop_lead(env.return_stack);

		env.return_stack = env.return_stack.place(boolean(x > y));
	}

	void GREATER_THAN_EQUAL(environment& env, let& exp) {

		let y = pop_lead(env.return_stack);
		let x = pop_lead(env.return_stack);

		env.return_stack = env.return_stack.place(boolean(x >= y));
	}

	void _EQUAL_(environment& env, let& exp) {
		let x = pop_lead(exp);
		exp = exp.place(op_call(OP_CODES::EQ_OP));
		exp = exp.place(x);
	}

	void _NOT_EQUAL_(environment& env, let& exp) {
		let x = pop_lead(exp);
		exp = exp.place(op_call(OP_CODES::NE_OP));
		exp = exp.place(x);
	}

	void _LESS_THAN_(environment& env, let& exp) {
		let x = pop_lead(exp);
		exp = exp.place(op_call(OP_CODES::LT_OP));
		exp = exp.place(x);
	}

	void _LESS_THAN_EQUAL_(environment& env, let& exp) {
		let x = pop_lead(exp);
		exp = exp.place(op_call(OP_CODES::LE_OP));
		exp = exp.place(x);
	}

	void _GREATER_THAN_(environment& env, let& exp) {
		let x = pop_lead(exp);
		exp = exp.place(op_call(OP_CODES::GT_OP));
		exp = exp.place(x);
	}

	void _GREATER_THAN_EQUAL_(environment& env, let& exp) {
		let x = pop_lead(exp);
		exp = exp.place(op_call(OP_CODES::GE_OP));
		exp = exp.place(x);
	}

	void equal(environment& env, let& exp) {

		let x = pop_lead(exp);

		while (exp.is()) {
			let y = pop_lead(exp);

			if (!(x == y)) {
				env.return_stack = env.return_stack.place(boolean(false));
				return;
			}
		}

		env.return_stack = env.return_stack.place(boolean(true));
		return;
	}

	void not_equal(environment& env, let& exp) {

		let x = pop_lead(exp);

		while (exp.is()) {
			let y = pop_lead(exp);

			if (!(x != y)) {
				env.return_stack = env.return_stack.place(boolean(false));
				return;
			}
		}

		env.return_stack = env.return_stack.place(boolean(true));
		return;
	}

	void less_than(environment& env, let& exp) {

		let x = pop_lead(exp);

		while (exp.is()) {
			let y = pop_lead(exp);

			if (!(x < y)) {
				env.return_stack = env.return_stack.place(boolean(false));
				return;
			}
		}

		env.return_stack = env.return_stack.place(boolean(true));
		return;
	}

	void less_than_equal(environment& env, let& exp) {

		let x = pop_lead(exp);

		while (exp.is()) {
			let y = pop_lead(exp);

			if (!(x <= y)) {
				env.return_stack = env.return_stack.place(boolean(false));
				return;
			}
		}

		env.return_stack = env.return_stack.place(boolean(true));
		return;
	}

	void greater_than(environment& env, let& exp) {

		let x = pop_lead(exp);

		while (exp.is()) {
			let y = pop_lead(exp);

			if (!(x > y)) {
				env.return_stack = env.return_stack.place(boolean(false));
				return;
			}
		}

		env.return_stack = env.return_stack.place(boolean(true));
		return;
	}

	void greater_than_equal(environment& env, let& exp) {

		let x = pop_lead(exp);

		while (exp.is()) {
			let y = pop_lead(exp);

			if (!(x >= y)) {
				env.return_stack = env.return_stack.place(boolean(false));
				return;
			}
		}

		env.return_stack = env.return_stack.place(boolean(true));
		return;
	}

	void IS_TRUE(environment& env, let& exp) {

		let truth = pop_lead(env.return_stack);

		env.return_stack = env.return_stack.place(boolean(truth.is()));
		return;
	}

	void IF_TRUE(environment& env, let& exp) {

		let truth = pop_lead(env.return_stack);

		let x = pop_lead(exp);
		let y = pop_lead(exp);

		if (truth.is()) {
			exp = exp.place(x);
			return;
		}

		exp = exp.place(y);
		return;
	}

	void LOGICAL_AND(environment& env, let& exp) {

		let y = pop_lead(env.return_stack);
		let x = pop_lead(env.return_stack);

		env.return_stack = env.return_stack.place(boolean(x.is() && y.is()));
		return;
	}

	void LOGICAL_OR(environment& env, let& exp) {

		let y = pop_lead(env.return_stack);
		let x = pop_lead(env.return_stack);

		env.return_stack = env.return_stack.place(boolean(x.is() || y.is()));
		return;
	}

	void LOGICAL_NOT(environment& env, let& exp) {

		let x = pop_lead(env.return_stack);

		env.return_stack = env.return_stack.place(boolean(!x.is()));
		return;
	}

	void LOGICAL_XOR(environment& env, let& exp) {

		let y = pop_lead(env.return_stack);
		let x = pop_lead(env.return_stack);

		int_t truth = static_cast<int_t>(x.is()) ^ static_cast<int_t>(y.is());

		env.return_stack = env.return_stack.place(boolean(truth));
		return;
	}
*/
} // end