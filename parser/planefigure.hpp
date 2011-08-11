#ifndef FILE_PLANEFIGURE
#define FILE_PLANEFIGURE

/**************************************************************************/
/*                                                                        */
/* File:    planefigure.hpp                                               */
/* Author:  Cristina Precup, cpcristinaprecup@gmail.com                   */
/* Date:    07/18/2011                                                    */
/*                                                                        */
/**************************************************************************/

#include <TopoDS_Shape.hxx>

class PlaneFigure
{
	public:
		typedef enum optyp1 {SUB, SECTION, ROOT} optyp;

	private:
		char * name;
		TopoDS_Shape prim;
		PlaneFigure * pf1, * pf2;
		optyp op;
		bool not_operation;

	public:
		PlaneFigure(TopoDS_Shape ashape, bool not_op = false);
		PlaneFigure(optyp aop, PlaneFigure * apf1, PlaneFigure *apf2 = NULL);
		~PlaneFigure();
		const char * Name () const { return name; }
		void SetName (const char * aname);
		TopoDS_Shape GetPrimitive();
		const bool Not_Op() const { return not_operation; }
	};

		inline ostream & operator<< (ostream & ost, const PlaneFigure & planefigure)
	{
	 // planefigure.Print (ost);
		return ost;
	}

#endif

