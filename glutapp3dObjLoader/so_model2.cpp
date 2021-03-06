
# include "so_model2.h"

SoModel2::SoModel2()
 {
   _numpoints = 0;
   _phong = false;
 }

void SoModel2::init ()
 {
	 // Load your shaders and link your programs here:
	 _vertexsh.load_and_compile(GL_VERTEX_SHADER, "../3dcolored.vert");
	 _fragsh.load_and_compile(GL_FRAGMENT_SHADER, "../flat.frag");
	 _prog.init_and_link(_vertexsh, _fragsh);

   _vshphong.load_and_compile ( GL_VERTEX_SHADER, "../mcol_phong.vert" );
   _fshphong.load_and_compile ( GL_FRAGMENT_SHADER, "../mcol_phong.frag" );
   _progphong.init_and_link ( _vshphong, _fshphong );

   gen_vertex_arrays(1); // will use 1 vertex array
   gen_buffers(2);       // will use 2 buffers: one for coordinates and one for colors
   _prog.uniform_locations(2); // will send 2 variables: the 2 matrices below
   _prog.uniform_location(0, "vTransf");
   _prog.uniform_location(1, "vProj");

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

void SoModel2::build ( GsModel& m )
 {
   int i;
   GsColor c;
   P.size(0); C.size(0); N.size(0);

   /* There are multiple ways to organize data to send to OpenGL. 
      Here we send material information per vertex but we only send the diffuse color
      information per vertex. The other components are uniforms sent to affect the whole object.
      This is a solution that keeps this code simple and is ok for most objects.
   */

   // build arrays:
   for ( i=0; i<m.F.size(); i++ )
    { GsModel::Face& f = m.F[i];
      P.push()=m.V[f.a]; P.push()=m.V[f.b]; P.push()=m.V[f.c]; 

      c = GsColor::darkgray;

      C.push()=c; C.push()=c; C.push()=c;
    }

   if ( m.M.size()>0 ) _mtl=m.M[0]; else _mtl.init();

   // send data to OpenGL buffers:
   glBindVertexArray ( va[0] );
   glEnableVertexAttribArray ( 0 );
   glEnableVertexAttribArray ( 1 );

   glBindBuffer ( GL_ARRAY_BUFFER, buf[0] );
   glBufferData ( GL_ARRAY_BUFFER, 3*sizeof(float)*P.size(), P.pt(), GL_STATIC_DRAW );
   glVertexAttribPointer ( 0, 3, GL_FLOAT, GL_FALSE, 0, 0 );

   glBindBuffer ( GL_ARRAY_BUFFER, buf[1] );
   glBufferData ( GL_ARRAY_BUFFER, 4*sizeof(gsbyte)*C.size(), C.pt(), GL_STATIC_DRAW );
   glVertexAttribPointer ( 1, 4, GL_UNSIGNED_BYTE, GL_FALSE, 0, 0 );

   glBindVertexArray(0); // break the existing vertex array object binding.

   std::cout<<"build ok.\n";

   // save size so that we can free our buffers and later draw the OpenGL arrays:
   _numpoints = P.size();

   // free non-needed memory:
   P.capacity(0); C.capacity(0); N.capacity(0);
 }

void SoModel2::draw ( const GsMat& tr, const GsMat& pr, const GsLight& l )
 {
   float f[4]; 
   float sh = (float)_mtl.shininess;
   if ( sh<0.001f ) sh=64;

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
    { glUseProgram ( _prog.id );
      glUniformMatrix4fv ( _prog.uniloc[0], 1, GL_FALSE, tr.e );
      glUniformMatrix4fv ( _prog.uniloc[1], 1, GL_FALSE, pr.e );
    }

   glBindVertexArray ( va[0] );
   glDrawArrays ( GL_TRIANGLES, 0, _numpoints );
   glBindVertexArray(0); // break the existing vertex array object binding.
 }

