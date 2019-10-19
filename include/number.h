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

#include <complex>
#include "var.h"

namespace Olly {

	/********************************************************************************************/
	//
	//                                 'number' class
	//
	//        The number class implements a mathematical number using the 'num_t'
	//        from the 'sys_types' header.  At this time the class supports complex 
	//        numerical operations through the C++ complex class implemented using
	//        the 'real_t' typedef from the 'sys_types' header.  
	//
	//        It should be noted that complex numbers must be entered with a comma
	//        separating the real and complex portions of the number.  
	//
	//        TODO: re-implement the class using arbitrary arithmetic.
	//
	/********************************************************************************************/

	class number {

	public:

		number();
		number(const number& obj);
		number(const num_t& value);
		number(str_t str);
		number(const int_t& value);
		number(const real_t& value);
		number(const real_t& real, const real_t& imgn);
		number(const unsigned long long& value);
		number(const unsigned long& value);
		number(const unsigned int& value);
		virtual ~number();

		friend stream_t& operator >> (stream_t& stream, number& self);

		friend str_t      __type__(const number& self);
		friend bool_t       __is__(const number& self);
		friend real_t     __comp__(const number& self, var other);
		friend void        __str__(stream_t& out, const number& self);
		friend void       __repr__(stream_t& out, const number& self);

		friend bool_t    __l_and__(const number& self, var other);
		friend bool_t     __l_or__(const number& self, var other);
		friend bool_t    __l_xor__(const number& self, var other);
		friend bool_t    __l_not__(const number& self);

		friend var         __add__(const number& self, var other);
		friend var         __sub__(const number& self, var other);
		friend var         __mul__(const number& self, var other);
		friend var         __div__(const number& self, var other);
		friend var         __mod__(const number& self, var other);

		friend var       __f_div__(const number& self, var other);
		friend var         __rem__(const number& self, var other);
		friend var         __pow__(const number& self, var other);

		friend bool_t      __nan__(const number& self);
		friend bool_t  __complex__(const number& self);

	private:
		num_t _value;
	};

	number::number() : _value(0.0, 0.0) {
	}

	number::number(const number& obj) : _value(obj._value) {
	}

	number::number(const num_t& value) : _value(value) {
	}

	number::number(str_t str) : _value(0.0, 0.0) {

		if (str == "") {
			return;
		}

		str = to_lower(str);

		if (str == "nan") {
			_value = num_t(std::numeric_limits<real_t>::quiet_NaN(), std::numeric_limits<real_t>::quiet_NaN());
			return;
		}

		if (str == "inf" || str == "+inf") {
			_value = num_t(std::numeric_limits<real_t>::infinity(), std::numeric_limits<real_t>::infinity());
			return;
		}

		if (str == "-inf") {
			_value = num_t(std::numeric_limits<real_t>::infinity(), std::numeric_limits<real_t>::infinity());
			return;
		}

		std::replace(str.begin(), str.end(), ',', ' ');

		tokens_t tokens = split(str, " ");

		real_t real = 0;
		real_t imgn = 0;

		if (tokens.size()) {

			if (tokens.back().back() == 'i' || tokens.back().back() == 'j' || tokens.size() > 1) {

				str_t t = tokens.back();
				t.pop_back();

				imgn = to<real_t>(t);

				tokens.pop_back();
			}
		}

		if (tokens.size()) {

			real = to<real_t>(tokens.front());
		}

		_value = num_t(real, imgn);
	}

	number::number(const int_t& value) : _value(value, 0.0) {
	}

	number::number(const real_t& value) : _value(value, 0.0) {
	}

	number::number(const real_t& real, const real_t& imgn) : _value(real, imgn) {
	}

	number::number(const unsigned long long& value) : _value(static_cast<real_t>(value), 0.0) {
	}

	number::number(const unsigned long& value) : _value(value, 0.0) {
	}

	number::number(const unsigned int& value) : _value(value, 0.0) {
	}

	number::~number() {
	}

	stream_t& operator >> (stream_t& stream, number& self) {

		self = number(stream.str());

		return stream;
	}

	str_t __type__(const number& self) {
		return "number";
	}

	bool_t __is__(const number& self) {

		if (__nan__(self)) {
			return false;
		}

		return (self._value.real() || self._value.imag() ? true : false);
	}

