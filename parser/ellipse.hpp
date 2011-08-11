#ifndef FILE_ELLIPSE
#define FILE_ELLIPSE

/******************************************************************************/
/*       File:    ellipse.hpp                                                 */
/*       Author:  Cristina Precup, cpcristinaprecup@gmail.com                 */
/*       Created: 08/07/2011                                                  */
/******************************************************************************/

/* 
 *	Ellipse primitive
 */

class Ellipse
{
protected:
	gp_Pnt center;
	Standard_Real majorRadius;
	Standard_Real minorRadius;

public:
	Ellipse (const gp_Pnt &acenter, Standard_Real majorRadius, Standard_Real minorRadius);
	~Ellipse ();
	TopoDS_Shape MakeEllipse ();
};

#endif

