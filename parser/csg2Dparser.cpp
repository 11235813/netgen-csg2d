/******************************************************************************/
/* File:    csg2Dparser.cpp                                                   */
/* Author:  Cristina Precup, cpcristinaprecup@gmail.com                       */
/* Date:    20/06/2011                                                        */
/******************************************************************************/

/**
 * @file csg2Dparser.cpp
 * The parser for the csg2d input files.
 */

#include "csg2Dparser.hpp"

static kwstruct defkw[] =
	{
		{ TOK_RECO2,				"algebraic2d" },
		{ TOK_PLANEFIGURE,	"planefigure" },
		{ TOK_TLO,					"tlo" },
		{ TOK_OR,						"or" },
		{ TOK_AND,					"and" },
		{ TOK_NOT,					"not" },
		{ TOKEN_TYPE(0), 0 }
	};

static primstruct defprim[] =
	{
		{ TOK_POLYGON,		"polygon" },
		{ TOK_CIRCLE,			"circle" },
		{ TOK_ELLIPSE,		"ellipse" },
		{ TOK_TRANSLATE,	"translate" },
		{ PRIMITIVE_TYPE(0), 0 }
	};

TopoDS_Shape geom;
TopoDS_Compound geometry;
BRep_Builder abuilder;
BRepOffsetAPI_Sewing sewer;

CSGScanner :: CSGScanner (istream & ascanin)
{
	scanin = &ascanin;
	token = TOK_END;
	num_value = 0;
	linenum = 1;
	shapes = new MyMapType();
	NTopLevelObjects = 0;
	colorFlags = NULL;
	colorValid = NULL;
}

void CSGScanner :: ReadNext ()
{
	char ch;

	// scan whitespaces
	do
	{
		scanin->get (ch);

		// end of file reached
		if (scanin->eof())
		{
			token = TOK_END;
			return;
		}
		if (ch == '\n')
			linenum++;

		// skip comment line
		if (ch == '#')
		{
			while (ch != '\n')
			{
				scanin->get (ch);
				if (scanin->eof())
				{
					token = TOK_END;
					return;
				}
			}
			linenum++;
		}
	}
	while (isspace (ch));

	switch (ch)
	{
		case '(': case ')':
		case '[': case ']':
		case '-':
		case '=': case ',': case ';':
		{
			token = TOKEN_TYPE (ch);
			break;
		}

		default:
		{
			if (isdigit (ch) || ch == '.')
			{
				scanin->putback (ch);
				(*scanin) >> num_value;
				token = TOK_NUM;
				return;
			}

			if (isalpha (ch))
			{
				string_value = string (1, ch);
				scanin->get (ch);
				while (isalnum(ch) || ch == '_')
				{
					string_value += ch;
					scanin->get (ch);
				}
				scanin->putback (ch);
			}

			int nr = 0;
			while (defkw[nr].kw)
			{ 
				if (string_value == defkw[nr].name)
				{
					token = defkw[nr].kw;
					return;
				}
				nr++;
			}

			nr = 0;
			while (defprim[nr].kw)
			{
				if (string_value == defprim[nr].name)
				{
					token = TOK_PRIMITIVE;
					prim_token = defprim[nr].kw;
					return;
				}
				nr++;
			}

			token = TOK_STRING;
		}
	}
}

void CSGScanner :: Error (const string & err)
{
	stringstream errstr;
	errstr << "Parsing error in line " << linenum << ": " << endl << err << endl;
	throw string(errstr.str());
}

void ParseChar (CSGScanner & scan, char ch)
{
	if (scan.GetToken() != TOKEN_TYPE(ch))
	scan.Error (string ("token '") + string (1, ch) + string("' expected"));
	scan.ReadNext();
}

double ParseNumber (CSGScanner & scan)
{
	if (scan.GetToken() == '-')
	{
		scan.ReadNext();
		return -ParseNumber (scan);
	}
	if (scan.GetToken() != TOK_NUM) 
		scan.Error ("number expected");
	double val = scan.GetNumValue();
	scan.ReadNext();
	return val;
}

gp_Vec ParseVector (CSGScanner & scan)
{
	double x, y, z;
	x = ParseNumber (scan);
	ParseChar(scan, ',');
	y = ParseNumber (scan);
	z = 0;
	gp_Vec v(x, y, z);
	return v;	
}  

CSGScanner & operator>> (CSGScanner & scan, char ch)
{
	if (scan.GetToken() != TOKEN_TYPE(ch))
		scan.Error (string ("token '") + string (1, ch) + string("' expected"));
	scan.ReadNext();
	return scan;
}

CSGScanner & operator>> (CSGScanner & scan, double & d)
{
	d = ParseNumber (scan);
	return scan;
}