	real_t __comp__(const number& self, var other) {

		const number* n = other.cast<number>();

		if (n) {
			if (__nan__(self) || __nan__(*n) || __complex__(self) || __complex__(*n)) {
				return NOT_A_NUMBER;
			}

			real_t x = self._value.real();
			real_t y = n->_value.real();

			if (x > y) {
				return 1.0;
			}

			if (x < y) {
				return -1.0;
			}

			return 0.0;
		}

		return NOT_A_NUMBER;
	}

	void __str__(stream_t& out, const number& self) {

		real_t real = self._value.real();
		real_t imag = self._value.imag();

		if (imag && !real) {

			out << imag << "j";
			return;
		}

		if (!imag && real) {

			out << real;
			return;
		}

		if (!real && !imag) {

			out << 0.0;
			return;
		}

		out << "(" << real << ",";

		out.setf(std::ios::showpos);

		out << imag << "j)";

		out.unsetf(std::ios::showpos);
	}

	void __repr__(stream_t& out, const number& self) {
		out << "\'";
		__str__(out, self);
		out << "\'";
	}


	bool_t __l_and__(const number& self, var other) {

		const number* n = other.cast<number>();

		if (n) {

			if (__nan__(self) || __nan__(*n) || __complex__(self) || __complex__(*n)) {
				return false;
			}

			return self._value.real() && n->_value.real();
		}

		return false;
	}

	bool_t __l_or__(const number& self, var other) {

		const number* n = other.cast<number>();

		if (n) {

			if (__nan__(self) || __nan__(*n) || __complex__(self) || __complex__(*n)) {
				return false;
			}

			return self._value.real() || n->_value.real();
		}

		return false;
	}

	bool_t __l_xor__(const number& self, var other) {

		const number* n = other.cast<number>();

		if (n) {

			if (__nan__(self) || __nan__(*n) || __complex__(self) || __complex__(*n)) {
				return false;
			}

			bool_t p = false;

			if (self._value.real()) {
				p = true;
			}

			bool_t q = false;

			if (n->_value.real()) {
				q = true;
			}

			return (bool_t)(p ^ q);
		}

		return false;
	}

	bool_t __l_not__(const number& self) {

		if (__nan__(self) || __complex__(self)) {
			return true;
		}

		return !self._value.real();
	}

	var __add__(const number& self, var other) {

		const number* n = other.cast<number>();

		if (n) {
			return number(self._value + n->_value);
		}
		return null();
	}

	var __sub__(const number& self, var other) {

		const number* n = other.cast<number>();

		if (n) {
			return number(self._value - n->_value);
		}
		return null();
	}

	var __mul__(const number& self, var other) {

		const number* n = other.cast<number>();

		if (n) {
			return number(self._value * n->_value);
		}
		return null();
	}

	var __div__(const number& self, var other) {

		const number* n = other.cast<number>();

		if (n) {
			return number(self._value / n->_value);
		}
		return null();
	}

	var __mod__(const number& self, var other) {

		const number* n = other.cast<number>();

		if (n) {

			if (__nan__(self) || __nan__(*n) || __complex__(self) || __complex__(*n)) {
				return number("nan");
			}

			return number((unsigned long long)self._value.real() % (unsigned long long)n->_value.real());
		}

		return null();
	}

	var __f_div__(const number& self, var other) {

		const number* n = other.cast<number>();

		if (n) {

			if (__nan__(self) || __nan__(*n) || __complex__(self) || __complex__(*n)) {
				return number("nan");
			}

			return number(std::floor(self._value.real() / n->_value.real()));
		}

		return null();
	}

	var __rem__(const number& self, var other) {

		const number* n = other.cast<number>();

		if (n) {

			if (__nan__(self) || __nan__(*n) || __complex__(self) || __complex__(*n)) {
				return number("nan");
			}

			real_t r = self._value.real() / n->_value.real();

			return number(r - std::floor(r));
		}

		return null();
	}

	var __pow__(const number& self, var other) {

		const number* n = other.cast<number>();

		if (n) {

			if (__nan__(self) || __nan__(*n)) {
				return number("nan");
			}

			return number(std::pow(self._value, n->_value));
		}

		return null();
	}

	bool_t __nan__(const number& self) {
		return std::isnan(self._value.real()) || std::isnan(self._value.imag());
	}

	bool_t __complex__(const number& self) {
		
		if (self._value.imag()) {
			return true;
		}

		return false;
	}

} // end
