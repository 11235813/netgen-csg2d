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

/*
 * It associates a name with a certain token keyword.
 */
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

/*
 * It associates a name with a certain type of primitive.
 */
static primstruct defprim[] =
	{
		{ TOK_POLYGON,		"polygon" },
		{ TOK_CIRCLE,			"circle" },
		{ TOK_ELLIPSE,		"ellipse" },
		{ TOK_TRANSLATE,	"translate" },
		{ PRIMITIVE_TYPE(0), 0 }
	};

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

/* 
 * Read the next scannable element from the input file.
 */
void CSGScanner :: ReadNext ()
{
	char ch;

	/* scan whitespaces */
	do
	{
		scanin->get (ch);

		if (scanin->eof()) /* End of file reached. */
		{
			token = TOK_END;
			return;
		}
		if (ch == '\n')
			linenum++;

		if (ch == '#') /* Skip comment line. */
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
			if (isdigit (ch) || ch == '.') /* Found a number. */
			{
				scanin->putback (ch);
				(*scanin) >> num_value;
				token = TOK_NUM;
				return;
			}

			if (isalpha (ch)) /* Found a name of something. */
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
				if (string_value == defkw[nr].name) /* Found a keyword. */
				{
					token = defkw[nr].kw;
					return;
				}
				nr++;
			}

			nr = 0;
			while (defprim[nr].kw)
			{
				if (string_value == defprim[nr].name) /* Found a type of primitive. */
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

/**  
 * Character parser.
 * Compares the possible tokens with the current one. If they are different, 
 * it prints out an error.
 * Further on, it reads the next token.
 * @param scan A CSGScanner & argument.
 * @param ch A char argument.
 * @see CSGScanner::GetToken()
 * @see TOKEN_TYPE
 * @see CSGScanner::Error()
 * @see CSGScanner::ReadNext()
 */
void ParseChar (CSGScanner & scan, char ch)
{
	if (scan.GetToken() != TOKEN_TYPE(ch))
		scan.Error (string ("token '") + string (1, ch) + string("' expected"));
	scan.ReadNext();
}

/**  
 * Number parser.
 * If the current input is not a number, it prints out an error.
 * Further on, it reads the next token.
 * @param scan A CSGScanner & argument
 * @see CSGScanner::GetToken()
 * @see CSGScanner::Error()
 * @see CSGScanner::ReadNext()
 */
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

/**  
 * Vector parser.
 * It parses the x and y values.
 * @param scan A CSGScanner & argument.
 * @return a 3D vector.
 * @see ParseNumber()
 */
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

/**  
 * It reads a character.
 * It prints out an error if the current character is not the expected one.
 * @param scan A CSGScanner & argument.
 * @param ch A char argument.
 * @return the scanner.
 * @see CSGScanner::GetToken()
 * @see TOKEN_TYPE
 * @see CSGScanner::Error()
 * @see CSGScanner::ReadNext()
 */
CSGScanner & operator>> (CSGScanner & scan, char ch)
{
	if (scan.GetToken() != TOKEN_TYPE(ch))
		scan.Error (string ("token '") + string (1, ch) + string("' expected"));
	scan.ReadNext();
	return scan;
}

/**  
 * It reads a double precision number.
 * It makes use of the ParseNumber() method.
 * @param scan A CSGScanner & argument.
 * @param d A double & argument.
 * @return the scanner.
 * @see ParseNumber()
 */
CSGScanner & operator>> (CSGScanner & scan, double & d)
{
	d = ParseNumber (scan);
	return scan;
}

/**  
 * It reads an integer number.
 * It makes use of the ParseNumber() method.
 * @param scan A CSGScanner & argument.
 * @param i An int & argument.
 * @return the scanner.
 * @see ParseNumber()
 */
CSGScanner & operator>> (CSGScanner & scan, int & i)
{
	i = int (ParseNumber (scan));
	return scan;
}

PlaneFigure * ParsePlaneFigure (CSGScanner & scan);
PlaneFigure * ParseTerm (CSGScanner & scan);
PlaneFigure * ParsePrimary (CSGScanner & scan);

/**  
 * Primitive parser.
 * This method is called wheneven a primitive is detected.
 * It checks to see which one of these primitives should be parsed:
 * 	- polygon
 * 	- circle
 * 	- ellipse
 * 	- translated primitive
 * Afterwards, it parses the data referring to this primitive.
 * @param scan A CSGScanner & argument.
 * @return the PlaneFigure which is defined.
 * @see CSGScanner::GetToken()
 * @see TOK_PRIMITIVE
 * @see CSGScanner::GetPrimitiveToken()
 * @see CSGScanner::GetStringValue()
 * @see CSGScanner::GetMyMapOfShapes()
 * @see CSGScanner::ReadNext()
 * @see CSGScanner::ParsePlaneFigure()
 * @see CSGScanner::ParseChar()
 * @see CSGScanner::ParseVector()
 * @see CSGScanner::Error()
 * @see Polygon::MakePolygon()
 * @see Circle::MakeCircle()
 * @see Ellipse::MakeEllipse()
 * @see PlaneFigure::PlaneFigure()
 * @see PlaneFigure::GetPrimitive()
 */
PlaneFigure * ParsePrimary (CSGScanner & scan)
{
	if (scan.GetToken() == TOK_PRIMITIVE) /* Found a primitive. */
		switch (scan.GetPrimitiveToken())
		{

			case TOK_POLYGON:
			{
				scan.ReadNext();
				int no; /* The number of vertices that determine the polygon. */
				int i;
				double x, y; /* The coordinates for one of the polygon's vertices. */
				gp_Pnt P;
				scan >> '(' >> no;
				if( no > 1)
        {
					i = 0;

					/* Set up a collection of points. */
					TColgp_Array1OfPnt *aarray = new TColgp_Array1OfPnt (1, no);
					while (i < no)
					{  
						i ++;
						scan >>';' >> x >> ',' >> y;
						P.SetCoord (x, y, 0);

						/* Add the point to the collection. */
						aarray->SetValue (i, P);
					}
					scan >> ')';
					Polygon  *apolygon = new Polygon (aarray);
					TopoDS_Shape nprim = apolygon->MakePolygon ();
					delete apolygon;
					apolygon = NULL;

					/* Define a PlaneFigure having the polygon shape. */
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
				double x, y, radius; /* Will contain the input data for the circle.
															* The x,y coordinates for the center point and
															* the	radius.*/
				scan.ReadNext();
				scan >> '(' >> x >> ',' >> y >> ';' >> radius >> ')';
				gp_Pnt center(x, y, 0);

				Circle *acircle = new Circle (center, radius);
				TopoDS_Shape nprim = acircle->MakeCircle();
				delete acircle;
				acircle = NULL;

				/* Define a PlaneFigure having the circle shape. */
				PlaneFigure *pf = new PlaneFigure (nprim);
				return pf;
			}

			case TOK_ELLIPSE:
			{
				double x, y, majorRadius, minorRadius; /* Will contain the input data 
																								* for	the ellipse. The x,y
																								* coordinates	for the center
																								* point	and the major and minor
																								*  radii.*/

				scan.ReadNext();
				scan >> '(' >> x >> ',' >> y >> ';' >> majorRadius >> ';' >>
						minorRadius >> ')';
				gp_Pnt center (x, y, 0);

				Ellipse *aellipse = new Ellipse (center, majorRadius, minorRadius);
				TopoDS_Shape nprim = aellipse->MakeEllipse();
				delete aellipse;
				aellipse = NULL;

				/* Define a PlaneFigure having the ellipse shape. */
				PlaneFigure *pf = new PlaneFigure (nprim);
				return pf;
			}

			case TOK_TRANSLATE:
			{
				scan.ReadNext();
				ParseChar(scan, '(');

				/* Set up the vector used to translate the planefigure. */
				gp_Vec vecLocation = ParseVector (scan);

				ParseChar(scan, ';');

				/* Define the PlaneFigure that is to be translated. */
				PlaneFigure * pf = ParsePlaneFigure (scan);
				ParseChar(scan, ')');

				/* Set up a transformation for translating a shape with the input vector. */
				gp_Trsf transformation;
				transformation.SetTranslation (vecLocation);
				TopLoc_Location location (transformation);
				
				TopoDS_Shape translatedShape = pf->GetPrimitive();

				/* Translate the shape. */
				translatedShape.Location (location);

				/* Define a PlaneFigure having the translated shape. */
				PlaneFigure * pf2 = new PlaneFigure (translatedShape);
				return pf2;
			}

			default:
			{
				scan.Error (string ("unknown primary ") + scan.GetStringValue());
			}
		}

	else if(scan.GetToken() == TOK_STRING &&
				scan.GetMyMapOfShapes()->find (scan.GetStringValue()) !=
				scan.GetMyMapOfShapes()->end())
	{
		/* Define a PlaneFigure having the shape of an already defined PlaneFigure
 		 * which is called scan.GetStringValue(). */
		PlaneFigure * pf = const_cast<PlaneFigure*> (new PlaneFigure
											(scan.GetMyMapOfShapes()->find
											(scan.GetStringValue())->second));
		scan.ReadNext();
		return pf;
	}

	/* Detect the NOT boolean operator. */
	else if (scan.GetToken() == TOK_NOT)
	{
		scan.ReadNext();

		/* Get the input data for the shape on which the NOT operator is applied. */
		PlaneFigure * pf2 = ParsePrimary (scan);
		TopoDS_Shape temp = pf2->GetPrimitive();

		/* Define a PlaneFigure having the shape as the 'negated' shape. */
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

/**  
 * Parse the current term.
 * This method calls the ParsePrimary method.
 * It checks to see if there is an AND boolean operator for two planefigures.
 * If yes, the corresponding boolean operation is applied on both shapes.
 * @param scan A CSGScanner & argument.
 * @return the resulted planefigure.
 * @see CSGScanner::GetToken()
 * @see TOKEN_TYPE
 * @see CSGScanner::ReadNext()
 * @see ParsePrimary()
 * @see PlaneFigure::PlaneFigure()
 * @see PlaneFigure::GetPrimitive()
 * @see PlaneFigure::Not_Op()
 */
PlaneFigure * ParseTerm (CSGScanner & scan)
{
	PlaneFigure * pf = ParsePrimary (scan);
	while (scan.GetToken() == TOK_AND)
	{
		scan.ReadNext();
		PlaneFigure * pf2 = ParsePrimary (scan);
		TopoDS_Shape temp;

		/* check for NOT operator */
		if (pf->Not_Op() == true && pf2->Not_Op() == false) /* not pf and pf2 */
			temp = BRepAlgo_Cut (pf2->GetPrimitive(), pf->GetPrimitive());
		else if (pf2->Not_Op() == true && pf->Not_Op() == false) /* pf and not pf2 */
					temp = BRepAlgo_Cut (pf->GetPrimitive(), pf2->GetPrimitive());
		else if (pf->Not_Op() == true && pf2->Not_Op() == true) /* not pf and not pf2 */
			temp = BRepAlgo_Fuse (pf->GetPrimitive(), pf2->GetPrimitive());
		else /* pf and pf2 */
			temp = BRepAlgo_Common (pf->GetPrimitive(), pf2->GetPrimitive());

		/* Define a PlaneFigure resulted from the boolean operation applied on
 		 * on the two shapes. */
		pf = new PlaneFigure (temp);
	}

	return pf;
}

/**  
 * Parse a planefigure.
 * This method calls the ParseTerm method.
 * It checks to see if there is an OR boolean operator for two planefigures.
 * If yes, the corresponding boolean operation is applied on both shapes.
 * @param scan A CSGScanner & argument.
 * @return the resulted planefigure.
 * @see CSGScanner::GetToken()
 * @see TOKEN_TYPE
 * @see CSGScanner::ReadNext()
 * @see ParseTerm()
 * @see PlaneFigure::PlaneFigure()
 * @see PlaneFigure::GetPrimitive()
 * @see PlaneFigure::Not_Op()
 */
PlaneFigure * ParsePlaneFigure (CSGScanner & scan)
{
	PlaneFigure * pf = ParseTerm (scan);
	while (scan.GetToken() == TOK_OR)
	{
		scan.ReadNext();
		PlaneFigure * pf2 = ParseTerm (scan);
		TopoDS_Shape temp;

		/* check for NOT operator */
		if (pf->Not_Op() == true && pf2->Not_Op() == false) /* not pf or pf2 */
			temp = BRepAlgo_Cut (pf->GetPrimitive(), pf2->GetPrimitive());
		else if (pf2->Not_Op() == true && pf->Not_Op() == false) /* pf or not pf2 */
			temp = BRepAlgo_Cut (pf2->GetPrimitive(), pf->GetPrimitive());
		else if (pf->Not_Op() == true && pf2->Not_Op() == true) /* not pf or not pf2 */
			temp = BRepAlgo_Common (pf->GetPrimitive(), pf2->GetPrimitive());
		else /* pf or pf2 */
			temp = BRepAlgo_Fuse (pf->GetPrimitive(), pf2->GetPrimitive());

		/* Define a PlaneFigure resulted from the boolean operation applied on
 		 * on the two shapes. */
		pf = new PlaneFigure (temp);
	}

	return pf;
}

/** 
 * Parse the flags for a defined planefigure or a TLO.
 * @param scan A CSGScanner & argument.
 * @param flags A Flags & argument.
 * @see CSGScanner::GetToken()
 * @see CSGScanner::ReadNext()
 * @see CSGScanner::GetStringValue()
 * @see TOKEN_TYPE
 * @see ParseChar()
 */
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
				{ /* string list */
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

/**
 * Main parsing function for CSG2D geometries.
 * This method checks for the 'algebraic2d' keyword of the CSG2D input files 
 * format. It defines planefigures, TLOs, flags for the TLOs and constructs the
 * final compound shape.
 * @param istr An istream argument. The CSG2D input file to be parsed.
 * @return the final compound shape.
 * @see CSGScanner::GetToken()
 * @see TOKEN_TYPE
 * @see CSGScanner::Error()
 * @see CSGScanner::ReadNext()
 * @see CSGScanner::GetMyMapOfShapes()
 * @see CSGScanner::GetColorFlags()
 * @see CSGScanner::GetColorValid()
 * @see ParseChar()
 * @see PlaneFigure::PlaneFigure()
 * @see PlaneFigure::GetPrimitive()
 */
TopoDS_Shape CSGScanner :: ParseCSG (istream & istr)
{
	CSGScanner scan (istr);
	builder.MakeCompound (geometry);
	Quantity_Color * colorFlags_aux = NULL; /* It retains the color for TLOs. */
	TopoDS_ListOfShape listOfShapes; /* TLOs. */
	vector<string> tlo_names;

	scan.ReadNext();
	try
	{
		/* Check for the 'algebraic2d' keyword. */
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
				/* A planefigure definition. */
 				scan.ReadNext();
 				if (scan.GetToken() != TOK_STRING)
	 				scan.Error ("name identifier expected");
 				string planefigurename = scan.GetStringValue();
 				scan.ReadNext();

 				ParseChar (scan, '=');
 				PlaneFigure * pf = ParsePlaneFigure (scan);

				/* Add the planefigure pf to the map of planefigure names and shapes. */
				scan.GetMyMapOfShapes()->insert (make_pair (planefigurename, 
																										pf->GetPrimitive()));
 				ParseChar (scan, ';');

 				cout << "define planefigure" << " " << planefigurename << endl;
			}

			else if (scan.GetToken() == TOK_TLO)
			{ 
				/* A top-level object definition. */
				scan.ReadNext();

				string name = scan.GetStringValue();
				scan.ReadNext();

				if (scan.GetToken() != TOK_STRING)
				{ 
					/* It is a planefigure TLO. */

					/* parse the TLO's flags. */
					Flags flags;
					ParseFlags (scan, flags);

					ParseChar (scan, ';');

					if (scan.GetMyMapOfShapes()->find (name) ==
							scan.GetMyMapOfShapes()->end())
						scan.Error ("Top-Level-Object "+name+" not defined");

					/* Add the planefigure to the compound shape along with all the other
  				 * TLOs. */
					sewer.Add (scan.GetMyMapOfShapes()->find (name)->second);

					/* Add this TLO planefigure to the list of TLOs. */
					listOfShapes.Prepend (scan.GetMyMapOfShapes()->find (name)->second);
					tlo_names.push_back(name);

					NTopLevelObjects++;

					/* Reallocate space for the colors of the TLOs */
					colorFlags_aux = (Quantity_Color*) realloc (colorFlags_aux, 
														NTopLevelObjects * sizeof (Quantity_Color));

 					//set the color flags 
					if (flags.NumListFlagDefined ("col")) /* if the TLO has a color defined. */
					{
						const Array<double> & col = flags.GetNumListFlag ("col");
						Quantity_Color color = Quantity_Color (col[0], col[1], col[2],
																	Quantity_TOC_RGB);

						/* Add the color to the array of colors. */
						colorFlags_aux[NTopLevelObjects - 1] = 
							colorFlags_aux[NTopLevelObjects - 1].Assign (color);
					}
					else /* No color defined for the TLO. */
						/* Set up the default color GREEN for the current TLO. */
						colorFlags_aux[NTopLevelObjects - 1] = 
							colorFlags_aux[NTopLevelObjects - 1].Assign (Quantity_Color 
								(0.0, 1.0, 0.0, Quantity_TOC_RGB));
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

	cout << NTopLevelObjects << " TLOs: " << endl;
	for (int i = 0; i < NTopLevelObjects; i ++)
		cout<<tlo_names[i]<<endl;

	if (NTopLevelObjects > 0)
	{ 
		/* 'Sew' all the TLOs. */
		sewer.Perform();
		bool first = true;
		
		/* Apply a fusion between all the shapes that were sewed. */
		TopoDS_Shape my_fuse;
		for (TopExp_Explorer exp0 (sewer.SewedShape(), TopAbs_FACE); exp0.More();
					exp0.Next())
		{
			if (first)
			{
				my_fuse = exp0.Current();
				first = false;
			}
			else
				my_fuse = BRepAlgoAPI_Fuse (my_fuse, exp0.Current());
		}

		/* Add this fusion to the compound object. */
		builder.Add (geometry, my_fuse);
		
		/* Traverse the components of the compound object, i.e., pieces of the previous TLOs.
 		 * A piece means any shape that is delimited by edges.*/
		int NComponents = 0;
		for (TopExp_Explorer exp0 (geometry, TopAbs_FACE); exp0.More(); exp0.Next())
		{
			NComponents++;
			bool has_common = false;
			int ntlo = 0;

			/* Traverse all the user-defined TLOs. */
			for (TopoDS_ListIteratorOfListOfShape it (listOfShapes); it.More() && 
																							(has_common == false); it.Next())
			{
				ntlo ++;

				/* Take the common part between the TLO and the component. */
				TopoDS_Shape common;
				common = BRepAlgoAPI_Common (exp0.Current(), it.Value());
				TopExp_Explorer exp2 (common, TopAbs_FACE, TopAbs_EDGE);
					
				if (exp2.More())
				{
					has_common = true;

					colorValid = (bool*) realloc (colorValid, NComponents * sizeof (bool));
					colorFlags = (Quantity_Color*) realloc (colorFlags, NComponents * 
																											sizeof (Quantity_Color));

					/* Set the color of the current component to be the color of the
 					 * current TLO. */
					GetColorFlags()[NComponents - 1] = GetColorFlags()[NComponents - 1].Assign
																					(colorFlags_aux[NTopLevelObjects - ntlo]);

					/* If the color is not the default color. */
					if (colorFlags_aux[NTopLevelObjects - ntlo].IsDifferent (Quantity_Color
																							(0.0, 1.0, 0.0, Quantity_TOC_RGB)))

						/* Set the color defined validity flag to true. */
						GetColorValid()[NComponents - 1] = true;
					else
						GetColorValid()[NComponents - 1] = false;
				}
			}
		}
		
		/* Return the final compound object. */
		return geometry;
	 }
	 return TopoDS_Shape();
}

