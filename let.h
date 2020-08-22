#pragma once
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

#include <algorithm>
#include <cctype>
#include <iostream>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

#ifdef _MSC_VER
#include "Windows.h"
#endif

/*  Place holder for porting over to another operating system.
#ifdef _MSC_VER

#else

#endif

*/
#ifdef _MSC_VER
	#define forceinline __forceinline
#elif defined(__GNUC__)
	#define forceinline inline __attribute__((__always_inline__))
#elif defined(__CLANG__)
	#if __has_attribute(__always_inline__)
		#define forceinline inline __attribute__((__always_inline__))
	#else
		#define forceinline inline
	#endif
#else
	#define forceinline inline
#endif

namespace Olly {

#if _WIN32 || _WIN64
#if _WIN64
	using int_t = long int;
#else
	using int_t = int;
#endif
#endif

#if __GNUC__
#if __x86_64__ || __ppc64__
	using int_t = long int;
#else
	using int_t = int;
#endif
#endif

	typedef		bool						bool_t;
	typedef		long double					real_t;
	typedef		std::string					str_t;
	typedef		std::stringstream			stream_t;
	typedef     std::vector<str_t>			tokens_t;

	static const real_t			NOT_A_NUMBER = std::numeric_limits<real_t>::quiet_NaN();
	static const				std::hash<str_t> DEFAULT_HASH_FUNCTION;

	static const str_t			ESCAPE_CHAR(" \t\r\n\a\f\v\b");

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
	//          Any object it holds can be safely cast to a point_ter of the object type.  
	//          If the type cast is made to an incorrect data type then a std::nothing_ptr 
	//          is returned instead.
	//
	//          The fundamental structure of the 'let' data type was inspired and extended 
	//          from a presentation entitled:
	//                 Title: Value Semantics and Concept-based Polymorphism
	//                 By - Sean Parent
	//                 (http://sean-parent.stlab.cc/papers-and-presentations)
	//
	/********************************************************************************************/
	const enum class OP_CODE;

	class let {

	public:

		let();
		template <typename T>          let(T  x);
		template <typename T>          let(T* x);

		template <typename T> T          as()                           const;  // Define a copy of the object as the specified type.
		template <typename T> const T* cast()                           const;  // Cast the object as an instance of the specified type.

		str_t             id()                                          const;  // Return the typeid of the object.
		bool_t       is_type(const let& other)                          const;  // Compair two objects by typeid.  
		std::size_t     hash()                                          const;  // Get the hash of an object.

		str_t           type()                                          const;  // The class generated type name.
		bool_t            is()                                          const;  // Is or is not the object defined.
		void             str(stream_t& out)                             const;  // String representation of the object.
		void            repr(stream_t& out)                             const;

		real_t          comp(const let& other)                          const;  // Compare two objects. 0 = equality, > 0 = grater than, < 0 = less than.
		bool_t            eq(const let& other)                          const;  // Equal to.
		bool_t            ne(const let& other)                          const;  // Not equal to.
		bool_t            ge(const let& other)                          const;  // Greater than equal to.
		bool_t            le(const let& other)                          const;  // Less than equal to.
		bool_t            gt(const let& other)                          const;  // Greater than.
		bool_t            lt(const let& other)                          const;  // Less than.

		bool_t         l_and(const let& other)                          const;  // Logical and.
		bool_t          l_or(const let& other)                          const;  // Logical or.
		bool_t         l_xor(const let& other)                          const;  // Logical exlusive or.
		bool_t         l_not()                                          const;  // Logical not.

		let              add(const let& other)                          const;  // Addition.
		let              sub(const let& other)                          const;  // Subtraction.
		let              mul(const let& other)                          const;  // Multiplication.
		let              div(const let& other)                          const;  // Division.
		let              mod(const let& other)                          const;  // Modulus.  
		let              neg()                                          const;  // Negation.

		let            f_div(const let& other)                          const;  // Floor divide.
		let              rem(const let& other)                          const;  // Remainder.
		let              pow(const let& other)                          const;  // Raise to the power of.

