/**************************************************************************/
/* File:    planefigure.cpp                                               */
/* Author:  Cristina Precup, cpcristinaprecup@gmail.com                   */
/* Date:    07/18/2011                                                    */
/**************************************************************************/

/**
 * @file planefigure.cpp
 * Generic 2D shapes type implementation.
 */

#include "csg2d.hpp"

PlaneFigure :: PlaneFigure(TopoDS_Shape ashape, bool not_op)
{
	prim = ashape;
	not_operation = not_op;
}

PlaneFigure :: ~PlaneFigure()
{
;
}

TopoDS_Shape PlaneFigure :: GetPrimitive()
{
	return prim;
}

const bool PlaneFigure :: Not_Op() const
{
	return not_operation;
}

