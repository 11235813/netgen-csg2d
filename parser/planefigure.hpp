#ifndef FILE_PLANEFIGURE
#define FILE_PLANEFIGURE

/**************************************************************************/
/* File:    planefigure.hpp                                               */
/* Author:  Cristina Precup, cpcristinaprecup@gmail.com                   */
/* Date:    07/18/2011                                                    */
/**************************************************************************/

#include <TopoDS_Shape.hxx>

class PlaneFigure
{
	public:

	private:
		char * name;
		TopoDS_Shape prim;
		PlaneFigure * pf1, * pf2;
		bool not_operation;

	public:
		PlaneFigure(TopoDS_Shape ashape, bool not_op = false);
		~PlaneFigure();
		const char * Name () const { return name; }
		void SetName (const char * aname);
		TopoDS_Shape GetPrimitive();
		const bool Not_Op() const { return not_operation; }
	};

		inline ostream & operator<< (ostream & ost, const PlaneFigure & planefigure)
	{
		return ost;
	}

#endif

