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

#include <regex>
#include <string>
#include "token_reader.h"
#include "text_reader.h"

#include "..\types\list.h"
#include "..\types\boolean.h"
#include "..\types\number.h"
#include "..\types\string.h"
#include "..\types\symbol.h"
#include "..\Oliver.h"

namespace Olly {

	/********************************************************************************************/
	//
	//                                    'parser' class definition
	//
	//        The parser class accepts individual characters from an instance of
	//        the text_reader.  The parser then appends each character together
	//        until a specific combination of characters or an individual character
	//        is recognized as invoking special behavior.  Else all words are
	//        defined by being split at whitespace.  
	//
	/********************************************************************************************/

	typedef		std::regex		regex_t;

	static const regex_t  REAL_REGEX("((\\+|-)?[[:digit:]]+)(\\.(([[:digit:]]+)?))?((e|E)((\\+|-)?)[[:digit:]]+)?", std::regex_constants::ECMAScript | std::regex_constants::optimize);

	class parser {

		typedef		char			char_t;

		text_reader			_input;    // The lexical code file to parse.
		tokens_t			_text;	   // The parsed code read from the input file.
		bool_t				_skip;     // Used to identify if a token exists within the bounds of a comment block.

	public:

		parser(str_t input);
		virtual ~parser();

		let parse();

	private:

		parser();
		parser(const parser& obj) = delete;

		int_t  is_regex_escape_char(const char& c);
		int_t  is_string_escape_char(const char& c);

		void process_word(str_t& word);

		str_t read_string();
		str_t read_number();
		str_t read_format();
		str_t read_regex();
		str_t list_op(const char& c);
		str_t object_op(const char& c);
		str_t expression_op(const char& c);
		str_t closure_op(const char& c);
		str_t scope_op(const char& c);

		void skip_comment_line();

		bool_t escape_char(char_t c);
		bool_t is_number(const str_t& word);

		let  compile();
		void compile(str_t& word, token_reader& code, let& exp);

		str_t collect_string(const str_t& stop, token_reader& code);
	};


	/********************************************************************************************/
	//                                'parser ' method definition
	/********************************************************************************************/

	parser::parser(str_t input) : _input(input), _text(), _skip() {
	}

	parser::~parser() {
	}

	let parser::parse() {
		/*
			Ensure that the file has content to parse.
		*/

		if (!_input.is()) {
			return expression();
		}

		while (_input.is()) {
			/*
				Loop through any whitespace at the start
				of the file.
			*/

			if (!escape_char(_input.peek())) {
				break;
			}

			_input.next();
		}

		str_t word;
		char c;

		// _text.push_back("(");

		while (_input.is()) {
			/*
				Grab each character and split each group
				of characters on whitespace or on a few
				special characters which have a specific
				semantic meaning.
			*/

			c = _input.next();

			if (!escape_char(c)) {

				if (word == "-") {
					_text.push_back("neg");
					word = "";
				}
				
				if (word == "+") {
					word = "";
				}
			}

			if (escape_char(c) && word != "") {
				/*
					If white space was encountered and
					there is a defined word, process
					the word.
				*/

				process_word(word);

				word = "";
			}

			else if (c == ',') {
				/*
					Consider commas an operator and
					split them from normal content.
				*/
				if (word.size()) {
					process_word(word);
					word = "";
				}

				if (!_skip) {

					_text.push_back(",");
				}

				c = ' ';
			}

			else if (c == '"') {
				/*
					A string identification was encountered.
					Process the string, and add it to _text
					so long as we are not in a comment block.
				*/

				if (word.size()) {
					process_word(word);
					word = "";
				}

				if (!_skip) {

					_text.push_back("\"");
					_text.push_back(read_string());
					_text.push_back("\"");
				}
			}

			else if (c == '\'') {
				/*
					A number was encountered.
				*/

				if (word.size()) {
					process_word(word);
					word = "";
				}

				if (!_skip) {

					_text.push_back("\'");
					_text.push_back(read_number());
					_text.push_back("\'");
				}
			}

			else if (c == '\\') {
				/*
					A regex identification was encountered.
					Process the regex, and add it to _text
					so long as we are not in a comment block.
				*/

				if (word.size()) {
					process_word(word);
					word = "";
				}

				if (!_skip) {

					_text.push_back("\\");
					_text.push_back(read_regex());
					_text.push_back("\\");
				}
			}

			else if (c == '`') {
				/*
					A format identification was encountered.
					Process the format, and add it to _text
					so long as we are not in a comment block.
				*/

				if (word.size()) {
					process_word(word);
					word = "";
				}

				if (!_skip) {

					_text.push_back("`");
					_text.push_back(read_format());
					_text.push_back("`");
				}
			}

			else if (c == '(' || c == ')') {
				/*
					An expression operator was encountered.
					Process the expression, and add it to _text
					so long as we are not in a comment block.
				*/

				if (word.size()) {
					process_word(word);
					word = "";
				}

				if (!_skip) {
					_text.push_back(expression_op(c));
				}

				c = ' ';
			}

			else if (c == ':' || c == ';') {
			/*
				An expression operator was encountered.
				Process the expression, and add it to _text
				so long as we are not in a comment block.
			*/

				if (word.size()) {
					process_word(word);
					word = "";
				}

				if (!_skip) {

					if (_input.peek() == '=') {
						_text.push_back(":=");
						c = _input.next();
					}
					else {
						_text.push_back(closure_op(c));
					}
					c = ' ';
				}
			}

			else if (c == '[' || c == ']') {
				/*
					A list operator was encountered.
					Process the list, and add it to _text
					so long as we are not in a comment block
				*/

				if (word.size()) {
					process_word(word);
					word = "";
				}

				if (!_skip) {
					_text.push_back(list_op(c));
				}

				c = ' ';
			}

			else if (c == '{' || c == '}') {
				/*
					An object operator was encountered.
					Process the object, and add it to _text
					so long as we are not in a comment block
				*/

				if (word.size()) {
					process_word(word);
					word = "";
				}

				if (!_skip) {
					_text.push_back(object_op(c));
				}

				c = ' ';
			}

			else if (c == '#') {
				/*
					A comment has probably been encountered.
					'#'  comment to the end of the line.
					'##' comments everything up to a second '##'.
					'#!' Reserved to implement a preprocessor type system?
				*/

				if (word.size()) {

					process_word(word);
					word = "";
				}

				if (_input.peek() == '#') {
					/*
						Comment blocks are handled by setting a
						bool_tean value to identify when a comment
						is found.  Else a function is called to
						handle the comment up to the next line.
					*/

					if (_skip) {

						_skip = false;
					}
					else {
						_skip = true;
					}

					while (_input.peek() == '#' && _input.is()) {

						_input.next();
					}
				}
				/*
				else if (_input.peek() == '!') {
				/*
					Define a shebang (crunch-bang) entry.
				*/
				/*
					_text.push("#!");

					_input.next();
					c = ' ';
				}
				*/

				else {
					skip_comment_line();
				}

			}

			else if (!escape_char(c)) {
				/*
					Append any non white space char
					to the word being formed.
				*/
				word += c;
			}
		}

		if (word != "") {
			/*
				Process any word left over after an
				eof in the input code is encountered.
			*/
			process_word(word);
		}

		// _text.push_back(")");

		if (_text.empty()) {
			return expression();
		}

		return compile();
	}

