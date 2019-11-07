#pragma once

#include "sys_types.h"

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

namespace Olly {


	/********************************************************************************************/
	//
	//                                'let' Class Definition
	//
	//          The 'let' class represents an immutable object wrapper.  It will accept 
	//          any object assigned to by the assignment operator '='.
	//              Example:  let a = 42;
	//
	//          The 'let' class supports both a functional and object oriented API.
	//          But it requires that any object utilizing its API override a series of
	//          functions described below, in order to utilize run-time polymorphism.
	//
	//          The 'let' class also supports both pass by reference and pass by value.
	//          Any object it holds to be safely cast to a point_ter of the object type.  
	//          If the type cast is made to an incorrect data type then a std::null_ptr 
	//          is returned instead.
	//
	//          The fundamental structure of the 'let' data type was inspired and extended 
	//          from a presentation entitled:
	//                 Title: Value Semantics and Concept-based Polymorphism
	//                 By - Sean Parent
	//                 (http://sean-parent.stlab.cc/papers-and-presentations)
	//
	/********************************************************************************************/

	class  null;
	class  lambda_t;
	struct environment;

	class let {

	public:

		let();
		template <typename T>          let(T x);
		template <typename T>          let(T* x);

		template <typename T> T        as()                             const;
		template <typename T> const T* cast()                           const;

		str_t             id()                                          const;
		bool_t       is_type(const let& other)                          const;
		std::size_t     hash()                                          const;

		str_t           type()                                          const;
		bool_t            is()                                          const;
		void             str(stream_t& out)                             const;
		void            repr(stream_t& out)                             const;

		real_t          comp(const let& other)                          const;
		bool_t            eq(const let& other)                          const;
		bool_t            ne(const let& other)                          const;
		bool_t            ge(const let& other)                          const;
		bool_t            le(const let& other)                          const;
		bool_t            gt(const let& other)                          const;
		bool_t            lt(const let& other)                          const;

		bool_t         l_and(const let& other)                          const;
		bool_t          l_or(const let& other)                          const;
		bool_t         l_xor(const let& other)                          const;
		bool_t         l_not()                                          const;

		let              add(const let& other)                          const;
		let              sub(const let& other)                          const;
		let              mul(const let& other)                          const;
		let              div(const let& other)                          const;
		let              mod(const let& other)                          const;

		let            f_div(const let& other)                          const;
		let              rem(const let& other)                          const;
		let              pow(const let& other)                          const;

		int_t            len()                                          const;
		let             lead()                                          const;
		let            place(const let& other)                          const;
		let            shift()                                          const;
		let          reverse()                                          const;
		let              get(const let& key)                            const;
		let              set(const let& key, const let& val)            const;

		bool_t     interable()                                          const;
		bool_t       nothing()						                    const;
		bool_t        atomic()                                          const;
		bool_t           nan()                                          const;
		bool_t       complex()                                          const;

		str_t           help()                                          const;

		bool_t    operator==(const let& other)                          const;
		bool_t    operator!=(const let& other)                          const;
		bool_t    operator>=(const let& other)                          const;
		bool_t    operator> (const let& other)                          const;
		bool_t    operator<=(const let& other)                          const;
		bool_t    operator< (const let& other)                          const;

		let        operator+(const let& other)                          const;
		let        operator-(const let& other)                          const;
		let        operator*(const let& other)                          const;
		let        operator/(const let& other)                          const;
		let        operator%(const let& other)                          const;

		int_t        integer()                                          const;

		lambda_t          eval(const environment& env, let& exp)        const;

	private:

		struct interface_t {

			/********************************************************************************************/
			//
			//                              'interface_t' Class Definition
			//
			//       A simple interface description allowing redirection of the 'let' data type.
			//
			/********************************************************************************************/

			virtual  ~interface_t()                                                     = default;
			virtual operator bool()                                               const = 0;

			virtual void*         __as()                                                = 0;
			virtual str_t         __id()                                          const = 0;
			virtual std::size_t   __hash()                                        const = 0;

			virtual str_t         __type()                                        const = 0;
			virtual bool_t        __is()                                          const = 0;
			virtual void          __str(stream_t& out)                            const = 0;
			virtual void          __repr(stream_t& out)                           const = 0;

			virtual real_t        __comp(const let& other)                        const = 0;

			virtual bool_t       __l_and(const let& other)                        const = 0;
			virtual bool_t       __l_or(const let& other)                         const = 0;
			virtual bool_t       __l_xor(const let& other)                        const = 0;
			virtual bool_t       __l_not()                                        const = 0;

			virtual let          __add(const let& other)                          const = 0;
			virtual let          __sub(const let& other)                          const = 0;
			virtual let          __mul(const let& other)                          const = 0;
			virtual let          __div(const let& other)                          const = 0;
			virtual let          __mod(const let& other)                          const = 0;

