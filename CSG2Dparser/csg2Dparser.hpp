#ifndef _CSG2DPARSER_HPP
#define _CSG2DPARSER_HPP

/******************************************************************************/
/* File:    csg2Dparser.hpp                                                   */
/* Author:  Cristina Precup, cpcristinaprecup@gmail.com                       */
/* Date:    20/06/2011                                                        */
/******************************************************************************/

/**  
 * @file csg2Dparser.hpp
 * The parser for the csg2d input files.
 */

#include "csg2d.hpp"
#include "planefigure.cpp"
#include "polygon.cpp"
#include "circle.cpp"
#include "ellipse.cpp"

#include <vector>
#include <string>
#include <map>
#include <utility>

#include <myadt.hpp>

using namespace netgen;
using namespace std;

/**  
 * @enum TOKEN_TYPE
 * Enum for tokens.
 * These are tokens that might appear in the csg2d input file.
 */
enum TOKEN_TYPE
	{
		TOK_MINUS = '-', /**< Minus token */
 		TOK_LP = '(', /**< Left parathesis token */
		OK_RP = ')', /**< Right paranthesis token */
		TOK_LSP = '[', /**< Left square bracket token */ 
		TOK_RSP = ']', /**< Right square bracket token */
		TOK_EQU = '=', /**< Equal token */
		TOK_COMMA = ',', /**< Comma token */ 
		TOK_SEMICOLON = ';', /**< Semicolon token */
		TOK_NUM = 100,
		TOK_STRING, TOK_PRIMITIVE,
		TOK_OR, /**< Boolean operator */
	 	TOK_AND, /**< Boolean operator */
		TOK_NOT, /**< Boolean operator */
		TOK_PLANEFIGURE,
		TOK_RECO2, /** The keyword for the csg2d input files: 'algebraic2d' */
 		TOK_TLO, /** Top-level object */
		TOK_END 
	};

/**  
 * @struct kwstruct
 * It associates a name with a certain token keyword.
 */
struct kwstruct
	{
		TOKEN_TYPE kw; /**< The token keyword */
		const char * name; /**< The name associated with the keyword */
  };

/**  
 * @enum PRITMIVE_TYPE
 * Enum for the possible types of primitive.
 */
enum PRIMITIVE_TYPE
	{
		TOK_POLYGON = 1, /**< A polygon primitive */
		TOK_CIRCLE, /**< A circle primitive */
		TOK_ELLIPSE, /**< An ellipse primitive */
		TOK_TRANSLATE /**< A translated primitive */
	};

/**  
 * @struct primstruct
 * It associates a name with a certain primitive keyword.
 */
struct primstruct
	{
		PRIMITIVE_TYPE kw; /**< The primitive keyword */
		const char * name; /**< The name of the primitive */
	};

/**
 * @typedef map<string, TopoDS_Shape> MyMapType
 * Can contain pairs of shapes and strings for their names.
 */
typedef map<string, TopoDS_Shape> MyMapType;

/** 
 * @class CSGScanner
 * Scanner for the csg2d input file.
 */
class CSGScanner
	{
		TOKEN_TYPE token; /**< The current token scanned. */
		PRIMITIVE_TYPE prim_token; /**< The last encountered primitive. */
		double num_value; 
		string string_value; 
		int linenum; /**< The current line number. */
		istream * scanin; /**< The name of the input file. */

		MyMapType * shapes; /**< All the pairs of shapes defined in the input file
														along with their names. */
		int NTopLevelObjects; /**< The number of top-level objects. */

		Quantity_Color * colorFlags; /**< Colors for all the top-level objects. */
		bool * colorValid; /**< Flags for valid definition of color for a certain
														top-level object. True, if a color is defined. */

		TopoDS_Compound geometry; /**< The final shape composed of all the TLOs. */
		BRep_Builder builder; /**< It builds the compound shape. */
		BRepOffsetAPI_Sewing sewer; /**< It assembles contigous shapes into one shape. */

  public:

		/**
 		 * A constructor.
 		 * Initializes the class members.
 		 * @param ascanin A istream & argument.
 		 */ 
		CSGScanner (istream & ascanin);

		/**  
 		 * Getter for the token type.
 		 * @return the token.
 		 * @see CSGScanner()
 		 */
		TOKEN_TYPE GetToken() const
		{ return token; }

		/**  
 		 * Getter for the number_value.
 		 * @return this number value.
 		 * @see CSGScanner()
 		 */
		double GetNumValue() const
		{ return num_value; }

		/**  
 		 * Getter for the string_value.
 		 * @return this string value.
 		 * @see CSGScanner()
 		 */
		const string & GetStringValue() const
		{ return string_value; }

		/**  
 		 * Getter for the first character from the string_value.
 		 * @return this character.
 		 * @see CSGScanner()
  	 */
		char GetCharValue() const
		{ return string_value[0]; }

		/**  
 		 * Getter for the prim_token.
 		 * @return this primitive token.
 		 * @see CSGScanner()
  	 */
		PRIMITIVE_TYPE GetPrimitiveToken() const
		{ return prim_token; }

		/**  
 		 * Getter for all the shapes defined.
 		 * @return these shapes.
 		 */
		MyMapType * GetMyMapOfShapes()
		{ return shapes; }

		/**  
 		 * Getter for the colors of the top-level objects.
 		 * @returns these colors.
 		 * @see CSGScanner()
  	 */
		Quantity_Color * GetColorFlags()
		{ return colorFlags; }

		/**
 		 * Setter for the colors of the top-level objects.
 		 * @param array Quantity_Color * argument.
 		 */
		void SetColorFlags (Quantity_Color * array)
		{
			free(colorFlags);
			colorFlags = array;
		}

		/**  
 		 * Getter for the validity flags of the colors.
 		 * @return these boolean flags.
 		 * @see CSGScanner()
 		 */
		bool * GetColorValid()
		{ return colorValid; }

		/**  
 		 * Setter for the validity flags of the colors.
 		 * @param valid A bool * argument.
 		 */
		void SetColorValid (bool * valid)
		{
			if (colorValid)
				free(colorValid);
			colorValid = valid;
		}

		/**  
 		 * Read the next scannable element from the input file.
 		 */
		void ReadNext();

		/**  
 		 * The csg2d file parser.
 		 * This is the main function which scans the whole file and acts according
 		 * the information read.
 		 * @param istr A istream & argument.
 		 */
		TopoDS_Shape ParseCSG(istream & istr);

		/**
 		 * Prints an error with the message err.
 		 * It makes use of the linenum variable in order to specify the line number
 		 * in which the error has occurred.
 		 * @param err A string & argument.
 		 */
		void Error (const string & err);
	};

	/**  
 	 * Overloaded >> operator.
 	 * Reads a character.
 	 */
	CSGScanner & operator>> (CSGScanner & scan, char ch);

	/**  
 	 * Overloaded >> operator.
 	 * Reads a double.
 	 */
	CSGScanner & operator>> (CSGScanner & scan, double & d);

	/**  
 	 * Overloaded >> operator.
	 * Reads an integer.
 	 */
	CSGScanner & operator>> (CSGScanner & scan, int & i);

#endif

