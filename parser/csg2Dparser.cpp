
  static kwstruct defkw[] =
    {
      { TOK_RECO2,    "algebraic2d" },
      { TOK_PLANEFIGURE,   "planefigure" },
      { TOK_TLO,     "tlo" },
//    { TOK_BOUNDINGBOX, "boundingbox" },
      { TOK_OR,      "or" },
      { TOK_AND,     "and" },
      { TOK_NOT,     "not" },
/*    { TOK_POINT,    "point" },
      { TOK_IDENTIFY, "identify" },
      { TOK_BOUNDARYCONDITION, "boundarycondition" },
      { TOK_BOUNDARYCONDITIONNAME, "boundaryconditionname" },
      { TOK_CONSTANT, "constant" },
*/
      { TOKEN_TYPE(0), 0 }
    };

  static primstruct defprim[] =
    {
      { TOK_POLYGON, "polygon" },
      { TOK_CIRCLE, "circle" },
      { TOK_ELLIPSE, "ellipse" },
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
  }


  void CSGScanner :: ReadNext ()
  {
    char ch;

    // scan whitespaces
    do
      {
	scanin->get(ch);

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
		scanin->get(ch);
		if (scanin->eof())
		  {
		    token = TOK_END;
		    return;
		  }
	      }
	    linenum++;
	  }
      }
    while (isspace(ch));

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
	      scanin->get(ch);
	      while (isalnum(ch) || ch == '_')
		{
		  string_value += ch;
		  scanin->get(ch);
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
      scan.Error (string ("token '") + string(1, ch) + string("' expected"));
    scan.ReadNext();
  }

  double ParseNumber(CSGScanner & scan)
  {
    if (scan.GetToken() == '-')
      {
	scan.ReadNext();
	return -ParseNumber (scan);
      }
    if (scan.GetToken() != TOK_NUM) scan.Error ("number expected");
    double val = scan.GetNumValue();
    scan.ReadNext();
    return val;
  }
  
  CSGScanner & operator>> (CSGScanner & scan, char ch)
  {
    if (scan.GetToken() != TOKEN_TYPE(ch))
      scan.Error (string ("token '") + string(1, ch) + string("' expected"));
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
              TColgp_Array1OfPnt *aarray = new TColgp_Array1OfPnt(1, no);
              while(i < no)
              {  
                i ++;
		    scan >>';' >> x >> ',' >> y;
                P.SetCoord(x, y, 0);
                aarray->SetValue(i, P);
              }
              scan >> ')';
	        TopoDS_Shape nprim = MakePolygon (aarray);
		//aBuilder.Add(geometry, nprim);
	      //geom->AddSurfaces (nprim);
	      //return new PlaneFigure (nprim);
	        PlaneFigure *pf = new PlaneFigure(nprim);
		  return pf;
            }
            else
            {
             scan.Error("Not enough points for defining a polygon.");
             return NULL; 
            }

	    }
         case TOK_CIRCLE:
         {
           double x, y, radius;
           scan.ReadNext();
	   scan >> '(' >> x >> ',' >> y >> ';' >> radius >> ')';
	   gp_Pnt center(x, y, 0);

	   TopoDS_Shape nprim = MakeCircle(center, radius);
	   PlaneFigure *pf = new PlaneFigure(nprim);
	   return pf;
         }
         case TOK_ELLIPSE:
         {
           double x, y, MajorRadius, MinorRadius;
           scan.ReadNext();
           scan >> '(' >> x >> ',' >> y >> ';' >> MajorRadius >> ';' >> MinorRadius >> ')';
           gp_Pnt center(x, y, 0);

           TopoDS_Shape nprim = MakeEllipse(center, MajorRadius, MinorRadius);
           PlaneFigure *pf = new PlaneFigure(nprim);
           return pf;
         }
	 default:
	   {
	     scan.Error (string ("unknown primary ") + scan.GetStringValue());
	   }

        }
    else if(scan.GetToken() == TOK_STRING && scan.GetMyMapOfShapes()->find(scan.GetStringValue())!= scan.GetMyMapOfShapes()->end())
    {
      PlaneFigure * pf = const_cast<PlaneFigure*> (new PlaneFigure(scan.GetMyMapOfShapes()->find(scan.GetStringValue())->second)); //take the value that is identified by the key
      scan.ReadNext();
      return pf;
    }
   else if (scan.GetToken() == TOK_NOT)

    {
	scan.ReadNext();
	PlaneFigure * pf2 = ParsePrimary (scan);
      //pf2->GetPrimitive().Reverse();
      TopoDS_Shape temp = pf2->GetPrimitive();/* pf2->GetPrimitive().Complemented()*/;
      pf2 = new PlaneFigure(pf2->GetPrimitive(), true);
      return pf2;
//	return new Solid (Solid::SUB, sol1);
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
    PlaneFigure * pf = ParsePrimary(scan);
    while (scan.GetToken() == TOK_AND)
      {
	scan.ReadNext();
	PlaneFigure * pf2 = ParsePrimary(scan);
      TopoDS_Shape temp;

	//check for not operation
      if(pf->Not_Op() == true && pf2->Not_Op() == false) //not pf and pf2
		temp = BRepAlgo_Cut(pf2->GetPrimitive(), pf->GetPrimitive());
      else if (pf2->Not_Op() == true && pf->Not_Op() == false) // pf and not pf2
            temp = BRepAlgo_Cut(pf->GetPrimitive(), pf2->GetPrimitive());
	else if (pf->Not_Op() == true && pf2->Not_Op() == true) //not pf and not pf2
		temp = BRepAlgo_Fuse(pf->GetPrimitive(), pf2->GetPrimitive());
      else //pf and pf2
		temp = BRepAlgo_Common(pf->GetPrimitive(), pf2->GetPrimitive());

      pf = new PlaneFigure(temp);

      }
    return pf;
  }


  PlaneFigure * ParsePlaneFigure (CSGScanner & scan)
  {
    PlaneFigure * pf = ParseTerm(scan);
    while (scan.GetToken() == TOK_OR)
      {
	scan.ReadNext();
	PlaneFigure * pf2 = ParseTerm(scan);
	TopoDS_Shape temp;

	//check for not operations
	if(pf->Not_Op() == true && pf2->Not_Op() == false) //not pf or pf2
            temp = BRepAlgo_Cut(pf->GetPrimitive(), pf2->GetPrimitive());
      else if (pf2->Not_Op() == true && pf->Not_Op() == false) // pf or not pf2
            temp = BRepAlgo_Cut(pf2->GetPrimitive(), pf->GetPrimitive());
      else if (pf->Not_Op() == true && pf2->Not_Op() == true) //not pf or not pf2
            temp = BRepAlgo_Common(pf->GetPrimitive(), pf2->GetPrimitive());
      else //pf or pf2
            temp = BRepAlgo_Fuse(pf->GetPrimitive(), pf2->GetPrimitive());

      pf = new PlaneFigure(temp);
      }
    return pf;
  }

  /*
    Main parsing function for CSG geometry
  */
  //CSGeometry * ParseCSG (istream & istr)
  TopoDS_Shape CSGScanner :: ParseCSG (istream & istr)
  {
    CSGScanner scan(istr);

    abuilder.MakeCompound(geometry);

//  geom = new CSGeometry;

    scan.ReadNext();
    if (scan.GetToken() != TOK_RECO2)  //keyword 'algebraic2d'
      //return 0;
	return TopoDS_Shape();   

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
/////////////harcoding///////////////////
             geom = pf->GetPrimitive();
////////////////////////////////
             scan.GetMyMapOfShapes()->insert(make_pair(planefigurename, pf->GetPrimitive()));
	/*  	Flags flags;
		ParseFlags (scan, flags);*/

//          geom->SetPlaneFigure (planefigurename.c_str(), new PlaneFigure (PlaneFigure::ROOT, pf));
	//	geom->SetFlags (planefigurename.c_str(), flags);

		 ParseChar (scan, ';');

		 //PrintMessage ("define planefigure ", planefigurename);
		 cout << "define planefigure" << " " << planefigurename << endl;
	      }

		else if (scan.GetToken() == TOK_TLO)

	      { // a TopLevelObject definition

		  scan.ReadNext();

		  string name = scan.GetStringValue();
		  scan.ReadNext();

		  if (scan.GetToken() != TOK_STRING)

		  { // a planefigure TLO

		  /*Flags flags;
		    ParseFlags (scan, flags);*/

		    ParseChar (scan, ';');

 		    if (scan.GetMyMapOfShapes()->find(name) == scan.GetMyMapOfShapes()->end())
		    //  scan.Error ("Top-Level-Object "+name+" not defined");
		    cout << endl << "Error: Top-Level-Object " << name << "not defined"<< endl;
		    //add the object to the compound shape with all the top level objects
               // abuilder.Add(geometry, scan.GetMyMapOfShapes()->find(name)->second);            
                sewer.Add(scan.GetMyMapOfShapes()->find(name)->second);
                NTopLevelObjects++;
/* 
		    int tlonr =
		      geom->SetTopLevelObject ((PlaneFigure*)geom->GetPlaneFigure(name));
		    TopLevelObject * tlo = geom->GetTopLevelObject (tlonr);
*/
/*
		    if (flags.NumListFlagDefined ("col"))
		      {
			const Array<double> & col =
			  flags.GetNumListFlag ("col");
			tlo->SetRGB (col[0], col[1], col[2]);
		      }

		    if (flags.GetDefineFlag ("transparent"))
		      tlo->SetTransparent (1);

		    tlo->SetMaterial (flags.GetStringFlag ("material", ""));
		    tlo->SetLayer (int(flags.GetNumFlag ("layer", 1)));
		    if (flags.NumFlagDefined ("maxh"))
		      tlo->SetMaxH (flags.GetNumFlag("maxh", 1e10));
*/
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
	cout << "caught error " << errstr << endl;
	//throw NgException (errstr);
      }


//    (*testout) << geom->GetNTopLevelObjects() << " TLOs:" << endl;
    cout << NTopLevelObjects << " TLOs: " << endl;
    for (int i = 0; i < NTopLevelObjects; i ++)
    {
      //.....ACCESS somehow the top level objects
    }
/*
    for (int i = 0; i < geom->GetNTopLevelObjects(); i++)
      {
	const TopLevelObject * tlo = geom->GetTopLevelObject(i);
	if (tlo->GetSolid())
	  (*testout) << i << ": " << *tlo->GetSolid() << endl;
      }
*/  
//    return geom;
     if(NTopLevelObjects > 0)
     { 
       sewer.Perform();
       abuilder.Add(geometry, sewer.SewedShape());
       return geometry;
     }
     return TopoDS_Shape();
  }