			virtual let          __f_div(const let& other)                        const = 0;
			virtual let          __rem(const let& other)                          const = 0;
			virtual let          __pow(const let& other)                          const = 0;

			virtual int_t        __len()                                          const = 0;
			virtual let          __lead()                                         const = 0;
			virtual let          __place(const let& other)                        const = 0;
			virtual let          __shift()                                        const = 0;
			virtual let          __reverse()                                      const = 0;
			virtual let          __get(const let& key)                            const = 0;
			virtual let          __set(const let& key, const let& val)            const = 0;

			virtual bool_t       __interable()                                    const = 0;
			virtual bool_t       __nothing()					                  const = 0;
			virtual bool_t       __atomic()                                       const = 0;
			virtual bool_t       __nan()                                          const = 0;
			virtual bool_t       __complex()                                      const = 0;

			virtual str_t        __help()                                         const = 0;

			virtual int_t        __integer()                                      const = 0;

			virtual lambda_t       __eval(const environment& env, let& exp)       const = 0;
		};

		template <typename T>
		struct data_t : interface_t {

			/******************************************************************************************/
			//
			//                                  'data_t' Class Definition
			//
			//             The int_terface implementation of the 'interface_t' defining a
			//             shared const point_ter of the data type passed to it.
			//
			/******************************************************************************************/

			data_t(T val);
			operator bool()                                               const;

			void* __as();
			str_t        __id()                                           const;
			std::size_t  __hash()                                         const;

			str_t        __type()                                         const;
			bool_t       __is()                                           const;
			void         __str(stream_t& out)                             const;
			void         __repr(stream_t& out)                            const;

			real_t       __comp(const let& other)                         const;

			bool_t       __l_and(const let& other)                        const;
			bool_t       __l_or(const let& other)                         const;
			bool_t       __l_xor(const let& other)                        const;
			bool_t       __l_not()                                        const;

			let          __add(const let& other)                          const;
			let          __sub(const let& other)                          const;
			let          __mul(const let& other)                          const;
			let          __div(const let& other)                          const;
			let          __mod(const let& other)                          const;

			let          __f_div(const let& other)                        const;
			let          __rem(const let& other)                          const;
			let          __pow(const let& other)                          const;

			int_t        __len()                                          const;
			let          __lead()                                         const;
			let          __place(const let& other)                        const;
			let          __shift()                                        const;
			let          __reverse()                                      const;
			let          __get(const let& key)                            const;
			let          __set(const let& key, const let& val)            const;

			bool_t       __interable()                                    const;
			bool_t       __nothing()						              const;
			bool_t       __atomic()	     					              const;
			bool_t       __nan()	     					              const;
			bool_t       __complex()	     					          const;

			str_t        __help()                                         const;

			int_t        __integer()                                      const;

			lambda_t       __eval(const environment& env, let& exp)       const;

			T            _data;
		};

		std::shared_ptr<const interface_t> _self;
	};

	/********************************************************************************************/
	//
	//                                 'null' Class Definition
	//
	//          A basic class definition of the value of nothing.  This is used within
	//          'let' class implementation to return a result of nothing for results
	//          which have either conflicting types, or in some cases as the default to
	//          return unless overridden.
	//
	//          This class also demonstrates the basic function methods that should be
	//          over written for proper object behavior.  
	//
	/********************************************************************************************/

	class null {

	public:

		null();
		null(const null& obj);
		null(str_t str);
		virtual ~null();

		friend str_t      __type__(const null& self);
		friend bool_t       __is__(const null& self);
		friend real_t     __comp__(const null& self, const let& other);
		friend void        __str__(stream_t& out, const null& self);
		friend void       __repr__(stream_t& out, const null& self);

		friend bool_t  __nothing__(const null& self);
	};


	/********************************************************************************************/
	//
	//                                 'node' Class Definition
	//
	//          The node class is implimented using Lisp inspired data nodes.  
	//          At one time this had been implimented with C++ std::vectors.  
	//          But given the imuttable nature of the interpreter this resulted 
	//          in a signifigant slow down during run time.  
	//
	/********************************************************************************************/

	class node {

		let _data;
		let _next;

	public:

		node();
		node(const node& exp);
		node(let obj);
		virtual ~node();

		friend str_t           __type__(const node& self);
		friend bool_t            __is__(const node& self);
		friend real_t          __comp__(const node& self, const let& other);
		friend void             __str__(stream_t& out, const node& self);
		friend void            __repr__(stream_t& out, const node& self);

		friend int_t            __len__(const node& self);
		friend let             __lead__(const node& self);
		friend let            __place__(const node& self, const let& other);
		friend let            __shift__(const node& self);
		friend let          __reverse__(const node& self);
	};

