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
		evaluator(evaluator& env);

		let eval(let exp);

	private:

		static const str_t STACK_UNDERFLOW;

		inline void eval();

		inline let get_result_stack() const;

		inline void prep_code_to_execute(let& exp) const;

		inline let  get_symbol(let& var) const;
		inline void set_constant(let& var, let& val, bool_t define_new = false);
		inline void set_variable(let& var, let& val, bool_t define_new = false);

		inline bool_t constant_not_defined(const str_t& symbol_name) const;

		inline void define_stack();
		inline void delete_stack();

		inline void define_enclosure();
		inline void delete_enclosure();

		inline void  set_expression_on_code(let exp);
		inline void  set_expression_on_stack(let exp);
		inline void  set_expression_to_outer_scope(let exp);
		inline let   get_expression_from_stack();
		inline let   get_expression_from_code();
		inline let  peek_expression_from_code();
		inline let   get_expression_from_outer_scope();
		inline void drop_expression_from_code();
		inline void drop_expression_from_stack();

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

	evaluator::evaluator(evaluator& env) : _constants(env._constants), _variables(env._variables), _stack(env._stack), _code(env._code) {
	}

	let evaluator::eval(let exp) {

		if (exp.type() != "expression") {
			return nothing();
		}

		prep_code_to_execute(exp);

		_code.push_back(exp);

		define_stack();
		define_enclosure();

		eval();

		return get_result_stack().reverse();
	}

	inline let Olly::evaluator::get_result_stack() const {

		if (!_stack.empty()) {
			return _stack[0];
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

		str_t symbol_name = repr(var);

		for (auto i = _variables.crbegin(); i != _variables.crend(); ++i) {

			auto v_itr = i->find(symbol_name);

			if (v_itr != i->end()) {
				return v_itr->second;
			}
		}

		symbol_name = to_upper(symbol_name);

		for (auto i = _constants.crbegin(); i != _constants.crend(); ++i) {

			auto c_itr = i->find(symbol_name);

			if (c_itr != i->end()) {
				return c_itr->second;
			}
		}

		return nothing();
	}

	inline void Olly::evaluator::set_constant(let& var, let& val, bool_t define_new) {

		if (!_constants.empty()) {

			str_t symbol_name = to_upper(repr(var));

			if (define_new) {

				auto c_itr = _constants.back().find(symbol_name);

				if (c_itr != _constants.back().end()) {
					
					_constants.back()[symbol_name] = val;
				}
			}

			else if (constant_not_defined(symbol_name)) {

				_constants.back()[symbol_name] = val;
			}
		}
	}

	inline void Olly::evaluator::set_variable(let& var, let& val, bool_t define_new) {

		if (!_variables.empty()) {

			str_t symbol_name = repr(var);

			if (constant_not_defined(to_upper(symbol_name))) {

				if (define_new) {
					_variables.back()[symbol_name] = val;
					return;
				}
				
				for (auto i = _variables.rbegin(); i != _variables.rend(); ++i) {

					auto v_itr = i->find(symbol_name);

					if (v_itr != i->end()) {
						(*i)[symbol_name] = val;
						return;
					}
				}
			}
			_variables.back()[symbol_name] = val;
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

	inline void Olly::evaluator::define_stack() {
		_stack.push_back(expression());
	}

	inline void Olly::evaluator::delete_stack() {
		_stack.pop_back();
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

		if (!_stack.empty()) {
			_stack.back() = _stack.back().place_lead(exp);
		}
	}

	inline void Olly::evaluator::set_expression_to_outer_scope(let exp) {

		if (!_code.empty()) {

			let scope = _code.back();
			_code.pop_back();

			set_expression_on_code(exp);
			_code.push_back(scope);
		}
	}

	inline let Olly::evaluator::get_expression_from_stack() {

		if (!_stack.empty()) {
			return pop_lead(_stack.back());
		}

		return error("stack_underflow");
	}

	inline let Olly::evaluator::get_expression_from_code() {

		if (!_code.empty()) {

			let a = pop_lead(_code.back());

			if (a.is_nothing() && expression_is_empty(_code.back())) {
				_code.pop_back();
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

	inline let Olly::evaluator::get_expression_from_outer_scope() {

		if (!_code.empty()) {

			let scope = _code.back();
			_code.pop_back();

			let a = get_expression_from_code();
			_code.push_back(scope);

			return a;
		}

		return nothing();
	}

	inline void Olly::evaluator::drop_expression_from_code() {

		if (!_code.empty()) {

			let a = pop_lead(_code.back());

			if (expression_is_empty(_code.back())) {
				_code.pop_back();
			}
		}
	}

	inline void Olly::evaluator::drop_expression_from_stack() {

		if (!_stack.empty()) {

			_stack.pop_back();
		}
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

			else if (exp.type() != "op_call") {
				set_expression_on_stack(exp);
			}

			else {

				OP_CODE opr = exp.op_code();

				if (opr < OP_CODE::FUNCTION_SCOPE_OPERATORS) {
					function_scope_operators(opr);
				}

				else if (opr < OP_CODE::POSTFIX_UNARY_OPERATORS) {
					postfix_unary_operators(opr);
				}

				else if (opr < OP_CODE::POSTFIX_BINARY_OPERATORS) {
					postfix_binary_operators(opr);
				}

				else if (opr < OP_CODE::EXTENDED_LOGIC_OPERATORS) {
					extended_logical_operators(opr);
				}

				else if (opr < OP_CODE::PREFIX_UNARY_OPERATORS) {
					prefix_unary_operators(opr);
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
			
		} while (!_code.empty() && run_continous);
	}

	inline void Olly::evaluator::function_scope_operators(OP_CODE& opr) {

		switch (opr) {

			case OP_CODE::NOTHING_OP:    // Nothing Operator
				// Do nothing.
				break;

			/****  Scope Enclosure Operators ****/

			case OP_CODE::begin_scope_op: {    // Define a new scope.
				define_stack();
				define_enclosure();
			}	break;

			case OP_CODE::end_scope_op: {    // Delete the current scope.
				delete_stack();
				delete_enclosure();
			}	break;

			case OP_CODE::abstruse_op: {    // Fetch an expression from outside the current scope.
				let a = get_expression_from_outer_scope();
				set_expression_on_stack(a);
			}	break;

			case OP_CODE::abstrude_op: {    // Set an expression to the external scope.
				let a = get_expression_from_stack();
				set_expression_to_outer_scope(a);
			}	break;

			case OP_CODE::CONST_op: {    // Define a constant variable.

				let var = get_expression_from_code();
				let val = get_expression_from_stack();

				if (var.type() == "symbol") {
					set_constant(var, val);
				}
			}	break;

			case OP_CODE::LET_op: {    // Define a variable.

				let var = get_expression_from_code();
				let val = get_expression_from_stack();

				if (var.type() == "symbol") {
					set_variable(var, val);
				}
			}	break;

			case OP_CODE::let_op: {  // Presupose the definition of a constant or variable.  

				let var = get_expression_from_code();
				let opr = get_expression_from_code();
				let val = get_expression_from_code();

				bool_t run = false;

				if (opr.type() == "op_call") {

					if (opr.op_code() == OP_CODE::const_op) {

						opr = op_call(OP_CODE::CONST_op);
						run = true;
					}
					else if (opr.op_code() == OP_CODE::eq_op) {

						opr = op_call(OP_CODE::LET_op);
						run = true;
					}
				}

				if (run) {
					set_expression_on_code(var);
					set_expression_on_code(opr);
					set_expression_on_code(val);
				}

			}	break;

			case OP_CODE::def_op: {  // Presupose the definition of a constant or variable.  

				let var = get_expression_from_code();
				let opr = get_expression_from_code();
				let val = get_expression_from_code();

				if (opr.type() == "op_call" && var.type() == "symbol") {

					if (opr.op_code() == OP_CODE::const_op) {

						set_constant(var, val, true);
					}
					else if (opr.op_code() == OP_CODE::eq_op) {

						set_variable(var, val, true);
					}
				}

			}	break;

			case OP_CODE::EXC_op: {    // Exclose - the current enclosure to the outer enclosure.

				if (_constants.size() >= 2) {

					map_t closure = _constants.back();
					_constants.pop_back();

					for (auto& it : closure) {
						_constants.back()[it.first] = it.second;
					}

					_constants.push_back(closure);
				}

				if (_variables.size() >= 2) {

					map_t closure = _variables.back();
					_variables.pop_back();

					for (auto& it : closure) {
						_variables.back()[it.first] = it.second;
					}

					_variables.push_back(closure);
				}

			}	break;

			case OP_CODE::ENC_op: {    // Enclose - the current closure to the sub enclusre.

				let code = get_expression_from_stack();

				let exp = pop_lead(code);

				if (exp.type() == "expression" && exp.lead().type() == "op_call" && exp.lead().op_code() == OP_CODE::begin_scope_op) {

					let l = pop_lead(exp);

					if (!_variables.empty() && !_variables.back().empty()) {

						let vars = expression();
						let oper = op_call(OP_CODE::LET_op);

						for (auto& it : _variables.back()) {

							let var = symbol(it.first);
							let val = it.second;

							vars = vars.place_lead(var);
							vars = vars.place_lead(oper);
							vars = vars.place_lead(val);
						}

						exp = exp.place_lead(vars);
					}

					if (!_constants.empty() && !_constants.back().empty()) {

						let consts = expression();
						let oper = op_call(OP_CODE::CONST_op);

						for (auto& it : _constants.back()) {

							let var = symbol(it.first);
							let val = it.second;

							consts = consts.place_lead(val);
							consts = consts.place_lead(oper);
							consts = consts.place_lead(var);
						}

						exp = exp.place_lead(consts);
					}
					exp = exp.place_lead(l);

					print("exp = " + str(exp));

					set_expression_on_stack(exp);

					while (code.is()) {
						let exp = pop_last(code);
						set_expression_on_code(exp);
					}
				}

			}	break;

			default:
				break;
		}
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

		set_expression_on_code(op);
		set_expression_on_code(x);
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

				if (p.is()) {

					if (is_operator_call(OP_CODE::else_op, peek_expression_from_code())) {
						drop_expression_from_code();
						drop_expression_from_code();
					}

					set_expression_on_code(q);
				}
				else if (is_operator_call(OP_CODE::else_op, peek_expression_from_code())) {
					drop_expression_from_code();
					print(peek_expression_from_code());
				}

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

					if (conditions.lead().type() == "op_call" && conditions.lead().op_code() == OP_CODE::begin_scope_op) {
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