CSGScanner & operator>> (CSGScanner & scan, int & i)
{
	i = int (ParseNumber (scan));
	return scan;
}

PlaneFigure * ParsePlaneFigure (CSGScanner & scan);
PlaneFigure * ParseTerm (CSGScanner & scan);
PlaneFigure * ParsePrimary (CSGScanner & scan);

PlaneFigure * ParsePrimary (CSGScanner & scan)
{
	if (scan.GetToken() == TOK_PRIMITIVE)
		switch (scan.GetPrimitiveToken())
		{

			case TOK_POLYGON:
			{
				scan.ReadNext();
				int no, i;
				double x, y;
				gp_Pnt P;
				scan >> '(' >> no;
				if( no > 1)
        {
					i = 0;
					TColgp_Array1OfPnt *aarray = new TColgp_Array1OfPnt (1, no);
					while (i < no)
					{  
						i ++;
						scan >>';' >> x >> ',' >> y;
						P.SetCoord (x, y, 0);
						aarray->SetValue (i, P);
					}
					scan >> ')';
					Polygon  *apolygon = new Polygon (aarray);
					TopoDS_Shape nprim = apolygon->MakePolygon ();
					delete apolygon;
					apolygon = NULL;
					PlaneFigure *pf = new PlaneFigure (nprim);
					return pf;
				}
				else
				{
					scan.Error ("Not enough points for defining a polygon.");
					return NULL; 
				}
			}

			case TOK_CIRCLE:
			{
				double x, y, radius;
				scan.ReadNext();
				scan >> '(' >> x >> ',' >> y >> ';' >> radius >> ')';
				gp_Pnt center(x, y, 0);

				Circle *acircle = new Circle (center, radius);
				TopoDS_Shape nprim = acircle->MakeCircle();
				delete acircle;
				acircle = NULL;
				PlaneFigure *pf = new PlaneFigure (nprim);
				return pf;
			}

			case TOK_ELLIPSE:
			{
				double x, y, majorRadius, minorRadius;
				scan.ReadNext();
				scan >> '(' >> x >> ',' >> y >> ';' >> majorRadius >> ';' >> minorRadius >> ')';
				gp_Pnt center (x, y, 0);

				Ellipse *aellipse = new Ellipse (center, majorRadius, minorRadius);
				TopoDS_Shape nprim = aellipse->MakeEllipse();
				delete aellipse;
				aellipse = NULL;
				PlaneFigure *pf = new PlaneFigure (nprim);
				return pf;
			}

			case TOK_TRANSLATE:
			{
				scan.ReadNext();
				ParseChar(scan, '(');
				gp_Vec vecLocation = ParseVector (scan);

				ParseChar(scan, ';');
				PlaneFigure * pf = ParsePlaneFigure (scan);
				ParseChar(scan, ')');
				gp_Trsf transformation;
				transformation.SetTranslation (vecLocation);
				TopLoc_Location location (transformation);
				
				TopoDS_Shape translatedShape = pf->GetPrimitive();
				translatedShape.Location (location);
				PlaneFigure * pf2 = new PlaneFigure (translatedShape);
				return pf2;
			}

			default:
			{
				scan.Error (string ("unknown primary ") + scan.GetStringValue());
			}
		}

	else if(scan.GetToken() == TOK_STRING && scan.GetMyMapOfShapes()->find (scan.GetStringValue()) != scan.GetMyMapOfShapes()->end())
	{
		PlaneFigure * pf = const_cast<PlaneFigure*> (new PlaneFigure(scan.GetMyMapOfShapes()->find (scan.GetStringValue())->second)); //take the value that is identified by the key
		scan.ReadNext();
		return pf;
	}

	else if (scan.GetToken() == TOK_NOT)
	{
		scan.ReadNext();
		PlaneFigure * pf2 = ParsePrimary (scan);
		TopoDS_Shape temp = pf2->GetPrimitive();
		pf2 = new PlaneFigure(pf2->GetPrimitive(), true);
		return pf2;
	}

	else if (scan.GetToken() == '(')
	{
		scan.ReadNext();
		PlaneFigure * pf1 = ParsePlaneFigure (scan);
		scan.ReadNext();
		return pf1;
	}

	scan.Error (string ("not a primary, name = ")+
	scan.GetStringValue());
	return 0;
}