	/********************************************************************************************/
	//
	//                              'expression' Class Definition
	//
	//          The expression class is a Lisp linked list inspired data type.  Which
	//          manages a list of node objects.  Strictly speaking this class is not
	//          necessary for this project.  The node class alone will suffice to allow
	//          all of the necessary functions and data types to be implimented on thier
	//          own.  But this class does allow for an extra layer of abstraction.  
	//
	/********************************************************************************************/

	class expression {

		let    _list;
		int_t  _len;

	public:

		expression();
		expression(const expression& exp);
		expression(let x);
		expression(let x, let y);
		virtual ~expression();

		friend str_t           __type__(const expression& self);
		friend bool_t            __is__(const expression& self);
		friend real_t          __comp__(const expression& self, const let& other);
		friend void             __str__(stream_t& out, const expression& self);
		friend void            __repr__(stream_t& out, const expression& self);

		friend int_t            __len__(const expression& self);
		friend let             __lead__(const expression& self);
		friend let            __place__(const expression& self, const let& other);
		friend let            __shift__(const expression& self);
		friend let          __reverse__(const expression& self);
	};

	/********************************************************************************************/
	//
	//          The next section impliments a function trampoline to facilitate recursive
	//          function calls without a seg fault.
	//
	//          The tail optimization code was modified from Andy Balaam's blog post.
	//          https://www.artificialworlds.net/blog/2012/04/30/tail-call-optimisation-in-cpp/
	//
	/********************************************************************************************/

	/********************************************************************************************/
	//
	//                                'environement' Class Definition
	//
	//			The Closure class wraps around a function passed to the class to allow
	//			it to then be wrapped in a Lambda object allowing it to be tramoplined 
	//          when called.  
	//
	/********************************************************************************************/

	struct environment {
		let constants;
		let variables;
		let return_stack;

		environment();
		environment(let c, let v, let s);
	};

	/********************************************************************************************/
	//
	//                                'Closure' Class Definition
	//
	//			The Closure class wraps around a function passed to the class to allow
	//			it to then be wrapped in a Lambda object allowing it to be tramoplined 
	//          when called.  
	//
	/********************************************************************************************/

	class lambda_t;

	class closure_t {

		typedef lambda_t(*function)(environment, let);

		function	  _func;		// Function
		environment	  _envr;		// Arguments
		let     	  _args;		// Arguments

	public:

		static const function NULL_FUNCTION;

		closure_t(function func, environment env, let arg);

		lambda_t operator()();
	};

	/********************************************************************************************/
	//
	//                                'lambda_t' Class Definition
	//
	//			The lambda_t class accepts a closure_t data type, and manages the closure
	//			to identify when it has completed running and what the result of the
	//			function closure evaluation is.  
	//
	/********************************************************************************************/

	class lambda_t {

	public:

		closure_t   lambda;

		lambda_t(closure_t clsr, let resl, bool_t fin);

		bool_t is_not_done() const;
		let         result() const;

	private:
		let       _result;
		bool_t    _finished;
	};

	/********************************************************************************************/
	//
	//                                Basic Primitive Declarations
	//
	//          These definitions exist to support the 'let' methods which do not have a
	//          specific operator overload.  Or in other cases to add a few useful support
	//          functions.
	//
	/********************************************************************************************/

	void print();                          // Simply print a new line.
	void print(const str_t& str);          // Accept any single string and print it with a std::endl.
	void print(const let& a);              // Accept any single 'let' and print it with a std::endl.

	str_t str(const  let& a);              // Convert any 'let' to a str_t.
	str_t repr(const let& a);              // Convert any 'let' to a str_t representation of the 'let'.

	let trampoline(lambda_t func);         // Tramploine a lambda until it returns a finished result.

	lambda_t result(let res);              // Return the finished result of a lambda invocation.  

	bool_t expression_is_empty(let expr);  // Determine if an expression is empty.

	let pop_lead(let& expr);               // Return the lead element of an expression and shift it.

	let  first(let p);                     // Return the first element of an expression.
	let second(let p);                     // Return the second element of an expression.

	/********************************************************************************************/
	//
	//                              'environment' Class Implimentation
	//
	/********************************************************************************************/

	environment::environment() : constants(expression()), variables(expression()), return_stack(expression()) {
	}

	environment::environment(let c, let v, let s) : constants(c), variables(v), return_stack(s) {
	}

	/********************************************************************************************/
	//
	//                                'lambda_t' Class Implimentation
	//
	/********************************************************************************************/

	lambda_t::lambda_t(closure_t clsr, let resl, bool_t fin) : lambda(clsr), _result(resl), _finished((!fin)) {
	}

	bool_t lambda_t::is_not_done() const {
		return _finished;
	}

	let lambda_t::result() const {
		return _result;
	}

	/********************************************************************************************/
	//
	//                                'closure_t' Class Implimentation
	//
	/********************************************************************************************/

