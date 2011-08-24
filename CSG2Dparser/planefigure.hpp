#ifndef FILE_PLANEFIGURE
#define FILE_PLANEFIGURE

/**************************************************************************/
/* File:    planefigure.hpp                                               */
/* Author:  Cristina Precup, cpcristinaprecup@gmail.com                   */
/* Date:    07/18/2011                                                    */
/**************************************************************************/

/**  
 * @file planefigure.hpp
 * Generic 2D shape type implementation.
 */

/**
 * @class PlaneFigure  
 * Class for the generic type of 2D shapes.
 * This class has as members: a shape and a flag for the 'not'
 * operation applied to it.
 */

class PlaneFigure
{
	public:

	private:
		TopoDS_Shape prim; /**< The shape. */
		bool not_operation; /**< The flag for the 'not' operation. */

	public:

		/** 
 		 * A constructor.
 		 * Initializes the prim shape with ashape parameter and the not_operation 
 		 * with not_op parameter.
 		 * @param ashape A TopoDS_Shape argument.
 		 * @param not_op A bool argument.
 		 * @see ~PlaneFigure()
 		 * @see GetPrimitive()
 		 * @see Not_Op()
		 */
		PlaneFigure(TopoDS_Shape ashape, bool not_op = false);

		/**  
 		 * A destructor.
 		 * @see PlaneFigure()
 		 */
		~PlaneFigure();

		/**  
 		 * Getter for the primitive shape prim.
 		 * @return the primitive shape.
	 	 */
		TopoDS_Shape GetPrimitive();

		/**  
 		 * Getter for the not_operation flag.
 		 * @return the truth value of this flag.
 		 */
		const bool Not_Op() const;
	};

#endif