PlaneFigure * ParseTerm (CSGScanner & scan)
{
	PlaneFigure * pf = ParsePrimary (scan);
	while (scan.GetToken() == TOK_AND)
	{
		scan.ReadNext();
		PlaneFigure * pf2 = ParsePrimary (scan);
		TopoDS_Shape temp;

		//check for NOT operation
		if (pf->Not_Op() == true && pf2->Not_Op() == false) //not pf and pf2
			temp = BRepAlgo_Cut (pf2->GetPrimitive(), pf->GetPrimitive());
		else if (pf2->Not_Op() == true && pf->Not_Op() == false) // pf and not pf2
					temp = BRepAlgo_Cut (pf->GetPrimitive(), pf2->GetPrimitive());
		else if (pf->Not_Op() == true && pf2->Not_Op() == true) //not pf and not pf2
			temp = BRepAlgo_Fuse (pf->GetPrimitive(), pf2->GetPrimitive());
		else //pf and pf2
			temp = BRepAlgo_Common (pf->GetPrimitive(), pf2->GetPrimitive());

		pf = new PlaneFigure (temp);
	}

	return pf;
}

PlaneFigure * ParsePlaneFigure (CSGScanner & scan)
{
	PlaneFigure * pf = ParseTerm (scan);
	while (scan.GetToken() == TOK_OR)
	{
		scan.ReadNext();
		PlaneFigure * pf2 = ParseTerm (scan);
		TopoDS_Shape temp;

		//check for NOT operations
		if (pf->Not_Op() == true && pf2->Not_Op() == false) //not pf or pf2
			temp = BRepAlgo_Cut (pf->GetPrimitive(), pf2->GetPrimitive());
		else if (pf2->Not_Op() == true && pf->Not_Op() == false) // pf or not pf2
			temp = BRepAlgo_Cut (pf2->GetPrimitive(), pf->GetPrimitive());
		else if (pf->Not_Op() == true && pf2->Not_Op() == true) //not pf or not pf2
			temp = BRepAlgo_Common (pf->GetPrimitive(), pf2->GetPrimitive());
		else //pf or pf2
			temp = BRepAlgo_Fuse (pf->GetPrimitive(), pf2->GetPrimitive());

		pf = new PlaneFigure (temp);
	}

	return pf;
}

void ParseFlags (CSGScanner & scan, Flags & flags)
{
	while (scan.GetToken() == '-')
	{
		scan.ReadNext();
		string name = scan.GetStringValue();
		scan.ReadNext();
		if (scan.GetToken() == '=')
		{
			scan.ReadNext();
			if (scan.GetToken() == TOK_STRING)
			{
				flags.SetFlag (name.c_str(), scan.GetStringValue().c_str());
				scan.ReadNext();
			}
			else if (scan.GetToken() == '[')
			{
				scan.ReadNext();
				if (scan.GetToken() == '-' || scan.GetToken() == TOK_NUM)
				{
					Array<double> vals;
					vals.Append (ParseNumber(scan));
					while (scan.GetToken() == ',')
					{
						scan.ReadNext();
						vals.Append (ParseNumber(scan));
					}
					ParseChar (scan, ']');
					flags.SetFlag (name.c_str(), vals);
				}
				else
				{ //string list
					Array<char*> vals;
					string val = scan.GetStringValue();
					vals.Append (new char[val.size() + 1]);
					strcpy (vals.Last(), val.c_str());
					scan.ReadNext();
					
					while (scan.GetToken() == ',')
					{
						scan.ReadNext();
						val = scan.GetStringValue();
						vals.Append (new char[val.size() + 1]);
						strcpy (vals.Last(), val.c_str());
						scan.ReadNext();
					}
					ParseChar (scan, ']');
					flags.SetFlag (name.c_str(), vals);
					for (int i = 0; i < vals.Size(); i ++)
						delete [] vals[i];
				}
			}
			else if (scan.GetToken() == TOK_NUM)
			{
				flags.SetFlag (name.c_str(), scan.GetNumValue());
				scan.ReadNext();
			}
		}
		else
		{
			flags.SetFlag (name.c_str());
		}
	}
}

/* 
 *	Main parsing function for CSG geometry
 */
