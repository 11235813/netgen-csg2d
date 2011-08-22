#ifndef FILE_ELLIPSE
#define FILE_ELLIPSE

/******************************************************************************/
/* File:    ellipse.hpp                                                       */
/* Author:  Cristina Precup, cpcristinaprecup@gmail.com                       */
/* Date:    08/07/2011                                                        */
/******************************************************************************/

/**  
 * @file ellipse.hpp
 * Ellipse primitive implementation.
 */

/** 
 * @class Ellipse
 *	Class for the ellipse primitive.
 *	This class implements an algorithm which is used to create an ellipse.
 */

class Ellipse
{
protected:
	gp_Pnt center; /**< The center of the ellipse. Stored as a 3D cartesian point.  */
	Standard_Real majorRadius; /**<The major radius of the ellipse.  */
	Standard_Real minorRadius; /**<The minor radius of the ellipse.  */

public:

	/** 
	 * A constructor.
	 * Initializes the center of the circle with the point acenter, the major
	 * radius with amajorRadius and the minor radius with aminorRadius.
	 * @param acenter A gp_Pnt argument.
	 * @param amajorRadius A Standard_Real value.
	 * @param aminorRadius A Standard_Real value.
	 * @see ~Ellipse()
	 * @see MakeEllipse()
	 */
	Ellipse (const gp_Pnt &acenter, Standard_Real amajorRadius, Standard_Real aminorRadius);

	/**
	 * A destructor.
	 * @see Ellipse() 
	 */
	~Ellipse ();

	/**
	 * Creates an ellipse represented as a bounded surface in the 3D space.
	 * It takes the edge of the ellipse, adds it to a wire and eventually, 
	 * creates a face resulted from taking the closed area determined by the wire
	 * on a bounded surface.
	 * It returns the face resulted.
	 * @return The face for the ellipse.
	 * @see Ellipse()
	 */
	TopoDS_Shape MakeEllipse ();
};

#endif