	closure_t::closure_t(function func, environment env, let arg) : _func(func), _envr(env), _args(arg) {
	}

	lambda_t closure_t::operator()() {
		return _func(_envr, _args);
	}

	/********************************************************************************************/
	//
	//                      'let' Class Function Default Template API.
	//
	//           The following function templates define the over-ridable functions which
	//           may be used to tailor the behavior of any object held within a 'let'.
	//
	//           Each function defined here defines the default behavior of each function
	//           which is not overwritten.
	//
	/********************************************************************************************/


	template<typename T>            /****  Hash Value  ****/
	std::size_t __hash__(const T& self);

	template<typename T>
	std::size_t __hash__(const T& self) {
		return DEFAULT_HASH_FUNCTION(repr(self));
	}


	template<typename T>            /****  Type Name  ****/
	str_t __type__(const T& self);

	template<typename T>
	str_t __type__(const T& self) {
		return typeid(self).name();
	}


	template<typename T>            /****  Boolean Conversion  ****/
	bool_t __is__(const T& self);

	template<typename T>
	bool_t __is__(const T& self) {
		return false;
	}


	template<typename T>            /****  String Conversion  ****/
	void __str__(stream_t& out, const T& self);

	template<typename T>
	void __str__(stream_t& out, const T& self) {
		out << "object<" << &self << "," << __type__(self) << ">";
	}


	template<typename T>            /****  String Representation  ****/
	void __repr__(stream_t& out, const T& self);

	template<typename T>
	void __repr__(stream_t& out, const T& self) {
		out << str(null());
	}


	template<typename T>            /****  Comparison Between Variables  ****/
	real_t __comp__(const T& self, const let& other);

	template<typename T>
	real_t __comp__(const T& self, const let& other) {
		return NOT_A_NUMBER;
	}


	template<typename T>            /****  Logical And  ****/
	bool_t __l_and__(const T& self, const let& other);

	template<typename T>
	bool_t __l_and__(const T& self, const let& other) {
		return false;
	}


	template<typename T>            /****  Logical Or  ****/
	bool_t __l_or__(const T& self, const let& other);

	template<typename T>
	bool_t __l_or__(const T& self, const let& other) {
		return false;
	}


	template<typename T>            /****  Logical Exclusive Or  ****/
	bool_t __l_xor__(const T& self, const let& other);

	template<typename T>
	bool_t __l_xor__(const T& self, const let& other) {
		return false;
	}


	template<typename T>            /****  Logical Negation  ****/
	bool_t __l_not__(const T& self);

	template<typename T>
	bool_t __l_not__(const T& self) {
		return true;
	}



	template<typename T>            /****  Addition or Concatenation  ****/
	let __add__(const T& self, const let& other);

	template<typename T>
	let __add__(const T& self, const let& other) {
		return null();
	}


	template<typename T>            /****  Subtraction or Deletion  ****/
	let __sub__(const T& self, const let& other);

	template<typename T>
	let __sub__(const T& self, const let& other) {
		return null();
	}


	template<typename T>            /****  Multiplication  ****/
	let __mul__(const T& self, const let& other);

	template<typename T>
	let __mul__(const T& self, const let& other) {
		return null();
	}


	template<typename T>            /****  Division  ****/
	let __div__(const T& self, const let& other);

	template<typename T>
	let __div__(const T& self, const let& other) {
		return null();
	}

	template<typename T>            /****  Modulation  ****/
	let __mod__(const T& self, const let& other);

	template<typename T>
	let __mod__(const T& self, const let& other) {
		return null();
	}


	template<typename T>            /****  Floor Division  ****/
	let __f_div__(const T& self, const let& other);

	template<typename T>
	let __f_div__(const T& self, const let& other) {
		return null();
	}


	template<typename T>            /****  Remainder  ****/
	let __rem__(const T& self, const let& other);

	template<typename T>
	let __rem__(const T& self, const let& other) {
		return null();
	}


	template<typename T>            /****  Raise to Power of  ****/
	let __pow__(const T& self, const let& other);

	template<typename T>
	let __pow__(const T& self, const let& other) {
		return null();
	}


	template<typename T>            /****  Length Of  ****/
	int_t __len__(const T& self);

	template<typename T>
	int_t __len__(const T& self) {
		return 0;
	}


	template<typename T>            /****  Lead Element Of  ****/
	let __lead__(const T& self);

	template<typename T>
	let __lead__(const T& self) {
		return null();
	}


	template<typename T>            /**** Perpend Lead Element Of  ****/
	let __place__(const T& self, const let& other);

	template<typename T>
	let __place__(const T& self, const let& other) {
		return null();
	}


	template<typename T>            /****  Left Shift Remove  ****/
	let __shift__(const T& self);