TopoDS_Shape CSGScanner :: ParseCSG (istream & istr)
{
	CSGScanner scan (istr);
	abuilder.MakeCompound (geometry);
	//retain the color of each TLO. If the color is not set, set it to the default GREEN
	Quantity_Color * colorFlags_aux = NULL;
	TopoDS_ListOfShape listOfShapes;
	vector<string> tlo_names;

	scan.ReadNext();
	try
	{
		//check for the 'algebraic2d' keyword
		if (scan.GetToken() != TOK_RECO2)
			scan.Error ("geo-file should start with algebraic2d");
	}
	catch (string errstr)
	{
		throw NgException (errstr);
		return TopoDS_Shape();
	}

	scan.ReadNext();

	try
	{
 		while (1)
 		{
	 		if (scan.GetToken() == TOK_END) break;

	 		if (scan.GetToken() == TOK_PLANEFIGURE)
	 		{
 				scan.ReadNext();
 				if (scan.GetToken() != TOK_STRING)
	 				scan.Error ("name identifier expected");
 				string planefigurename = scan.GetStringValue();
 				scan.ReadNext();

 				ParseChar (scan, '=');
 				PlaneFigure * pf = ParsePlaneFigure (scan);
				geom = pf->GetPrimitive();
				scan.GetMyMapOfShapes()->insert (make_pair (planefigurename, pf->GetPrimitive()));
 				ParseChar (scan, ';');

 				cout << "define planefigure" << " " << planefigurename << endl;
			}

			else if (scan.GetToken() == TOK_TLO)
			{ 
				// a TopLevelObject definition
				scan.ReadNext();

				string name = scan.GetStringValue();
				scan.ReadNext();

				if (scan.GetToken() != TOK_STRING)
				{ 
					// a planefigure TLO

					Flags flags;
					ParseFlags (scan, flags);

					ParseChar (scan, ';');

					if (scan.GetMyMapOfShapes()->find (name) == scan.GetMyMapOfShapes()->end())
						scan.Error ("Top-Level-Object "+name+" not defined");

					//add the object to the compound shape with all the top level objects
					sewer.Add (scan.GetMyMapOfShapes()->find (name)->second);

					listOfShapes.Prepend (scan.GetMyMapOfShapes()->find (name)->second);
					tlo_names.push_back(name);

					NTopLevelObjects++;
					colorFlags_aux = (Quantity_Color*) realloc (colorFlags_aux, NTopLevelObjects * sizeof (Quantity_Color));

 					//set the color flags 
					if (flags.NumListFlagDefined ("col"))
					{
						const Array<double> & col = flags.GetNumListFlag ("col");
						Quantity_Color color = Quantity_Color (col[0], col[1], col[2], Quantity_TOC_RGB);
						colorFlags_aux[NTopLevelObjects - 1] = colorFlags_aux[NTopLevelObjects - 1].Assign (color);
					}
					else
						colorFlags_aux[NTopLevelObjects - 1] = colorFlags_aux[NTopLevelObjects - 1].Assign (Quantity_Color (0.0, 1.0, 0.0, Quantity_TOC_RGB));
				}
			}

			else
			{
				cout << "read unidentified token " << scan.GetToken()
		 				<< " (as char: \"" << char(scan.GetToken()) << "\")"
		 				<< " string = " << scan.GetStringValue() << endl;
				scan.ReadNext();
			}
		}
	}
	catch (string errstr)
	{
		throw NgException (errstr);
	}


//    (*testout) << geom->GetNTopLevelObjects() << " TLOs:" << endl;
	cout << NTopLevelObjects << " TLOs: " << endl;
	for (int i = 0; i < NTopLevelObjects; i ++)
		cout<<tlo_names[i]<<endl;

	if (NTopLevelObjects > 0)
	{ 
		sewer.Perform();
		bool first = true;
		TopoDS_Shape my_fuse;
		for (TopExp_Explorer exp0 (sewer.SewedShape(), TopAbs_FACE); exp0.More(); exp0.Next())
		{
			if (first)
			{
				my_fuse = exp0.Current();
				first = false;
			}
			else
				my_fuse = BRepAlgoAPI_Fuse (my_fuse, exp0.Current());
		}
		abuilder.Add (geometry, my_fuse);
		
		int NComponents = 0;
		for (TopExp_Explorer exp0 (geometry, TopAbs_FACE); exp0.More(); exp0.Next())
		{
			NComponents++;
			bool has_common = false;
			int ntlo = 0;
			for (TopoDS_ListIteratorOfListOfShape it (listOfShapes); it.More() && (has_common == false); it.Next())
			{
				ntlo ++;
					
				TopoDS_Shape common;
				common = BRepAlgoAPI_Common (exp0.Current(), it.Value());
				TopExp_Explorer exp2 (common, TopAbs_FACE, TopAbs_EDGE);
					
				if (exp2.More())
				{
					has_common = true;
					colorValid = (bool*) realloc (colorValid, NComponents * sizeof (bool));
					colorFlags = (Quantity_Color*) realloc (colorFlags, NComponents * sizeof (Quantity_Color));
					GetColorFlags()[NComponents - 1] = GetColorFlags()[NComponents - 1].Assign (colorFlags_aux[NTopLevelObjects - ntlo]);
					if (colorFlags_aux[NTopLevelObjects - ntlo].IsDifferent (Quantity_Color (0.0, 1.0, 0.0, Quantity_TOC_RGB)))
						GetColorValid()[NComponents - 1] = true;
					else
						GetColorValid()[NComponents - 1] = false;
				}
			}
		}
		
		return geometry;
	 }
	 return TopoDS_Shape();
}

