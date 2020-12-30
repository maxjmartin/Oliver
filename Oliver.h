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
//			along with Oliver.If not, see <https://www.gnu.org/licenses/>.
//			
/********************************************************************************************/

#include <map>
#include <vector>

#include ".\eval\eval_functions.h"

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
	//                              'lambda' Class Definition
	//
	//          The lambda class is a doubled ended queue.  It is implimented
	//          using two __node_queue__ data types representing the front and back
	//          portions of the queue.  
	//
	/********************************************************************************************/


	/********************************************************************************************/
	//
	//                                'eval' Function Definitions
	//
	/********************************************************************************************/

	class evaluator {

		typedef     std::map<str_t, let>  map_t;
		typedef     std::vector<let>	  stack_t;
		typedef     std::vector<map_t>	  closure_t;

		closure_t     _constants;
		closure_t     _variables;
		stack_t       _stack;
		stack_t       _code;

	public:
		evaluator();
		evaluator(evaluator& env) = delete;

		let eval(let exp);

	private:

		static const str_t STACK_UNDERFLOW;

		inline void eval();

		inline let get_result_stack() const;

		inline void prep_code_to_execute(let& exp) const;

		inline let  get_symbol(let& var) const;
		inline void set_constant(let& var, let& val);
		inline void set_variable(let& var, let& val);

		inline bool_t constant_not_defined(const str_t& symbol_name) const;
		inline bool_t variable_not_defined(const str_t& symbol_name) const;

		inline void define_enclosure(let& lam);
		inline void define_enclosure();
		inline void delete_enclosure();

		inline void  set_expression_on_code(let exp);
		inline void set_expression_on_stack(let exp);

		inline let get_expression_from_stack();
		inline let  get_expression_from_code();
		inline let peek_expression_from_code();

		inline bool_t is_operator_call(OP_CODE opr, let val);

		inline void   function_scope_operators(OP_CODE& opr);
		inline void     prefix_unary_operators(OP_CODE& opr);
		inline void    postfix_unary_operators(OP_CODE& opr);
		inline void	    infix_binary_operators(OP_CODE& opr);
		inline void   postfix_binary_operators(OP_CODE& opr);
		inline void extended_logical_operators(OP_CODE& opr);
		inline void      abstraction_operators(OP_CODE& opr);
	};

	const str_t evaluator::STACK_UNDERFLOW = "stack underflow";

	/********************************************************************************************/
	//
	//                                'Oliver' Function Implimentation
	//
	/********************************************************************************************/

	evaluator::evaluator() : _constants(), _variables(), _stack(), _code() {
	}

	// evaluator::evaluator(evaluator& env) : _constants(env._constants), _variables(env._variables), _stack(env._stack), _code(env._code) {
	// }

	let evaluator::eval(let exp) {

		if (exp.type() != "expression") {
			return nothing();
		}
		
		prep_code_to_execute(exp);

		_code.push_back(exp);

		define_enclosure();

		eval();

		return get_result_stack();
	}

	inline let Olly::evaluator::get_result_stack() const {

		if (!_stack.empty()) {
			
			let result = expression();

			for (auto i = _stack.crbegin(); i != _stack.crend(); ++i) {

				result = result.place_lead(*i);
			}

			return result;
		}

		return expression();
	}

	inline void Olly::evaluator::prep_code_to_execute(let& exp) const {

		while (exp.is() && exp.len() == 1) {

			if (exp.lead().type() != "expression") {
				break;
			}
			exp = exp.lead();
		}
	}

	inline let Olly::evaluator::get_symbol(let& var) const {

		str_t symbol_name = str(var);

		for (auto i = _constants.crbegin(); i != _constants.crend(); ++i) {

			auto c_itr = i->find(symbol_name);

			if (c_itr != i->end()) {
				return c_itr->second;
			}
		}

		for (auto i = _variables.crbegin(); i != _variables.crend(); ++i) {

			auto v_itr = i->find(symbol_name);

			if (v_itr != i->end()) {
				return v_itr->second;
			}
		}

		return error("undefined");
	}

	inline void Olly::evaluator::set_constant(let& var, let& val) {

		while (val.type() == "symbol") {
			val = get_symbol(val);
		}

		if (!_constants.empty()) {

			str_t symbol_name = str(var);

			if (constant_not_defined(symbol_name) && variable_not_defined(symbol_name)) {

				_constants.back()[symbol_name] = val;
			}
		}
	}

	inline void Olly::evaluator::set_variable(let& var, let& val) {

		while (val.type() == "symbol") {
			val = get_symbol(val);
		}

		if (!_variables.empty()) {

			str_t symbol_name = str(var);

			if (constant_not_defined(symbol_name)) {

				_variables.back()[symbol_name] = val;
			}
		}
	}

	inline bool_t Olly::evaluator::constant_not_defined(const str_t& symbol_name) const {

		for (auto i = _constants.crbegin(); i != _constants.crend(); ++i) {

			auto c_itr = i->find(symbol_name);

			if (c_itr != i->end()) {
				return false;
			}
		}

		return true;
	}

	inline bool_t Olly::evaluator::variable_not_defined(const str_t& symbol_name) const {

		for (auto i = _variables.crbegin(); i != _variables.crend(); ++i) {

			auto c_itr = i->find(symbol_name);

			if (c_itr != i->end()) {
				return false;
			}
		}

		return true;
	}

	inline void Olly::evaluator::define_enclosure(let& lam) {

		const lambda* l = lam.cast<lambda>();

		_constants.push_back(l->constants());
		_variables.push_back(l->variables());
	}

	inline void Olly::evaluator::define_enclosure() {
		_constants.push_back(map_t());
		_variables.push_back(map_t());
	}

	inline void Olly::evaluator::delete_enclosure() {
		_constants.pop_back();
		_variables.pop_back();
	}

	inline void Olly::evaluator::set_expression_on_code(let exp) {

		if (!_code.empty()) {
			_code.back() = _code.back().place_lead(exp);
		}
	}

	inline void Olly::evaluator::set_expression_on_stack(let exp) {
		_stack.emplace_back(exp);
	}

	inline let Olly::evaluator::get_expression_from_stack() {

		if (!_stack.empty()) {

			let val = _stack.back();
			_stack.pop_back();

			return val;
		}

		return error("stack_underflow");
	}

	inline let Olly::evaluator::get_expression_from_code() {

		if (!_code.empty()) {

			let a = pop_lead(_code.back());

			if (a.is_nothing() && expression_is_empty(_code.back())) {
				_code.pop_back();
				return get_expression_from_code();
			}

			return a;
		}

		return nothing();
	}

	inline let Olly::evaluator::peek_expression_from_code() {

		if (!_code.empty()) {

			return _code.back().lead();
		}

		return nothing();
	}

	inline bool_t Olly::evaluator::is_operator_call(OP_CODE opr, let val) {
		
		if (val.type() == "op_call" && val.op_code() == opr) {
			return true;
		}

		return false;
	}

	inline void Olly::evaluator::eval() {

		bool_t run_continous = true;

		do {

			let exp = get_expression_from_code();

			// print("exp = " + str(exp) + " : " + exp.type());
			// print("stack = " + str(_stack.back()));

			while (exp.type() == "symbol") {
				exp = get_symbol(exp);
			}

			if (exp.type() == "expression") {

				prep_code_to_execute(exp);

				if (!expression_is_empty(exp)) {
					_code.push_back(exp);
				}
			}

			else if (exp.type() == "lambda") {

				let args = exp.lead();
				let body = exp.last();

				define_enclosure(exp);

				while (args.is()) {

					let var = pop_lead(args);
					let val = get_expression_from_code();

					if (var.type() == "symbol") {
						set_variable(var, val);
					}
				}

				set_expression_on_code(op_call(OP_CODE::end_scope_op));
				set_expression_on_code(body);
			}

			else if (exp.type() != "op_call"  && !exp.is_nothing()) {
				set_expression_on_stack(exp);
			}

			else {

				OP_CODE opr = exp.op_code();

				if (opr > OP_CODE::NOTHING_OP && opr < OP_CODE::END_OPERATORS_OP) {

					if (opr < OP_CODE::FUNCTION_SCOPE_OPERATORS) {
						function_scope_operators(opr);
					}

					else if (opr < OP_CODE::POSTFIX_UNARY_OPERATORS) {
						postfix_unary_operators(opr);
					}

					else if (opr < OP_CODE::PREFIX_UNARY_OPERATORS) {
						prefix_unary_operators(opr);
					}

					else if (opr < OP_CODE::POSTFIX_BINARY_OPERATORS) {
						postfix_binary_operators(opr);
					}

					else if (opr < OP_CODE::EXTENDED_LOGIC_OPERATORS) {
						extended_logical_operators(opr);
					}

					else if (opr < OP_CODE::INFIX_BINARY_OPERATORS) {
						infix_binary_operators(opr);
					}

					else if (opr < OP_CODE::ABSTRACTION_OPERATORS) {
						abstraction_operators(opr);
					}

					switch (opr) {

					case OP_CODE::BREAK_op:
						run_continous = false;
						break;

					case OP_CODE::END_op:

						if (!_code.empty()) {
							_code.pop_back();
						}
						break;

					case OP_CODE::EXPR_op: {

						if (!_code.empty()) {
							_stack.back() = _stack.back().place_lead(_code.back());
						}

					}	break;

					case OP_CODE::STACK_op: {

						if (!_stack.empty()) {
							_stack.back() = _stack.back().place_lead(_stack.back().reverse());
						}

					}	break;

					case OP_CODE::PRINT_op: {

						let a = get_expression_from_stack();

						stream_t out;

						a.str(out);

						std::cout << out.str();

					}	break;

					case OP_CODE::REPR_op: {

						let a = get_expression_from_stack();

						stream_t out;

						a.repr(out);

						std::cout << out.str();

					}	break;

					case OP_CODE::ENDL_op: {

						std::cout << std::endl;

					}	break;

					case OP_CODE::DUP_op:
						// DUP(env, exp);
						break;

					case OP_CODE::SWAP_op:
						// DEPTH(env, exp);
						break;

					case OP_CODE::OVER_op:
						// DEPTH(env, exp);
						break;

					default:
						break;
					}

				}
			}
			
		} while (!_code.empty() && run_continous);
	}

	inline void Olly::evaluator::function_scope_operators(OP_CODE& opr) {

		switch (opr) {

			case OP_CODE::NOTHING_OP:    // Nothing Operator
				// Do nothing.
				break;

			/****  Scope Enclosure Operators ****/

			case OP_CODE::end_scope_op: {    // Delete the current enclosure scope.
				delete_enclosure();
			}	break;

			case OP_CODE::let_op: {  // Presupose the definition of a constant or variable.  

				let vars = get_expression_from_code();
				let oper = get_expression_from_code();
				let vals = get_expression_from_code();

				if (oper.op_code() == OP_CODE::apply_op) {

					if (vals.type() == "expression") {

						let l = pop_lead(vals);

						while (l.type() == "symbol") {
							l = get_symbol(l);
						}

						vals = vals.place_lead(l);

						Olly::evaluator olly;
						vals = olly.eval(vals);

						print("vals = " + str(vals));
					}

					oper = op_call(OP_CODE::eq_op);
				}

				if (vars.type() != "expression") {
					vars = expression(vars);
				}

				if (vals.type() != "expression") {
					vals = expression(vals);
				}

				if (oper.type() == "op_call") {

					while (vars.is()) {

						let var = pop_lead(vars);
						let val = pop_lead(vals);

						if (oper.op_code() == OP_CODE::const_op) {

							if (var.type() == "symbol") {
								set_constant(var, val);
							}
						}
						else if (oper.op_code() == OP_CODE::eq_op) {

							if (var.type() == "symbol") {
								set_variable(var, val);
							}
						}
					}
				}

			}	break;

			case OP_CODE::def_op: {  // Presupose the definition of a constant or variable.  

				let var  = get_expression_from_code();
				let args = get_expression_from_code();
				let body = get_expression_from_code();

				let lam = lambda(args, body);

				set_expression_on_code(lam);
				set_expression_on_code(op_call(OP_CODE::eq_op));
				set_expression_on_code(var);
				set_expression_on_code(op_call(OP_CODE::let_op));


			}	break;

			case OP_CODE::bind_op: {  // Bind variables or constants to an enclosure.

				let args = get_expression_from_code();
				let oper = get_expression_from_code();
				let lamb = get_expression_from_code();

				let function_name = lamb;

				while (lamb.type() == "symbol") {
					lamb = get_symbol(lamb);
				}

				if (lamb.type() == "lambda") {

					OP_CODE op = oper.op_code();

					auto l = lambda(lamb);

					while (args.is()) {

						let arg = pop_lead(args);
						let val = get_symbol(arg);

						while (val.type() == "symbol") {
							val = get_symbol(val);
						}

						switch (op) {

						case OP_CODE::const_op:
							l.bind_constant(arg, val);
							break;

						case OP_CODE::eq_op:
							l.bind_variable(arg, val);
							break;

						default:
							break;
						}
					}

					lamb = l;

					set_variable(function_name, lamb);
				}
				
			}	break;

			case OP_CODE::return_op: {  // Return an expression from a function, then quite the function.

				let args = get_expression_from_code();

				if (args.type() != "expression") {
					args = expression(args);
				}

				let queue = expression();

				while (args.is()) {

					let a = pop_lead(args);

					while (a.type() == "symbol") {
						a = get_symbol(a);
					}

					queue = queue.place_lead(a);
				}

				let end = op_call(OP_CODE::end_scope_op);
				let itr = get_expression_from_code();

				while (end != itr) {
					itr = get_expression_from_code();
				}

				while (queue.is()) {

					let a = pop_lead(queue);

					set_expression_on_code(a);
				}
				set_expression_on_code(end);

			} break;

			case OP_CODE::relent_op: { // Place the next expression on to the stack without evaluation.  

				let val = get_expression_from_code();

				while (val.type() == "symbol") {
					val = get_symbol(val);
				}
				set_expression_on_stack(val);
			} break;

			default:
				break;
		}
	}

	inline void Olly::evaluator::postfix_unary_operators(OP_CODE& opr) {

		let x = get_expression_from_stack();
		let y;

		switch (opr) {

			case OP_CODE::IS_op:
				y = boolean(x.is());
				break;

			case OP_CODE::NEG_op:
				y = x.neg();
				break;

			case OP_CODE::NOT_op:	
				y = boolean(x.l_not());
				break;

			default:
				break;
		}

		set_expression_on_stack(y);
	}

	inline void Olly::evaluator::prefix_unary_operators(OP_CODE& opr) {

		let  x = get_expression_from_code();
		let op;

		switch (opr) {

		case OP_CODE::neg_op:
			op = op_call(OP_CODE::NEG_op);
			break;

		case OP_CODE::not_op:
			op = op_call(OP_CODE::NOT_op);
			break;

		default:
			break;
		}

		let exp = expression();
		exp = exp.place_lead(op);
		exp = exp.place_lead(x);

		set_expression_on_code(exp);
	}

	inline void Olly::evaluator::postfix_binary_operators(OP_CODE& opr) {

		let y = get_expression_from_stack();
		let x = get_expression_from_stack();

		let z;

		switch (opr) {

			/**** Logical Operators ****/

			case OP_CODE::AND_op:
				z = boolean(x.l_and(y));
				break;

			case OP_CODE::OR_op:
				z = boolean(x.l_or(y));
				break;

			case OP_CODE::XOR_op:
				z = boolean(x.l_xor(y));
				break;


			/**** Comparision Operations ****/

			case OP_CODE::EQ_op:
				z = boolean(x == y);
				break;

			case OP_CODE::NE_op: 
				z = boolean(x != y);
				break;

			case OP_CODE::LT_op:
				z = boolean(x < y);
				break;

			case OP_CODE::LE_op: 
				z = boolean(x <= y);
				break;

			case OP_CODE::GT_op:
				z = boolean(x > y);
				break;

			case OP_CODE::GE_op: 
				z = boolean(x >= y);
				break;


			/**** Base Mathmatical Operations ****/

			case OP_CODE::ADD_op:
				z = x + y;
				break;

			case OP_CODE::SUB_op: 
				z = x - y;
				break;

			case OP_CODE::MUL_op: 
				z = x * y;
				break;

			case OP_CODE::DIV_op: 
				z = x / y;
				break;

			case OP_CODE::MOD_op:
				z = x % y;
				break;

			case OP_CODE::FDIV_op: 
				z = x.f_div(y);
				break;

			case OP_CODE::REM_op: 
				z = x.rem(y);
				break;

			case OP_CODE::POW_op:
				z = x.pow(y);
				break;

			default:
				break;
		}

		set_expression_on_stack(z);
	}

	inline void Olly::evaluator::infix_binary_operators(OP_CODE& opr) {

		let  x = get_expression_from_code();
		let op;

		switch (opr) {

			/**** Logical Operators ****/

			case OP_CODE::and_op:
				op = op_call(OP_CODE::AND_op);
				break;

			case OP_CODE::or_op:
				op = op_call(OP_CODE::OR_op);
				break;

			case OP_CODE::xor_op:
				op = op_call(OP_CODE::XOR_op);
				break;


			/**** Comparision Operations ****/

			case OP_CODE::eq_op:
				op = op_call(OP_CODE::EQ_op);
				break;

			case OP_CODE::ne_op:
				op = op_call(OP_CODE::NE_op);
				break;

			case OP_CODE::lt_op:
				op = op_call(OP_CODE::LT_op);
				break;

			case OP_CODE::le_op:
				op = op_call(OP_CODE::LE_op);
				break;

			case OP_CODE::gt_op:
				op = op_call(OP_CODE::GT_op);
				break;

			case OP_CODE::ge_op:
				op = op_call(OP_CODE::GE_op);
				break;


			/**** Base Mathmatical Operations ****/

			case OP_CODE::add_op:
				op = op_call(OP_CODE::ADD_op);
				break;

			case OP_CODE::sub_op:
				op = op_call(OP_CODE::SUB_op);
				break;

			case OP_CODE::mul_op:
				op = op_call(OP_CODE::MUL_op);
				break;

			case OP_CODE::div_op:
				op = op_call(OP_CODE::DIV_op);
				break;

			case OP_CODE::mod_op:
				op = op_call(OP_CODE::MOD_op);
				break;

			case OP_CODE::fdiv_op:
				op = op_call(OP_CODE::FDIV_op);
				break;

			case OP_CODE::rem_op:
				op = op_call(OP_CODE::REM_op);
				break;

			case OP_CODE::pow_op:
				op = op_call(OP_CODE::POW_op);
				break;

			default:
				break;
		}

		set_expression_on_code(op);
		set_expression_on_code(x);
	}

	inline void Olly::evaluator::extended_logical_operators(OP_CODE& opr) {

		switch (opr) {

			case OP_CODE::imply_op: {    // Logical implication.
				/*
					(p -> q) ^ (!p -> r)

					(p IMPLY q)(P NOT IMPLY r) OR
				*/

				let p = get_expression_from_stack();
				let q = get_expression_from_code();

			}	break;

			case OP_CODE::else_op:   // Logical implication.
				// No action taken unless found in relationship to an implication.  
				// Which is handled in the implication operation application.
				if (is_operator_call(OP_CODE::imply_op, peek_expression_from_code())) {
					set_expression_on_stack(boolean(true));
				}
				break;

			case OP_CODE::cond_op: {    // Conditional logical implications.
				let conditions = get_expression_from_code();

				if (conditions.type() == "expression") {

					if (conditions.lead().type() == "op_call") {
						conditions = conditions.shift_lead();
					}

					let props = expression();
					let quots = expression();

					while (conditions.is()) {

						let p = pop_lead(conditions);
						let q = pop_lead(conditions);

						if (q.type() == "expression") {

							props = props.place_lead(p);
							quots = quots.place_lead(q);
						}
						else {
							conditions = nothing();
						}
					}

					if (props.is()) {

						let exp       = expression();
						let impl_oper = op_call(OP_CODE::imply_op);
						let else_oper = op_call(OP_CODE::else_op);

						exp = exp.place_lead(expression());
						exp = exp.place_lead(else_oper);
						exp = exp.place_lead(pop_lead(quots));
						exp = exp.place_lead(impl_oper);
						exp = exp.place_lead(pop_lead(props));

						while (props.is()) {

							let new_exp = expression();

							new_exp = new_exp.place_lead(exp);
							new_exp = new_exp.place_lead(else_oper);

							new_exp = new_exp.place_lead(pop_lead(quots));
							new_exp = new_exp.place_lead(impl_oper);
							new_exp = new_exp.place_lead(pop_lead(props));

							exp = new_exp;
						}
						print(exp);
						set_expression_on_code(exp);
					}
				}

			}	break;

			case OP_CODE::loop_op: {

				let p = get_expression_from_code();
				let q = get_expression_from_code();

				_code.push_back(expression());

				set_expression_on_code(q);
				set_expression_on_code(p);
				set_expression_on_code(op_call(OP_CODE::loop_op));
				set_expression_on_code(op_call(OP_CODE::END_op));
				set_expression_on_code(op_call(OP_CODE::else_op));
				set_expression_on_code(q);
				set_expression_on_code(op_call(OP_CODE::imply_op));
				set_expression_on_code(p);

			}	break;

			case OP_CODE::is_const_op: {   // Postfix is constant.
				let x = get_expression_from_code();

				let truth = boolean(false);

				if (x.type() == "symbol") {

					str_t symbol_name = to_upper(repr(x));

					for (auto i = _constants.crbegin(); i != _constants.crend(); ++i) {

						auto c_itr = i->find(symbol_name);

						if (c_itr != i->end()) {
							truth = boolean(true);
						}
					}
				}

				if (x.type() == "op_call") {
					truth = boolean(true);
				}

				set_expression_on_stack(truth);
			}	break;

			case OP_CODE::is_var_op: {   // Postfix is variable.
				let x = get_expression_from_code();

				let truth = boolean(false);

				if (x.type() == "symbol") {

					str_t symbol_name = repr(x);

					for (auto i = _variables.crbegin(); i != _variables.crend(); ++i) {

						auto v_itr = i->find(symbol_name);

						if (v_itr != i->end()) {
							truth = boolean(true);
						}
					}
				}

				set_expression_on_stack(truth);
			}	break;

			default:
				break;
		}
	}

	inline void Olly::evaluator::abstraction_operators(OP_CODE& opr) {


		switch (opr) {

			default:
				break;
		}
	}

} // end