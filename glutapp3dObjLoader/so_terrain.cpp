
// Ensure the header file is included only once in multi-file projects
#ifndef SO_MODEL3_H
#define SO_MODEL3_H

// Include needed header files
# include <gsim/gs_mat.h>
# include <gsim/gs_light.h>
# include <gsim/gs_color.h>
# include <gsim/gs_array.h>
# include <gsim/gs_model.h>
# include "ogl_tools.h"
# include <gsim/gs_image.h>
# include "Perlin.cpp"

// Scene objects should be implemented in their own classes; and
// here is an example of how to organize a scene object in a class.
// Scene object axis:
class Block
{
    
}

class Terrain : public GlObjects
 { private :
    GlShader _vshgou, _fshgou, _vshphong, _fshphong;
    GlProgram _proggouraud, _progphong;
	GsImage pic1;
	gsuint _texid1;

	GsArray<GsVec2> T;
    GsArray<GsVec>   P; // coordinates
    GsArray<GsColor> C; // diffuse colors per face
    GsArray<GsVec>   N; // normals
    GsMaterial _mtl;    // main material
    int _numpoints;     // just saves the number of points
    bool _phong;

    Chunk chunk_data; // 3D terrain composition (25x25x10)
    int grass_id;
    int stone_id;
    int air_id;

    Block blocks;

   public :
    Terrain ()
    {
        _numpoints = 0;
        _phong = false;
        chunk_data = Chunk(3); // 3 types of blocks : dirt, stone, and air
        chunk_data.normalize();

        double mean = 0;
        for (int i = 0; i < 25; ++i)
        {
            for (int j = 0; j < 25; ++j)
            {
                mean += chunk_data.get(i, j, 0);
            }
        }
        mean /= 25*25;
        grass_id = floor(mean + 0.5); // set grass block to match most common surface block
        switch (grass_id)
        {
            case 1:
                air_id = 2;
                stone_id = 3;
                break;
            case 2:
                air_id = 1;
                stone_id = 3;
                break;
            case 3:
                air_id = 2;
                stone_id = 1;
                break;    
        }
    }
    void phong ( bool b ) { _phong=b; }
    bool phong () const { return _phong; }
    void init(const GlProgram& _prog)
    {

        set_program ( _prog );
       gen_vertex_arrays(1); // will use 1 vertex array
       gen_buffers(3);       // will use 3 buffers
       _proggouraud.uniform_locations(11); // will send 9 variables
       _proggouraud.uniform_location(0, "vTransf");
       _proggouraud.uniform_location(1, "vProj");
       _proggouraud.uniform_location(2, "lPos");
       _proggouraud.uniform_location(3, "la");
       _proggouraud.uniform_location(4, "ld");
       _proggouraud.uniform_location(5, "ls");
       _proggouraud.uniform_location(6, "ka");
       _proggouraud.uniform_location(7, "kd");
       _proggouraud.uniform_location(8, "ks");
       _proggouraud.uniform_location(9, "sh");
       _proggouraud.uniform_location(10, "Tex1");

       if (!pic1.load("../background.png")) { std::cout << "COULD NOT LOAD IMAGE!\n"; exit(1); }
       glGenTextures(1, &_texid1); // generated ids start at 1
       glBindTexture(GL_TEXTURE_2D, _texid1);

       glTexImage2D(GL_TEXTURE_2D, 0, 4, pic1.w(), pic1.h(), 0, GL_RGBA, GL_UNSIGNED_BYTE, pic1.data());
       glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
       glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
       glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
       glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
       glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

       glGenerateMipmap(GL_TEXTURE_2D);
       glBindTexture(GL_TEXTURE_2D, 0);
       glBindVertexArray(0);

       pic1.init(0, 0); // free image from CPU
    }
    void build ()
    {

    }
    void draw ( const GsMat& tr, const GsMat& pr, const GsLight& l );
 };

#endif // SO_MODEL_H
