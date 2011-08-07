#ifndef FILE_POLYGON
#define FILE_POLYGON

/******************************************************************************/
/*       File:    polygon.hpp                                                 */
/*       Author:  Cristina Precup, cpcristinaprecup@gmail.com                 */
/*       Created: 08/08/2011                                                  */
/******************************************************************************/

/*
 *	Polygon primitive
 */

class Polygon
{
protected:
	TColgp_Array1OfPnt * arrayOfPoints;

public:
	Polygon (TColgp_Array1OfPnt *  aarrayOfPoints);
	~Polygon ();
	TopoDS_Shape MakePolygon ();
};

#endif