	template<typename T>
	let __shift__(const T& self) {
		return null();
	}


	template<typename T>            /****  Reverse The Elements Of  ****/
	let __reverse__(const T& self);

	template<typename T>
	let __reverse__(const T& self) {
		return null();
	}


	template<typename T>            /****  Retrieve A Selection From  ****/
	let __get__(const T& self, const let& other);

	template<typename T>
	let __get__(const T& self, const let& other) {
		return null();
	}


	template<typename T>            /****  Set A Selection Of  ****/
	let __set__(const T& self, const let& other, const let& val);

	template<typename T>
	let __set__(const T& self, const let& other, const let& val) {
		return null();
	}


	template<typename T>            /****  Is A interable  ****/
	bool_t __interable__(const T& self);

	template<typename T>
	bool_t __interable__(const T& self) {
		return false;
	}


	template<typename T>            /****  Is None Type  ****/
	bool_t __nothing__(const T& self);

	template<typename T>
	bool_t __nothing__(const T& self) {
		return false;
	}


	template<typename T>            /****  Is Atomic Type  ****/
	bool_t __atomic__(const T& self);

	template<typename T>
	bool_t __atomic__(const T& self) {
		return false;
	}


	template<typename T>            /****  Is Not A Number  ****/
	bool_t __nan__(const T& self);

	template<typename T>
	bool_t __nan__(const T& self) {
		return false;
	}


	template<typename T>            /****  Is A Complex Number  ****/
	bool_t __complex__(const T& self);

	template<typename T>
	bool_t __complex__(const T& self) {
		return false;
	}


	template<typename T>            /****  Return A Help String ****/
	str_t __help__(const T& self);

	template<typename T>
	str_t __help__(const T& self) {
		return "No object documentation available.";
	}


	template<typename T>            /****  Return An Integer Representation ****/
	int_t __integer__(const T& self);

	template<typename T>
	int_t __integer__(const T& self) {
		return 0;
	}


	template<typename T>            /****  Invoke An Evaluation Call ****/
	lambda_t __eval__(const T& self, const environment& env, let& exp);

	template<typename T>
	lambda_t __eval__(const T& self, const environment& env, let& exp) {
		return result(self);
	}

	/********************************************************************************************/
	//
	//                                 'null' Class Implimentation
	//
	/********************************************************************************************/

	null::null() {
	}

	null::null(const null& obj) {
	}

	null::null(str_t str) {
	}

	null::~null() {
	}

	/*
	stream_t& operator >> (stream_t& stream, null& self) {

		// TODO: Does this even need to be there?
		self = null();

		return stream;
	}
	*/

	str_t __type__(const null& self) {
		return "null";
	}

	bool_t __is__(const null& self) {
		return false;
	}

	real_t __comp__(const null& self, const let& other) {
		return NOT_A_NUMBER;
	}

	void __str__(stream_t& out, const null& self) {
		out << "null";
	}

	void __repr__(stream_t& out, const null& self) {
		__str__(out, self);
	}

	bool_t __nothing__(const null& self) {
		return true;
	}

	/********************************************************************************************/
	//
	//                                 'node' Class Implimentation
	//
	/********************************************************************************************/

	node::node() : _data(), _next() {
	}

	node::node(const node& exp) : _data(exp._data), _next(exp._next) {
	}

	node::node(let object) : _data(object), _next() {
	}

	node::~node() {
	}

	std::string __type__(const node& self) {
		return "node";
	}

	bool_t __is__(const node& self) {

		if (self._data.nothing()) {
			return false;
		}

		return true;
	}

	real_t __comp__(const node& self, const let& other) {
		/*
			This can probably be removed.  Because an node
			always evaluates to the stack, there would never be
			a change to actually perform the comparison
			between two nodes.

			However this may be useful under other circumstances.
		*/

		const node* p = other.cast<node>();

		if (p) {

			let a = self;
			let b = *p;

			while (a.is() && b.is()) {

				if (a.lead() != b.lead()) {
					return NOT_A_NUMBER;
				}

				a = a.shift();
				b = b.shift();
			}

			if (!a.is() && !b.is()) {
				return 0.0;
			}
		}

		return NOT_A_NUMBER;
	}

	void __str__(stream_t& out, const node& self) {

		if (!__is__(self)) {
			out << "()";
			return;
		}

		out << "(";

		let e = self;

		bool_t next = false;

		do {
			out << str(e.lead());

			e = e.shift();

			next = e.is();

			if (next) {
				out << ", ";
			}

		} while (next);

		out << ")";
	}

	void __repr__(stream_t& out, const node& self) {

		if (!__is__(self)) {
			out << "()";
			return;
		}

		out << "(";

		let e = self;

		bool_t next = false;

		do {
			out << repr(e.lead());

			e = e.shift();

			next = e.is();

			if (next) {
				out << ", ";
			}

		} while (next);

		out << ")";
	}

