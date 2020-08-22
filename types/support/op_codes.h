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

#include <map>

namespace Olly {

	/********************************************************************************************/
	//
	//			                       Interpreter Operator ENUM
	//			
	/********************************************************************************************/

	const enum class OP_CODE {
		NOTHING_OP = 0,

			begin_scope_op, end_scope_op, abstruse_op, abstrude_op,
			CONST_op, LET_op, const_op, let_op, def_op,
			EXC_op, ENC_op, 

		FUNCTION_SCOPE_OPERATORS,

			IS_op, NEG_op, NOT_op,

		POSTFIX_UNARY_OPERATORS,

			AND_op, OR_op, XOR_op,
			EQ_op, NE_op, LT_op, LE_op, GT_op, GE_op,
			ADD_op, SUB_op, MUL_op, DIV_op, MOD_op, FDIV_op, REM_op, POW_op,

		POSTFIX_BINARY_OPERATORS,

			imply_op, else_op, cond_op, is_const_op, is_var_op, LOOP_op, loop_op,

		EXTENDED_LOGIC_OPERATORS,

			neg_op, not_op,

		PREFIX_UNARY_OPERATORS,

			and_op, or_op, xor_op, 
			eq_op, ne_op, lt_op, le_op, gt_op, ge_op,
			add_op, sub_op, mul_op, div_op, mod_op, fdiv_op, rem_op, pow_op,

		INFIX_BINARY_OPERATORS,

			// ADD ABSTRACTION OPERATORS HERE 

		ABSTRACTION_OPERATORS,

			DEPTH_op, DUP_op, SWAP_op, OVER_op, DRop_op, ROT_op, DUP2_op, SWAP2_op, OVER2_op, DRop2_op,

		STACK_CODE_OPERATORS,
		
			BREAK_op, END_op,

		EVALUATION_OPERATORS,

		// Not sorted.
		EXPR_op, STACK_op,
		PRINT_op, REPR_op, ENDL_op,

		
		LEAD_op, SHIFT_op, PLACE_op, lead_op, shift_op, place_op,
		
		IN_EQ_op, IN_NE_op, IN_LT_op, IN_LE_op, IN_GT_op, IN_GE_op,
		IS_TRUE_op, IF_TRUE_op,  if_op
	};

	/********************************************************************************************/
	//
	//			                       Interpreter Operator Map
	//			
	/********************************************************************************************/

	static const std::map<str_t, OP_CODE> OPERATORS = {

		{ "none",      OP_CODE::NOTHING_OP },   { "nothing",  OP_CODE::NOTHING_OP },


		{ ":",	   OP_CODE::begin_scope_op },	{ "<==",     OP_CODE::abstruse_op },
		{ ";",	     OP_CODE::end_scope_op },	{ "==>",	 OP_CODE::abstrude_op },

		{ ":=",	         OP_CODE::const_op },
		{ "let",	       OP_CODE::let_op },
		{ "ENC",	       OP_CODE::ENC_op },
		{ "def",           OP_CODE::def_op },
		
		{ "loop",		  OP_CODE::loop_op },
		{ "imply",		 OP_CODE::imply_op },   { "else",		 OP_CODE::else_op },

		{ "=",			    OP_CODE::eq_op },
		{ "!=",			    OP_CODE::ne_op },
		{ "<",			    OP_CODE::lt_op },
		{ "<=",			    OP_CODE::le_op },
		{ ">",			    OP_CODE::gt_op },
		{ ">=",			    OP_CODE::ge_op },
		
		{ "and",		   OP_CODE::and_op },
		{ "or",			    OP_CODE::or_op },
		{ "not",		   OP_CODE::not_op },
		{ "xor",		   OP_CODE::xor_op },
		{ "cond",		  OP_CODE::cond_op },
		{ "const?",	  OP_CODE::is_const_op },
		{ "var?",		OP_CODE::is_var_op },
		{ "?",			    OP_CODE::IS_op },

		{ "+",		       OP_CODE::add_op },
		{ "-",		       OP_CODE::sub_op },
		{ "*",		       OP_CODE::mul_op },
		{ "/",		       OP_CODE::div_op },
		{ "mod",	       OP_CODE::mod_op },
		{ "neg",	       OP_CODE::neg_op },
		{ "//",		      OP_CODE::fdiv_op },
		{ "rem",           OP_CODE::rem_op },
		{ "**",            OP_CODE::pow_op },
		
		{ "EXPR",	         OP_CODE::EXPR_op },
		{ "STACK",	        OP_CODE::STACK_op },

		{ "PR",	            OP_CODE::PRINT_op },
		{ "RP",	             OP_CODE::REPR_op },
		{ "EL",	             OP_CODE::ENDL_op },

		{ "DEPTH",			OP_CODE::DEPTH_op },
		{ "DUP",		   	  OP_CODE::DUP_op },
		{ "SWAP",			 OP_CODE::SWAP_op },
		{ "OVER",			 OP_CODE::OVER_op },
		{ "DRop",			 OP_CODE::DRop_op },
		{ "ROT",			  OP_CODE::ROT_op },
		{ "DUP2",			 OP_CODE::DUP2_op },
		{ "SWAP2",			OP_CODE::SWAP2_op },
		{ "OVER2",		    OP_CODE::OVER2_op },
		{ "DRop2",			OP_CODE::DRop2_op },

		{ "LEAD",			 OP_CODE::LEAD_op },
		{ "SHIFT",			OP_CODE::SHIFT_op },
		{ "PLACE",		    OP_CODE::PLACE_op },
		{ "lead",			 OP_CODE::lead_op },
		{ "shift",			OP_CODE::shift_op },
		{ "place",			OP_CODE::place_op },

	};

} // end
