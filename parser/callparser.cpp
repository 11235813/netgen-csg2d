/**************************************************************************/
/*                                                                        */
/* File:    callparser.cpp                                                */
/* Author:  Cristina Precup, cpcristinaprecup@gmail.com                   */
/* Date:    07/18/2011                                                    */
/*                                                                        */
/**************************************************************************/
#include "csg2Dparser.hpp"
#include "csg2Dparser.cpp"

#include <BRepTools.hxx>

#include <IGESControl_Controller.hxx>
#include <IGESControl_Writer.hxx>
#include <Interface_Static.hxx>
#include <TopoDS_Shape.hxx>
#include <TopoDS_Compound.hxx>

#include <iostream>
#include <string>

using namespace std;

//extern TopoDS_Compound ParseCSG (istream & istr);

void PrintMessage(const char *s1, const char *s2)
{ 
  cout << s1 << " " << s2 << endl;	
  //Ng_PrintDest(MyStr(" ")+s1+s2+MyStr("\n"));
}

int main(int argc, char* argv[])
{
  string filename = "extremeExample.geo";
  const char * cfilename = filename.c_str();
  if (strcmp (&cfilename[strlen(cfilename)-3], "geo") == 0)
  {
    PrintMessage ("Load CSG geometry file ", cfilename);
    ifstream infile(cfilename);
    CSGScanner *s = new CSGScanner(infile);
    TopoDS_Shape geom = s->ParseCSG (infile);
    if (geom.IsNull())
    {
	cout << "geo-file should start with algebraic2d" << endl;
      return 1;
    }
    ///debug
    IGESControl_Controller::Init();
    IGESControl_Writer writer(Interface_Static::CVal("XSTEP.iges.unit"),Interface_Static::IVal("XSTEP.iges.writebrep.mode"));
    writer.AddShape(geom);
    writer.ComputeModel();

    writer.Write("myIGES.igs");
///////write in a BRep file////////
    BRepTools * brep_writer = new BRepTools();
    brep_writer->Write(geom, "myBRep.brep");
///////////////

  }
  return 0;
  
}

