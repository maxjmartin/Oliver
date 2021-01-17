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
	using int_type = long int;
#else
	using int_type = int;
#endif
#endif

#if __GNUC__
#if __x86_64__ || __ppc64__
	using int_type = long int;
#else
	using int_type = int;
#endif
#endif

	typedef		bool						bool_type;
	typedef		long double					real_type;
	typedef		std::size_t					size_type;
	typedef		std::string					str_type;
	typedef		std::stringstream			stream_type;
	typedef     std::vector<str_type>		tokens_type;

	static const real_type			 NOT_A_NUMBER = std::numeric_limits<real_type>::quiet_NaN();
	static const std::hash<str_type> DEFAULT_HASH_FUNCTION;

	static const str_type			 ESCAPE_CHAR(" \t\r\n\a\f\v\b");

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
	//          Any object it holds can be safely cast to a pointer of the object type.  
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
		struct interface_type;

	public:

		let();
		template <typename T>          let(T  x);
		template <typename T>          let(T* x);

		template <typename T> const T* cast()                              const;  // Cast the object as an instance of the specified type.

		str_type             id()                                          const;  // Return the typeid of the object.
		bool_type       is_type(const let& other)                          const;  // Compair two objects by typeid.  
		size_type          hash()                                          const;  // Get the hash of an object.

		str_type           type()                                          const;  // The class generated type name.
		bool_type            is()                                          const;  // Is or is not the object defined.
		void                str(stream_type& out)                          const;  // String representation of the object.
		void               repr(stream_type& out)                          const;

		real_type          comp(const let& other)                          const;  // Compare two objects. 0 = equality, > 0 = grater than, < 0 = less than.
		bool_type            eq(const let& other)                          const;  // Equal to.
		bool_type            ne(const let& other)                          const;  // Not equal to.
		bool_type            ge(const let& other)                          const;  // Greater than equal to.
		bool_type            le(const let& other)                          const;  // Less than equal to.
		bool_type            gt(const let& other)                          const;  // Greater than.
		bool_type            lt(const let& other)                          const;  // Less than.

		bool_type         l_and(const let& other)                          const;  // Logical and.
		bool_type          l_or(const let& other)                          const;  // Logical or.
		bool_type         l_xor(const let& other)                          const;  // Logical exlusive or.
		bool_type         l_not()                                          const;  // Logical not.

		let                 add(const let& other)                          const;  // Addition.
		let                 sub(const let& other)                          const;  // Subtraction.
		let                 mul(const let& other)                          const;  // Multiplication.
		let                 div(const let& other)                          const;  // Division.
		let                 mod(const let& other)                          const;  // Modulus.  
		let                 neg()                                          const;  // Negation.

		let               f_div(const let& other)                          const;  // Floor divide.
		let                 rem(const let& other)                          const;  // Remainder.
		let                 pow(const let& other)                          const;  // Raise to the power of.

		size_type          size()                                          const;  // Size of an object.
		let                lead()                                          const;  // Lead element of an object.
		let                last()                                          const;  // Last element of an object.
		let          place_lead(const let& other)                          const;  // Place an object as the lead element.
		let          shift_lead()                                          const;  // Remove the lead element from an object.
		let          place_last(const let& other)                          const;  // Place an object as the last element.
		let         shift_last()                                           const;  // Remove the last element from an object.
		let            reverse()                                           const;  // Reverse the order of an object's elements.
		
		let                 get(const let&   key)                          const;  // Get an element from a collection.
		let                 set(const let&   key, const let& val)          const;  // Set the value of an element in a collection.

		bool_type      iterable()                                          const;
		bool_type    is_nothing()						                   const;
		bool_type        is_set()						                   const;
		bool_type        atomic()                                          const;
		bool_type           nan()                                          const;
		bool_type       complex()                                          const;

		OP_CODE         op_code()                                          const;

		str_type           help()                                          const;  // Define a string description of the object.

		// General object operator overloads.  

		bool_type    operator==(const let& other)                          const;
		bool_type    operator!=(const let& other)                          const;
		bool_type    operator>=(const let& other)                          const;
		bool_type    operator> (const let& other)                          const;
		bool_type    operator<=(const let& other)                          const;
		bool_type    operator< (const let& other)                          const;

		let           operator+(const let& other)                          const;
		let           operator-(const let& other)                          const;
		let           operator*(const let& other)                          const;
		let           operator/(const let& other)                          const;
		let           operator%(const let& other)                          const;

	private:

		struct interface_type {

			/********************************************************************************************/
			//
			//                              'interface_type' Class Definition
			//
			//       A simple interface description allowing redirection of the 'let' data type.
			//
			/********************************************************************************************/

			virtual  ~interface_type() = default;
			virtual operator bool()                                                 const = 0;

			virtual void*            _vptr()                                              = 0;
			virtual str_type         _id()                                          const = 0;
			virtual std::size_t      _hash()                                        const = 0;

			virtual str_type         _type()                                        const = 0;
			virtual bool_type        _is()                                          const = 0;
			virtual void             _str(stream_type& out)                         const = 0;
			virtual void             _repr(stream_type& out)                        const = 0;

			virtual real_type        _comp(const let& other)                        const = 0;

			virtual let             _add(const let& other)                          const = 0;
			virtual let             _sub(const let& other)                          const = 0;
			virtual let             _mul(const let& other)                          const = 0;
			virtual let             _div(const let& other)                          const = 0;
			virtual let             _mod(const let& other)                          const = 0;
			virtual let             _neg()                                          const = 0;

			virtual let             _f_div(const let& other)                        const = 0;
			virtual let             _rem(const let& other)                          const = 0;
			virtual let             _pow(const let& other)                          const = 0;

			virtual size_type       _size()                                         const = 0;
			virtual let             _lead()                                         const = 0;
			virtual let             _last()                                         const = 0;
			virtual let             _place_lead(const let& other)                   const = 0;
			virtual let             _shift_lead()                                   const = 0;
			virtual let             _place_last(const let& other)                   const = 0;
			virtual let             _shift_last()                                   const = 0;
			virtual let             _reverse()                                      const = 0;

			virtual let             _get(const let&   key)                          const = 0;
			virtual let             _set(const let&   key, const let& val)          const = 0;

			virtual bool_type       _iterable()                                     const = 0;
			virtual bool_type       _is_nothing()					                const = 0;
			virtual bool_type       _is_set()					                    const = 0;
			virtual bool_type       _atomic()                                       const = 0;
			virtual bool_type       _nan()                                          const = 0;
			virtual bool_type       _complex()                                      const = 0;

			virtual str_type        _help()                                         const = 0;

			virtual OP_CODE         _op_code()                                      const = 0;
		};

		template <typename T>
		struct data_type : interface_type {

			/******************************************************************************************/
			//
			//                                 'data_type' Class Definition
			//
			//             The int_typeerface implementation of the 'interface_type' data type.  
			//             Defining a shared const point_typeer of the data type passed to it.
			//
			/******************************************************************************************/

			data_type(T val);
			operator bool()                                                 const;

			void*           _vptr();
			str_type        _id()                                           const;
			std::size_t     _hash()                                         const;

			str_type        _type()                                         const;
			bool_type       _is()                                           const;
			void            _str(stream_type& out)                          const;
			void            _repr(stream_type& out)                         const;

			real_type       _comp(const let& other)                         const;

			let             _add(const let& other)                          const;
			let             _sub(const let& other)                          const;
			let             _mul(const let& other)                          const;
			let             _div(const let& other)                          const;
			let             _mod(const let& other)                          const;
			let             _neg()                                          const;

			let             _f_div(const let& other)                        const;
			let             _rem(const let& other)                          const;
			let             _pow(const let& other)                          const;

			size_type       _size()                                         const;
			let             _lead()                                         const;
			let             _last()                                         const;
			let             _place_lead(const let& other)                   const;
			let             _shift_lead()                                   const;
			let             _place_last(const let& other)                   const;
			let             _shift_last()                                   const;
			let             _reverse()                                      const;

			let             _get(const let&   key)                          const;
			let             _set(const let&   key, const let& val)          const;

			bool_type       _iterable()                                     const;
			bool_type       _is_nothing()						            const;
			bool_type       _is_set()						                const;
			bool_type       _atomic()	     					            const;
			bool_type       _nan()	     					                const;
			bool_type       _complex()	     					            const;

			str_type        _help()                                         const;

			OP_CODE         _op_code()                                      const;

			T               _data;
		};

		std::shared_ptr<const interface_type> _self;
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
		virtual ~nothing();

		friend str_type      _type_(const nothing& self);
		friend bool_type       _is_(const nothing& self);
		friend real_type     _comp_(const nothing& self, const let& other);

		friend void           _str_(stream_type& out, const nothing& self);
		friend void          _repr_(stream_type& out, const nothing& self);

		friend bool_type  _is_nothing_(const nothing& self);
	};

	/********************************************************************************************/
	//
	//                                Basic Primitive Declarations
	//
	//          These definitions add a few useful and some necessary support functions.
	//
	/********************************************************************************************/

	inline void print();                             // Simply print a new line.
	inline void print(const str_type& str);          // Accept any single string and print it with a std::endl.
	inline void print(const let& a);                 // Accept any single 'let' and print it with a std::endl.

	str_type  str(const  let& a);                    // Convert any 'let' to a str_type.
	str_type repr(const let& a);                     // Convert any 'let' to a str_type representation of the 'let'.

	inline bool_type expression_is_empty(let expr);  // Determine if an expression is empty.
	inline bool_type object_is_empty(let expr);      // Determine if an object is empty.

	inline let pop_lead(let& expr);                  // Return the lead element of an object and shift it.
	inline let pop_last(let& expr);                  // Return the last element of an object and shift it.

	inline let  first(let p);                        // Return the first element of an object.
	inline let second(let p);                        // Return the second element of an object.
	inline let  third(let p);                        // Return the third element of an object.

	inline str_type to_lower(str_type str);          // Set all text in a str_type to lower case.
	inline str_type to_upper(str_type str);          // Set all text in a str_type to upper case.

	inline void ltrim(str_type& s);                  // Mutable remove left white space.
	inline void rtrim(str_type& s);                  // Mutable remove right white space.
	inline void lrtrim(str_type& s);                 // Mutable remove left and right white space.

	inline str_type left_trim(str_type s);           // Copy and remove left white space.
	inline str_type right_trim(str_type s);          // Copy and remove right white space.
	inline str_type trim(str_type s);                // Copy and remove left and right white space.

	static tokens_type split(str_type str, char delim);
	static tokens_type split(const str_type& str, str_type delim);

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
	std::size_t _hash_(const T& self);

	template<typename T>
	std::size_t _hash_(const T& self) {
		return DEFAULT_HASH_FUNCTION(repr(self));
	}


	template<typename T>            /****  Type Name  ****/
	str_type _type_(const T& self);

	template<typename T>
	str_type _type_(const T& self) {
		return typeid(self).name();
	}


	template<typename T>            /****  Boolean Conversion  ****/
	bool_type _is_(const T& self);

	template<typename T>
	bool_type _is_(const T& self) {
		return false;
	}


	template<typename T>            /****  String Conversion  ****/
	void _str_(stream_type& out, const T& self);

	template<typename T>
	void _str_(stream_type& out, const T& self) {
		out << "object<" << &self << "," << _type_(self) << ">";
	}


	template<typename T>            /****  String Representation  ****/
	void _repr_(stream_type& out, const T& self);

	template<typename T>
	void _repr_(stream_type& out, const T& self) {
		out << str(nothing());
	}


	template<typename T>            /****  Comparison Between Variables  ****/
	real_type _comp_(const T& self, const let& other);

	template<typename T>
	real_type _comp_(const T& self, const let& other) {
		return NOT_A_NUMBER;
	}


	template<typename T>            /****  Addition or Concatenation  ****/
	let _add_(const T& self, const let& other);

	template<typename T>
	let _add_(const T& self, const let& other) {
		return nothing();
	}


	template<typename T>            /****  Subtraction or Deletion  ****/
	let _sub_(const T& self, const let& other);

	template<typename T>
	let _sub_(const T& self, const let& other) {
		return nothing();
	}


	template<typename T>            /****  Multiplication  ****/
	let _mul_(const T& self, const let& other);

	template<typename T>
	let _mul_(const T& self, const let& other) {
		return nothing();
	}


	template<typename T>            /****  Division  ****/
	let _div_(const T& self, const let& other);

	template<typename T>
	let _div_(const T& self, const let& other) {
		return nothing();
	}

	template<typename T>            /****  Modulation  ****/
	let _mod_(const T& self, const let& other);

	template<typename T>
	let _mod_(const T& self, const let& other) {
		return nothing();
	}


	template<typename T>            /****  Negation  ****/
	let _neg_(const T& self);

	template<typename T>
	let _neg_(const T& self) {
		return nothing();
	}


	template<typename T>            /****  Floor Division  ****/
	let _f_div_(const T& self, const let& other);

	template<typename T>
	let _f_div_(const T& self, const let& other) {
		return nothing();
	}


	template<typename T>            /****  Remainder  ****/
	let _rem_(const T& self, const let& other);

	template<typename T>
	let _rem_(const T& self, const let& other) {
		return nothing();
	}


	template<typename T>            /****  Raise to Power of  ****/
	let _pow_(const T& self, const let& other);

	template<typename T>
	let _pow_(const T& self, const let& other) {
		return nothing();
	}


	template<typename T>            /****  Length Of  ****/
	size_type _size_(const T& self);

	template<typename T>
	size_type _size_(const T& self) {
		return 0;
	}


	template<typename T>            /****  Lead Element Of  ****/
	let _lead_(const T& self);

	template<typename T>
	let _lead_(const T& self) {
		return nothing();
	}


	template<typename T>            /****  Last Element Of  ****/
	let _last_(const T& self);

	template<typename T>
	let _last_(const T& self) {
		return nothing();
	}


	template<typename T>            /**** Perpend Lead Element Of  ****/
	let _place_lead_(const T& self, const let& other);

	template<typename T>
	let _place_lead_(const T& self, const let& other) {
		return nothing();
	}


	template<typename T>            /****  Left Shift Remove  ****/
	let _shift_lead_(const T& self);

	template<typename T>
	let _shift_lead_(const T& self) {
		return nothing();
	}


	template<typename T>            /**** Postpend Last Element Of  ****/
	let _place_last_(const T& self, const let& other);

	template<typename T>
	let _place_last_(const T& self, const let& other) {
		return nothing();
	}


	template<typename T>            /****  Right Shift Remove  ****/
	let _shift_last_(const T& self);

	template<typename T>
	let _shift_last_(const T& self) {
		return nothing();
	}


	template<typename T>            /****  Reverse The Elements Of  ****/
	let _reverse_(const T& self);

	template<typename T>
	let _reverse_(const T& self) {
		return nothing();
	}


	template<typename T>            /****  Retrieve A Selection From  ****/
	let _get_(const T& self, const let& other);

	template<typename T>
	let _get_(const T& self, const let& other) {
		return nothing();
	}


	template<typename T>            /****  Set A Selection Of  ****/
	let _set_(const T& self, const let& other, const let& val);

	template<typename T>
	let _set_(const T& self, const let& other, const let& val) {
		return nothing();
	}


	template<typename T>            /****  Is Something iterabley Ordered  ****/
	bool_type _iterable_(const T& self);

	template<typename T>
	bool_type _iterable_(const T& self) {
		return false;
	}


	template<typename T>            /****  Is Nothing Type  ****/
	bool_type _is_nothing_(const T& self);

	template<typename T>
	bool_type _is_nothing_(const T& self) {
		return false;
	}


	template<typename T>            /****  Is Set Type  ****/
	bool_type _is_set_(const T& self);

	template<typename T>
	bool_type _is_set_(const T& self) {
		return false;
	}


	template<typename T>            /****  Is Atomic Type  ****/
	bool_type _atomic_(const T& self);

	template<typename T>
	bool_type _atomic_(const T& self) {
		return false;
	}


	template<typename T>            /****  Is Not A Number  ****/
	bool_type _nan_(const T& self);

	template<typename T>
	bool_type _nan_(const T& self) {
		return false;
	}


	template<typename T>            /****  Is A Complex Number  ****/
	bool_type _complex_(const T& self);

	template<typename T>
	bool_type _complex_(const T& self) {
		return false;
	}


	template<typename T>            /****  Return A Help String ****/
	str_type _help_(const T& self);

	template<typename T>
	str_type _help_(const T& self) {
		return "No object documentation available.";
	}


	template<typename T>            /****  Return An Operation Code ****/
	OP_CODE _op_code_(const T& self);

	template<typename T>
	OP_CODE _op_code_(const T& self) {
		return OP_CODE::NOTHING_OP;
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

	nothing::~nothing() {
	}

	str_type _type_(const nothing& self) {
		return "nothing";
	}

	bool_type _is_(const nothing& self) {
		return false;
	}

	real_type _comp_(const nothing& self, const let& other) {
		return other.is_nothing() ? 0.0 : NOT_A_NUMBER;
	}

	void _str_(stream_type& out, const nothing& self) {
		out << "nothing";
	}

	void _repr_(stream_type& out, const nothing& self) {
		_str_(out, self);
	}

	bool_type _is_nothing_(const nothing& self) {
		return true;
	}

	

	/********************************************************************************************/
	//
	//                                'let' Class Implementation
	//
	/********************************************************************************************/

	let::let() : _self(std::make_shared<data_type<Olly::nothing>>(Olly::nothing())) {
	}

	template <typename T>
	let::let(T x) : _self(std::make_shared<data_type<T>>(std::move(x))) {
	}

	template <typename T>
	let::let(T* x) : _self(std::make_shared<data_type<T>>(x)) {
	}

	template <typename T> const T* let::cast() const {

		let n = T();

		if (is_type(n)) {

			const T* p = static_cast<T*>(const_cast<interface_type*>(_self.get())->_vptr());

			if (p) {
				return p;
			}
		}

		return nullptr;
	}

	str_type let::id() const {
		return _self->_id();
	}

	bool_type let::is_type(const let& other) const {
		return _self->_id() == other._self->_id();
	}

	std::size_t let::hash() const {
		return _self->_hash();
	}

	str_type let::type() const {
		return _self->_type();
	}

	bool_type let::is() const {
		return const_cast<interface_type*>(_self.get())->_is();
	}

	void let::str(stream_type& out) const {
		_self->_str(out);
	}

	void let::repr(stream_type& out) const {
		_self->_repr(out);
	}

	real_type let::comp(const let& other) const {
		return _self->_comp(other);
	}

	bool_type let::eq(const let& other) const {
		return (comp(other) == 0 ? true : false);
	}

	bool_type let::ne(const let& other) const {
		return (comp(other) != 0 ? true : false);
	}

	bool_type let::ge(const let& other) const {
		return (comp(other) >= 0 ? true : false);
	}

	bool_type let::le(const let& other) const {
		return (comp(other) <= 0 ? true : false);
	}

	bool_type let::gt(const let& other) const {
		return (comp(other) > 0 ? true : false);
	}

	bool_type let::lt(const let& other) const {
		return (comp(other) < 0 ? true : false);
	}


	bool_type let::l_and(const let& other) const {
		return _self->_is() && other.is();
	}

	bool_type let::l_or(const let& other) const {
		return _self->_is() || other.is();
	}

	bool_type let::l_xor(const let& other) const {
		return _self->_is() != other.is();
	}

	bool_type let::l_not() const {
		return !_self->_is();
	}

	let let::add(const let& other) const {
		return _self->_add(other);
	}

	let let::sub(const let& other) const {
		return _self->_sub(other);
	}

	let let::mul(const let& other) const {
		return _self->_mul(other);
	}

	let let::div(const let& other) const {
		return _self->_div(other);
	}

	let let::mod(const let& other) const {
		return _self->_mod(other);
	}

	let let::neg() const {
		return _self->_neg();
	}

	let let::f_div(const let& other) const {
		return _self->_f_div(other);
	}

	let let::rem(const let& other) const {
		return _self->_rem(other);
	}

	let let::pow(const let& other) const {
		return _self->_pow(other);
	}

	size_type let::size() const {
		return _self->_size();
	}

	let let::lead() const {
		return _self->_lead();
	}

	let let::last() const {
		return _self->_last();
	}

	let let::place_lead(const let& other) const {
		return _self->_place_lead(other);
	}

	let let::shift_lead() const {
		return _self->_shift_lead();
	}

	let let::place_last(const let& other) const {
		return _self->_place_last(other);
	}

	let let::shift_last() const {
		return _self->_shift_last();
	}

	let let::reverse() const {
		return _self->_reverse();
	}

	let let::get(const let& other) const {
		return _self->_get(other);
	}

	let let::set(const let& other, const let& val) const {
		return _self->_set(other, val);
	}

	bool_type let::iterable() const {
		return _self->_iterable();
	}

	bool_type let::is_nothing() const {
		return _self->_is_nothing();
	}

	bool_type let::is_set() const {
		return _self->_is_set();
	}

	bool_type let::atomic() const {
		return _self->_atomic();
	}

	bool_type let::nan() const {
		return _self->_nan();
	}

	bool_type let::complex() const {
		return _self->_complex();
	}

	OP_CODE let::op_code() const {
		return _self->_op_code();
	}

	str_type let::help() const {
		return _self->_help();
	}

	bool_type let::operator==(const let& other) const {
		return eq(other);
	}

	bool_type let::operator!=(const let& other) const {
		return ne(other);
	}

	bool_type let::operator>=(const let& other) const {
		return ge(other);
	}

	bool_type let::operator> (const let& other) const {
		return gt(other);
	}

	bool_type let::operator<=(const let& other) const {
		return le(other);
	}

	bool_type let::operator< (const let& other) const {
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
	//                                'data_type' Class Implementation
	//
	/********************************************************************************************/

	template <typename T>
	let::data_type<T>::data_type(T val) : _data(std::move(val)) {
	}

	template <typename T>
	let::data_type<T>::operator bool() const {
		return _is();
	}

	template <typename T>
	void* let::data_type<T>::_vptr() {
		return &_data;
	}

	template <typename T>
	str_type let::data_type<T>::_id() const {
		return typeid(_data).name();
	}

	template <typename T>
	std::size_t let::data_type<T>::_hash() const {
		return _hash_(_data);
	}

	template <typename T>
	str_type let::data_type<T>::_type() const {
		return _type_(_data);
	}

	template <typename T>
	bool_type let::data_type<T>::_is() const {
		return _is_(_data);
	}

	template <typename T>
	void let::data_type<T>::_str(stream_type& out) const {
		_str_(out, _data);
	}

	template <typename T>
	void let::data_type<T>::_repr(stream_type& out) const {
		_repr_(out, _data);
	}

	template <typename T>
	real_type let::data_type<T>::_comp(const let& other) const {
		return _comp_(_data, other);
	}

	template <typename T>
	let let::data_type<T>::_add(const let& other) const {
		return _add_(_data, other);
	}

	template <typename T>
	let let::data_type<T>::_sub(const let& other) const {
		return _sub_(_data, other);
	}

	template <typename T>
	let let::data_type<T>::_mul(const let& other) const {
		return _mul_(_data, other);
	}

	template <typename T>
	let let::data_type<T>::_div(const let& other) const {
		return _div_(_data, other);
	}

	template <typename T>
	let let::data_type<T>::_mod(const let& other) const {
		return _mod_(_data, other);
	}

	template <typename T>
	let let::data_type<T>::_neg() const {
		return _neg_(_data);
	}

	template <typename T>
	let let::data_type<T>::_f_div(const let& other) const {
		return _f_div_(_data, other);
	}

	template <typename T>
	let let::data_type<T>::_rem(const let& other) const {
		return _rem_(_data, other);
	}

	template <typename T>
	let let::data_type<T>::_pow(const let& other) const {
		return _pow_(_data, other);
	}

	template <typename T>
	size_type let::data_type<T>::_size() const {
		return _size_(_data);
	}

	template <typename T>
	let let::data_type<T>::_lead() const {
		return _lead_(_data);
	}

	template <typename T>
	let let::data_type<T>::_last() const {
		return _last_(_data);
	}

	template <typename T>
	let let::data_type<T>::_place_lead(const let& other) const {
		return _place_lead_(_data, other);
	}

	template <typename T>
	let let::data_type<T>::_shift_lead() const {
		return _shift_lead_(_data);
	}

	template <typename T>
	let let::data_type<T>::_place_last(const let& other) const {
		return _place_last_(_data, other);
	}

	template <typename T>
	let let::data_type<T>::_shift_last() const {
		return _shift_last_(_data);
	}

	template <typename T>
	let let::data_type<T>::_reverse() const {
		return _reverse_(_data);
	}

	template <typename T>
	let let::data_type<T>::_get(const let& key) const {
		return _get_(_data, key);
	}

	template <typename T>
	let let::data_type<T>::_set(const let& key, const let& val) const {
		return _set_(_data, key, val);
	}

	template <typename T>
	bool_type let::data_type<T>::_iterable() const {
		return _iterable_(_data);
	}

	template <typename T>
	bool_type let::data_type<T>::_is_nothing() const {
		return _is_nothing_(_data);
	}

	template <typename T>
	bool_type let::data_type<T>::_is_set() const {
		return _is_set_(_data);
	}

	template <typename T>
	bool_type let::data_type<T>::_atomic() const {
		return _atomic_(_data);
	}

	template <typename T>
	bool_type let::data_type<T>::_nan() const {
		return _nan_(_data);
	}

	template <typename T>
	bool_type let::data_type<T>::_complex() const {
		return _complex_(_data);
	}

	template <typename T>
	str_type let::data_type<T>::_help() const {
		return _help_(_data);
	}

	template <typename T>
	OP_CODE let::data_type<T>::_op_code() const {
		return _op_code_(_data);
	}

	/********************************************************************************************/
	//
	//                            Basic Primitive Implementations
	//
	/********************************************************************************************/

	void print() {
		std::cout << std::endl;
	}

	void print(const str_type& str) {
		std::cout << str << std::endl;
	}

	void print(const let& a) {
		print(str(a));
	}

	str_type str(const let& a) {
		/*
			Convert a 'let' to its string representation.
		*/

		stream_type stream;

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

	str_type repr(const let& a) {
		/*
			Convert a 'let' to its representation as a string.
		*/

		stream_type stream;

		stream << std::boolalpha;

		a.repr(stream);

		return stream.str();
	}

	inline bool_type expression_is_empty(let expr) {
		/*
			Return true if the expresion is empty.
		*/
		return !expr.is();
	}

	inline bool_type object_is_empty(let expr) {
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

	inline str_type to_lower(str_type str) {

		std::transform(str.begin(), str.end(), str.begin(), ::tolower);

		return str;
	}

	inline str_type to_upper(str_type str) {

		std::transform(str.begin(), str.end(), str.begin(), ::toupper);

		return str;
	}

	inline void ltrim(str_type& s) {
		s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](int ch) {
			return !std::isspace(ch);
			}));
	}

	inline void rtrim(str_type& s) {
		s.erase(std::find_if(s.rbegin(), s.rend(), [](int ch) {
			return !std::isspace(ch);
			}).base(), s.end());
	}

	inline void lrtrim(str_type& s) {
		ltrim(s);
		rtrim(s);
	}

	inline str_type left_trim(str_type s) {
		ltrim(s);
		return s;
	}

	inline str_type right_trim(str_type s) {
		rtrim(s);
		return s;
	}

	inline str_type trim(str_type s) {
		lrtrim(s);
		return s;
	}

	tokens_type split(str_type str, char delim) {

		std::stringstream stream;
		stream.str(str);

		tokens_type tokens;

		while (std::getline(stream, str, delim)) {

			tokens.push_back(str);
		}

		return tokens;
	}

	tokens_type split(const str_type& str, str_type delim) {

		if (delim == "") {
			return split(str, ESCAPE_CHAR);
		}

		tokens_type tokens, buffer, temp;

		char d;

		d = delim.back();
		delim.pop_back();

		tokens = split(str, d);

		while (delim.size()) {

			d = delim.back();
			delim.pop_back();

			buffer = tokens;
			tokens.clear();

			for (size_type i = 0, stop = buffer.size(); i < stop; i += 1) {

				temp = split(buffer.at(i), d);

				for (size_type j = 0, stop = temp.size(); j < stop; j += 1) {

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