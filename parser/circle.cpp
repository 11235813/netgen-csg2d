/**************************************************************************/
/* File:    circle.cpp                                                    */
/* Author:  Cristina Precup, cpcristinaprecup@gmail.com                   */
/* Date:    07/19/2011                                                    */
/**************************************************************************/

/**
 * @file circle.cpp
 * Circle primitive implementation.
 */

#include "csg2d.hpp"

Circle :: Circle (const gp_Pnt &acenter, Standard_Real aradius)
{
	center = acenter;
	radius = aradius;
}

Circle :: ~Circle	()
{
;
}

TopoDS_Shape Circle :: MakeCircle ()
{
	/*
	 * Create the support geometry: a circle represented in 3D.
	 */
	gp_Ax2 axis;
	gp_Circ acircle (axis, radius);
	acircle.SetLocation (center);

	/* Create the face corresponding to this circle. */
	TopoDS_Edge W = BRepBuilderAPI_MakeEdge (acircle);
	BRepBuilderAPI_MakeWire mkWire;
	mkWire.Add (W);
	TopoDS_Wire myWire = mkWire.Wire ();
	TopoDS_Face myFace = BRepBuilderAPI_MakeFace (myWire);  

	return myFace;
}