	int_t __len__(const node& self) {

		if (!__is__(self)) {
			return 0;
		}

		int_t size = 1;

		let next = self._next;

		while (next.is()) {

			size += 1;

			next = next.shift();
		}

		return size;
	}

	let __lead__(const node& self) {

		return self._data;
	}

	let __place__(const node& self, const let& other) {

		if (other.nothing()) {
			return self;
		}

		node a(other);

		if (__is__(self)) {

			a._next = self;
		}

		return a;
	}

	let __shift__(const node& self) {

		if (self._next.nothing()) {

			return node();
		}

		return self._next;
	}

	let __reverse__(const node& self) {

		if (__atomic__(self)) {

			return self;
		}

		let a = node();

		let next = self;

		while (next.is()) {

			a = a.place(next.lead());

			next = next.shift();
		}

		return a;
	}

	/********************************************************************************************/
	//
	//                               'expression' Class Implimentation
	//
	/********************************************************************************************/

	expression::expression() : _list(node()), _len(0) {
	}

	expression::expression(const expression& exp) : _list(exp._list), _len(exp._len) {
	}

	expression::expression(let x) : _list(node(x)), _len(0) {
		if (_list.is()) {
			_len = 1;
		}
	}

	expression::expression(let x, let y) : _list(node()), _len(0) {

		_list = _list.place(y);
		_list = _list.place(x);

		_len = _list.len();
	}

	expression::~expression() {
	}

	stream_t& operator >> (stream_t& stream, expression& self) {

		self = expression();

		return stream;
	}

	std::string __type__(const expression& self) {
		return "expression";
	}

	bool_t __is__(const expression& self) {

		if (self._len) {
			return true;
		}

		return false;
	}

	real_t __comp__(const expression& self, const let& other) {
		/*
			This can probably be removed.  Because an expression
			always evaluates to the stack, there would never be
			a change to actually perform the comparison
			between two expressions.

			However this may be useful under other circumstances.
		*/

		const expression* e = other.cast<expression>();

		if (e) {

			if (self._len == e->_len) {
				return (self._list == e->_list);
			}
		}

		return NOT_A_NUMBER;
	}

	void __str__(stream_t& out, const expression& self) {

		if (!__is__(self)) {
			out << "()";
			return;
		}

		out << str(self._list);
	}

	void __repr__(stream_t& out, const expression& self) {

		if (!__is__(self)) {
			out << "()";
			return;
		}

		out << repr(self._list);
	}

	int_t __len__(const expression& self) {

		return self._len;
	}

	let __lead__(const expression& self) {

		return self._list.lead();
	}

	let __place__(const expression& self, const let& other) {

		if (other.nothing()) {
			return self;
		}

		expression e = self;

		e._list = e._list.place(other);
		e._len += 1;

		return e;
	}

	let __shift__(const expression& self) {

		if (self._len == 0) {
			return null();
		}

		expression e = self;

		e._list = e._list.shift();
		e._len -= 1;

		return e;
	}

	let __reverse__(const expression& self) {

		if (self._len < 2) {
			return self;
		}

		expression e = self;

		e._list = e._list.reverse();

		return e;
	}

	/********************************************************************************************/
	//
	//                                'let' Class Implementation
	//
	/********************************************************************************************/

	let::let() : _self(std::make_shared<data_t<null>>(null())) {
	}

	template <typename T>
	let::let(T x) : _self(std::make_shared<data_t<T>>(std::move(x))) {
	}

	template <typename T>
	let::let(T* x) : _self(std::make_shared<data_t<T>>(x)) {
	}

	template <typename T> T let::as() const {

		T value = T();

		if (_self->__id() == typeid(value).name()) {

			memcpy(&value, const_cast<interface_t*>(_self.get())->__as(), sizeof(T));
		}

		return value;
	}

	template <typename T> const T* let::cast() const {

		const T* p = static_cast<T*>(const_cast<interface_t*>(_self.get())->__as());

		if (p) {
			return p;
		}

		return nullptr;
	}

	str_t let::id() const {
		return _self->__id();
	}

	bool_t let::is_type(const let& other) const {
		return _self->__id() == other._self->__id();
	}

	std::size_t let::hash() const {
		return _self->__hash();
	}

	str_t let::type() const {
		return _self->__type();
	}

	bool_t let::is() const {
		return const_cast<interface_t*>(_self.get())->__is();
	}

	void let::str(stream_t& out) const {
		_self->__str(out);
	}

	void let::repr(stream_t& out) const {
		_self->__repr(out);
	}

	real_t let::comp(const let& other) const {
		return _self->__comp(other);
	}

	bool_t let::eq(const let& other) const {
		return (comp(other) == 0 ? true : false);
	}