		int_t            len()                                          const;  // Length of an object.
		let             lead()                                          const;  // Lead element of an object.
		let             last()                                          const;  // Last element of an object.
		let       place_lead(const let& other)                          const;  // Place an object as the lead element.
		let       shift_lead()                                          const;  // Remove the lead element from an object.
		let       place_last(const let& other)                          const;  // Place an object as the last element.
		let       shift_last()                                          const;  // Remove the last element from an object.
		let          reverse()                                          const;  // Reverse the order of an object's elements.

		let              get(const let&   key)                          const;  // Get an element from a collection.
		let              set(const let&   key, const let& val)          const;  // Set the value of an element in a collection.
		
		bool_t     interable()                                          const;
		bool_t    is_nothing()						                    const;
		bool_t        is_set()						                    const;
		bool_t        atomic()                                          const;
		bool_t           nan()                                          const;
		bool_t       complex()                                          const;

		int_t        integer()                                          const;
		OP_CODE      op_code()                                          const;

		str_t           help()                                          const;  // Define a string description of the object.

		// General object operator overloads.  

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

	private:

		struct interface_t {

			/********************************************************************************************/
			//
			//                              'interface_t' Class Definition
			//
			//       A simple interface description allowing redirection of the 'let' data type.
			//
			/********************************************************************************************/

			virtual  ~interface_t() = default;
			virtual operator bool()                                               const = 0;

			virtual void* __as() = 0;
			virtual str_t         __id()                                          const = 0;
			virtual std::size_t   __hash()                                        const = 0;

			virtual str_t         __type()                                        const = 0;
			virtual bool_t        __is()                                          const = 0;
			virtual void          __str(stream_t& out)                            const = 0;
			virtual void          __repr(stream_t& out)                           const = 0;

			virtual real_t        __comp(const let& other)                        const = 0;

			virtual let          __add(const let& other)                          const = 0;
			virtual let          __sub(const let& other)                          const = 0;
			virtual let          __mul(const let& other)                          const = 0;
			virtual let          __div(const let& other)                          const = 0;
			virtual let          __mod(const let& other)                          const = 0;
			virtual let          __neg()                                          const = 0;

			virtual let          __f_div(const let& other)                        const = 0;
			virtual let          __rem(const let& other)                          const = 0;
			virtual let          __pow(const let& other)                          const = 0;

			virtual int_t        __len()                                          const = 0;
			virtual let          __lead()                                         const = 0;
			virtual let          __last()                                         const = 0;
			virtual let          __place_lead(const let& other)                   const = 0;
			virtual let          __shift_lead()                                   const = 0;
			virtual let          __place_last(const let& other)                   const = 0;
			virtual let          __shift_last()                                   const = 0;
			virtual let          __reverse()                                      const = 0;

			virtual let          __get(const let&   key)                          const = 0;
			virtual let          __set(const let&   key, const let& val)          const = 0;

			virtual bool_t       __interable()                                    const = 0;
			virtual bool_t       __is_nothing()					                  const = 0;
			virtual bool_t       __is_set()					                      const = 0;
			virtual bool_t       __atomic()                                       const = 0;
			virtual bool_t       __nan()                                          const = 0;
			virtual bool_t       __complex()                                      const = 0;

			virtual str_t        __help()                                         const = 0;

			virtual int_t        __integer()                                      const = 0;
			virtual OP_CODE      __op_code()                                      const = 0;
		};

		template <typename T>
		struct data_t : interface_t {

			/******************************************************************************************/
			//
			//                                  'data_t' Class Definition
			//
			//             The int_terface implementation of the 'interface_t' data type.  
			//             Defining a shared const point_ter of the data type passed to it.
			//
			/******************************************************************************************/

			data_t(T val);
			operator bool()                                               const;

			void*        __as();
			str_t        __id()                                           const;
			std::size_t  __hash()                                         const;

			str_t        __type()                                         const;
			bool_t       __is()                                           const;
			void         __str(stream_t& out)                             const;
			void         __repr(stream_t& out)                            const;

			real_t       __comp(const let& other)                         const;

			let          __add(const let& other)                          const;
			let          __sub(const let& other)                          const;
			let          __mul(const let& other)                          const;
			let          __div(const let& other)                          const;
			let          __mod(const let& other)                          const;
			let          __neg()                                          const;