	int_t parser::is_regex_escape_char(const char& c) {
		/*
			Determine if an encountered character
			could be a part of an escaped char
			like '\t' or '\"' for examples.
		*/

		for (char o : "\\") {  // -[]/{}()*+?.\\^$|
			// TODO - flesh this out.  

			if (c == o) {
				return true;
			}
		}

		return false;
	}

	int_t parser::is_string_escape_char(const char& c) {
		/*
			Determine if an encountered character
			could be a part of an escaped char
			like '\t' or '\"' for examples.
		*/

		for (char o : "\'\"\\abfnrtv") {

			if (c == o) {
				return true;
			}
		}

		return false;
	}

	void parser::process_word(str_t& word) {
		/*
			Check that we are not within a comment block.
			Else ensure we have a work to handle and place
			it on the back of the text queue.
		*/

		if (_skip) {
			return;
		}

		if (word != "") {
			_text.push_back(word);
		}
	}

	str_t parser::read_format() {
		/*
			Read each character including
			whitespace and form a word to
			be used to define a format
			object.
		*/

		str_t str;

		char c;

		while (_input.is()) {

			c = _input.next();

			if (c == '`') {
				break;
			}

			else {
				str += c;
			}
		}

		if (str.empty()) {
			return "";
		}

		return str;
	}

	str_t parser::read_string() {
		/*
			Read each character including
			whitespace and form a word to
			be used to define a string of
			characters.

			When a '\' character is found
			check to see if it is used to
			define an escaped character.
			If it is append the escaped
			character to the string.
		*/

		bool_t escaped = false;

		str_t str;

		char c;

		while (_input.is()) {

			c = _input.next();

			if (escaped) {

				if (c == '\\') {
					str += '\\';
				}
				else if (c == 'a') {
					str += '\a';
				}
				else if (c == 'b') {
					str += '\b';
				}
				else if (c == 'f') {
					str += '\f';
				}
				else if (c == 'n') {
					str += '\n';
				}
				else if (c == 'r') {
					str += '\r';
				}
				else if (c == 't') {
					str += '\t';
				}
				else if (c == 'v') {
					str += '\v';
				}
				else {
					str += c;
				}

				escaped = false;
			}
			else if ((c == '\\') && (is_string_escape_char(_input.peek()))) {

				escaped = true;
			}
			else if (c == '"') {
				break;
			}
			else {
				str += c;
			}
		}

		return str;
	}

	str_t parser::read_number() {
		/*
			Read each character including
			whitespace and form a word to
			be used to define a string of
			characters.

			When a '\' character is found
			check to see if it is used to
			define an escaped character.
			If it is append the escaped
			character to the string.
		*/

		str_t str;

		char c;

		while (_input.is()) {

			c = _input.next();

			if (c == '\'') {
				break;
			}
			else {
				str += c;
			}
		}

		return str;
	}

