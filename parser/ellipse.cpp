/**************************************************************************/
/*                                                                        */
/* File:    ellipse.cpp                                                   */
/* Author:  Cristina Precup, cpcristinaprecup@gmail.com                   */
/* Date:    08/06/2011                                                    */
/*                                                                        */
/**************************************************************************/
#include <BRepBuilderAPI_MakeEdge.hxx>
#include <BRepBuilderAPI_MakeFace.hxx>
#include <BRepBuilderAPI_MakeWire.hxx>

#include <gp_Pnt.hxx>
#include <gp_Ax2.hxx>
#include <gp_Elips.hxx>
#include <gp_Dir.hxx>

#include <TopoDS_Edge.hxx>
#include <TopoDS_Wire.hxx>

TopoDS_Shape
MakeEllipse(const gp_Pnt &center, const Standard_Real MajorRadius, const Standard_Real MinorRadius)
{
  //create an ellipse represented in 3d
  gp_Ax2 axis;
  gp_Elips aellipse(axis, MajorRadius, MinorRadius);
  aellipse.SetLocation(center);

  TopoDS_Edge W = BRepBuilderAPI_MakeEdge(aellipse);
  BRepBuilderAPI_MakeWire mkWire;
  mkWire.Add(W);
  TopoDS_Wire myWireProfile = mkWire.Wire();
  TopoDS_Face myFaceProfile = BRepBuilderAPI_MakeFace(myWireProfile);  
  
  return myFaceProfile;
}

