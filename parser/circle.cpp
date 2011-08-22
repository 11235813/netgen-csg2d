/**************************************************************************/
/* File:    circle.cpp                                                    */
/* Author:  Cristina Precup, cpcristinaprecup@gmail.com                   */
/* Date:    07/19/2011                                                    */
/**************************************************************************/

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
	 *	create a circle represented in 3d
	 */
	gp_Ax2 axis;
	gp_Circ acircle (axis, radius);
	acircle.SetLocation (center);

	TopoDS_Edge W = BRepBuilderAPI_MakeEdge (acircle);
	BRepBuilderAPI_MakeWire mkWire;
	mkWire.Add (W);
	TopoDS_Wire myWireProfile = mkWire.Wire ();
	TopoDS_Face myFaceProfile = BRepBuilderAPI_MakeFace (myWireProfile);  

	return myFaceProfile;
}

