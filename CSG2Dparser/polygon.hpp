#ifndef FILE_POLYGON
#define FILE_POLYGON

/******************************************************************************/
/*       File:    polygon.hpp                                                 */
/*       Author:  Cristina Precup, cpcristinaprecup@gmail.com                 */
/*       Date:    08/08/2011                                                  */
/******************************************************************************/

/**  
 * @file polygon.hpp
 * Polygon primitive implementation.
 */

/**
 * @class Polygon
 * Class for the polygon primitive.
 * This class implements an algorithm which is used to create a polygon.  
 */
class Polygon
{
protected:
	TColgp_Array1OfPnt * arrayOfPoints; /**< Points that determine the vertices.  */

public:

	/** 
 	 * A constructor.
 	 * Assigns the array of points.
 	 * @param aarrayOfPoints A TColgp_Array1OfPnt * argument.
 	 * @see ~Polygon()
 	 * @see MakePolygon()
 	 */
	Polygon (TColgp_Array1OfPnt *  aarrayOfPoints);

	/**
 	 * A destructor.
 	 * @see Polygon()
 	 */	
	~Polygon ();

	/** 
 	 * Creates a closed polygon represented as a bounded surface in the 3D space.
 	 * It constructs a polygon by adding all the points in arrayOfPoints, adds it
 	 * to the wire and eventually, creates a face resulted from taking the closed
 	 * area determined by the wire on a bounded surface.
 	 * It returns the face resulted.
 	 * @return The face for the polygon.
 	 * @see Polygon()
   */
	TopoDS_Shape MakePolygon ();
};

#endif

