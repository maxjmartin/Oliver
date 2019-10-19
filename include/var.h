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
	//                                'var' Class Definition
	//
	//          The 'var' class represents an immutable object wrapper.  It will accept 
	//          any object assigned to by the assignment operator '='.
	//              Example:  var a = 42;
	//
	//          The 'var' class supports both a functional and object oriented API.
	//          But it requires that any object utilizing its API override a series of
	//          functions described below, in order to utilize run-time polymorphism.
	//
	//          The 'var' class also supports both pass by reference and pass by value.
	//          It also can allow any object it holds to be safely cast to a point_ter of
	//          the object type.  If the type cast is made to an incorrect data type
	//          then a std::null_ptr is returned instead.
	//
	//          The fundamental structure of the 'var' data type was inspired and extended 
	//          from a presentation entitled:
	//                 Title: Value Semantics and Concept-based Polymorphism
	//                 By - Sean Parent
	//                 (http://sean-parent.stlab.cc/papers-and-presentations)
	//
	/********************************************************************************************/

	class null;  // Forward class definition of the null data type.

	class var {

	public:

		var();
		template <typename T>         var(T x);
		template <typename T>         var(T* x);

		template <typename T> T        as()                             const;
		template <typename T> const T* cast()                           const;

		str_t             id()                                          const;
		bool_t       is_type(var other)                                 const;
		std::size_t     hash()                                          const;

		str_t           type()                                          const;
		bool_t            is()                                          const;
		void             str(stream_t& out)                             const;
		void            repr(stream_t& out)                             const;

		real_t          comp(var other)                                 const;
		bool_t            eq(var other)                                 const;
		bool_t            ne(var other)                                 const;
		bool_t            ge(var other)                                 const;
		bool_t            le(var other)                                 const;
		bool_t            gt(var other)                                 const;
		bool_t            lt(var other)                                 const;

		bool_t         l_and(var other)                                 const;
		bool_t          l_or(var other)                                 const;
		bool_t         l_xor(var other)                                 const;
		bool_t         l_not()                                          const;

		var              add(var other)                                 const;
		var              sub(var other)                                 const;
		var              mul(var other)                                 const;
		var              div(var other)                                 const;
		var              mod(var other)                                 const;

		var            f_div(var other)                                 const;
		var              rem(var other)                                 const;
		var              pow(var other)                                 const;

		int_t            len()                                          const;
		var             lead()                                          const;
		var            place(var other)                                 const;
		var            shift()                                          const;
		var          reverse()                                          const;

		var            slice(var start, var stop, var skip)             const;
		var              set(var val, var start, var stop, var skip)    const;

		bool_t     interable()                                          const;
		bool_t       nothing()						                    const;
		bool_t        atomic()                                          const;
		bool_t           nan()                                          const;
		bool_t       complex()                                          const;

		str_t           help()                                          const;

		bool_t    operator==(const var& other)                          const;
		bool_t    operator!=(const var& other)                          const;
		bool_t    operator>=(const var& other)                          const;
		bool_t    operator> (const var& other)                          const;
		bool_t    operator<=(const var& other)                          const;
		bool_t    operator< (const var& other)                          const;

		var        operator+(const var& other)                          const;
		var        operator-(const var& other)                          const;
		var        operator*(const var& other)                          const;
		var        operator/(const var& other)                          const;
		var        operator%(const var& other)                          const;

	private:

		struct interface_t {

			/********************************************************************************************/
			//
			//                              'interface_t' Class Definition
			//
			//       A simple interface description allowing redirection of the 'var' data type.
			//
			/********************************************************************************************/

			virtual  ~interface_t() = default;
			virtual operator bool()                                               const = 0;

			virtual void*         __as()                                                = 0;
			virtual str_t         __id()                                          const = 0;
			virtual std::size_t   __hash()                                        const = 0;

			virtual str_t         __type()                                        const = 0;
			virtual bool_t        __is()                                          const = 0;
			virtual void          __str(stream_t& out)                            const = 0;
			virtual void          __repr(stream_t& out)                           const = 0;

			virtual real_t        __comp(var other)                               const = 0;

			virtual bool_t       __l_and(var other)                               const = 0;
			virtual bool_t       __l_or(var other)                                const = 0;
			virtual bool_t       __l_xor(var other)                               const = 0;
			virtual bool_t       __l_not()                                        const = 0;

			virtual var          __add(var other)                                 const = 0;
			virtual var          __sub(var other)                                 const = 0;
			virtual var          __mul(var other)                                 const = 0;
			virtual var          __div(var other)                                 const = 0;
			virtual var          __mod(var other)                                 const = 0;

			virtual var          __f_div(var other)                               const = 0;
			virtual var          __rem(var other)                                 const = 0;
			virtual var          __pow(var other)                                 const = 0;

			virtual int_t        __len()                                          const = 0;
			virtual var          __lead()                                         const = 0;
			virtual var          __place(var other)                               const = 0;
			virtual var          __shift()                                        const = 0;
			virtual var          __reverse()                                      const = 0;
			virtual var          __slice(var start, var stop, var skip)           const = 0;
			virtual var          __set(var val, var start, var stop, var skip)    const = 0;

			virtual bool_t       __interable()                                    const = 0;
			virtual bool_t       __nothing()					                  const = 0;
			virtual bool_t       __atomic()                                       const = 0;
			virtual bool_t       __nan()                                          const = 0;
			virtual bool_t       __complex()                                      const = 0;

			virtual str_t        __help()                                         const = 0;
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

			void*        __as()                                                ;
			str_t        __id()                                           const;
			std::size_t  __hash()                                         const;

			str_t        __type()                                         const;
			bool_t       __is()                                           const;
			void         __str(stream_t& out)                             const;
			void         __repr(stream_t& out)                            const;

			real_t       __comp(var other)                                const;

			bool_t       __l_and(var other)                               const;
			bool_t       __l_or(var other)                                const;
			bool_t       __l_xor(var other)                               const;
			bool_t       __l_not()                                        const;

			var          __add(var other)                                 const;
			var          __sub(var other)                                 const;
			var          __mul(var other)                                 const;
			var          __div(var other)                                 const;
			var          __mod(var other)                                 const;

			var          __f_div(var other)                               const;
			var          __rem(var other)                                 const;
			var          __pow(var other)                                 const;

			int_t        __len()                                          const;
			var          __lead()                                         const;
			var          __place(var other)                               const;
			var          __shift()                                        const;
			var          __reverse()                                      const;
			var          __slice(var start, var stop, var skip)           const;
			var          __set(var val, var start, var stop, var skip)    const;

			bool_t       __interable()                                    const;
			bool_t       __nothing()						              const;
			bool_t       __atomic()	     					              const;
			bool_t       __nan()	     					              const;
			bool_t       __complex()	     					          const;

			str_t        __help()                                         const;

			T            _data;
		};

		std::shared_ptr<const interface_t> _self;
	};

	/********************************************************************************************/
	//
	//                                 'null' Class Definition
	//
	//          A basic class definition of the value of nothing.  This is used within
	//          'var' class implementation to return a result of nothing for results
	//          which have either conflicting types, or in some cases as the default to
	//          return unless overridden.
	//
	//          This class also demonstrates the basic function methods that should be
	//          over written for proper object behavior.  And is a useful data type on
	//          its own.
	//
	/********************************************************************************************/

	class null {

	public:

		null();
		null(const null& obj);
		null(str_t str);
		virtual ~null();

		// friend stream_t& operator >> (stream_t& stream, null& self);
		// TODO: Determine if the stream >> operator needs or even should be supported.  

		friend str_t      __type__(const null& self);
		friend bool_t       __is__(const null& self);
		friend real_t     __comp__(const null& self, var other);
		friend void        __str__(stream_t& out, const null& self);
		friend void       __repr__(stream_t& out, const null& self);

		friend bool_t  __nothing__(const null& self);
	};


	/********************************************************************************************/
	//
	//                                Basic Primitive Declarations
	//
	//          These definitions exist to support the 'var' methods which do not have a
	//          specific operator overload.  Or in other cases to add a few useful support
	//          functions.
	//
	/********************************************************************************************/

	void print();                          // Simply print a new line.
	void print(const str_t& str);          // Accept any single string and print it with a std::endl.
	void print(const var& a);              // Accept any single 'var' and print it with a std::endl.

	str_t str(const  var& a);              // Convert any 'var' to a str_t.
	str_t repr(const var& a);              // Convert any 'var' to a str_t representation of the 'var'.


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

	void print(const var& a) {
		print(str(a));
	}

	str_t str(const var& a) {
		/*
			Convert a 'var' to its string representation.
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

	str_t repr(const var& a) {
		/*
			Convert a 'var' to its representation as a string.
		*/

		stream_t stream;

		stream << std::boolalpha;

		a.repr(stream);

		return stream.str();
	}


	/********************************************************************************************/
	//
	//                      'var' Class Function Default Template API.
	//
	//           The following function templates define the over-ridable functions which
	//           may be used to tailor the behavior of any object held within a 'var'.
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
	real_t __comp__(const T& self, var other);

	template<typename T>
	real_t __comp__(const T& self, var other) {
		return NOT_A_NUMBER;
	}


	template<typename T>            /****  Logical And  ****/
	bool_t __l_and__(const T& self, var other);

	template<typename T>
	bool_t __l_and__(const T& self, var other) {
		return false;
	}


	template<typename T>            /****  Logical Or  ****/
	bool_t __l_or__(const T& self, var other);

	template<typename T>
	bool_t __l_or__(const T& self, var other) {
		return false;
	}


	template<typename T>            /****  Logical Exclusive Or  ****/
	bool_t __l_xor__(const T& self, var other);

	template<typename T>
	bool_t __l_xor__(const T& self, var other) {
		return false;
	}


	template<typename T>            /****  Logical Negation  ****/
	bool_t __l_not__(const T& self);

	template<typename T>
	bool_t __l_not__(const T& self) {
		return true;
	}

	

	template<typename T>            /****  Addition or Concatenation  ****/
	var __add__(const T& self, var other);

	template<typename T>
	var __add__(const T& self, var other) {
		return null();
	}


	template<typename T>            /****  Subtraction or Deletion  ****/
	var __sub__(const T& self, var other);

	template<typename T>
	var __sub__(const T& self, var other) {
		return null();
	}


	template<typename T>            /****  Multiplication  ****/
	var __mul__(const T& self, var other);

	template<typename T>
	var __mul__(const T& self, var other) {
		return null();
	}


	template<typename T>            /****  Division  ****/
	var __div__(const T& self, var other);

	template<typename T>
	var __div__(const T& self, var other) {
		return null();
	}

	template<typename T>            /****  Modulation  ****/
	var __mod__(const T& self, var other);

	template<typename T>
	var __mod__(const T& self, var other) {
		return null();
	}


	template<typename T>            /****  Floor Division  ****/
	var __f_div__(const T& self, var other);

	template<typename T>
	var __f_div__(const T& self, var other) {
		return null();
	}


	template<typename T>            /****  Remainder  ****/
	var __rem__(const T& self, var other);

	template<typename T>
	var __rem__(const T& self, var other) {
		return null();
	}


	template<typename T>            /****  Raise to Power of  ****/
	var __pow__(const T& self, var other);

	template<typename T>
	var __pow__(const T& self, var other) {
		return null();
	}


	template<typename T>            /****  Length Of  ****/
	int_t __len__(const T& self);

	template<typename T>
	int_t __len__(const T& self) {
		return 0;
	}


	template<typename T>            /****  Lead Element Of  ****/
	var __lead__(const T& self);

	template<typename T>
	var __lead__(const T& self) {
		return null();
	}


	template<typename T>            /**** Perpend Lead Element Of  ****/
	var __place__(const T& self, var other);

	template<typename T>
	var __place__(const T& self, var other) {
		return null();
	}


	template<typename T>            /****  Left Shift Remove  ****/
	var __shift__(const T& self);

	template<typename T>
	var __shift__(const T& self) {
		return null();
	}


	template<typename T>            /****  Reverse The Elements Of  ****/
	var __reverse__(const T& self);

	template<typename T>
	var __reverse__(const T& self) {
		return null();
	}


	template<typename T>            /****  Retrieve A Selection From  ****/
	var __slice__(const T& self, var start, var stop, var skip);

	template<typename T>
	var __slice__(const T& self, var start, var stop, var skip) {
		return null();
	}


	template<typename T>            /****  Set A Selection Of  ****/
	var __set__(const T& self, var val, var start, var stop, var skip);

	template<typename T>
	var __set__(const T& self, var val, var start, var stop, var skip) {
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

	real_t __comp__(const null& self, var other) {
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
	//                                'var' Class Implementation
	//
	/********************************************************************************************/

	var::var() : _self(std::make_shared<data_t<null>>(null())) {
	}

	template <typename T>
	var::var(T x) : _self(std::make_shared<data_t<T>>(std::move(x))) {
	}

	template <typename T>
	var::var(T* x) : _self(std::make_shared<data_t<T>>(x)) {
	}

	template <typename T> T var::as() const {

		T value = T();

		if (_self->__id() == typeid(value).name()) {

			memcpy(&value, const_cast<interface_t*>(_self.get())->__as(), sizeof(T));
		}

		return value;
	}

	template <typename T> const T* var::cast() const {

		const T* p = static_cast<T*>(const_cast<interface_t*>(_self.get())->__as());

		if (p) {
			return p;
		}

		return nullptr;
	}

	str_t var::id() const {
		return _self->__id();
	}

	bool_t var::is_type(var other) const {
		return _self->__id() == other._self->__id();
	}

	std::size_t var::hash() const {
		return _self->__hash();
	}

	str_t var::type() const {
		return _self->__type();
	}

	bool_t var::is() const {
		return const_cast<interface_t*>(_self.get())->__is();
	}

	void var::str(stream_t& out) const {
		_self->__str(out);
	}

	void var::repr(stream_t& out) const {
		_self->__repr(out);
	}

	real_t var::comp(var other) const {
		return _self->__comp(other);
	}

	bool_t var::eq(var other) const {
		return (comp(other) == 0 ? true : false);
	}

	bool_t var::ne(var other) const {
		return (comp(other) != 0 ? true : false);
	}

	bool_t var::ge(var other) const {
		return (comp(other) >= 0 ? true : false);
	}

	bool_t var::le(var other) const {
		return (comp(other) <= 0 ? true : false);
	}

	bool_t var::gt(var other) const {
		return (comp(other) > 0 ? true : false);
	}

	bool_t var::lt(var other) const {
		return (comp(other) < 0 ? true : false);
	}

	bool_t var::l_and(var other) const {
		return _self->__l_and(other);
	}

	bool_t var::l_or(var other) const {
		return _self->__l_or(other);
	}

	bool_t var::l_xor(var other) const {
		return _self->__l_xor(other);
	}

	bool_t var::l_not() const {
		return _self->__l_not();
	}

	var var::add(var other) const {
		return _self->__add(other);
	}

	var var::sub(var other) const {
		return _self->__sub(other);
	}

	var var::mul(var other) const {
		return _self->__mul(other);
	}

	var var::div(var other) const {
		return _self->__div(other);
	}

	var var::mod(var other) const {
		return _self->__mod(other);
	}

	var var::f_div(var other) const {
		return _self->__f_div(other);
	}

	var var::rem(var other) const {
		return _self->__rem(other);
	}

	var var::pow(var other) const {
		return _self->__pow(other);
	}

	int_t var::len() const {
		return _self->__len();
	}

	var var::lead() const {
		return _self->__lead();
	}

	var var::place(var other) const {
		return _self->__place(other);
	}

	var var::shift() const {
		return _self->__shift();
	}

	var var::reverse() const {
		return _self->__reverse();
	}

	var var::slice(var start, var stop, var skip) const {
		return _self->__slice(start, stop, skip);
	}

	var var::set(var val, var start, var stop, var skip) const {
		return _self->__slice(start, stop, skip);
	}

	bool_t var::interable() const {
		return _self->__interable();
	}

	bool_t var::nothing() const {
		return _self->__nothing();
	}

	bool_t var::atomic() const {
		return _self->__atomic();
	}

	bool_t var::nan() const {
		return _self->__nan();
	}

	bool_t var::complex() const {
		return _self->__complex();
	}

	str_t var::help() const {
		return _self->__help();
	}

	bool_t var::operator==(const var& other) const {
		return eq(other);
	}

	bool_t var::operator!=(const var& other) const {
		return ne(other);
	}

	bool_t var::operator>=(const var& other) const {
		return ge(other);
	}

	bool_t var::operator> (const var& other) const {
		return gt(other);
	}

	bool_t var::operator<=(const var& other) const {
		return le(other);
	}

	bool_t var::operator< (const var& other) const {
		return lt(other);
	}

	var var::operator+(const var& other) const {
		return add(other);
	}

	var var::operator-(const var& other) const {
		return sub(other);
	}

	var var::operator*(const var& other) const {
		return mul(other);
	}

	var var::operator/(const var& other) const {
		return div(other);
	}

	var var::operator%(const var& other) const {
		return mod(other);
	}

	/********************************************************************************************/
	//
	//                                'data_t' Class Implementation
	//
	/********************************************************************************************/

	template <typename T>
	var::data_t<T>::data_t(T val) : _data(std::move(val)) {
	}

	template <typename T>
	var::data_t<T>::operator bool() const {
		return __is();
	}

	template <typename T>
	void* var::data_t<T>::__as() {
		return &_data;
	}

	template <typename T>
	str_t var::data_t<T>::__id() const {
		return typeid(_data).name();
	}

	template <typename T>
	std::size_t var::data_t<T>::__hash() const {
		return __hash__(_data);
	}

	template <typename T>
	str_t var::data_t<T>::__type() const {
		return __type__(_data);
	}

	template <typename T>
	bool_t var::data_t<T>::__is() const {
		return __is__(_data);
	}

	template <typename T>
	void var::data_t<T>::__str(stream_t& out) const {
		__str__(out, _data);
	}

	template <typename T>
	void var::data_t<T>::__repr(stream_t& out) const {
		__repr__(out, _data);
	}

	template <typename T>
	real_t var::data_t<T>::__comp(var other) const {
		return __comp__(_data, other);
	}

	template <typename T>
	bool_t var::data_t<T>::__l_and(var other) const {
		return __l_and__(_data, other);
	}

	template <typename T>
	bool_t var::data_t<T>::__l_or(var other) const {
		return __l_or__(_data, other);
	}

	template <typename T>
	bool_t var::data_t<T>::__l_xor(var other) const {
		return __l_xor__(_data, other);
	}

	template <typename T>
	bool_t var::data_t<T>::__l_not() const {
		return __l_not__(_data);
	}

	template <typename T>
	var var::data_t<T>::__add(var other) const {
		return __add__(_data, other);
	}

	template <typename T>
	var var::data_t<T>::__sub(var other) const {
		return __sub__(_data, other);
	}

	template <typename T>
	var var::data_t<T>::__mul(var other) const {
		return __mul__(_data, other);
	}

	template <typename T>
	var var::data_t<T>::__div(var other) const {
		return __div__(_data, other);
	}

	template <typename T>
	var var::data_t<T>::__mod(var other) const {
		return __mod__(_data, other);
	}

	template <typename T>
	var var::data_t<T>::__f_div(var other) const {
		return __f_div__(_data, other);
	}

	template <typename T>
	var var::data_t<T>::__rem(var other) const {
		return __rem__(_data, other);
	}

	template <typename T>
	var var::data_t<T>::__pow(var other) const {
		return __pow__(_data, other);
	}

	template <typename T>
	int_t var::data_t<T>::__len() const {
		return __len__(_data);
	}

	template <typename T>
	var var::data_t<T>::__lead() const {
		return __lead__(_data);
	}

	template <typename T>
	var var::data_t<T>::__place(var other) const {
		return __place__(_data, other);
	}

	template <typename T>
	var var::data_t<T>::__shift() const {
		return __shift__(_data);
	}

	template <typename T>
	var var::data_t<T>::__reverse() const {
		return __reverse__(_data);
	}

	template <typename T>
	var var::data_t<T>::__slice(var start, var stop, var skip) const {
		return __slice__(_data, start, stop, skip);
	}

	template <typename T>
	var var::data_t<T>::__set(var val, var start, var stop, var skip) const {
		return __slice__(_data, start, stop, skip);
	}

	template <typename T>
	bool_t var::data_t<T>::__interable() const {
		return __interable__(_data);
	}

	template <typename T>
	bool_t var::data_t<T>::__nothing() const {
		return __nothing__(_data);
	}

	template <typename T>
	bool_t var::data_t<T>::__atomic() const {
		return __atomic__(_data);
	}

	template <typename T>
	bool_t var::data_t<T>::__nan() const {
		return __nan__(_data);
	}

	template <typename T>
	bool_t var::data_t<T>::__complex() const {
		return __complex__(_data);
	}

	template <typename T>
	str_t var::data_t<T>::__help() const {
		return __help__(_data);
	}

} // end namespace