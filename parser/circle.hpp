#ifndef FILE_CIRCLE
#define FILE_CIRCLE

/******************************************************************************/
/* File:    circle.hpp                                                        */
/* Author:  Cristina Precup, cpcristinaprecup@gmail.com                       */
/* Date:    08/07/2011                                                        */
/******************************************************************************/

/**  
 * @file circle.hpp
 * Circle primitive implementation.
 */

/**
 * @class Circle
 * Class for the circle primitive.
 * This class implements an algorithm which is used to create a circle.  
 */
class Circle
{
protected:
	gp_Pnt center; /**< The center of the circle. Stored as a 3D cartesian point.  */
	Standard_Real radius; /**< The radius of the circle. */

public:

	/** 
 	 * A constructor.
 	 * Initializes the center of the circle with the point acenter and the radius
 	 * with aradius.
 	 * @param acenter A gp_Pnt argument.
 	 * @param aradius A Standard_Real value.
 	 * @see ~Circle()
 	 * @see MakeCircle()
	 */ 
	Circle (const gp_Pnt &acenter, Standard_Real aradius);

	/**
 	 * A destructor.
 	 * @see Circle()
 	 */
	~Circle ();

	/**
 	 * Creates a circle represented as a bounded surface in the 3D space.
 	 * It takes the edge of the circle, adds it to a wire and eventually, creates
 	 * a face resulted from taking the closed area determined by the wire on a 
 	 * bounded surface.
 	 * It returns the face resulted.
 	 * @return The face for the circle.
 	 * @see Circle()
   */
	TopoDS_Shape MakeCircle ();
};

#endif

