/**************************************************************************/
/* File:    polygon.cpp                                                   */
/* Author:  Cristina Precup, cpcristinaprecup@gmail.com                   */
/* Date:    07/18/2011                                                    */
/**************************************************************************/

/**  
 * @file polygon.cpp
 * Polygon primitive implementation.
 */

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
   *	Create the support geometry: a closed polygon represented in 3D. 
   */
	int i, lower, upper;
	lower = arrayOfPoints->Lower ();
	upper = arrayOfPoints->Upper ();
	
	if(abs (upper - lower) >= 1)
	{ 
		gp_Pnt P1 = arrayOfPoints->Value (lower);
		gp_Pnt P2 = arrayOfPoints->Value (lower + 1);

		/* Construct an open polygon. */
		BRepBuilderAPI_MakePolygon apolygon (P1, P2);

		/* Take the next points and add them to the wire. */
	  for(i = lower + 2; i <= upper; i ++)
		{
			gp_Pnt P = arrayOfPoints->Value (i);
			apolygon.Add (P);
		}
		apolygon.Close ();

		/* Create the face corresponding to this polygon. */
		TopoDS_Wire W = apolygon;
    BRepBuilderAPI_MakeWire mkWire;
		mkWire.Add (W);
		TopoDS_Wire myWire = mkWire.Wire ();
		TopoDS_Face myFace = BRepBuilderAPI_MakeFace (myWire);
        
		return myFace;
	}
	
	return TopoDS_Shape();
}

