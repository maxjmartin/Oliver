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
#include "boolean.h"
#include "number.h"
#include "string.h"
#include "symbol.h"
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

	let  get_symbol(environment env, let key);
	bool get_symbol(let variables, let key, let& value);

	let set_variable(let variables, let key, let value);
	let set_constant(let constants, let key, let value);

	void PRINT(environment& env, let& exp);
	void print(environment& env, let& exp);

	void REPR(environment& env, let& exp);
	void repr(environment& env, let& exp);

	/********************************************************************************************/
	//
	//                                 Function Implimentations
	//
	/********************************************************************************************/

	let append_buffer(let buffer, let list) {

		while (buffer.is()) {
			list = list.place(pop_lead(buffer));
		}

		return list;
	}

	let get_symbol(environment env, let key) {

		let value;

		if (get_symbol(env.constants, key, value) || get_symbol(env.variables, key, value)) {
			return value;
		}

		return null();
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
				
				pair = expression(key, value);

				vars = vars.place(pair);

				return append_buffer(buffer, vars);
			}

			buffer = buffer.place(pair);
		}

		variables = variables.place(expression(key, value));

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

		constants = constants.place(expression(key, value));

		return constants;
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

	void REPR(environment& env, let& exp) {

		let arg = pop_lead(env.return_stack);

		stream_t stream;

		stream << std::boolalpha;

		arg.repr(stream);

		std::cout << stream.str();
	}

	void repr(environment& env, let& exp) {

		stream_t stream;

		stream << std::boolalpha;

		while (exp.is()) {
			pop_lead(exp).repr(stream);
		}

		std::cout << stream.str() << std::endl;
	}

} // end