			let          __f_div(const let& other)                        const;
			let          __rem(const let& other)                          const;
			let          __pow(const let& other)                          const;

			int_t        __len()                                          const;
			let          __lead()                                         const;
			let          __last()                                         const;
			let          __place_lead(const let& other)                   const;
			let          __shift_lead()                                   const;
			let          __place_last(const let& other)                   const;
			let          __shift_last()                                   const;
			let          __reverse()                                      const;

			let          __get(const let&   key)                          const;
			let          __set(const let&   key, const let& val)          const;

			bool_t       __interable()                                    const;
			bool_t       __is_nothing()						              const;
			bool_t       __is_set()						                  const;
			bool_t       __atomic()	     					              const;
			bool_t       __nan()	     					              const;
			bool_t       __complex()	     					          const;

			str_t        __help()                                         const;

			int_t        __integer()                                      const;
			OP_CODE      __op_code()                                      const;

			T            _data;
		};

		std::shared_ptr<const interface_t> _self;
	};

	/********************************************************************************************/
	//
	//                                 'nothing' Class Definition
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

	class nothing {

	public:

		nothing();
		nothing(const nothing& obj);
		nothing(str_t str);
		virtual ~nothing();

		friend str_t      __type__(const nothing& self);
		friend bool_t       __is__(const nothing& self);
		friend real_t     __comp__(const nothing& self, const let& other);
		friend void        __str__(stream_t& out, const nothing& self);
		friend void       __repr__(stream_t& out, const nothing& self);

		friend bool_t  __is_nothing__(const nothing& self);
	};


	/********************************************************************************************/
	//
	//                                 '__node__' Class Definition
	//
	//          The __node__ class is implimented using Lisp inspired data nodes.  It
	//          is used to define the data lists as in Lisp.  
	//
	/********************************************************************************************/

	class __node__ {

		let _data;
		let _next;

	public:

		__node__();
		__node__(const __node__& exp);
		__node__(let obj);
		virtual ~__node__();

		friend str_t           __type__(const __node__& self);
		friend bool_t            __is__(const __node__& self);
		friend real_t          __comp__(const __node__& self, const let& other);
		friend void             __str__(stream_t& out, const __node__& self);
		friend void            __repr__(stream_t& out, const __node__& self);

		friend int_t            __len__(const __node__& self);
		friend let             __lead__(const __node__& self);
		friend let             __last__(const __node__& self);
		friend let       __place_lead__(const __node__& self, const let& other);
		friend let       __shift_lead__(const __node__& self);
		friend let          __reverse__(const __node__& self);
		friend let              __get__(const __node__& self, const let& key);
		friend let              __set__(const __node__& self, const let& key, const let& val);
	};

	/********************************************************************************************/
	//
	//                              'expression' Class Definition
	//
	//          The expression class is a Lisp linked list inspired data type.  Which
	//          manages a list of __node__ objects.  Strictly speaking this class is not
	//          necessary for this project.  The __node__ class alone would suffice to 
	//          allow all of the necessary functions and data types to be implimented on 
	//          thier own.  But this class does allow for an extra layer of abstraction, 
	//          and facilitates easier code comprehension.  
	//
	/********************************************************************************************/

	class expression {

		let    _queue;
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
		friend let             __last__(const expression& self);
		friend let       __place_lead__(const expression& self, const let& other);
		friend let       __shift_lead__(const expression& self);
		friend let          __reverse__(const expression& self);
		friend let              __get__(const expression& self, const let& key);
		friend let              __set__(const expression& self, const let& key, const let& val);
	};

	class scope {

		let    _queue;
		int_t  _len;

	public:

		scope();
		scope(const scope& exp);
		scope(let x);
		virtual ~scope();

		friend str_t           __type__(const scope& self);
		friend bool_t            __is__(const scope& self);
		friend real_t          __comp__(const scope& self, const let& other);
		friend void             __str__(stream_t& out, const scope& self);
		friend void            __repr__(stream_t& out, const scope& self);

