
# include <iostream>
# include <gsim/gs.h>
# include "app_window.h"

AppWindow::AppWindow ( const char* label, int x, int y, int w, int h )
          :GlutWindow ( label, x, y, w, h )
 {
   initPrograms ();
   addMenuEntry ( "Option 0", evOption0 );
   addMenuEntry ( "Option 1", evOption1 );
   _viewaxis = true;
   _fovy = GS_TORAD(60.0f);
   _rotx = .6;
   _roty = -2.0;
   _w = w;
   _h = h;
   scale = 0.01;
   headRotate = 0;
   lArmRotate = 0;
   rArmRotate = 0;
   bodyRotate = 0;
   lLegRotate = 0;
   rLegRotate = 0;
   moveX = 0.0;
   moveY = 0.0;
   moveZ = 0.0;
   lLegCheck = true;
   rLegCheck = true;
   lArmCheck = true;
   rArmCheck = true;
   moving = 0;
   seconds = 0.0;
   frames = 0.0;
   cloudFrames = 0.0;
   camMode = false;
   curveIndex = 0;

   for (int i = 0; i < 6; i++) {

	   myCurvePoints.push() = GsVec(-1.0+(0.4*i), 0.8, -1.0 + (0.4*i));
	   myCurvePoints.push() = GsVec(-0.8+(0.4*i), 0.4, -0.8 + (0.4*i));

   }

   for (int i = 0; i < 12; i++) {

	   cout << myCurvePoints[i] << endl;

   }

   curvePath = evaluate_bspline(25, 3, myCurvePoints);

   for (int i = 0; i < curvePath.size(); i++) {

	   cout << curvePath[i] << endl;

   }


 }

void AppWindow::initPrograms ()
 {
   // Init my scene objects:
   _axis.init ();
   head.init();
   body.init();
   lArm.init();
   rArm.init();
   lLeg.init();
   rLeg.init();
   headShadow.init();
   bodyShadow.init();
   lArmShadow.init();
   rArmShadow.init();
   lLegShadow.init();
   rLegShadow.init();
   background.init();

   myCubes.init();
   myCloud.init();

   

   // set light:
   _light.set ( GsVec(1.5,1.5,1.5), GsColor(90,90,90,255), GsColor::white, GsColor::white );

   // Load demo model:
   loadModel ( 1 );
 }

static void printInfo ( GsModel& m )
 {
   std::cout<<"V:  "<<m.V.size()<<"\n";
   std::cout<<"F:  "<<m.F.size()<<"\n";
   std::cout<<"N:  "<<m.N.size()<<"\n";
   std::cout<<"M:  "<<m.M.size()<<"\n";
   std::cout<<"Fn: "<<m.Fn.size()<<"\n";
   std::cout<<"Fm: "<<m.Fm.size()<<"\n";
 }

void AppWindow::loadModel ( int model )
 {
   float f = 0.01f;
   GsString file;

   // Normal Models

   file = "../models/steveHead.obj";
   std::cout << "Loading " << file << "...\n";
   if (!gsm1.load(file)) std::cout << "Error!\n";
   printInfo(gsm1);
   gsm1.scale(f); // to fit our camera space
   head.build(gsm1);

   file = "../models/steveBody.obj";
   std::cout << "Loading " << file << "...\n";
   if (!gsm2.load(file)) std::cout << "Error!\n";
   printInfo(gsm2);
   gsm2.scale(f); // to fit our camera space
   body.build(gsm2);

   file = "../models/steveLeftArm.obj";
   std::cout << "Loading " << file << "...\n";
   if (!gsm3.load(file)) std::cout << "Error!\n";
   printInfo(gsm3);
   gsm3.scale(f); // to fit our camera space
   lArm.build(gsm3);

   file = "../models/steveRightArm.obj";
   std::cout << "Loading " << file << "...\n";
   if (!gsm4.load(file)) std::cout << "Error!\n";
   printInfo(gsm4);
   gsm4.scale(f); // to fit our camera space
   rArm.build(gsm4);

   file = "../models/steveLeftLeg.obj";
   std::cout << "Loading " << file << "...\n";
   if (!gsm5.load(file)) std::cout << "Error!\n";
   printInfo(gsm5);
   gsm5.scale(f); // to fit our camera space
   lLeg.build(gsm5);

   file = "../models/steveRightLeg.obj";
   std::cout << "Loading " << file << "...\n";
   if (!gsm6.load(file)) std::cout << "Error!\n";
   printInfo(gsm6);
   gsm6.scale(f); // to fit our camera space
   rLeg.build(gsm6);


   // Shadow Models


   file = "../models/steveHead.obj";
   std::cout << "Loading " << file << "...\n";
   if (!gsm7.load(file)) std::cout << "Error!\n";
   printInfo(gsm7);
   gsm7.scale(f); // to fit our camera space
   headShadow.build(gsm7);

   file = "../models/steveBody.obj";
   std::cout << "Loading " << file << "...\n";
   if (!gsm8.load(file)) std::cout << "Error!\n";
   printInfo(gsm8);
   gsm8.scale(f); // to fit our camera space
   bodyShadow.build(gsm8);

   file = "../models/steveLeftArm.obj";
   std::cout << "Loading " << file << "...\n";
   if (!gsm9.load(file)) std::cout << "Error!\n";
   printInfo(gsm9);
   gsm9.scale(f); // to fit our camera space
   lArmShadow.build(gsm9);

   file = "../models/steveRightArm.obj";
   std::cout << "Loading " << file << "...\n";
   if (!gsm10.load(file)) std::cout << "Error!\n";
   printInfo(gsm10);
   gsm10.scale(f); // to fit our camera space
   rArmShadow.build(gsm10);

   file = "../models/steveLeftLeg.obj";
   std::cout << "Loading " << file << "...\n";
   if (!gsm11.load(file)) std::cout << "Error!\n";
   printInfo(gsm11);
   gsm11.scale(f); // to fit our camera space
   lLegShadow.build(gsm11);

   file = "../models/steveRightLeg.obj";
   std::cout << "Loading " << file << "...\n";
   if (!gsm12.load(file)) std::cout << "Error!\n";
   printInfo(gsm12);
   gsm12.scale(f); // to fit our camera space
   rLegShadow.build(gsm12);


   // Background

   
   background.build();
   myCloud.build();

   redraw();

 }

