
# include "so_model3.h"

SoModel3::SoModel3()
 {
   _numpoints = 0;
   _phong = false;
 }

void SoModel3::init ()
 {
	 // Load programs:
	 _vshgou.load_and_compile(GL_VERTEX_SHADER, "../vsh_smtl_gouraud.glsl");
	 _fshgou.load_and_compile(GL_FRAGMENT_SHADER, "../fsh_gouraud.glsl");
	 _proggouraud.init_and_link(_vshgou, _fshgou);

   _vshphong.load_and_compile ( GL_VERTEX_SHADER, "../mcol_phong.vert" );
   _fshphong.load_and_compile ( GL_FRAGMENT_SHADER, "../mcol_phong.frag" );
   _progphong.init_and_link ( _vshphong, _fshphong );

   // Define buffers needed:
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

   _progphong.uniform_locations ( 9 ); // will send 9 variables
   _progphong.uniform_location ( 0, "vTransf" );
   _progphong.uniform_location ( 1, "vProj" );
   _progphong.uniform_location ( 2, "lPos" );
   _progphong.uniform_location ( 3, "la" );
   _progphong.uniform_location ( 4, "ld" );
   _progphong.uniform_location ( 5, "ls" );
   _progphong.uniform_location ( 6, "ka" );
   _progphong.uniform_location ( 7, "ks" );
   _progphong.uniform_location ( 8, "sh" );
 }

void SoModel3::build ()
 {
   int i;
   GsColor c;
   P.size(0); T.size(0); N.size(0);

   /* There are multiple ways to organize data to send to OpenGL. 
      Here we send material information per vertex but we only send the diffuse color
      information per vertex. The other components are uniforms sent to affect the whole object.
      This is a solution that keeps this code simple and is ok for most objects.
   */

   P.push() = GsPnt(1.0, 0.0, 1.0);
   P.push() = GsPnt(1.0, 0.0, -1.0);
   P.push() = GsPnt(-1.0, 0.0, -1.0);

   P.push() = GsPnt(-1.0, 0.0, -1.0);
   P.push() = GsPnt(-1.0, 0.0, 1.0);
   P.push() = GsPnt(1.0, 0.0, 1.0);

   N.push() = GsPnt(0.0, 1.0, 0.0);
   N.push() = GsPnt(0.0, 1.0, 0.0);
   N.push() = GsPnt(0.0, 1.0, 0.0);

   N.push() = GsPnt(0.0, 1.0, 0.0);
   N.push() = GsPnt(0.0, 1.0, 0.0);
   N.push() = GsPnt(0.0, 1.0, 0.0);

   T.push() = GsPnt2(0.0, 0.0);
   T.push() = GsPnt2(0.0, 1.0);
   T.push() = GsPnt2(1.0, 1.0);

   T.push() = GsPnt2(1.0, 1.0);
   T.push() = GsPnt2(1.0, 0.0);
   T.push() = GsPnt2(0.0, 0.0);

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

void SoModel3::draw ( const GsMat& tr, const GsMat& pr, const GsLight& l )
 {
   float f[4]; 
   float sh = (float)_mtl.shininess;
   if ( sh<0.001f ) sh=64;

   glBindTexture(GL_TEXTURE_2D, _texid1);

   if ( _phong )
    { glUseProgram ( _progphong.id );
      glUniformMatrix4fv ( _progphong.uniloc[0], 1, GL_FALSE, tr.e );
      glUniformMatrix4fv ( _progphong.uniloc[1], 1, GL_FALSE, pr.e );
      glUniform3fv ( _progphong.uniloc[2], 1, l.pos.e );
      glUniform4fv ( _progphong.uniloc[3], 1, l.amb.get(f) );
      glUniform4fv ( _progphong.uniloc[4], 1, l.dif.get(f) );
      glUniform4fv ( _progphong.uniloc[5], 1, l.spe.get(f) );
      glUniform4fv ( _progphong.uniloc[6], 1, _mtl.ambient.get(f) );
      glUniform4fv ( _progphong.uniloc[7], 1, _mtl.specular.get(f) );
      glUniform1fv ( _progphong.uniloc[8], 1, &sh );
    }
   else
    {
		glUseProgram(_proggouraud.id);
		glUniformMatrix4fv(_proggouraud.uniloc[0], 1, GL_FALSE, tr.e);
		glUniformMatrix4fv(_proggouraud.uniloc[1], 1, GL_FALSE, pr.e);
		glUniform3fv(_proggouraud.uniloc[2], 1, l.pos.e);
		glUniform4fv(_proggouraud.uniloc[3], 1, l.amb.get(f));
		glUniform4fv(_proggouraud.uniloc[4], 1, l.dif.get(f));
		glUniform4fv(_proggouraud.uniloc[5], 1, l.spe.get(f));
		glUniform4fv(_proggouraud.uniloc[6], 1, _mtl.ambient.get(f));
		glUniform4fv(_proggouraud.uniloc[7], 1, _mtl.diffuse.get(f));
		glUniform4fv(_proggouraud.uniloc[8], 1, _mtl.specular.get(f));
		glUniform1fv(_proggouraud.uniloc[9], 1, &sh);
    }

   glBindVertexArray ( va[0] );
   glDrawArrays ( GL_TRIANGLES, 0, _numpoints );
   glBindVertexArray(0); // break the existing vertex array object binding.
 }

