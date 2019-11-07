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

	let get_symbol(let env, let key);
	let set_symbol(let env, let key, let value);

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

	let get_symbol(let env, let key) {
		
		let vars = env;

		while (vars.is()) {

			let pair = pop_lead(vars);

			if (first(pair) == key) {
				return second(pair);
			}
		}

		return null();
	}

	let set_symbol(let env, let key, let value) {
		
		let vars   = env;
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

		env = env.place(expression(key, value));

		return env;
	}

} // end