// mouse events are in window coordinates, but your 2D scene is in [0,1]x[0,1],
// so make here the conversion when needed
GsVec2 AppWindow::windowToScene ( const GsVec2& v )
 {
   // GsVec2 is a lighteweight class suitable to return by value:
   return GsVec2 ( (2.0f*(v.x/float(_w))) - 1.0f,
                    1.0f - (2.0f*(v.y/float(_h))) );
 }

// Called every time there is a window event
void AppWindow::glutKeyboard ( unsigned char key, int x, int y )
 {
	 const float incr = GS_TORAD(2.5f);
	 const float incf = 0.05f;


   switch ( key )
    { 

   case ' ':
	   if (camMode == false) {

		   _fovy = GS_TORAD(60.0f) + 0.3;
		   _rotx = 1.2;
		   _roty = -2.0;
		   frames = seconds;
		   camMode = true;
	   }

	   else {

		   _fovy = GS_TORAD(60.0f);

		   _rotx = .6;
		   _roty = -2.0;
		   camMode = false;

	   }
	   break;
   case 27: exit(1); break;// Esc was pressed
   case 'q': if (headRotate < 15) headRotate++; moving = 0; break;
   case 'a': if(headRotate > -15) headRotate--; moving = 0; break;
   case 'w': if (lArmRotate < 15) lArmRotate++; moving = 0; break;
   case 's': if (lArmRotate > -15) lArmRotate--; moving = 0; break;
   case 'e': if (rArmRotate < 15) rArmRotate++; moving = 0; break;
   case 'd': if (rArmRotate > -15) rArmRotate--; moving = 0; break;
   case 'r': if (lLegRotate < 15) lLegRotate++; moving = 0; break;
   case 'f': if (lLegRotate > -15) lLegRotate--; moving = 0; break;
   case 't': if (rLegRotate < 15) rLegRotate++; moving = 0; break;
   case 'g': if (rLegRotate > -15) rLegRotate--; moving = 0; break;
   case 'y': bodyRotate++; moving = 0; break;
   case 'h': bodyRotate--; moving = 0; break;

   case 'i': _rotx += incr; break;
   case 'k': _rotx -= incr; break;
   case 'j': _roty += incr; break;
   case 'l': _roty -= incr; break;
   case 'o':  _fovy -= incf; break;
   case 'p':  _fovy += incf; break;
	   
	}

   redraw();
 }

