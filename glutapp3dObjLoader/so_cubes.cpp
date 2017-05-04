
# include "so_cubes.h"

SoCubes::SoCubes() {
	_numpoints = 0;
	_phong = false;
}

// init is called only once:

void SoCubes::createCube(float& myX, float& myY, float& myZ, float& sideLength, int& blockType) {

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


	// Textures

	// Top Dirt (Grass)

	if (blockType == 1) {

		// Top
		T.push() = GsPnt2(0.25, 0.0);
		T.push() = GsPnt2(0.5, 0.0);
		T.push() = GsPnt2(0.5, 0.25);

		T.push() = GsPnt2(0.5, 0.25);
		T.push() = GsPnt2(0.25, 0.25);
		T.push() = GsPnt2(0.25, 0.0);

		// Bottom
		T.push() = GsPnt2(0.0, 0.0);
		T.push() = GsPnt2(0.25, 0.0);
		T.push() = GsPnt2(0.25, 0.25);

		T.push() = GsPnt2(0.25, 0.25);
		T.push() = GsPnt2(0.0, 0.25);
		T.push() = GsPnt2(0.0, 0.0);

		// 4 Sides

		for (int i = 0; i < 4; i++) {

			T.push() = GsPnt2(0.5, 0.0);
			T.push() = GsPnt2(0.75, 0.0);
			T.push() = GsPnt2(0.75, 0.25);

			T.push() = GsPnt2(0.75, 0.25);
			T.push() = GsPnt2(0.5, 0.25);
			T.push() = GsPnt2(0.5, 0.0);

		}

	}

	// Dirt

	else if (blockType == 0) {

		// 6 Sides

		for (int i = 0; i < 6; i++) {

			T.push() = GsPnt2(0.0, 0.0);
			T.push() = GsPnt2(0.25, 0.0);
			T.push() = GsPnt2(0.25, 0.25);

			T.push() = GsPnt2(0.25, 0.25);
			T.push() = GsPnt2(0.0, 0.25);
			T.push() = GsPnt2(0.0, 0.0);

		}

	}

	// Stone

	else if (blockType == 2) {

		// 6 Sides

		for (int i = 0; i < 6; i++) {

			T.push() = GsPnt2(0.0, 0.25);
			T.push() = GsPnt2(0.25, 0.25);
			T.push() = GsPnt2(0.25, 0.5);

			T.push() = GsPnt2(0.25, 0.5);
			T.push() = GsPnt2(0.0, 0.5);
			T.push() = GsPnt2(0.0, 0.25);

		}

	}

	else if (blockType == 3) {

		for (int i = 0; i < 6; i++) {

			T.push() = GsPnt2(0.0, 0.5);
			T.push() = GsPnt2(0.25, 0.5);
			T.push() = GsPnt2(0.25, 0.75);

			T.push() = GsPnt2(0.25, 0.75);
			T.push() = GsPnt2(0.0, 0.75);
			T.push() = GsPnt2(0.0, 0.5);

		}

	}

}

int SoCubes::checkTop(int* topDirts) {

	int height = 10;

	for (int k = 0; k < height; k++) {

		if (topDirts[k] == 1 || topDirts[k] == 2) {

			return 1;

		}

	}

	return 0;

}

void SoCubes::init ()
 {

	 // Load programs:
	 _vshgou.load_and_compile(GL_VERTEX_SHADER, "../vsh_smtl_gouraud.glsl");
	 _fshgou.load_and_compile(GL_FRAGMENT_SHADER, "../fsh_gouraud.glsl");
	 _proggouraud.init_and_link(_vshgou, _fshgou);

	 _vshphong.load_and_compile(GL_VERTEX_SHADER, "../mcol_phong.vert");
	 _fshphong.load_and_compile(GL_FRAGMENT_SHADER, "../mcol_phong.frag");
	 _progphong.init_and_link(_vshphong, _fshphong);

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

	 if (!pic1.load("../MCTextures.png")) { std::cout << "COULD NOT LOAD IMAGE!\n"; exit(1); }
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

	 _progphong.uniform_locations(9); // will send 9 variables
	 _progphong.uniform_location(0, "vTransf");
	 _progphong.uniform_location(1, "vProj");
	 _progphong.uniform_location(2, "lPos");
	 _progphong.uniform_location(3, "la");
	 _progphong.uniform_location(4, "ld");
	 _progphong.uniform_location(5, "ls");
	 _progphong.uniform_location(6, "ka");
	 _progphong.uniform_location(7, "ks");
	 _progphong.uniform_location(8, "sh");
 }

// build may be called everytime the object changes (not the case for this axis object):

void SoCubes::build ( )
 {

	P.size(0); T.size(0); N.size(0);
   
	float xCord = 1.0;
   float yCord = -0.1;
   float zCord = 1.0;

   float sideLen = 2.0 / 50;

   int blockType[50][50][10] = {0};

   for (int k = 0; k < 10; ++k)
   {
	   for (int j = 0; j < 50; ++j)
	   {
		   for (int i = 0; i < 50; ++i)
		   {


			   if (data.get(i,j,k) == data.dirt_id) {

				   if (checkTop(blockType[i][j]) == 0) {
					   blockType[i][j][k] = 1;
				   }

			   }

			   else if (data.get(i,j,k) == data.stone_id) {

				   
				   blockType[i][j][k] = 2;
				   
			   }

			   else if (data.get(i,j,k) == data.air_id) {

				   blockType[i][j][k] = 3;

			   }

			   
			   if (blockType[i][j][k] != 3) {
				   createCube(xCord, yCord, zCord, sideLen, blockType[i][j][k]);
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

   // save size so that we can free our buffers and later draw the OpenGL arrays:
   _numpoints = P.size();

   // free non-needed memory:
   P.capacity(0); T.capacity(0); N.capacity(0);
 }

// draw will be called everytime we need to display this object:
void SoCubes::draw(const GsMat& tr, const GsMat& pr, const GsLight& l)
{
	float f[4];
	float sh = (float)_mtl.shininess;
	if (sh<0.001f) sh = 64;

	glBindTexture(GL_TEXTURE_2D, _texid1);

	if (_phong)
	{
		glUseProgram(_progphong.id);
		glUniformMatrix4fv(_progphong.uniloc[0], 1, GL_FALSE, tr.e);
		glUniformMatrix4fv(_progphong.uniloc[1], 1, GL_FALSE, pr.e);
		glUniform3fv(_progphong.uniloc[2], 1, l.pos.e);
		glUniform4fv(_progphong.uniloc[3], 1, l.amb.get(f));
		glUniform4fv(_progphong.uniloc[4], 1, l.dif.get(f));
		glUniform4fv(_progphong.uniloc[5], 1, l.spe.get(f));
		glUniform4fv(_progphong.uniloc[6], 1, _mtl.ambient.get(f));
		glUniform4fv(_progphong.uniloc[7], 1, _mtl.specular.get(f));
		glUniform1fv(_progphong.uniloc[8], 1, &sh);
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

	glBindVertexArray(va[0]);
	glDrawArrays(GL_TRIANGLES, 0, _numpoints);
	glBindVertexArray(0); // break the existing vertex array object binding.
}


