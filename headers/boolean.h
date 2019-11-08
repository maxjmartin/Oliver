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

namespace Olly {

	/********************************************************************************************/
	//
	//                              'boolean' Class Definition
	//
	//        The boolean class defines a fuzzy boolean object, defined as a weighted
	//        term.  That is the weight of the boolean defines the threshold of certainty
	//        which the term is considered true if equal to or greater than.  Both the 
	//        term and weight are bound within the range of 0.0 to 1.0.  
	//
	/********************************************************************************************/


	class boolean {

		real_t _term;
		real_t _weight;

	public:

		boolean();
		boolean(const boolean& obj);
		boolean(std::string str);
		boolean(const bool_t& n, real_t t = 1.0);
		boolean(const real_t& n, real_t t = 1.0);
		virtual ~boolean();

		friend  std::stringstream& operator >> (std::stringstream& stream, boolean& self);

		friend str_t         __type__(const boolean& self);
		friend bool_t          __is__(const boolean& self);
		friend real_t        __comp__(const boolean& self, const let& other);
		friend void           __str__(std::stringstream& out, const boolean& self);
		friend void          __repr__(std::stringstream& out, const boolean& self);
	};


	boolean::boolean() : _term(0.0), _weight(1.0) {
	}

	boolean::boolean(const boolean& obj) : _term(obj._term), _weight(obj._weight) {
	}

	boolean::boolean(std::string str) : _term(0.0), _weight(1.0) {

		if (str == "true" || str == "1") {

			_term = 1.0;
			return;
		}

		if (str == "false" || str == "0") {

			_term = 0.0;
			return;
		}

		if (str == "undef" || str == "undefined") {

			_term = NOT_A_NUMBER;
			_weight = NOT_A_NUMBER;
			return;
		}
	}

	boolean::boolean(const bool_t& n, real_t t) : _term(0.0), _weight(NOT_A_NUMBER) {

		if (n) {
			_term = 1.0;
		}

		if (t <= 1.0 && t >= 0) {
			_weight = t;
		}
	}

	boolean::boolean(const real_t& n, real_t t) : _term(NOT_A_NUMBER), _weight(NOT_A_NUMBER) {

		if (n <= 1.0 && n >= 0) {
			_term = n;
		}

		if (t <= 1.0 && t >= 0) {
			_weight = t;
		}
	}

	boolean::~boolean() {
	}

	std::stringstream& operator >> (std::stringstream& stream, boolean& self) {

		self = boolean(stream.str());

		return stream;
	}

	std::string __type__(const boolean& self) {
		return "boolean";
	}

	bool __is__(const boolean& self) {
		return self._term >= self._weight ? true : false;
	}

	real_t __comp__(const boolean& self, const let& other) {

		const boolean* b = other.cast<boolean>();

		if (b) {

			bool p = self._term >= self._weight ? true : false;
			bool q = b->_term >= b->_weight ? true : false;

			if (p > q) {
				return 1.0;
			}

			if (p < q) {
				return -1.0;
			}

			return 0.0;
		}

		return NOT_A_NUMBER;
	}

	void __str__(std::stringstream& out, const boolean& self) {

		if (self._term != self._term) {

			out << "undefined";

			return;
		}

		out << __is__(self);
	}

	void __repr__(std::stringstream& out, const boolean& self) {

		out << "('" << self._term << "' '" << self._weight << "' _bool)";
	}

} // end