void AppWindow::animate() {

	 if (lLegRotate < 15 && lLegCheck) {
		 lLegRotate += 2;

		 if (lLegRotate >= 15) {
			 lLegCheck = false;
		 }

	 }

	 else if (lLegRotate > -15 && !lLegCheck) {
		 lLegRotate -= 2;

		 if (lLegRotate <= -15) {
			 lLegCheck = true;
		 }

	 }


	 if (rLegRotate > -15 && rLegCheck) {
		 rLegRotate -= 2;

		 if (rLegRotate <= -15) {
			 rLegCheck = false;
		 }

	 }

	 else if (rLegRotate < 15 && !rLegCheck) {
		 rLegRotate += 2;

		 if (rLegRotate >= 15) {
			 rLegCheck = true;
		 }

	 }


	 if (lArmRotate > -15 && lArmCheck) {
		 lArmRotate -= 2;

		 if (lArmRotate <= -15) {
			 lArmCheck = false;
		 }

	 }

	 else if (lArmRotate < 15 && !lArmCheck) {
		 lArmRotate += 2;

		 if (lArmRotate >= 15) {
			 lArmCheck = true;
		 }

	 }

	 if (rArmRotate < 15 && rArmCheck) {
		 rArmRotate += 2;

		 if (rArmRotate >= 15) {
			 rArmCheck = false;
		 }

	 }

	 else if (rArmRotate > -15 && !rArmCheck) {
		 rArmRotate -= 2;

		 if (rArmRotate <= -15) {
			 rArmCheck = true;
		 }

	 }

 }

void AppWindow::glutSpecial ( int key, int x, int y )
 {
   bool rd=true;
   const float incr=GS_TORAD(2.5f);
   const float incf=0.05f;
   switch ( key )
    { case GLUT_KEY_LEFT:
		
		moveZ -= 0.01;
		if (moving == 0 || moving == 1 || moving == 2 || moving == 3) {

			headRotate = 0;
			lArmRotate = 0;
			rArmRotate = 0;
			lLegRotate = 0;
			rLegRotate = 0;
			bodyRotate = 15;
			moving = 4;

		}
		animate();

		break;
      case GLUT_KEY_RIGHT:

		  moveZ += 0.01;
		  if (moving == 0 || moving == 1 || moving == 2 || moving == 4) {

			  headRotate = 0;
			  lArmRotate = 0;
			  rArmRotate = 0;
			  lLegRotate = 0;
			  rLegRotate = 0;
			  bodyRotate = -15;
			  moving = 3;

		  }
		  animate();

		  break;

      case GLUT_KEY_UP:

		  moveX += 0.01;
		  if (moving == 0 || moving == 2 || moving == 3 || moving == 4) {

			  headRotate = 0;
			  lArmRotate = 0;
			  rArmRotate = 0;
			  lLegRotate = 0;
			  rLegRotate = 0;
			  bodyRotate = 0;
			  moving = 1;

		  }
		  animate();

		  break;
      case GLUT_KEY_DOWN:

		  moveX -= 0.01;
		  if (moving == 0 || moving == 1 || moving == 3 || moving == 4) {

			  headRotate = 0;
			  lArmRotate = 0;
			  rArmRotate = 0;
			  lLegRotate = 0;
			  rLegRotate = 0;
			  bodyRotate = 50;
			  moving = 2;

		  }
		  animate(); 

		  break;

      default: return; // return without rendering
	}
   if (rd) redraw(); // ask the window to be rendered when possible
 }

void AppWindow::glutMouse ( int b, int s, int x, int y )
 {
   // The mouse is not used in this example.
   // Recall that a mouse click in the screen corresponds
   // to a whole line traversing the 3D scene.
 }

void AppWindow::glutMotion ( int x, int y )
 {
 }

void AppWindow::glutMenu ( int m )
 {
   std::cout<<"Menu Event: "<<m<<std::endl;
 }

void AppWindow::glutReshape ( int w, int h )
 {
   // Define that OpenGL should use the whole window for rendering
   glViewport( 0, 0, w, h );
   _w=w; _h=h;
 }