	bool_t let::ne(const let& other) const {
		return (comp(other) != 0 ? true : false);
	}

	bool_t let::ge(const let& other) const {
		return (comp(other) >= 0 ? true : false);
	}

	bool_t let::le(const let& other) const {
		return (comp(other) <= 0 ? true : false);
	}

	bool_t let::gt(const let& other) const {
		return (comp(other) > 0 ? true : false);
	}

	bool_t let::lt(const let& other) const {
		return (comp(other) < 0 ? true : false);
	}

	bool_t let::l_and(const let& other) const {
		return _self->__l_and(other);
	}

	bool_t let::l_or(const let& other) const {
		return _self->__l_or(other);
	}

	bool_t let::l_xor(const let& other) const {
		return _self->__l_xor(other);
	}

	bool_t let::l_not() const {
		return _self->__l_not();
	}

	let let::add(const let& other) const {
		return _self->__add(other);
	}

	let let::sub(const let& other) const {
		return _self->__sub(other);
	}

	let let::mul(const let& other) const {
		return _self->__mul(other);
	}

	let let::div(const let& other) const {
		return _self->__div(other);
	}

	let let::mod(const let& other) const {
		return _self->__mod(other);
	}

	let let::f_div(const let& other) const {
		return _self->__f_div(other);
	}

	let let::rem(const let& other) const {
		return _self->__rem(other);
	}

	let let::pow(const let& other) const {
		return _self->__pow(other);
	}

	int_t let::len() const {
		return _self->__len();
	}

	let let::lead() const {
		return _self->__lead();
	}

	let let::place(const let& other) const {
		return _self->__place(other);
	}

	let let::shift() const {
		return _self->__shift();
	}

	let let::reverse() const {
		return _self->__reverse();
	}

	let let::get(const let& other) const {
		return _self->__get(other);
	}

	let let::set(const let& other, const let& val) const {
		return _self->__set(other, val);
	}

	bool_t let::interable() const {
		return _self->__interable();
	}

	bool_t let::nothing() const {
		return _self->__nothing();
	}

	bool_t let::atomic() const {
		return _self->__atomic();
	}

	bool_t let::nan() const {
		return _self->__nan();
	}

	bool_t let::complex() const {
		return _self->__complex();
	}

	str_t let::help() const {
		return _self->__help();
	}

	bool_t let::operator==(const let& other) const {
		return eq(other);
	}

	bool_t let::operator!=(const let& other) const {
		return ne(other);
	}

	bool_t let::operator>=(const let& other) const {
		return ge(other);
	}

	bool_t let::operator> (const let& other) const {
		return gt(other);
	}

	bool_t let::operator<=(const let& other) const {
		return le(other);
	}

	bool_t let::operator< (const let& other) const {
		return lt(other);
	}

	let let::operator+(const let& other) const {
		return add(other);
	}

	let let::operator-(const let& other) const {
		return sub(other);
	}

	let let::operator*(const let& other) const {
		return mul(other);
	}

	let let::operator/(const let& other) const {
		return div(other);
	}

	let let::operator%(const let& other) const {
		return mod(other);
	}

	int_t let::integer() const {
		return _self->__integer();
	}

	lambda_t let::eval(const environment& env, let& exp) const {
		return _self->__eval(env, exp);
	}

	/********************************************************************************************/
	//
	//                                'data_t' Class Implementation
	//
	/********************************************************************************************/

	template <typename T>
	let::data_t<T>::data_t(T val) : _data(std::move(val)) {
	}

	template <typename T>
	let::data_t<T>::operator bool() const {
		return __is();
	}

	template <typename T>
	void* let::data_t<T>::__as() {
		return &_data;
	}

	template <typename T>
	str_t let::data_t<T>::__id() const {
		return typeid(_data).name();
	}

	template <typename T>
	std::size_t let::data_t<T>::__hash() const {
		return __hash__(_data);
	}

	template <typename T>
	str_t let::data_t<T>::__type() const {
		return __type__(_data);
	}

	template <typename T>
	bool_t let::data_t<T>::__is() const {
		return __is__(_data);
	}

	template <typename T>
	void let::data_t<T>::__str(stream_t& out) const {
		__str__(out, _data);
	}

	template <typename T>
	void let::data_t<T>::__repr(stream_t& out) const {
		__repr__(out, _data);
	}

	template <typename T>
	real_t let::data_t<T>::__comp(const let& other) const {
		return __comp__(_data, other);
	}

	template <typename T>
	bool_t let::data_t<T>::__l_and(const let& other) const {
		return __l_and__(_data, other);
	}

	template <typename T>
	bool_t let::data_t<T>::__l_or(const let& other) const {
		return __l_or__(_data, other);
	}

	template <typename T>
	bool_t let::data_t<T>::__l_xor(const let& other) const {
		return __l_xor__(_data, other);
	}

