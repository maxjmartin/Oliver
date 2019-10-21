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

#include "parser.h"
#include "var.h"

#include "expression.h"
#include "set.h"
#include "number.h"
#include "string.h"

namespace Olly {

	/********************************************************************************************/
	//
	//                                The Oliver Interpreter Runtime
	//
	//          The Oliver Interpreter runtime is defined below.  It is design to run
	//			as a stack inpterpreter evaluation of the code expression passed to it.
	// 
	//          The tail optimization code was modified from Andy Balaam's blog post.
	//          https://www.artificialworlds.net/blog/2012/04/30/tail-call-optimisation-in-cpp/
	//
	/********************************************************************************************/

	/********************************************************************************************/
	//
	//                                'Closure' Class Definition
	//
	//			The Closure class wraps arounf a function passed to the class to allow
	//			it to then wrapped in a Lambda object allowing it to be tramoplined when
	//			called.  
	//
	/********************************************************************************************/

	class Lambda;	

	class Closure {

		typedef Lambda(*function)(var, var, var);

		function _func;		// Function
		var      _args;		// Arguments
		var      _resl;		// Resolution
		var      _envr;		// Environment

	public:
		
		static const function NULL_FUNCTION;

		Closure(function func, var args, var resl, var envr);

		Lambda operator()();
	};

	/*
		Define a null function, and a null lambda function 
		to be used to define a completed Lambda result later.
	*/ 
	const Closure::function Closure::NULL_FUNCTION = nullptr;
	const Closure NULL_LAMBDA(Closure::NULL_FUNCTION, null(), null(), null());

	/********************************************************************************************/
	//
	//                                'Lambda' Class Definition
	//
	//			The Lambda class accepts a Closure data type, and manages the closure
	//			to identify when it has completed running and what the result of the
	//			function closure evaluation is.  
	//
	/********************************************************************************************/

	class Lambda {

	public:

		Closure   lambda;

		Lambda(Closure clsr, var resl, bool_t fin);

		bool_t not_done() const;
		var range() const;

	private:
		var       _result;
		bool_t    _finished;
	};

	/********************************************************************************************/
	//
	//                       'function_call' Trampoline Function Definitions
	//
	/********************************************************************************************/

	var    function_call(Lambda func);
	Lambda function_call(var env, var expr, var stack);

	/********************************************************************************************/
	//
	//                                Helper Function Definitions
	//
	/********************************************************************************************/

	bool_t expression_is_empty(var expr);

	Lambda result(var stack);

	var pop_lead(var& expr);

	/********************************************************************************************/
	//
	//                                'Oliver' Function Definitions
	//
	/********************************************************************************************/
	
	var       Oliver  (var expr);
	var       Oliver  (var env, var expr);
	Lambda  __Oliver__(var env, var expr, var stack);

	/********************************************************************************************/
	//
	//                                'Lambda' Class Implimentation
	//
	/********************************************************************************************/

	Lambda::Lambda(Closure clsr, var resl, bool_t fin) : lambda(clsr), _result(resl), _finished((!fin))  {
	}

	bool_t Lambda::not_done() const {
		return _finished;
	}

	var Lambda::range() const {
		return _result;
	}

	/********************************************************************************************/
	//
	//                                'Closure' Class Implimentation
	//
	/********************************************************************************************/

	Closure::Closure(function func, var args, var resl, var envr) : _func(func), _args(args), _resl(resl), _envr(envr) {
	}

	Lambda Closure::operator()() {
		return _func(_args, _resl, _envr);
	}

	/********************************************************************************************/
	//
	//                       'function_call' Trampoline Function Implimentation
	//
	/********************************************************************************************/

	var function_call(Lambda func) {
		/*
			Trampoline the lambda function calls to
			allow tail function call optimization.
			Once the function call is done processing
			return the range of the fuction's result.
		*/
		while (func.not_done()) {
			func = func.lambda();
		}
		return func.range();
	}

	Lambda function_call(var env, var expr, var stack) {
		/*
			Invoke an instance of the current function call environment.
		*/
		return Lambda(Closure(__Oliver__, env, expr, stack), null(), false);
	}

	/********************************************************************************************/
	//
	//                                 Helper Function Implimentations
	//
	/********************************************************************************************/

	bool_t expression_is_empty(var expr) {
		/*
			Return true if the expresion is empty.  
		*/
		return !expr.is();
	}

	Lambda result(var stack) {
			/*
				Package the stack in a null lambda call,
				which has finished running.  
			*/
		return Lambda(NULL_LAMBDA, stack, true);
	}

	var pop_lead(var& expr) {
		var a = expr.lead();
		expr = expr.shift();
		return a;
	}

	/********************************************************************************************/
	//
	//                                'Oliver' Function Implimentation
	//
	/********************************************************************************************/

	var Oliver(var expr) {
		return Oliver(expression(), expr);
	}

	var Oliver(var env, var expr) {
		return function_call(Lambda(Closure(__Oliver__, env, expr, expression()), null(), false));
	}

	var Oliver(var env, var expr, var stack) {
		return function_call(Lambda(Closure(__Oliver__, env, expr, stack), null(), false));
	}

	Lambda __Oliver__(var env, var expr, var stack) {

		if (expression_is_empty(expr)) {
			/*
				If there are no argumenta within the expression return the stack.
			*/
			return result(stack);
		}

		var a = pop_lead(expr);

		auto t_name = a.type();

		if (t_name == "symbol") {

			// a = get_symbolic_value(arg);
		}

		if (t_name == "expression") {
			/*
				Evaluate the expression with its own stack during runtime.  
				But maintain the current environment.
			*/
			a = Oliver(env, a);

			stack = stack.place(a);
		}
		else if (t_name != "op_call") {
			stack = stack.place(a);
		}
		else {

			switch (a.integer()) {

			case 11:	// STACK = Print the current stack queue.
			{
				print(stack);
			}
				break;

			case 12:	// CODE = Print the current code queue.
			{
				print(expr);
			}
			break;

			case 21:	// POP = Remove the top element of the stack.
			{
				stack = stack.shift();
			}
			break;

			case 22:	// DUP = Duplicate the top element of the stack.
			{
				stack = stack.place(stack.lead());
			}
			break;

			case 23:	// SWAP = Change the order of the top two elements of the stack.
			{
				stack = stack.place(stack.lead());
			}
			break;

			case 24:	// ROLL = Roll or Rotate all elements of the stack.  
			{			// Takea a single numerical integer argument.  
						// arg < 0  Roll top elements to the bottom of the stack.  
						// arg > 0  Roll bottom elements to the top of the stack.
				stack = stack.place(stack.lead());
			}
			break;


			default:
				break;
			}
		}

		return function_call(env, expr, stack);
	}

} // end