// here we will redraw the scene according to the current state of the application.
void AppWindow::glutDisplay ()
 {
   // Clear the rendering window
   glClear ( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

   // Build a cross with some lines (if not built yet):
   if ( _axis.changed ) // needs update
    { _axis.build(1.0f); // axis has radius 1.0
    }

   if (myCubes.changed) {
	   myCubes.build();
   }

   // Define our scene transformation:
   GsMat rx, ry, stransf;
   rx.rotx ( _rotx );
   ry.roty ( _roty );
   stransf = rx*ry; // set the scene transformation matrix

   // Define our projection transformation:
   // (see demo program in gltutors-projection.7z, we are replicating the same behavior here)
   GsMat camview, persp, sproj;

   GsVec eye(0,0,2), center(0,0,0), up(0,1,0);
   camview.lookat ( eye, center, up ); // set our 4x4 "camera" matrix

   float aspect=4.0f/3.0f, znear=0.1f, zfar=100.0f;
   persp.perspective ( _fovy, aspect, znear, zfar ); // set our 4x4 perspective matrix

   // Our matrices are in "line-major" format, so vertices should be multiplied on the 
   // right side of a matrix multiplication, therefore in the expression below camview will
   // affect the vertex before persp, because v' = (persp*camview)*v = (persp)*(camview*v).
   sproj = persp * camview; // set final scene projection

   //  Note however that when the shader receives a matrix it will store it in column-major 
   //  format, what will cause our values to be transposed, and we will then have in our 
   //  shaders vectors on the left side of a multiplication to a matrix.

   // Draw:
   // if ( _viewaxis ) _axis.draw ( stransf, sproj );

   myCubes.draw(stransf, sproj, _light);

   GsMat cloudTransformation;
   computeCloudTransformation(cloudTransformation);
   myCloud.draw(stransf * cloudTransformation, sproj, _light);

   GsMat headTransformation;
   computeHeadTransformation(headTransformation);
   head.draw ( stransf * headTransformation, sproj, _light );
 
   GsMat bodyTransformation;
   computeBodyTransformation(bodyTransformation);
   body.draw(stransf * bodyTransformation, sproj, _light);
   
   GsMat lArmTransformation;
   computeLArmTransformation(lArmTransformation);
   lArm.draw(stransf * lArmTransformation, sproj, _light);

   GsMat rArmTransformation;
   computeRArmTransformation(rArmTransformation);
   rArm.draw(stransf * rArmTransformation, sproj, _light);
   
   GsMat lLegTransformation;
   computeLLegTransformation(lLegTransformation);
   lLeg.draw(stransf * lLegTransformation, sproj, _light);

   GsMat rLegTransformation;
   computeRLegTransformation(rLegTransformation);
   rLeg.draw(stransf * rLegTransformation, sproj, _light);



   GsMat headShadowMatrix;
   computeShadowMatrix(headShadowMatrix);
   headShadow.draw(stransf * headShadowMatrix * headTransformation, sproj, _light);

   GsMat bodyShadowMatrix;
   computeShadowMatrix(bodyShadowMatrix);
   bodyShadow.draw(stransf * bodyShadowMatrix  * bodyTransformation, sproj, _light);

   GsMat lArmShadowMatrix;
   computeShadowMatrix(lArmShadowMatrix);
   lArmShadow.draw(stransf * lArmShadowMatrix  * lArmTransformation, sproj, _light);

   GsMat rArmShadowMatrix;
   computeShadowMatrix(rArmShadowMatrix);
   rArmShadow.draw(stransf * rArmShadowMatrix  * rArmTransformation, sproj, _light);

   GsMat lLegShadowMatrix;
   computeShadowMatrix(lLegShadowMatrix);
   lLegShadow.draw(stransf * lLegShadowMatrix  * lLegTransformation, sproj, _light);

   GsMat rLegShadowMatrix;
   computeShadowMatrix(rLegShadowMatrix);
   rLegShadow.draw(stransf * rLegShadowMatrix  * rLegTransformation, sproj, _light);

   //GsMat backgroundMatrix;
   //computeBackgroundTransformation(backgroundMatrix);
   //background.draw(stransf*backgroundMatrix, sproj, _light);

   // Swap buffers and draw:
   glFlush();         // flush the pipeline (usually not necessary)
   glutSwapBuffers(); // we were drawing to the back buffer, now bring it to the front
}

void AppWindow::computeCloudTransformation(GsMat& transformation) {

	GsMat movement;

	movement.translation(curvePath[curveIndex]);

	transformation = movement;

}

void AppWindow::computeHeadTransformation(GsMat& transformation) {

	GsMat rotation1;

	rotation1.roty((3.1415 / 50.0) * headRotate);


	GsMat rotation2;

	rotation2.roty((3.1415 / 50.0) * bodyRotate);


	GsMat movement;

	movement.translation(GsVec(moveX, moveY, moveZ));

	transformation = movement * rotation2 * rotation1;

}

void AppWindow::computeLArmTransformation(GsMat& transformation) {

	GsMat translateFix1;

	translateFix1.translation(GsVec(0.0, -5.3, 1.1) * scale);

	GsMat rotationFix;

	rotationFix.rotx(-1.57);

	GsMat translateFix2;

	translateFix2.translation(GsVec(0.0, 6.5, -1.1) * scale);


	GsMat rotation1;

	rotation1.rotz((3.1415 / 50.0) * lArmRotate);


	GsMat rotation2;

	rotation2.roty((3.1415 / 50.0) * bodyRotate);


	GsMat movement;

	movement.translation(GsVec(moveX, moveY, moveZ));

	transformation = movement * rotation2 * translateFix2 * rotation1 * rotationFix * translateFix1;

}

void AppWindow::computeRArmTransformation(GsMat& transformation) {

	GsMat translateFix1;

	translateFix1.translation(GsVec(0.0, -5.3, -1.1) * scale);

	GsMat rotationFix;

	rotationFix.rotx(1.57);

	GsMat translateFix2;

	translateFix2.translation(GsVec(0.0, 6.5, 1.1) * scale);



	GsMat rotation1;

	rotation1.rotz((3.1415 / 50.0) * rArmRotate);


	GsMat rotation2;

	rotation2.roty((3.1415 / 50.0) * bodyRotate);


	GsMat movement;

	movement.translation(GsVec(moveX, moveY, moveZ));

	transformation = movement * rotation2 * translateFix2 * rotation1 * rotationFix * translateFix1;

}

void AppWindow::computeLLegTransformation(GsMat& transformation) {

	GsMat translate1;

	translate1.translation(GsVec(0.0, -3.75, 0.0) * scale);

	GsMat rotation1;

	rotation1.rotz((3.1415 / 50.0) * lLegRotate);

	GsMat rotation2;

	rotation2.roty((3.1415 / 50.0) * bodyRotate);

	GsMat movement;

	movement.translation(GsVec(moveX, moveY, moveZ));

	transformation = movement * translate1.inverse() * rotation2 * rotation1 * translate1;

}

void AppWindow::computeRLegTransformation(GsMat& transformation) {

	GsMat translate1;

	translate1.translation(GsVec(0.0, -3.75, 0.0) * scale);

	GsMat rotation1;

	rotation1.rotz((3.1415 / 50.0) * rLegRotate);

	GsMat rotation2;

	rotation2.roty((3.1415 / 50.0) * bodyRotate);

	GsMat movement;

	movement.translation(GsVec(moveX, moveY, moveZ));

	transformation = movement * translate1.inverse() * rotation2 * rotation1 * translate1;

}

void AppWindow::computeBodyTransformation(GsMat& transformation) {

	GsMat rotation1;

	rotation1.roty((3.1415 / 50.0) * bodyRotate);

	GsMat movement;

	movement.translation(GsVec(moveX, moveY, moveZ));

	transformation = movement * rotation1;

}

void AppWindow::computeBackgroundTransformation(GsMat& transformation) {

	GsMat translate1;

	translate1.translation(GsVec(0.0, -0.01, 0.0));

	transformation = translate1;

}

void AppWindow::computeShadowMatrix(GsMat& shadowMatrix) {


	float groundDotLight;

	float ground[4] = { 0, 0.25, 0, 0 };
	float light[4] = { _light.pos[0], _light.pos[1], _light.pos[2], 0 };

	groundDotLight = ground[0] * light[0] + ground[1] * light[1] + ground[2] * light[2] + ground[3] * light[3];

	GsMat Proj;

	Proj.e[0] = groundDotLight - light[0] * ground[0];
	Proj.e[1] = 0.0 - light[0] * ground[1];
	Proj.e[2] = 0.0 - light[0] * ground[2];
	Proj.e[3] = 0.0 - light[0] * ground[3];

	Proj.e[4] = 0.0 - light[1] * ground[0];
	Proj.e[5] = groundDotLight - light[1] * ground[1];
	Proj.e[6] = 0.0 - light[1] * ground[2];
	Proj.e[7] = 0.0 - light[1] * ground[3];

	Proj.e[8] = 0.0 - light[2] * ground[0];
	Proj.e[9] = 0.0 - light[2] * ground[1];
	Proj.e[10] = groundDotLight - light[2] * ground[2];
	Proj.e[11] = 0.0 - light[2] * ground[3];

	Proj.e[12] = 0.0 - light[3] * ground[0];
	Proj.e[13] = 0.0 - light[3] * ground[1];
	Proj.e[14] = 0.0 - light[3] * ground[2];
	Proj.e[15] = groundDotLight - light[3] * ground[3];


	shadowMatrix = Proj;

}

void AppWindow::glutIdle() {

	seconds = glutGet(GLUT_ELAPSED_TIME) / 1000.0;

	if (camMode) {

		if (seconds > frames) {

			frames += 0.03;
			_roty += 0.03;

		}

	}

	if (seconds > cloudFrames) {

		cloudFrames += 0.1;
		if (curveIndex == curvePath.size() - 1) {
			curveIndex = 0;
		}
		else {
			curveIndex++;
		}

	}

	glutPostRedisplay();

}