	str_t parser::read_regex() {
		/*
			Read each character including
			whitespace and form a word to
			be used to define a regex of
			characters.

			When a '\' character is found
			check to see if it is used to
			define an escaped character.
			If it is append the escaped
			character to the regex.
		*/

		bool_t escaped = false;

		str_t str;

		char c;

		while (_input.is()) {

			c = _input.next();

			if (escaped) {

				if (c == '\\') {
					str += '\\';
				}
				else {
					str += c;
				}

				escaped = false;
			}
			else if ((c == '\\') && (is_string_escape_char(_input.peek()))) {

				escaped = true;
			}
			else if (c == '\\' && !escaped) {
				break;
			}
			else {
				str += c;
			}
		}

		return str;
	}

	str_t parser::list_op(const char& c) {
		/*
			Validate which list operator is provided.
		*/
		if (c == '[') {

			return "[";
		}

		return "]";
	}

	str_t parser::object_op(const char& c) {
		/*
			Validate which object operator is provided.
		*/
		if (c == '{') {

			return "{";
		}

		return "}";
	}

	str_t parser::expression_op(const char& c) {
		/*
			Validate which expression operator is provided.
		*/
		if (c == '(') {

			return "(";
		}

		return ")";
	}

	str_t parser::closure_op(const char& c) {
		/*
			Validate which expression operator is provided.
		*/
		if (c == ':') {

			return ":";
		}

		return ";";
	}

	str_t parser::scope_op(const char& c) {
		/*
			Validate whichscope operator is provided.
		*/
		if (c == ':') {

			return ":";
		}

		return ";";
	}

	void parser::skip_comment_line() {
		/*
			Skip all other characters until a newline is encountered.
		*/
		char c;

		while (_input.is()) {

			c = _input.next();

			if (c == '\n') {
				break;
			}
		}
	}
	bool_t parser::escape_char(char_t c) {

		if (c < 32) {
			return true;
		}

		if (::isspace(c)) {
			return true;
		}

		return false;
	}

	bool_t parser::is_number(const str_t& word) {
		return std::regex_match(word, REAL_REGEX);
	}

	let parser::compile() {

		token_reader code(_text);

		if (!code.is()) {
			return expression();
		}

		str_t word;
		let exp = expression();

		while (code.is()) {

			word = code.next();

			compile(word, code, exp);
		}

		while (code.is()) {

			word = code.next();

			compile(word, code, exp);
		}

		return exp.reverse();
	}

	void parser::compile(str_t& word, token_reader& code, let& exp) {

		if (word == ")" || word == ";" || word == "]" || word == "}") {
			return;
		}

		if (word == "\"") {
			exp = exp.place_lead(string(collect_string("\"", code)));
			return;
		}

		if (word == "\'") {
			exp = exp.place_lead(number(collect_string("\'", code)));
			return;
		}

		if (word == "(") {

			let e = expression();

			while (code.is() && word != ")") {

				word = code.next();

				compile(word, code, e);
			}
			word = "";

			exp = exp.place_lead(e.reverse());

			return;
		}

		if (word == ":") {

			let e = scope();

			e = e.place_lead(op_call(OP_CODE::begin_scope_op));

			while (code.is() && word != ";") {

				word = code.next();

				compile(word, code, e);
			}
			word = "";

			e = e.place_lead(op_call(OP_CODE::end_scope_op));

			exp = exp.place_lead(e.reverse());

			return;
		}

		if (word == "[") {

			let e = expression();

			while (code.is() && word != "]") {

				word = code.next();

				compile(word, code, e);
			}
			word = "";

			let l = list(e);

			exp = exp.place_lead(l);

			return;
		}

		if (word == "{") {

			let e = expression();

			while (code.is() && word != "}") {

				word = code.next();

				compile(word, code, e);
			}
			word = "";

			// exp = exp.place_lead(set(e));

			return;
		}

		if (word != "") {

			auto it = OPERATORS.find(word);

			if (it != OPERATORS.end()) {

				exp = exp.place_lead(op_call(it->second));
				return;
			}

			str_t upper_case = to_upper(word);

			if (upper_case == "TRUE"  || upper_case == "FALSE"    ||
				upper_case == "1"     || upper_case == "0"        ||
				upper_case == "UNDEF" || upper_case == "UNDEFINED") {

				exp = exp.place_lead(boolean(upper_case));
				return;
			}

			if (upper_case == "ELSE") {
				exp = exp.place_lead(boolean(true));
			}

			else if (upper_case != "NOTHING") {
				exp = exp.place_lead(symbol(word));
			}
		}
		return;
	}

	str_t parser::collect_string(const str_t& stop, token_reader& code) {

		str_t word = code.next();
		str_t text = "";

		while (word != stop && word != "") {

			if (word != "") {
				text += word;
				word = code.next();
			}
		}

		return text;
	}

} // end Olly