		friend int_t            __len__(const scope& self);
		friend let             __lead__(const scope& self);
		friend let             __last__(const scope& self);
		friend let       __place_lead__(const scope& self, const let& other);
		friend let       __shift_lead__(const scope& self);
		friend let          __reverse__(const scope& self);
		friend let              __get__(const scope& self, const let& key);
		friend let              __set__(const scope& self, const let& key, const let& val);
	};

	/********************************************************************************************/
	//
	//                                Basic Primitive Declarations
	//
	//          These definitions add a few useful and some necessary support functions.
	//
	/********************************************************************************************/

	inline void print();                          // Simply print a new line.
	inline void print(const str_t& str);          // Accept any single string and print it with a std::endl.
	inline void print(const let& a);              // Accept any single 'let' and print it with a std::endl.

	str_t str(const  let& a);                     // Convert any 'let' to a str_t.
	str_t repr(const let& a);                     // Convert any 'let' to a str_t representation of the 'let'.

	inline bool_t expression_is_empty(let expr);  // Determine if an expression is empty.
	inline bool_t object_is_empty(let expr);      // Determine if an object is empty.

	inline let pop_lead(let& expr);               // Return the lead element of an object and shift it.
	inline let pop_last(let& expr);               // Return the last element of an object and shift it.

	inline let  first(let p);                     // Return the first element of an object.
	inline let second(let p);                     // Return the second element of an object.
	inline let  third(let p);                     // Return the third element of an object.

	inline str_t to_lower(str_t str);             // Set all text in a str_t to lower case.
	inline str_t to_upper(str_t str);             // Set all text in a str_t to upper case.

	inline void ltrim(str_t& s);                  // Mutable remove left white space.
	inline void rtrim(str_t& s);                  // Mutable remove right white space.
	inline void lrtrim(str_t& s);                 // Mutable remove left and right white space.

	inline str_t left_trim(str_t s);              // Copy and remove left white space.
	inline str_t right_trim(str_t s);             // Copy and remove right white space.
	inline str_t trim(str_t s);                   // Copy and remove left and right white space.

	static tokens_t split(str_t str, char delim);
	static tokens_t split(const str_t& str, str_t delim);

