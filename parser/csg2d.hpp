#ifndef FILE_CSG2D
#define FILE_CSG2D

/******************************************************************************/
/*       File:    csg2d.hpp                                                   */
/*       Author:  Cristina Precup, cpcristinaprecup@gmail.com                 */
/*       Created: 08/07/2011                                                  */
/******************************************************************************/

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

#include "circle.hpp"
#include "ellipse.hpp"
#include "polygon.hpp"

#endif

