
// Ensure the header file is included only once in multi-file projects
#ifndef SO_CUBES_H
#define SO_CUBES_H

// Include needed header files
# include <gsim/gs_color.h>
# include <gsim/gs_array.h>
# include <gsim/gs_vec.h>
# include "ogl_tools.h"
# include "Perlin.cpp"

// Scene objects should be implemented in their own classes; and
// here is an example of how to organize a scene object in a class.
// Scene object axis:
class SoCubes : public GlObjects
 { private :
    GlShader _vsh, _fsh;
    GlProgram _prog;
    GsArray<GsVec>   P; // coordinates
    GsArray<GsColor> C; // color
	Terrain data;
    int _numpoints;     // just saves the number of points

   public :
    SoCubes ();
    void init ();
    void build ();
    void draw ( GsMat& tr, GsMat& pr );
 };

#endif // SO_CUBES_H