	template <typename T>
	bool_t let::data_t<T>::__l_not() const {
		return __l_not__(_data);
	}

	template <typename T>
	let let::data_t<T>::__add(const let& other) const {
		return __add__(_data, other);
	}

	template <typename T>
	let let::data_t<T>::__sub(const let& other) const {
		return __sub__(_data, other);
	}

	template <typename T>
	let let::data_t<T>::__mul(const let& other) const {
		return __mul__(_data, other);
	}

	template <typename T>
	let let::data_t<T>::__div(const let& other) const {
		return __div__(_data, other);
	}

	template <typename T>
	let let::data_t<T>::__mod(const let& other) const {
		return __mod__(_data, other);
	}

	template <typename T>
	let let::data_t<T>::__f_div(const let& other) const {
		return __f_div__(_data, other);
	}

	template <typename T>
	let let::data_t<T>::__rem(const let& other) const {
		return __rem__(_data, other);
	}

	template <typename T>
	let let::data_t<T>::__pow(const let& other) const {
		return __pow__(_data, other);
	}

	template <typename T>
	int_t let::data_t<T>::__len() const {
		return __len__(_data);
	}

	template <typename T>
	let let::data_t<T>::__lead() const {
		return __lead__(_data);
	}

	template <typename T>
	let let::data_t<T>::__place(const let& other) const {
		return __place__(_data, other);
	}

	template <typename T>
	let let::data_t<T>::__shift() const {
		return __shift__(_data);
	}

	template <typename T>
	let let::data_t<T>::__reverse() const {
		return __reverse__(_data);
	}

	template <typename T>
	let let::data_t<T>::__get(const let& key) const {
		return __get__(_data, key);
	}

	template <typename T>
	let let::data_t<T>::__set(const let& key, const let& val) const {
		return __set__(_data, key, val);
	}

	template <typename T>
	bool_t let::data_t<T>::__interable() const {
		return __interable__(_data);
	}

	template <typename T>
	bool_t let::data_t<T>::__nothing() const {
		return __nothing__(_data);
	}

	template <typename T>
	bool_t let::data_t<T>::__atomic() const {
		return __atomic__(_data);
	}

	template <typename T>
	bool_t let::data_t<T>::__nan() const {
		return __nan__(_data);
	}

	template <typename T>
	bool_t let::data_t<T>::__complex() const {
		return __complex__(_data);
	}

	template <typename T>
	str_t let::data_t<T>::__help() const {
		return __help__(_data);
	}

	template <typename T>
	int_t let::data_t<T>::__integer() const {
		return __integer__(_data);
	}

	template <typename T>
	lambda_t let::data_t<T>::__eval(const environment& env, let& exp) const {
		return __eval__(_data, env, exp);
	}

	/********************************************************************************************/
	//
	//                            Basic Primitive Implementations
	//
	/********************************************************************************************/

	void print() {
		std::cout << std::endl;
	}

	void print(const str_t& str) {
		std::cout << str << std::endl;
	}

	void print(const let& a) {
		print(str(a));
	}

	str_t str(const let& a) {
		/*
			Convert a 'let' to its string representation.
		*/

		stream_t stream;

		stream << std::boolalpha;

		if (a.type() == "format") {
			/*
				The 'format' data type must be printed using
				its string representation, else it would only
				impart its formating to the stream instead of
				being printed to it.
			*/
			a.repr(stream);
		}
		else {
			a.str(stream);
		}

		return stream.str();
	}

	str_t repr(const let& a) {
		/*
			Convert a 'let' to its representation as a string.
		*/

		stream_t stream;

		stream << std::boolalpha;

		a.repr(stream);

		return stream.str();
	}

	const environment NULL_ENVIRONMENT = environment();
	const closure_t::function closure_t::NULL_FUNCTION = nullptr;
	const closure_t NULL_CLOSURE(closure_t::NULL_FUNCTION, NULL_ENVIRONMENT, null());

	let trampoline(lambda_t func) {
		/*
			Trampoline the lambda function calls to
			allow tail function call optimization.
			Once the function call is done processing
			return the range of the fuction's result.
		*/
		while (func.is_not_done()) {
			func = func.lambda();
		}
		return func.result();
	}

	lambda_t result(let res) {
		/*
			Package the stack in a null lambda call,
			which has finished running.
		*/
		return lambda_t(NULL_CLOSURE, res, true);
	}

	bool_t expression_is_empty(let expr) {
		/*
			Return true if the expresion is empty.
		*/
		return !expr.is();
	}

	let pop_lead(let& expr) {

		let a = expr.lead();

		expr = expr.shift();

		return a;
	}

	let first(let p) {
		return p.lead();
	}

	let second(let p) {
		return p.shift().lead();
	}

} // end namespace