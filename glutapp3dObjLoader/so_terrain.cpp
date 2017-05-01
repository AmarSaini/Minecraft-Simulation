

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

    double x, y, z; // coords of upper top left vertex
    bool air; // is this an air block?

public:
    Block(bool air = false)
    {
        this->air = air;
        _numpoints = 0;
    }

    bool air()
    {
        air = !air;
        return air;
    }

    void set(double i, double j, double k)
    {
        x = i; y = j; z = k;
    }

    virtual void init(const GlProgram& _prog) // redefine with proper .png
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
    virtual void build()
    {
        // overide in child
    }
    void draw ( const GsMat& tr, const GsMat& pr, const GsLight& l )
    {
        if (!air) // draw if not air
        {
            float f[4]; 
            float sh = (float)_mtl.shininess;
            if ( sh<0.001f ) sh=64;

            glBindTexture(GL_TEXTURE_2D, _texid1);
            
            glUseProgram ( _progphong.id );
            glUniformMatrix4fv ( _progphong.uniloc[0], 1, GL_FALSE, tr.e );
            glUniformMatrix4fv ( _progphong.uniloc[1], 1, GL_FALSE, pr.e );
            glUniform3fv ( _progphong.uniloc[2], 1, l.pos.e );
            glUniform4fv ( _progphong.uniloc[3], 1, l.amb.get(f) );
            glUniform4fv ( _progphong.uniloc[4], 1, l.dif.get(f) );
            glUniform4fv ( _progphong.uniloc[5], 1, l.spe.get(f) );
            glUniform4fv ( _progphong.uniloc[6], 1, _mtl.ambient.get(f) );
            glUniform4fv ( _progphong.uniloc[7], 1, _mtl.specular.get(f) );
            glUniform1fv ( _progphong.uniloc[8], 1, &sh );

            glBindVertexArray ( va[0] );
            glDrawArrays ( GL_TRIANGLES, 0, _numpoints );
            glBindVertexArray(0); // break the existing vertex array object binding.
        }
    }
}

class Dirt: public Block
{
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

    double x, y, z; // coords of upper top left vertex
    bool air; // is this an air block?

public:
    Dirt(bool air = false)
    {
        this->air = air;
        _numpoints = 0;
    }

    virtual void init(const GlProgram& _prog)
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

