/**************************************************************************/
/*                                                                        */
/* File:    ellipse.cpp                                                   */
/* Author:  Cristina Precup, cpcristinaprecup@gmail.com                   */
/* Date:    08/06/2011                                                    */
/*                                                                        */
/**************************************************************************/

#include "csg2d.hpp"

Ellipse :: Ellipse (const gp_Pnt &acenter, Standard_Real amajorRadius, Standard_Real aminorRadius)
{
	center = acenter;
	majorRadius = amajorRadius;
	minorRadius = aminorRadius;
}

Ellipse :: ~Ellipse ()
{
;
}

TopoDS_Shape Ellipse :: MakeEllipse ()
{
	/*
	 * create an ellipse represented in 3d
	 */
	gp_Ax2 axis;
	gp_Elips aellipse (axis, majorRadius, minorRadius);
	aellipse.SetLocation (center);

	TopoDS_Edge W = BRepBuilderAPI_MakeEdge (aellipse);
	BRepBuilderAPI_MakeWire mkWire;
	mkWire.Add (W);
	TopoDS_Wire myWireProfile = mkWire.Wire ();
	TopoDS_Face myFaceProfile = BRepBuilderAPI_MakeFace (myWireProfile);  

	return myFaceProfile;
}

