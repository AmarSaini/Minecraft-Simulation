
// Ensure the header file is included only once in multi-file projects
#ifndef APP_WINDOW_H
#define APP_WINDOW_H

# include <gsim/gs_color.h>
# include <gsim/gs_array.h>
# include <gsim/gs_light.h>
# include <gsim/gs_vec.h>
# include "ogl_tools.h"
# include "glut_window.h"
# include "so_axis.h"
# include "so_model.h"
# include "so_model2.h"
# include "so_model3.h"
# include "so_cubes.h"
# include "curve_eval.h"
# include "so_cloud.h"

class chunkStruc {
public:
	SoCubes* myChunk;
	double topBound;
	double rightBound;
	bool drawnTop;
	bool drawnRight;

	chunkStruc() {

	}

	chunkStruc(SoCubes* myChunk1, double topBound1, double rightBound1) {

		myChunk = myChunk1;
		topBound = topBound1;
		rightBound = rightBound1;
		drawnTop = false;
		drawnRight = false;

	}

};

// The functionality of your application should be implemented inside AppWindow
class AppWindow : public GlutWindow
 { private :
    // OpenGL shaders and programs:
//    GlShader _vshflat, _fshflat, _vshgou, _fshgou, _vshphong, _fshphong;
  //  GlProgram _progflat, _proggou, _progphong;

    // My scene objects:
    SoAxis _axis;
    SoModel head, body, lArm, rArm, lLeg, rLeg;
	SoModel2 headShadow, bodyShadow, lArmShadow, rArmShadow, lLegShadow, rLegShadow;
	SoModel3 background;

	Terrain data;

	vector<chunkStruc*> myChunks;
	vector<GsMat*> chunkLocs;

	SoCloud myCloud;

	GsArray<GsVec> myCurvePoints;
	GsArray<GsVec> curvePath;

    // Scene data:
    bool  _viewaxis;
	GsModel gsm1, gsm2, gsm3, gsm4, gsm5, gsm6;
	GsModel gsm7, gsm8, gsm9, gsm10, gsm11, gsm12;
	GsModel gsm13;
    GsLight _light;
    
    // App data:
    enum MenuEv { evOption0, evOption1 };
    float _rotx, _roty, _fovy;
    int _w, _h;

	float scale;
	int headRotate, lArmRotate, rArmRotate;
	int bodyRotate, lLegRotate, rLegRotate;
	float moveX, moveY, moveZ;
	bool lLegCheck, rLegCheck;
	bool lArmCheck, rArmCheck;
	int moving;
	float seconds;
	float frames;
	float cloudFrames;
	bool camMode;
	int curveIndex;

   public :
    AppWindow ( const char* label, int x, int y, int w, int h );
    void initPrograms ();
    void loadModel ( int model );
	void animate();
    GsVec2 windowToScene ( const GsVec2& v );
	void checkNewChunks();
	void computeCloudTransformation(GsMat& transformation);
	void computeHeadTransformation(GsMat& transformation);
	void computeLArmTransformation(GsMat& transformation);
	void computeRArmTransformation(GsMat& transformation);
	void computeLLegTransformation(GsMat& transformation);
	void computeRLegTransformation(GsMat& transformation);
	void computeBodyTransformation(GsMat& transformation);
	void computeBackgroundTransformation(GsMat& transformation);
	void computeShadowMatrix(GsMat& shadowMatrix);

   private : // functions derived from the base class
    virtual void glutMenu ( int m );
    virtual void glutKeyboard ( unsigned char key, int x, int y );
    virtual void glutSpecial ( int key, int x, int y );
    virtual void glutMouse ( int b, int s, int x, int y );
    virtual void glutMotion ( int x, int y );
    virtual void glutDisplay ();
    virtual void glutReshape ( int w, int h );
	virtual void glutIdle();
 };

#endif // APP_WINDOW_H
