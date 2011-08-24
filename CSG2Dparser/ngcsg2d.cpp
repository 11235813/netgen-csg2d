#define OCCGEOMETRY

/**************************************************************************/
/* File:    ngcsg2d.cpp                                                   */
/* Author:  Cristina Precup, cpcristinaprecup@gmail.com                   */
/* Date:    18/07/2011                                                    */
/**************************************************************************/

/**  
 * @file ngcsg2d.cpp
 * The main csg2d file.
 */

#include "csg2Dparser.cpp"

#include <BRepTools.hxx>

#include <meshing.hpp>
#include <occgeom.hpp>
#include <visual.hpp>

using namespace netgen;

extern DLL_HEADER NetgenGeometry * ng_geometry;

/**  
 * @class CSGRegister
 * The geometry register.
 */
class CSGRegister : public GeometryRegister
{
public:

	/** 
 	 * It loads the CSG2D input file.
 	 * @param filename A string argument.
 	 * @return a netgen geometry.
 	 */
	virtual NetgenGeometry * Load (string filename) const;

	/** 
 	 * Get the visual scene for the geometry.
 	 * @param geom A NetgenGeometry * argument.
 	 * @return a pointer to a VisualScene.
 	 */
	virtual VisualScene * GetVisualScene (const NetgenGeometry * geom) const;
};

NetgenGeometry * CSGRegister :: Load (string filename) const
{
	const char * cfilename = filename.c_str();
	if (strcmp (&cfilename[strlen(cfilename)-5], "geo2d") == 0)
	{
		PrintMessage (1, "Load CSG 2D geometry file: ", cfilename);

		ifstream infile(cfilename);

		/* Set up a scanner for the CSG2D input file. */
		CSGScanner *s = new CSGScanner(infile);

		/* Take the final compound shape obtained after parsing the file. */
		TopoDS_Shape geom = s->ParseCSG (infile);
		if (geom.IsNull())
		{
			return NULL;
		}
		
		OCCGeometry * occgeo;
		occgeo = new OCCGeometry;
		occgeo->shape = geom;

		occgeo->SetColorValid(s->GetColorValid());
		occgeo->SetFaceColors(s->GetColorFlags());

		occgeo->changed = 1;
		occgeo->BuildFMap();

		occgeo->CalcBoundingBox();
		PrintContents (occgeo);
		
		return occgeo;
	}
	
	return NULL;
}

VisualScene * CSGRegister :: GetVisualScene (const NetgenGeometry * geom) const
{
	return NULL;
}

#ifdef WIN32
extern "C" __declspec(dllexport) int Ng_csg2d_Init (Tcl_Interp * interp);
#else
extern "C" int Ng_csg2d_Init (Tcl_Interp * interp);
#endif

/** 
 * Load the CSG2D library.
 * @param interp A Tcl_Interp * argument.
 * @return a value that suggests that the loading was done or not.
 */
int Ng_csg2d_Init (Tcl_Interp * interp)
{
	cout<<endl<<"Loading CSG2D library..."<<endl;
	geometryregister.Append (new CSGRegister);
	return TCL_OK;
}

