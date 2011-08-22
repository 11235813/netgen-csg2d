#ifndef FILE_CSG2D
#define FILE_CSG2D

/******************************************************************************/
/* File:    csg2d.hpp                                                         */
/* Author:  Cristina Precup, cpcristinaprecup@gmail.com                       */
/* Date:    08/07/2011                                                        */
/******************************************************************************/

/**  
 * @file csg2d.hpp
 * Headers inclusion.
 */

#include <BRepBuilderAPI_MakeEdge.hxx>
#include <BRepBuilderAPI_MakeFace.hxx>
#include <BRepBuilderAPI_MakeWire.hxx>
#include <BRepBuilderAPI_MakePolygon.hxx>

#include <gp_Pnt.hxx>
#include <gp_Ax2.hxx>
#include <gp_Dir.hxx>
#include <gp_Circ.hxx>
#include <gp_Elips.hxx>

#include <TopoDS_Edge.hxx>
#include <TopoDS_Wire.hxx>
#include <TopoDS_Face.hxx>
#include <TopoDS_Shape.hxx>

#include <TColgp_Array1OfPnt.hxx>
#include <BRepAlgoAPI_Fuse.hxx>
#include <BRepAlgoAPI_Common.hxx>
#include <BRepAlgo_Cut.hxx>
#include <BRepAlgo_Fuse.hxx>
#include <BRepAlgo_Common.hxx>
#include <BRepAlgo_BooleanOperations.hxx>
#include <BRep_Builder.hxx>
#include <BRepOffsetAPI_Sewing.hxx>
#include <gp_Vec.hxx>
#include <gp_Trsf.hxx>
#include <Quantity_Color.hxx>
#include <TopExp_Explorer.hxx>
#include <TopAbs_ShapeEnum.hxx>
#include <TopoDS_ListIteratorOfListOfShape.hxx>
#include <TopoDS_ListOfShape.hxx>
#include <TopoDS_Compound.hxx>


#include "circle.hpp"
#include "ellipse.hpp"
#include "polygon.hpp"
#include "planefigure.hpp"

#endif

