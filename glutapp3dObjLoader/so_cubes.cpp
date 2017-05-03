
# include "so_cubes.h"

SoCubes::SoCubes()
 {
   _numpoints = 0;
 }

// init is called only once:

void SoCubes::createCube(GsColor myColor, double& myX, double& myY, double& myZ, double& sideLength) {

	GsVec topNorthEast = GsVec(myX, myY, myZ);
	GsVec topSouthEast = GsVec(myX, myY, myZ - sideLength);
	GsVec topSouthWest = GsVec(myX - sideLength, myY, myZ - sideLength);
	GsVec topNorthWest = GsVec(myX - sideLength, myY, myZ);

	GsVec bottomNorthEast = GsVec(myX, myY - sideLength, myZ);
	GsVec bottomSouthEast = GsVec(myX, myY - sideLength, myZ - sideLength);
	GsVec bottomSouthWest = GsVec(myX - sideLength, myY - sideLength, myZ - sideLength);
	GsVec bottomNorthWest = GsVec(myX - sideLength, myY - sideLength, myZ);

	// Top Face

	P.push() = topNorthEast;
	P.push() = topSouthEast;
	P.push() = topSouthWest;

	P.push() = topSouthWest;
	P.push() = topNorthWest;
	P.push() = topNorthEast;

	// Bottom Face

	P.push() = bottomNorthEast;
	P.push() = bottomSouthEast;
	P.push() = bottomSouthWest;

	P.push() = bottomSouthWest;
	P.push() = bottomNorthWest;
	P.push() = bottomNorthEast;


	// Front Face

	P.push() = topSouthWest;
	P.push() = topSouthEast;
	P.push() = bottomSouthEast;

	P.push() = bottomSouthEast;
	P.push() = bottomSouthWest;
	P.push() = topSouthWest;

	// Right Face

	P.push() = topSouthEast;
	P.push() = topNorthEast;
	P.push() = bottomNorthEast;

	P.push() = bottomNorthEast;
	P.push() = bottomSouthEast;
	P.push() = topSouthEast;


	// Back Face

	P.push() = topNorthEast;
	P.push() = topNorthWest;
	P.push() = bottomNorthWest;

	P.push() = bottomNorthWest;
	P.push() = bottomNorthEast;
	P.push() = topNorthEast;


	// Left Face

	P.push() = topNorthWest;
	P.push() = topSouthWest;
	P.push() = bottomSouthWest;

	P.push() = bottomSouthWest;
	P.push() = bottomNorthWest;
	P.push() = topNorthWest;


	for (int i = 0; i < 36; i++) {

		C.push() = myColor;

	}

}

void SoCubes::init ()
 {

	//data.print();

   // Build program:
   _vsh.load_and_compile ( GL_VERTEX_SHADER, "../mcol_flat.vert" );
   _fsh.load_and_compile ( GL_FRAGMENT_SHADER, "../flat.frag" );
   _prog.init_and_link ( _vsh, _fsh );

   // Define buffers needed:
   gen_vertex_arrays ( 1 ); // will use 1 vertex array
   gen_buffers ( 2 );       // will use 2 buffers: one for coordinates and one for colors
   _prog.uniform_locations ( 2 ); // will send 2 variables: the 2 matrices below
   _prog.uniform_location ( 0, "vTransf" );
   _prog.uniform_location ( 1, "vProj" );
 }

// build may be called everytime the object changes (not the case for this axis object):
void SoCubes::build ( )
 {

   P.size(0); C.size(0); // set size to zero
   
   double xCord = 1.0;
   double yCord = -0.1;
   double zCord = 1.0;

   double sideLen = 2.0/50;

   for (int k = 0; k < 10; ++k)
   {
	   for (int j = 0; j < 50; ++j)
	   {
		   for (int i = 0; i < 50; ++i)
		   {
			   if (data.chunk[i][j][k] == data.air_id) {

				   createCube(GsColor::blue, xCord, yCord, zCord, sideLen);
				   
			   }
			   else if (data.chunk[i][j][k] == data.stone_id) {

				   createCube(GsColor::gray, xCord, yCord, zCord, sideLen);
				   
			   }
			   else if (data.chunk[i][j][k] == data.dirt_id) {

				   createCube(GsColor::brown, xCord, yCord, zCord, sideLen);
				   
			   }

			   zCord -= sideLen;

		   }

		   zCord = 1.0;
		   xCord -= sideLen;
	   }

	   xCord = 1.0;
	   yCord -= sideLen;

   }


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

   // save size so that we can free our buffers and later draw the OpenGL arrays:
   _numpoints = P.size();

   // free non-needed memory:
   P.capacity(0); C.capacity(0);
 }

// draw will be called everytime we need to display this object:
void SoCubes::draw ( GsMat& tr, GsMat& pr )
 {
   // Prepare program:
   glUseProgram ( _prog.id );
   glUniformMatrix4fv ( _prog.uniloc[0], 1, GL_FALSE, tr.e );
   glUniformMatrix4fv ( _prog.uniloc[1], 1, GL_FALSE, pr.e );

   // Draw:
   glBindVertexArray ( va[0] );
   glDrawArrays ( GL_TRIANGLES, 0, _numpoints );
   glBindVertexArray(0); // break the existing vertex array object binding.
 }

