/**************************************************************************/
/*                                                                        */
/* File:    polygon.cpp                                                   */
/* Author:  Cristina Precup, cpcristinaprecup@gmail.com                   */
/* Date:    07/18/2011                                                    */
/*                                                                        */
/**************************************************************************/

#include "csg2d.hpp"

Polygon :: Polygon (TColgp_Array1OfPnt * aarrayOfPoints)
{
	arrayOfPoints = aarrayOfPoints;
}

Polygon :: ~Polygon ()
{
	delete arrayOfPoints;
	arrayOfPoints = NULL;
}

TopoDS_Shape Polygon :: MakePolygon ()
{
	/*
   *	Construct a closed polygon  
   */
	int i, lower, upper;
	lower = arrayOfPoints->Lower ();
	upper = arrayOfPoints->Upper ();
	
	if(abs (upper - lower) >= 1)
	{ 
		gp_Pnt P1 = arrayOfPoints->Value (lower);
		gp_Pnt P2 = arrayOfPoints->Value (lower + 1);
		//construct an open polygon
		BRepBuilderAPI_MakePolygon apolygon (P1, P2);
		//take the next points and add them to the wire
	  for(i = lower + 2; i <= upper; i ++)
		{
			gp_Pnt P = arrayOfPoints->Value (i);
			apolygon.Add (P);
		}
		apolygon.Close ();
		TopoDS_Wire W = apolygon;
    BRepBuilderAPI_MakeWire mkWire;
		mkWire.Add (W);
		TopoDS_Wire myWireProfile = mkWire.Wire ();
		TopoDS_Face myFaceProfile = BRepBuilderAPI_MakeFace (myWireProfile);
        
		return myFaceProfile;
	}
	
	return TopoDS_Shape();
}

