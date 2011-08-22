/**************************************************************************/
/* File:    ellipse.cpp                                                   */
/* Author:  Cristina Precup, cpcristinaprecup@gmail.com                   */
/* Date:    08/06/2011                                                    */
/**************************************************************************/

/**  
 * @file ellipse.cpp
 * Ellipse primitive implementation.
 */

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
	 * Create the support geometry: an ellipse represented in 3D.
	 */
	gp_Ax2 axis;
	gp_Elips aellipse (axis, majorRadius, minorRadius);
	aellipse.SetLocation (center);

	/* Create the face corresponding to the ellipse. */
	TopoDS_Edge W = BRepBuilderAPI_MakeEdge (aellipse);
	BRepBuilderAPI_MakeWire mkWire;
	mkWire.Add (W);
	TopoDS_Wire myWire = mkWire.Wire ();
	TopoDS_Face myFace = BRepBuilderAPI_MakeFace (myWire);  

	return myFace;
}

