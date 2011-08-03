/**************************************************************************/
/*                                                                        */
/* File:    planefigure.cpp                                                */
/* Author:  Cristina Precup, cpcristinaprecup@gmail.com                   */
/* Date:    07/18/2011                                                    */
/*                                                                        */
/**************************************************************************/
PlaneFigure :: PlaneFigure(TopoDS_Shape ashape, bool not_op)
{
  prim = ashape;
  name = NULL;
  not_operation = not_op;
}

PlaneFigure :: PlaneFigure(optyp aop, PlaneFigure * apf1, PlaneFigure * apf2)
{
  op = aop;
  pf1 = apf1;
  pf2 = apf2;
  prim = TopoDS_Shape();
  name = NULL;
}

PlaneFigure :: ~PlaneFigure()
{
  delete [] name;

  switch(op)
  {
    case SECTION:
    {
      if(pf1->op != ROOT) delete pf1;
      if(pf2->op != ROOT) delete pf2;
      break;
    }
  }
}

void PlaneFigure :: SetName (const char * aname) 
{
    delete [] name;
    name = new char[strlen (aname)+1];
    strcpy (name, aname);
}

TopoDS_Shape PlaneFigure :: GetPrimitive()
{
 return prim;
}