	template<typename T> T to(std::string str);

	
	/********************************************************************************************/
	//
	//                      'let' Class Function Default Template API.
	//
	//           The following function templates define the over-ridable functions which
	//           may be used to tailor the behavior of any object held within a 'let'.
	//
	//           Each function defined here defines the default behavior of each function
	//           which is invoked if a function is not overwritten for a defined class.
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
		out << str(nothing());
	}


	template<typename T>            /****  Comparison Between Variables  ****/
	real_t __comp__(const T& self, const let& other);

	template<typename T>
	real_t __comp__(const T& self, const let& other) {
		return NOT_A_NUMBER;
	}


	template<typename T>            /****  Addition or Concatenation  ****/
	let __add__(const T& self, const let& other);

	template<typename T>
	let __add__(const T& self, const let& other) {
		return nothing();
	}


	template<typename T>            /****  Subtraction or Deletion  ****/
	let __sub__(const T& self, const let& other);

	template<typename T>
	let __sub__(const T& self, const let& other) {
		return nothing();
	}


	template<typename T>            /****  Multiplication  ****/
	let __mul__(const T& self, const let& other);

	template<typename T>
	let __mul__(const T& self, const let& other) {
		return nothing();
	}


	template<typename T>            /****  Division  ****/
	let __div__(const T& self, const let& other);

	template<typename T>
	let __div__(const T& self, const let& other) {
		return nothing();
	}

	template<typename T>            /****  Modulation  ****/
	let __mod__(const T& self, const let& other);

	template<typename T>
	let __mod__(const T& self, const let& other) {
		return nothing();
	}


	template<typename T>            /****  Negation  ****/
	let __neg__(const T& self);

	template<typename T>
	let __neg__(const T& self) {
		return nothing();
	}


	template<typename T>            /****  Floor Division  ****/
	let __f_div__(const T& self, const let& other);

	template<typename T>
	let __f_div__(const T& self, const let& other) {
		return nothing();
	}


	template<typename T>            /****  Remainder  ****/
	let __rem__(const T& self, const let& other);

	template<typename T>
	let __rem__(const T& self, const let& other) {
		return nothing();
	}


	template<typename T>            /****  Raise to Power of  ****/
	let __pow__(const T& self, const let& other);

	template<typename T>
	let __pow__(const T& self, const let& other) {
		return nothing();
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
		return nothing();
	}


	template<typename T>            /****  Last Element Of  ****/
	let __last__(const T& self);

	template<typename T>
	let __last__(const T& self) {
		return nothing();
	}


	template<typename T>            /**** Perpend Lead Element Of  ****/
	let __place_lead__(const T& self, const let& other);

	template<typename T>
	let __place_lead__(const T& self, const let& other) {
		return nothing();
	}


	template<typename T>            /****  Left Shift Remove  ****/
	let __shift_lead__(const T& self);

	template<typename T>
	let __shift_lead__(const T& self) {
		return nothing();
	}


	template<typename T>            /**** Postpend Last Element Of  ****/
	let __place_last__(const T& self, const let& other);

	template<typename T>
	let __place_last__(const T& self, const let& other) {
		return nothing();
	}


	template<typename T>            /****  Right Shift Remove  ****/
	let __shift_last__(const T& self);

	template<typename T>
	let __shift_last__(const T& self) {
		return nothing();
	}


	template<typename T>            /****  Reverse The Elements Of  ****/
	let __reverse__(const T& self);

	template<typename T>
	let __reverse__(const T& self) {
		return nothing();
	}


	template<typename T>            /****  Retrieve A Selection From  ****/
	let __get__(const T& self, const let& other);

	template<typename T>
	let __get__(const T& self, const let& other) {
		return nothing();
	}


	template<typename T>            /****  Set A Selection Of  ****/
	let __set__(const T& self, const let& other, const let& val);

	template<typename T>
	let __set__(const T& self, const let& other, const let& val) {
		return nothing();
	}



	template<typename T>            /****  Is A interable  ****/
	bool_t __interable__(const T& self);

	template<typename T>
	bool_t __interable__(const T& self) {
		return false;
	}


	template<typename T>            /****  Is Nothing Type  ****/
	bool_t __is_nothing__(const T& self);

	template<typename T>
	bool_t __is_nothing__(const T& self) {
		return false;
	}


	template<typename T>            /****  Is Set Type  ****/
	bool_t __is_set__(const T& self);

	template<typename T>
	bool_t __is_set__(const T& self) {
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


	template<typename T>            /****  Return An Operation Code ****/
	OP_CODE __op_code__(const T& self);

	template<typename T>
	OP_CODE __op_code__(const T& self) {
		return OP_CODE();
	}


	/********************************************************************************************/
	//
	//                                 'nothing' Class Implimentation
	//
	/********************************************************************************************/

	nothing::nothing() {
	}

	nothing::nothing(const nothing& obj) {
	}

	nothing::nothing(str_t str) {
	}

	nothing::~nothing() {
	}

	str_t __type__(const nothing& self) {
		return "nothing";
	}

	bool_t __is__(const nothing& self) {
		return false;
	}

	real_t __comp__(const nothing& self, const let& other) {
		return NOT_A_NUMBER;
	}

	void __str__(stream_t& out, const nothing& self) {
		out << "nothing";
	}

	void __repr__(stream_t& out, const nothing& self) {
		__str__(out, self);
	}

	bool_t __is_nothing__(const nothing& self) {
		return true;
	}

	/********************************************************************************************/
	//
	//                                 '__node__' Class Implimentation
	//
	/********************************************************************************************/

	__node__::__node__() : _data(), _next() {
	}

	__node__::__node__(const __node__& exp) : _data(exp._data), _next(exp._next) {
	}

	__node__::__node__(let object) : _data(object), _next() {
	}

	__node__::~__node__() {
	}

	std::string __type__(const __node__& self) {
		return "__node__";
	}

	bool_t __is__(const __node__& self) {

		if (self._data.is_nothing()) {
			return false;
		}

		return true;
	}

	real_t __comp__(const __node__& self, const let& other) {

		const __node__* p = other.cast<__node__>();

		if (p) {

			let a = self;
			let b = *p;

			while (a.is() && b.is()) {

				if (a.lead() != b.lead()) {
					return NOT_A_NUMBER;
				}

				a = a.shift_lead();
				b = b.shift_lead();
			}

			if (!a.is() && !b.is()) {
				return 0.0;
			}
		}

		return NOT_A_NUMBER;
	}

	void __str__(stream_t& out, const __node__& self) {

		if (!__is__(self)) {
			return;
		}

		let e = self;

		bool_t next = false;

		do {
			out << str(e.lead());

			e = e.shift_lead();

			next = e.is();

			if (next) {
				out << " ";
			}

		} while (next);
	}

	void __repr__(stream_t& out, const __node__& self) {

		if (!__is__(self)) {
			return;
		}

		let e = self;

		bool_t next = false;

		do {
			out << repr(e.lead());

			e = e.shift_lead();

			next = e.is();

			if (next) {
				out << " ";
			}

		} while (next);
	}

	int_t __len__(const __node__& self) {

		if (!__is__(self)) {
			return 0;
		}

		int_t size = 1;

		let next = self._next;

		while (next.is()) {

			size += 1;

			next = next.shift_lead();
		}

		return size;
	}

	let __lead__(const __node__& self) {

		return self._data;
	}

	let __last__(const __node__& self) {

		if (self._next.is_nothing()) {

			return self;
		}

		let next = self;

		while (next.shift_lead().is()) {

			next = next.shift_lead();
		}

		return next;
	}

	let __place_lead__(const __node__& self, const let& other) {

		if (other.is_nothing()) {
			return self;
		}

		__node__ a(other);

		if (__is__(self)) {

			a._next = self;
		}

		return a;
	}

	let __shift_lead__(const __node__& self) {

		if (self._next.is_nothing()) {

			return __node__();
		}

		return self._next;
	}

	let __reverse__(const __node__& self) {

		if (__atomic__(self)) {

			return self;
		}

		let a = __node__();

		let next = self;

		while (next.is()) {

			a = a.place_lead(next.lead());

			next = next.shift_lead();
		}

		return a;
	}

	let __get__(const __node__& self, const let& key) {
				
		let next = self;

		while (next.is()) {

			if (key == first(next.lead())) {
				return second(next.lead());
			}

			next = next.shift_lead();
		}

		return nothing();
	}

	let __set__(const __node__& self, const let& key, const let& val) {

		let next = self;

		let new_pair = __node__();

		bool_t is_set = false;

		while (next.is()) {

			if (key < first(next.lead()) && not is_set) {

				let old_pair = next.lead();
				// let new_pair = __expression__<'(', ')'>();

				new_pair = new_pair.place_lead(old_pair.lead());
				new_pair = new_pair.place_last(val);

				is_set = true;
			}

			new_pair = new_pair.place_lead(next.lead());

			next = next.shift_lead();
		}

		return next.reverse();
	}

	/********************************************************************************************/
	//
	//                               'expression' Class Implimentation
	//
	/********************************************************************************************/

	expression::expression() : _queue(__node__()), _len(0) {
	}

	expression::expression(const expression& exp) : _queue(exp._queue), _len(exp._len) {
	}

	expression::expression(let x) : _queue(__node__(x)), _len(0) {
		if (_queue.is()) {
			_len = 1;
		}
	}

	expression::expression(let x, let y) : _queue(__node__()), _len(0) {

		_queue = _queue.place_lead(y);

		if (_queue.len() > _len) {
			_len += 1;
		}

		_queue = _queue.place_lead(x);

		if (_queue.len() > _len) {
			_len += 1;
		}
	}

	expression::~expression() {
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

		const expression* e = other.cast<expression >();

		if (e) {

			if (self._len == e->_len) {
				return (self._queue == e->_queue);
			}
		}

		return NOT_A_NUMBER;
	}

	void __str__(stream_t& out, const expression& self) {

		if (!__is__(self)) {
			out << "()";
			return;
		}

		out << "(";

		self._queue.str(out);

		out << ")";
	}

	void __repr__(stream_t& out, const expression& self) {

		if (!__is__(self)) {
			out << "()";
			return;
		}

		out << "(";

		self._queue.repr(out);

		out << ")";
	}

	int_t __len__(const expression& self) {

		return self._len;
	}

	let __lead__(const expression& self) {

		return self._queue.lead();
	}

	let __last__(const expression& self) {

		return self._queue.last();
	}

	let __place_lead__(const expression& self, const let& other) {

		if (other.is_nothing()) {
			return self;
		}

		expression e = self;

		e._queue = e._queue.place_lead(other);
		e._len += 1;

		return e;
	}

	let __shift_lead__(const expression& self) {

		if (self._len == 0) {
			return nothing();
		}

		expression e = self;

		e._queue = e._queue.shift_lead();
		e._len -= 1;

		return e;
	}

	let __reverse__(const expression& self) {

		if (self._len < 2) {
			return self;
		}

		expression e = self;

		e._queue = e._queue.reverse();

		return e;
	}

	let __get__(const expression& self, const let& key) {

		return self._queue.get(key);
	}

	let __set__(const expression& self, const let& key, const let& val) {

		let new_queue = expression(self._queue.set(key, val));

		return new_queue;
	}

	/********************************************************************************************/
	//
	//                               'scope' Class Implimentation
	//
	/********************************************************************************************/

	scope::scope() : _queue(__node__()), _len(0) {
	}

	scope::scope(const scope& exp) : _queue(exp._queue), _len(exp._len) {
	}

	scope::scope(let x) : _queue(__node__(x)), _len(0) {
		if (_queue.is()) {
			_len = 1;
		}
	}

	scope::~scope() {
	}

	std::string __type__(const scope& self) {
		return "expression";
	}

	bool_t __is__(const scope& self) {

		if (self._len) {
			return true;
		}

		return false;
	}

	real_t __comp__(const scope& self, const let& other) {

		const scope* e = other.cast<scope >();

		if (e) {

			if (self._len == e->_len) {
				return (self._queue == e->_queue);
			}
		}

		return NOT_A_NUMBER;
	}

	void __str__(stream_t& out, const scope& self) {

		if (self._len <= 2) {
			out << ":;";
			return;
		}

		self._queue.str(out);
	}

	void __repr__(stream_t& out, const scope& self) {

		if (self._len <= 2) {
			out << ":;";
			return;
		}

		self._queue.repr(out);
	}

	int_t __len__(const scope& self) {

		return self._len;
	}

	let __lead__(const scope& self) {

		return self._queue.lead();
	}

	let __last__(const scope& self) {

		return self._queue.last();
	}

	let __place_lead__(const scope& self, const let& other) {

		if (other.is_nothing()) {
			return self;
		}

		scope e = self;

		e._queue = e._queue.place_lead(other);
		e._len += 1;

		return e;
	}

	let __shift_lead__(const scope& self) {

		if (self._len == 0) {
			return nothing();
		}

		scope e = self;

		e._queue = e._queue.shift_lead();
		e._len -= 1;

		return e;
	}

	let __reverse__(const scope& self) {

		if (self._len < 2) {
			return self;
		}

		scope e = self;

		e._queue = e._queue.reverse();

		return e;
	}

	let __get__(const scope& self, const let& key) {

		return self._queue.get(key);
	}

	let __set__(const scope& self, const let& key, const let& val) {

		let new_queue = scope(self._queue.set(key, val));

		return new_queue;
	}

	/********************************************************************************************/
	//
	//                                'let' Class Implementation
	//
	/********************************************************************************************/

	let::let() : _self(std::make_shared<data_t<Olly::nothing>>(Olly::nothing())) {
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
		return _self->__is() && other.is();
	}

	bool_t let::l_or(const let& other) const {
		return _self->__is() || other.is();
	}

	bool_t let::l_xor(const let& other) const {
		return _self->__is() != other.is();
	}

	bool_t let::l_not() const {
		return !_self->__is();
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

	let let::neg() const {
		return _self->__neg();
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

	let let::last() const {
		return _self->__last();
	}

	let let::place_lead(const let& other) const {
		return _self->__place_lead(other);
	}

	let let::shift_lead() const {
		return _self->__shift_lead();
	}

	let let::place_last(const let& other) const {
		return _self->__place_last(other);
	}

	let let::shift_last() const {
		return _self->__shift_last();
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

	bool_t let::is_nothing() const {
		return _self->__is_nothing();
	}

	bool_t let::is_set() const {
		return _self->__is_set();
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

	int_t let::integer() const {
		return _self->__integer();
	}

	OP_CODE let::op_code() const {
		return _self->__op_code();
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
	let let::data_t<T>::__neg() const {
		return __neg__(_data);
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
	let let::data_t<T>::__last() const {
		return __last__(_data);
	}

	template <typename T>
	let let::data_t<T>::__place_lead(const let& other) const {
		return __place_lead__(_data, other);
	}

	template <typename T>
	let let::data_t<T>::__shift_lead() const {
		return __shift_lead__(_data);
	}

	template <typename T>
	let let::data_t<T>::__place_last(const let& other) const {
		return __place_last__(_data, other);
	}

	template <typename T>
	let let::data_t<T>::__shift_last() const {
		return __shift_last__(_data);
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
	bool_t let::data_t<T>::__is_nothing() const {
		return __is_nothing__(_data);
	}

	template <typename T>
	bool_t let::data_t<T>::__is_set() const {
		return __is_set__(_data);
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
	OP_CODE let::data_t<T>::__op_code() const {
		return __op_code__(_data);
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

	inline bool_t expression_is_empty(let expr) {
		/*
			Return true if the expresion is empty.
		*/
		return !expr.is();
	}

	inline bool_t object_is_empty(let expr) {
		/*
			Return true if the expresion is empty.
		*/
		return !expr.is();
	}

	inline let pop_lead(let& expr) {

		let a = expr.lead();

		expr = expr.shift_lead();

		return a;
	}

	inline let pop_last(let& expr) {

		let a = expr.last();

		expr = expr.shift_last();

		return a;
	}

	inline let first(let p) {
		return p.lead();
	}

	inline let second(let p) {
		return p.shift_lead().lead();
	}

	inline let third(let p) {
		return p.shift_lead().shift_lead().lead();
	}

	inline str_t to_lower(str_t str) {

		std::transform(str.begin(), str.end(), str.begin(), ::tolower);

		return str;
	}

	inline str_t to_upper(str_t str) {

		std::transform(str.begin(), str.end(), str.begin(), ::toupper);

		return str;
	}

	inline void ltrim(str_t& s) {
		s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](int ch) {
			return !std::isspace(ch);
			}));
	}

	inline void rtrim(str_t& s) {
		s.erase(std::find_if(s.rbegin(), s.rend(), [](int ch) {
			return !std::isspace(ch);
			}).base(), s.end());
	}

	inline void lrtrim(str_t& s) {
		ltrim(s);
		rtrim(s);
	}

	inline str_t left_trim(str_t s) {
		ltrim(s);
		return s;
	}

	inline str_t right_trim(str_t s) {
		rtrim(s);
		return s;
	}

	inline str_t trim(str_t s) {
		lrtrim(s);
		return s;
	}

	tokens_t split(str_t str, char delim) {

		std::stringstream stream;
		stream.str(str);

		tokens_t tokens;

		while (std::getline(stream, str, delim)) {

			tokens.push_back(str);
		}

		return tokens;
	}

	tokens_t split(const str_t& str, str_t delim) {

		if (delim == "") {
			return split(str, ESCAPE_CHAR);
		}

		tokens_t tokens, buffer, temp;

		char d;

		d = delim.back();
		delim.pop_back();

		tokens = split(str, d);

		while (delim.size()) {

			d = delim.back();
			delim.pop_back();

			buffer = tokens;
			tokens.clear();

			for (int_t i = 0, stop = buffer.size(); i < stop; i += 1) {

				temp = split(buffer.at(i), d);

				for (int_t j = 0, stop = temp.size(); j < stop; j += 1) {

					tokens.push_back(temp.at(j));
				}

				temp.clear();
			}
		}

		return tokens;
	}

	template<typename T> T to(std::string str) {

		T n;

		std::stringstream stream;

		try {
			stream << trim(str);
			stream >> n;
		}
		catch (...) {
			n = T();
		}

		return n;
	}

} // end Olly