#define OCCGEOMETRY

#include "csg2Dparser.cpp"

#include <BRepTools.hxx>

#include <meshing.hpp>
#include <occgeom.hpp>
#include <visual.hpp>

using namespace netgen;

extern DLL_HEADER NetgenGeometry * ng_geometry;
//static VisualSceneGeometry2d vsgeom2d;



class CSG2dRegister : public GeometryRegister
{
public:
	virtual NetgenGeometry * Load (string filename) const;
	virtual VisualScene * GetVisualScene (const NetgenGeometry * geom) const;
};

NetgenGeometry *  CSG2dRegister :: Load (string filename) const
{
	const char * cfilename = filename.c_str();
	if (strcmp (&cfilename[strlen(cfilename)-5], "geo2d") == 0)
	{
		PrintMessage (1, "Load CSG 2D geometry file ", cfilename);

		ifstream infile(cfilename);

		CSGScanner *s = new CSGScanner(infile);
		TopoDS_Shape geom = s->ParseCSG (infile);
		if (geom.IsNull())
		{
			cout << "geo-file should start with algebraic2d" << endl;
			return NULL;
		}
		///////write in a BRep file////////
		BRepTools * brep_writer = new BRepTools();
		brep_writer->Write(geom, "myBRep.brep");
		/////////////////
		OCCGeometry * occgeo;
		occgeo = new OCCGeometry;

		occgeo->shape = geom;

		occgeo->face_colours = Handle_XCAFDoc_ColorTool();
		occgeo->face_colours.Nullify();
		
		//occgeo->face_colours = s->GetColorTool();
		occgeo->changed = 1;
		occgeo->BuildFMap();

		occgeo->CalcBoundingBox();
		PrintContents (occgeo);
		
		return occgeo;
/* 
SplineGeometry2d * hgeom = new SplineGeometry2d();
hgeom -> Load (cfilename);
return hgeom;
*/
	}
	
	return NULL;
}

VisualScene * CSG2dRegister :: GetVisualScene (const NetgenGeometry * geom) const
{
 /* SplineGeometry2d * geometry = dynamic_cast<SplineGeometry2d*> (ng_geometry);
	if (geometry)
		{
vsgeom2d.SetGeometry (geometry);
return &vsgeom2d;
		}
*/
	return NULL;
}

#ifdef WIN32
extern "C" __declspec(dllexport) int Ng_csg2d_Init (Tcl_Interp * interp);
#else
extern "C" int Ng_csg2d_Init (Tcl_Interp * interp);
#endif

int Ng_csg2d_Init (Tcl_Interp * interp)
{
	cout<<endl<<"Loading CSG2d library..."<<endl;
	geometryregister.Append (new CSG2dRegister);
	return TCL_OK;
}