       if (!pic1.load("../dirt.png")) { std::cout << "COULD NOT LOAD IMAGE!\n"; exit(1); }
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
    void build() // assuming size of dimensions is 1
    {
        // top face
        P.push() = GsPnt(x, y, z); // top left
        P.push() = GsPnt(x + 1, y, z); // top right
        P.push() = GsPnt(x + 1, y, z - 1); // bot right

        P.push() = GsPnt(x + 1, y, z - 1); // bot right
        P.push() = GsPnt(x, y, z - 1); // bot left
        P.push() = GsPnt(x, y, z); // top left

        for (int n = 0; n < 6; ++n)
            N.push() = GsPnt(0, 1, 0); 

        // bot face
        P.push() = GsPnt(x, y - 1, z); // top left
        P.push() = GsPnt(x + 1, y - 1, z); // top right
        P.push() = GsPnt(x + 1, y - 1, z - 1); // bot right

        P.push() = GsPnt(x + 1, y - 1, z - 1); // bot right
        P.push() = GsPnt(x, y - 1, z - 1); // bot left
        P.push() = GsPnt(x, y - 1, z); // top left

        for (int n = 0; n < 6; ++n)
            N.push() = GsPnt(0, -1, 0); 

        // left face
        P.push() = GsPnt(x, y, z); // top left
        P.push() = GsPnt(x, y, z - 1); // top right
        P.push() = GsPnt(x, y - 1, z - 1); // bot right

        P.push() = GsPnt(x, y - 1, z - 1); // bot right
        P.push() = GsPnt(x, y - 1, z); // bot left
        P.push() = GsPnt(x, y, z); // top left

        for (int n = 0; n < 6; ++n)
            N.push() = GsPnt(-1, 0, 0); 

        // right face
        P.push() = GsPnt(x + 1, y, z - 1); // top left
        P.push() = GsPnt(x + 1, y, z); // top right
        P.push() = GsPnt(x + 1, y - 1, z); // bot right

        P.push() = GsPnt(x + 1, y - 1, z); // bot right
        P.push() = GsPnt(x + 1, y - 1, z - 1); // bot left
        P.push() = GsPnt(x + 1, y, z - 1); // top left

        for (int n = 0; n < 6; ++n)
            N.push() = GsPnt(1, 0, 0); 

        // front face
        P.push() = GsPnt(x, y, z - 1); // top left
        P.push() = GsPnt(x + 1, y, z - 1); // top right
        P.push() = GsPnt(x + 1, y - 1, z - 1); // bot right

        P.push() = GsPnt(x + 1, y - 1, z - 1); // bot right
        P.push() = GsPnt(x, y - 1, z - 1); // bot left
        P.push() = GsPnt(x, y, z - 1); // top left

        for (int n = 0; n < 6; ++n)
            N.push() = GsPnt(0, 0, -1); 

        // back face
        P.push() = GsPnt(x, y, z); // top left
        P.push() = GsPnt(x + 1, y, z); // top right
        P.push() = GsPnt(x + 1, y - 1, z); // bot right

        P.push() = GsPnt(x + 1, y - 1, z); // bot right
        P.push() = GsPnt(x, y - 1, z); // bot left
        P.push() = GsPnt(x, y, z); // top left

        for (int n = 0; n < 6; ++n)
            N.push() = GsPnt(0, 0, 1); 

        // send data to OpenGL buffers:
       glBindVertexArray(va[0]);
       glEnableVertexAttribArray(0);
       glEnableVertexAttribArray(1);
       glEnableVertexAttribArray(2);

       glBindBuffer(GL_ARRAY_BUFFER, buf[0]);
       glBufferData(GL_ARRAY_BUFFER, 3 * sizeof(float)*P.size(), P.pt(), GL_STATIC_DRAW);
       glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

       glBindBuffer(GL_ARRAY_BUFFER, buf[1]);
       glBufferData(GL_ARRAY_BUFFER, 3 * sizeof(float)*N.size(), N.pt(), GL_STATIC_DRAW);
       glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);

       glBindBuffer(GL_ARRAY_BUFFER, buf[2]);
       glBufferData(GL_ARRAY_BUFFER, 2 * sizeof(float)*T.size(), T.pt(), GL_STATIC_DRAW);
       glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, 0);

       glBindVertexArray(0); // break the existing vertex array object binding.

       std::cout<<"build ok.\n";

       // save size so that we can free our buffers and later draw the OpenGL arrays:
       _numpoints = P.size();

       // free non-needed memory:
       P.capacity(0); T.capacity(0); N.capacity(0);
    }
}

class Stone : public Dirt
{
public:
    Stone(bool air = false)
    {
        this->air = air;
        _numpoints = 0;
    }
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

       if (!pic1.load("../stone.png")) { std::cout << "COULD NOT LOAD IMAGE!\n"; exit(1); }
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
}

class Terrain : public GlObjects
 { private :
    bool _phong;

    Chunk chunk_data; // 3D terrain composition (25x25x10)
    int grass_id;
    int stone_id;
    int air_id;

    Block* blocks[25][25][10]; // terrain objects

   public :
    Terrain ()
    {
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
    
    void build ()
    {
        for (int i = 0; i < 25; ++i)
        {
            for (int j = 0; j < 25; ++j)
            {
                for (int k = 0; k < 10; ++k)
                {
                    switch (chunk_data[i][j][k])
                    {
                        case (air_id):
                            blocks[i][j][k] = new Block(true);
                            break;
                        case (grass_id):
                            blocks[i][j][k] = new Dirt();
                            break;
                        case (stone_id):
                            blocks[i][j][k] = new Stone();
                            break;
                    } 
                }
            }
        }
    }
    void draw ( const GsMat& tr, const GsMat& pr, const GsLight& l )
    {
        for (int i = 0; i < 25; ++i)
        {
            for (int j = 0; j < 25; ++j)
            {
                for (int k = 0; k < 10; ++k)
                {
                    blocks[i][j][k]->draw(tr, pr, l);
                }
            }
        }
    }
 };
