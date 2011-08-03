/**************************************************************************/
/*                                                                        */
/* File:    circle.cpp                                                    */
/* Author:  Cristina Precup, cpcristinaprecup@gmail.com                   */
/* Date:    07/19/2011                                                    */
/*                                                                        */
/**************************************************************************/
#include <BRepBuilderAPI_MakeEdge.hxx>
#include <BRepPrimAPI_MakeSphere.hxx>
#include <BRepBuilderAPI_MakeFace.hxx>
#include <BRepBuilderAPI_MakeWire.hxx>

#include <gp_Pnt.hxx>
#include <gp_Ax2.hxx>
#include <gp_Circ.hxx>
#include <gp_Dir.hxx>

#include <TopoDS_Edge.hxx>
#include <TopoDS_Wire.hxx>

TopoDS_Shape
MakeCircle(const gp_Pnt2d &P, const Standard_Real Radius)
{
  //create a circle represented in 3d
  gp_Pnt center(P.X(), P.Y(), 0);
  gp_Ax2 axis;
  gp_Circ acircle(axis, Radius);
  acircle.SetLocation(center);

  TopoDS_Edge W = BRepBuilderAPI_MakeEdge(acircle);
  BRepBuilderAPI_MakeWire mkWire;
  mkWire.Add(W);
  TopoDS_Wire myWireProfile = mkWire.Wire();
  TopoDS_Face myFaceProfile = BRepBuilderAPI_MakeFace(myWireProfile);  
  
  return myFaceProfile;
}

