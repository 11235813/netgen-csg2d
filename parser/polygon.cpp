/**************************************************************************/
/*                                                                        */
/* File:    polygon.cpp                                                   */
/* Author:  Cristina Precup, cpcristinaprecup@gmail.com                   */
/* Date:    07/18/2011                                                    */
/*                                                                        */
/**************************************************************************/
#include <BRepBuilderAPI_MakePolygon.hxx>
#include <BRepBuilderAPI_MakeWire.hxx>
#include <BRepBuilderAPI_MakeFace.hxx>

#include <gp_Pnt.hxx>

#include <TColgp_Array1OfPnt.hxx>

#include <TopoDS_Wire.hxx>
#include <TopoDS_Face.hxx>
#include <TopoDS_Shape.hxx>

TopoDS_Shape
MakePolygon(const TColgp_Array1OfPnt * aarray)
{
      int i, lower, upper;
	lower = aarray->Lower();
      upper = aarray->Upper();
	///construct a closed polygon defined by no points
	if(abs(upper - lower) >= 1)
      { 
        gp_Pnt P1 = aarray->Value(lower);
        gp_Pnt P2 = aarray->Value(lower + 1);
        //construct an open polygon
        BRepBuilderAPI_MakePolygon apolygon(P1, P2);
        //take the next points and add them to the wire
	  for(i = lower + 2; i < upper; i ++)
        {
          gp_Pnt P = aarray->Value(i);
          apolygon.Add(P);
        }
        apolygon.Close();
        TopoDS_Wire W = apolygon;
        
	  BRepBuilderAPI_MakeWire mkWire;
	  mkWire.Add(W);
	  TopoDS_Wire myWireProfile = mkWire.Wire();
	  TopoDS_Face myFaceProfile = BRepBuilderAPI_MakeFace(myWireProfile);
        
        return myFaceProfile;
      }
      return TopoDS_Shape();
}

