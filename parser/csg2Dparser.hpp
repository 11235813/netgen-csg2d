#include "planefigure.hpp"
#include "planefigure.cpp"
#include "polygon.cpp"
#include "circle.cpp"
#include "ellipse.cpp"

#include <TopExp_Explorer.hxx>
#include <TopoDS_Face.hxx>
#include <TopAbs_ShapeEnum.hxx>
//#include <TopoDS.hxx>
#include <BRepAlgoAPI_Fuse.hxx>
#include <BRepAlgoAPI_Common.hxx>
#include <TopoDS_ListIteratorOfListOfShape.hxx>
#include <TopoDS_ListOfShape.hxx>

#include <TopoDS_Shape.hxx>
#include <TopoDS_Wire.hxx>
#include <TopoDS_Compound.hxx>

#include <TColgp_Array1OfPnt.hxx>

#include <BRepAlgo_Cut.hxx>
#include <BRepAlgo_Fuse.hxx>
#include <BRepAlgo_Common.hxx>
#include <BRepAlgo_BooleanOperations.hxx>
//#include <BRepBuilderAPI_FindPlane.hxx>
#include <BRep_Builder.hxx>
#include <BRepOffsetAPI_Sewing.hxx>

//#include <Quantity_Array1OfColor.hxx>
#include <Quantity_Color.hxx>
//#include <Quantity_TypeOfColor.hxx>
//#include <Quantity_NameOfColor.hxx>
//#include <XCAFDoc_ColorTool.hxx>
//#include <XCAFDoc_ColorType.hxx>
//#include <TDF_Label.hxx>

#include <gp_Vec.hxx>
#include <gp_Trsf.hxx>
//#include <TopLoc_Location.hxx>

#include <string>
#include <map>
#include <utility>
#include <math.h>

#include <myadt.hpp>
using namespace netgen;
using namespace std;

#ifndef _CSG2DPARSER_HPP
#define _CSG2DPARSER_HPP

enum TOKEN_TYPE
	{
		TOK_MINUS = '-', TOK_LP = '(', OK_RP = ')', TOK_LSP = '[', TOK_RSP = ']',
		TOK_EQU = '=', TOK_COMMA = ',', TOK_SEMICOLON = ';',
		TOK_NUM = 100, TOK_STRING, TOK_PRIMITIVE,
		TOK_OR, TOK_AND, TOK_NOT,
		/*   TOK_SINGULAR, TOK_EDGE, TOK_POINT, TOK_IDENTIFY,
		*/
		TOK_PLANEFIGURE,
		TOK_RECO2, TOK_TLO,/* TOK_BOUNDINGBOX,
		TOK_BOUNDARYCONDITION, TOK_BOUNDARYCONDITIONNAME,
		TOK_CONSTANT,
		*/
		TOK_END };

struct kwstruct
	{
		TOKEN_TYPE kw;
		const char * name;
  };

enum PRIMITIVE_TYPE
	{
		TOK_POLYGON = 1,
		TOK_CIRCLE,
		TOK_ELLIPSE,
		TOK_TRANSLATE
	};

struct primstruct
	{
		PRIMITIVE_TYPE kw;
		const char * name;
	};

typedef map<string, TopoDS_Shape> MyMapType;

class CSGScanner
	{
		TOKEN_TYPE token;
		PRIMITIVE_TYPE prim_token;
		double num_value;
		string string_value;

		int linenum;
		istream * scanin;

		MyMapType * shapes;

		int NTopLevelObjects;

		Quantity_Color * colorFlags;
		bool * colorValid;

  public:

		CSGScanner (istream & ascanin);

		TOKEN_TYPE GetToken() const
		{ return token; }

		double GetNumValue() const
		{ return num_value; }

		const string & GetStringValue() const
		{ return string_value; }

		char GetCharValue() const
		{ return string_value[0]; }

		PRIMITIVE_TYPE GetPrimitiveToken() const
		{ return prim_token; }

		MyMapType * GetMyMapOfShapes()
		{ return shapes; }

		Quantity_Color * GetColorFlags()
		{ return colorFlags; }

		void SetColorFlags (Quantity_Color * array)
		{
			free(colorFlags);
			colorFlags = array;
		}

		bool * GetColorValid()
		{ return colorValid; }

		void SetColorValid (bool * valid)
		{
			if (colorValid)
				free(colorValid);
			colorValid = valid;
		}

		void ReadNext();

		TopoDS_Shape ParseCSG(istream & istr);
		/*
		CSGScanner & Parse (char ch);
		CSGScanner & Parse (int & i);
		CSGScanner & Parse (double & d);
		CSGScanner & Parse (Point<3> & p);
		CSGScanner & Parse (Vec<3> & p);
		*/
		void Error (const string & err);
	};


	CSGScanner & operator>> (CSGScanner & scan, char ch);
	CSGScanner & operator>> (CSGScanner & scan, double & d);
	CSGScanner & operator>> (CSGScanner & scan, int & i);
	//CSGScanner & operator>> (CSGScanner & scan, Point<3> & p);

#endif

