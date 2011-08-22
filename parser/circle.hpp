#ifndef FILE_CIRCLE
#define FILE_CIRCLE

/******************************************************************************/
/* File:    circle.hpp                                                        */
/* Author:  Cristina Precup, cpcristinaprecup@gmail.com                       */
/* Date:    08/07/2011                                                        */
/******************************************************************************/

/* 
 *	Circle primitive
 */

class Circle
{
protected:
	gp_Pnt center;
	Standard_Real radius;

public:
	Circle (const gp_Pnt &acenter, Standard_Real aradius);
	~Circle ();
	TopoDS_Shape MakeCircle ();
};

#endif

