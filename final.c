/*
 *  FinalProject
 *
 *  m             Toggles light (sun) movement
 *  p             Toggles first person/perspective projection
 *  +/-           Change field of view of perspective
 *  x             Toggle axes display
 *  arrow keys    Change view angle
 *  PgDn/PgUp     Zoom in and out (in perspective view)
 *  0             Reset view angle
 * 	1			  view angle primary
 *  2			  View Angle titik 0.0.0
 *  3			  View Angle di atas mobil
 *  4			  View Angle pengkolan
 *  a			  View Angle X+
 *  d			  View Angle X-
 *  w			  View Angle Y+
 *  s			  View Angle Y-
 *  q			  View Angle Z+
 *  e			  View Angle Z-
 *  ESC           Exit
 *
 */
#include "JTKPOLBAN.h"
#include "loadtexbmp.c"
#include "fatal.c"
#include "Errcheck.c"
#include "print.c"
#include "project.c"
#ifndef GL_CLAMP_TO_EDGE
	#define GL_CLAMP_TO_EDGE 0x812F
#endif
#define CAR_SUM 2



int carArrayPosition[CAR_SUM] = {0,0};
Point positionCar [CAR_SUM] = {{-2, 0.3, 2}, {-3.000000, 0.300000, 6.000000}};
float speedCar [CAR_SUM] = {0.025, 0.05};
//routeCar carAnimation[CAR_SUM];

routeCar carAnimation[CAR_SUM][8] = { //masih mirror
{
	{{-2.000000, 0.300000, 2.000000}, 	{-2.000000, 0.300000, -30.000000},  0,0,1,0}, //jalan1
	{{-2.000000, 0.300000, -30.000000}, {10.000000, 0.300000, -30.000000},	1,0,0,1}, //jalan2
	{{10.000000, 0.300000, -30.000000}, {10.000000, 0.300000, 6.000000}, 	0,0,1,1}, //jalan3
	{{10.000000, 0.300000, 6.000000}, 	{30.000000, 0.300000, 6.000000}, 	1,0,0,1}, //jalan4
	{{30.000000, 0.300000, 6.000000}, 	{30.000000, 0.300000, 18.000000}, 	0,0,1,1}, //jalan5
	{{30.000000, 0.300000, 18.000000}, 	{-22.000000, 0.300000, 18.000000}, 	1,0,0,0}, //jalan6
	{{-22.000000, 0.300000, 18.000000},	{-22.000000, 0.300000, 6.000000}, 	0,0,1,0}, //jalan7
	{{-22.000000, 0.300000, 6.000000}, 	{-2.000000, 0.300000, 6.000000}, 	1,0,0,1} //jalan8
},

{
	{{-3.000000, 0.300000, 6.000000}, 	{-23.000000, 0.300000, 6.000000},	1,0,0,0}, //jalan1
	{{-23.000000, 0.300000, 6.000000}, {-23.000000, 0.300000, 19.000000},	0,0,1,1}, //jalan2
	{{-23.000000, 0.300000, 19.000000}, {31.000000, 0.300000, 19.000000},	1,0,0,1}, //jalan3
	{{31.000000, 0.300000, 19.000000}, 	{31.000000, 0.300000, 5.000000},	0,0,1,0}, //jalan4
	{{30.000000, 0.300000, 5.000000}, 	{11.000000, 0.300000, 5.000000},	1,0,0,0}, //jalan5
	{{11.000000, 0.300000, 5.000000},	{11.000000, 0.300000, -30.000000},	0,0,1,0}, //jalan6
	{{10.000000, 0.300000, -30.000000},	{-3.000000, 0.300000, -30.000000},	1,0,0,0}, //jalan7
	{{-3.00000, 0.300000, -30.000000},	{-3.00000, 0.300000, 2.000000},	0,0,1,1} //jalan8
}
};

int axes=0;       //  Display axes
int mode=0;       //  Projection mode
int move=1;       //  Move light
int th=110;         //  Azimuth of view angle
int ph=0;         //  Elevation of view angle
int fov=55;       //  Field of view (for perspective)
int light=1;      //  Lighting
double asp=1.333;  //  Aspect ratio
double dim=8.0;   //  Size of world

// Light values
int one       =   1;  // Unit value
int distance  =   25;  // Light distance
int inc       =  10;  // Ball increment
int smooth    =   1;  // Smooth/Flat shading
int local     =   0;  // Local Viewer Model
int emission  =  30;  // Emission intensity (%)
int ambient   =  80;  // Ambient intensity (%)
int diffuse   = 100;  // Diffuse intensity (%)
int specular  =   0;  // Specular intensity (%)
int shininess =   0;  // Shininess (power of two)
float shiny   =   1;  // Shininess (value)
int zh        =  90;  // Light azimuth
float ylight  = 13;  // Elevation of light

int at0=100;      //  Constant  attenuation %
int at1=20;        //  Linear    attenuation %
int at2=20;        //  Quadratic attenuation %

double fpMoveInc = 0.02; //Multiplier for how much to move each keystroke in FP mode

//First person camera location
double fpX = 0;
double fpY = 0.7;
double fpZ = -0.3;

//x, y, z for refrence point in glLookAt() for FP mode
double refX = 5;
double refY = 5;
double refZ = 0;

//Texture Variables
int tMode = 0;
float texScale = 0.5;
GLuint
_textureSkyboxBack,
_textureSkyboxFront,
_textureSkyboxLeft,
_textureSkyboxRight,
_textureSkyboxTop,
_textureGlass,
_textureWoodFence,
_textureAsphalt,
_textureBasicMetal,
_textureBrownBrick,
_textureCarWheel,
_textureCinderBlock,
_textureGarageDoor,
_textureGrass,
_textureGreyBrick,
_textureHedge,
_textureSidewalk,
_textureWalkway,
_textureWhiteBrick,
_textureCarGrill,
_textureFrontDoor2,
_textureWindow1,
_textureMetalRoof,
_textureTireTread,
_textureWarehouseWindow,
_textureHeadLamp,
_textureCarbonFiber,
_textureWoodBeam,
_textureStartLine,
_textureBend,
_textureSand,
_textureUltraJaya,
_textureOishi,
_textureLemonia,
_textureKeepSmile,
_textureFresTea,
_textureChocodot,
_textureChitato,
_textureBurgerKing,
_textureUsb,
_textureNikitaNabila;

//Light Vecotrs
float Ambient[]   = {0.01*80 ,0.01*80 ,0.01*80 ,1.0};
float Diffuse[]   = {1.0,1.0,1.0,1.0};
float Specular[]  = {0.01*0,0.01*0,0.01*0,1.0};

int compareFloat(float f1, float f2){
	float epsilon = 0.001;
	return fabs(f1 - f2) < epsilon;
}

/*
 *  Draw a cube
 *     at (x,y,z)
 *     dimensions (dx,dy,dz)
 *     rotated th about the y axis
 */
static void cube(double x,double y,double z,
                 double dx,double dy,double dz,
                 double th)
{
   //  Save transformation
   glPushMatrix();
   //  Offset, scale and rotate
   glTranslated(x,y,z);
   glRotated(th,0,1,0);
   glScaled(dx,dy,dz);

   //Texture repitition values
   float texRepX = 1.0;
   float texRepY = 1.0;

   //  Cube
   glBegin(GL_QUADS);
   //  Front
   texRepX = dx/texScale;
   texRepY = dy/texScale;
   glNormal3f( 0, 0, 1);
   glTexCoord2f(0.0,0.0); glVertex3f(-1,-1, 1);
   glTexCoord2f(texRepX,0.0); glVertex3f(+1,-1, 1);
   glTexCoord2f(texRepX,texRepY); glVertex3f(+1,+1, 1);
   glTexCoord2f(0.0,texRepY); glVertex3f(-1,+1, 1);
   //  Back
   texRepX = dx/texScale;
   texRepY = dy/texScale;
   glNormal3f( 0, 0,-1);
   glTexCoord2f(0.0,0.0); glVertex3f(+1,-1,-1);
   glTexCoord2f(texRepX,0.0); glVertex3f(-1,-1,-1);
   glTexCoord2f(texRepX,texRepY); glVertex3f(-1,+1,-1);
   glTexCoord2f(0.0,texRepY); glVertex3f(+1,+1,-1);
   //  Right
   texRepX = dz/texScale;
   texRepY = dy/texScale;
   glNormal3f(+1, 0, 0);
   glTexCoord2f(0.0,0.0); glVertex3f(+1,-1,+1);
   glTexCoord2f(texRepX,0.0); glVertex3f(+1,-1,-1);
   glTexCoord2f(texRepX,texRepY); glVertex3f(+1,+1,-1);
   glTexCoord2f(0.0,texRepY); glVertex3f(+1,+1,+1);
   //  Left
   texRepX = dz/texScale;
   texRepY = dy/texScale;
   glNormal3f(-1, 0, 0);
   glTexCoord2f(0.0,0.0); glVertex3f(-1,-1,-1);
   glTexCoord2f(texRepX,0.0); glVertex3f(-1,-1,+1);
   glTexCoord2f(texRepX,texRepY); glVertex3f(-1,+1,+1);
   glTexCoord2f(0.0,texRepY); glVertex3f(-1,+1,-1);
   //  Top
   texRepX = dx/texScale;
   texRepY = dz/texScale;
   glNormal3f( 0,+1, 0);
   glTexCoord2f(0.0,0.0); glVertex3f(-1,+1,+1);
   glTexCoord2f(texRepX,0.0); glVertex3f(+1,+1,+1);
   glTexCoord2f(texRepX,texRepY); glVertex3f(+1,+1,-1);
   glTexCoord2f(0.0,texRepY); glVertex3f(-1,+1,-1);
   //  Bottom
   texRepX = dx/texScale;
   texRepY = dz/texScale;
   glNormal3f( 0,-one, 0);
   glTexCoord2f(0.0,0.0); glVertex3f(-1,-1,-1);
   glTexCoord2f(texRepX,0.0); glVertex3f(+1,-1,-1);
   glTexCoord2f(texRepX,texRepY); glVertex3f(+1,-1,+1);
   glTexCoord2f(0.0,texRepY); glVertex3f(-1,-1,+1);
   //  End
   glEnd();
   //  Undo transofrmations
   glPopMatrix();
}



/*
 *  Draw a 
 *     at (x,y,z)
 *     dimensions (dx,dy,dz)
 *     rotated th about the y axis
 */
static void circle(double x,double y,double z,
                 double dx,double dy,double dz,
                 double th, double radius)
{
   //  Save transformation
   glPushMatrix();
   //  Offset, scale and rotate
   glTranslated(x,y,z);
   glRotated(th,0,1,0);
   glScaled(dx,dy,dz);
   
   

   //Texture repitition values
   float texRepX = 1.0;
   float texRepY = 1.0;

   texRepX = dx/texScale;
   texRepY = dz/texScale;
	int i;
	float x_now; 
			float z_now;
	const unsigned int triangles = 20; // number of triangles
	const float twoPi = 2.0f * 3.14159f;
	float delta = twoPi / triangles;
	// Top
	glBegin(GL_TRIANGLE_FAN);
	glVertex3f(0.0,+1.0,0.0); // origin
	for(i = 0; i <= triangles/4; i++){
		x_now =  (radius * cos((i) *  delta));
		z_now =  (radius * sin((i) * delta));
		glTexCoord2f(x_now,z_now); glVertex3f(x_now,+1.0,z_now);
	}
	glEnd();
	
	// Bottom
	glBegin(GL_TRIANGLE_FAN);
	glVertex3f(0.0,-1.0,0.0); // origin
	for(i = 0; i <= triangles/4; i++){
		x_now =  (radius * cos((i) *  delta));
		z_now =  (radius * sin((i) * delta));
		glTexCoord2f(x_now,z_now); glVertex3f(x_now,-1.0,z_now);
	}
	glEnd();
	
	// Side Circular
	glBegin(GL_TRIANGLE_STRIP);
		for(i = 0; i <= triangles/4;i++) {
			x_now =  (radius * cos((i) *  delta));
			z_now =  (radius * sin((i) * delta));
			glTexCoord2f(x_now,z_now); glVertex3f(x_now,+1.0,z_now);
			glTexCoord2f(x_now,z_now); glVertex3f(x_now,-1.0,z_now);
		}
	glEnd();
   glPopMatrix();
}


/*
 *  Draw vertex in polar coordinates with normal
 */
static void Vertex(double th,double ph)
{
   double x = Sin(th)*Cos(ph);
   double y = Cos(th)*Cos(ph);
   double z =         Sin(ph);
   //  For a sphere at the origin, the position
   //  and normal vectors are the same
   glNormal3d(x,y,z);
   glVertex3d(x,y,z);
}

/*
 *  Draw a ball
 *     at (x,y,z)
 *     radius (r)
 */
static void ball(double x,double y,double z,double r)
{
   int th,ph;
   float yellow[] = {1.0,1.0,0.0,1.0};
   float Emission[]  = {1.0,1.0,1.0,1.0};
   //  Save transformation
   glPushMatrix();
   //  Offset, scale and rotate
   glTranslated(x,y,z);
   glScaled(r,r,r);
   //  White ball
   glColor3f(1,1,0.9);
   glMaterialf(GL_FRONT,GL_SHININESS,shiny);
   glMaterialfv(GL_FRONT,GL_SPECULAR,yellow);
   glMaterialfv(GL_FRONT,GL_EMISSION,Emission);
   //  Bands of latitude
   for (ph=-90;ph<90;ph+=inc)
   {
      glBegin(GL_QUAD_STRIP);
      for (th=0;th<=360;th+=2*inc)
      {
         Vertex(th,ph);
         Vertex(th,ph+inc);
      }
      glEnd();
   }
   //  Undo transofrmations
   glPopMatrix();
}

/*
 *  Draw a section of the car body
 *     at (x,y,z)
 *     dimensions (dx,dy,dz)
 *     rotated th about the y axis
 *     w (1 to color windows for car body, 0 otherwise)
 */
static void body(double x,double y,double z,
                 double dx,double dy,double dz,
                 double th,
                 int w)
{
   //  Set specular color to white
   float white[] = {1,1,1,1};
   float black[] = {0,0,0,1};
   glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS,shiny);
   glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,white);
   glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,black);

   glBindTexture(GL_TEXTURE_2D,_textureBasicMetal);
   texScale = 0.1;

   glEnable(GL_POLYGON_OFFSET_FILL);

   //  Save transformation
   glPushMatrix();
   //  Offset
   glTranslated(x,y,z);
   glRotated(th,0,1,0);
   glScaled(dx,dy,dz);

   //Texture repitition values
   float texRepX = 1.0;
   float texRepY = 1.0;

   //  Cube
   glBegin(GL_QUADS);

   //  Top
   texRepX = dx/texScale;
   texRepY = dz/texScale;
   glNormal3f( 0,+1, 0);
   glTexCoord2f(0.0,0.0); glVertex3f(-1,+1,+1);
   glTexCoord2f(texRepX,0.0); glVertex3f(+1,+1,+1);
   glTexCoord2f(texRepX,texRepY); glVertex3f(+1,+1,-1);
   glTexCoord2f(0.0,texRepY); glVertex3f(-1,+1,-1);
   //  Bottom
   texRepX = dx/texScale;
   texRepY = dz/texScale;
   glNormal3f( 0,-one, 0);
   glTexCoord2f(0.0,0.0); glVertex3f(-1,-1,-1);
   glTexCoord2f(texRepX,0.0); glVertex3f(+1,-1,-1);
   glTexCoord2f(texRepX,texRepY); glVertex3f(+1,-1,+1);
   glTexCoord2f(0.0,texRepY); glVertex3f(-1,-1,+1);

   //  Front
   texRepX = dx/texScale;
   texRepY = dy/texScale;
   glNormal3f( 0, 0, 1);
   glTexCoord2f(0.0,0.0); glVertex3f(-1,-1, 1);
   glTexCoord2f(texRepX,0.0); glVertex3f(+1,-1, 1);
   glTexCoord2f(texRepX,texRepY); glVertex3f(+1,+1, 1);
   glTexCoord2f(0.0,texRepY); glVertex3f(-1,+1, 1);
   //  Back
   texRepX = dx/texScale;
   texRepY = dy/texScale;
   glNormal3f( 0, 0,-1);
   glTexCoord2f(0.0,0.0); glVertex3f(+1,-1,-1);
   glTexCoord2f(texRepX,0.0); glVertex3f(-1,-1,-1);
   glTexCoord2f(texRepX,texRepY); glVertex3f(-1,+1,-1);
   glTexCoord2f(0.0,texRepY); glVertex3f(+1,+1,-1);

   //  End
   glEnd();

   glDisable(GL_POLYGON_OFFSET_FILL);
   //Color and texture to add windows
   if(w == 1) {
      glColor3f(0.8, 0.8, 1);
      glBindTexture(GL_TEXTURE_2D,_textureGlass);
      texRepX = 1.0;
      texRepY = 1.0;
      glBegin(GL_QUADS);
      //  Front
      glNormal3f(0, 0, 1);
      glTexCoord2f(0.0,0.0); glVertex3f(-0.8,-1, 1);
      glTexCoord2f(texRepX,0.0); glVertex3f(+0.8,-1, 1);
      glTexCoord2f(texRepX,texRepY); glVertex3f(+0.8,+1, 1);
      glTexCoord2f(0.0,texRepY); glVertex3f(-0.8,+1, 1);
      //  Back
      glNormal3f(0, 0,-1);
      glTexCoord2f(0.0,0.0); glVertex3f(+0.8,-1,-1);
      glTexCoord2f(texRepX,0.0); glVertex3f(-0.8,-1,-1);
      glTexCoord2f(texRepX,texRepY); glVertex3f(-0.8,+1,-1);
      glTexCoord2f(0.0,texRepY); glVertex3f(+0.8,+1,-1);
      glEnd();
   }
   glEnable(GL_POLYGON_OFFSET_FILL);
   
   //  Undo transformations
   glPopMatrix();
}

static void wheel(double x,double y,double z,
                 double dx,double dy,double dz,
                 double th,
                 int d,
                 int s)
{
   //  Set specular color to white
   float white[] = {1,1,1,1};
   float black[] = {0,0,0,1};
   //Turn off shininess for the rubber tire
   glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS,0);
   glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,white);
   glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,black);

   //  Save transformation
   glPushMatrix();
   //  Offset
   glTranslated(x,y,z);
   glRotated(th,0,1,0);
   glScaled(dx,dy,dz);

   glBindTexture(GL_TEXTURE_2D,_textureCarWheel);

   glColor3f(0.8,0.8,0.8);
   //Tire
   glBegin(GL_TRIANGLE_FAN);
   glNormal3f(0, 0, -1);
   glTexCoord2f(0.5,0.5);
   glVertex3f(0, 0, -0.05);
   for (th=0;th<=360;th+=s)
   {
      double ph = d-90;
      glTexCoord2f(0.5*Cos(th)+0.5,0.5*Sin(th)+0.5);
      glVertex3d(Sin(th)*Cos(ph), Cos(th)*Cos(ph), -0.05);
   }
   glEnd();

   glBegin(GL_TRIANGLE_FAN);
   glNormal3f(0, 0, 1);
   glTexCoord2f(0.5,0.5);
   glVertex3f(0, 0, 0.05);
   for (th=360;th>=0;th-=s)
   {
      double ph = d-90;
      glTexCoord2f(0.5*Cos(th)+0.5,0.5*Sin(th)+0.5);
      glVertex3d(Sin(th)*Cos(ph) , Cos(th)*Cos(ph), 0.05);
   }
   glEnd();

   glBindTexture(GL_TEXTURE_2D,_textureTireTread);

   glColor3f(0.5,0.5,0.55);
   glBegin(GL_QUAD_STRIP);
   for (th=0;th<=360;th+=s)
   {
      double ph = d-90;
      glNormal3f(Sin(th)*Cos(ph), Cos(th)*Cos(ph), 0);
      glTexCoord2f(0,0.1*th); glVertex3d(Sin(th)*Cos(ph), Cos(th)*Cos(ph), -0.05);
      glTexCoord2f(1,0.1*th); glVertex3d(Sin(th)*Cos(ph), Cos(th)*Cos(ph), 0.05);
   }
   // glNormal3f(Sin(0)*Cos(-90), Cos(0)*Cos(-90), 0);
   // glVertex3d(Sin(0)*Cos(-90), Cos(0)*Cos(-90), -0.05);
   // glVertex3d(Sin(0)*Cos(-90), Cos(0)*Cos(-90), 0.05);
   glEnd();

   //  Undo transformations
   glPopMatrix();
}

static void bumper(double x,double y,double z,
                 double dx,double dy,double dz,
                 double th,
                 int m)
{
   //  Set specular color to white
   float white[] = {1,1,1,1};
   float black[] = {0,0,0,1};
   glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS,shiny);
   glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,white);
   glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,black);

   //  Save transformation
   glPushMatrix();
   //  Offset
   glTranslated(x,y,z);
   glRotated(th,0,1,0);
   glScaled(dx,dy,dz);

   //Texture repitition values
   float texRepX = 1.0;
   float texRepY = 1.0;

   glBindTexture(GL_TEXTURE_2D,_textureBasicMetal);
   texScale = 0.1;

   //Offset the bumper so that the lights and grill are drawn directly on the surface
   glEnable(GL_POLYGON_OFFSET_FILL);

   //Bumper

   //Base
   texRepX = dx/texScale;
   texRepY = dy/texScale;
   glBegin(GL_POLYGON);
   glNormal3f(0, -1, 0);
   glTexCoord2f(0.0,0.0); glVertex3f(0,0,0.4);
   glTexCoord2f(texRepX,0.0); glVertex3f(0,0,-0.4);
   glTexCoord2f(texRepX,texRepY); glVertex3f(0.1,0,-0.35);
   glTexCoord2f(0.0,texRepY); glVertex3f(0.1,0,0.35);
   glEnd();

   //Front Panels
   glBegin(GL_QUADS);
   texRepX = dx/texScale;
   texRepY = dy/texScale;
   glNormal3f(0.447214, 0, 0.894427);
   glTexCoord2f(0.0,0.0); glVertex3f(0.1, 0, 0.35);
   glTexCoord2f(texRepX,0.0); glVertex3f(0.1, 0.2, 0.35);
   glTexCoord2f(texRepX,texRepY); glVertex3f(0, 0.2, 0.4);
   glTexCoord2f(0.0,texRepY); glVertex3f(0, 0, 0.4);

   texRepX = dx/texScale;
   texRepY = dy/texScale;
   glNormal3f(1, 0, 0);
   glTexCoord2f(0.0,0.0); glVertex3f(0.1, 0, -0.35);
   glTexCoord2f(texRepX,0.0); glVertex3f(0.1, 0.2, -0.35);
   glTexCoord2f(texRepX,texRepY); glVertex3f(0.1, 0.2, 0.35);
   glTexCoord2f(0.0,texRepY); glVertex3f(0.1, 0, 0.35);

   texRepX = dx/texScale;
   texRepY = dy/texScale;
   glNormal3f(0.447214, 0, -0.894427);
   glTexCoord2f(0.0,0.0); glVertex3f(0.1, 0, -0.35);
   glTexCoord2f(texRepX,0.0); glVertex3f(0, 0, -0.4);
   glTexCoord2f(texRepX,texRepY); glVertex3f(0, 0.2, -0.4);
   glTexCoord2f(0.0,texRepY); glVertex3f(0.1, 0.2, -0.35);
   
   glEnd();

   //Upper Bumper
   glBegin(GL_QUADS);
   texRepX = dx/texScale;
   texRepY = dy/texScale;
   glNormal3f(0.447214, 0.894427, 0);
   glTexCoord2f(0.0,0.0); glVertex3f(0, 0.25, 0.35);
   glTexCoord2f(texRepX,0.0); glVertex3f(0.1, 0.2, 0.35);
   glTexCoord2f(texRepX,texRepY); glVertex3f(0.1, 0.2, -0.35);
   glTexCoord2f(0.0,texRepY); glVertex3f(0, 0.25, -0.35);
   glEnd();

   glBegin(GL_TRIANGLES);
   texRepX = dx/texScale;
   texRepY = dy/texScale;
   glNormal3f(0.333333, 0.666667, 0.666667);
   glTexCoord2f(0.0, 0.0); glVertex3f(0, 0.2, 0.4);
   glTexCoord2f(texRepX/2, texRepY); glVertex3f(0.1, 0.2, 0.35);
   glTexCoord2f(texRepX, 0.0); glVertex3f(0, 0.25, 0.35);

   texRepX = dx/texScale;
   texRepY = dy/texScale;
   glNormal3f(0.333333, 0.666667, -0.666667);
   glTexCoord2f(0.0, 0.0); glVertex3f(0, 0.25, -0.35);
   glTexCoord2f(texRepX/2, texRepY); glVertex3f(0.1, 0.2, -0.35);
   glTexCoord2f(texRepX, 0.0); glVertex3f(0, 0.2, -0.4);
   glEnd();

   //  Disable polygon offset
   glDisable(GL_POLYGON_OFFSET_FILL);

   if (m == 1) {
      glColor3f(0.2,0.2,0.2);
      glBindTexture(GL_TEXTURE_2D,_textureCarGrill);

      //Grill
      glBegin(GL_QUADS);
      glNormal3f(1, 0, 0);
      glTexCoord2f(0.0,0.0); glVertex3f(0.1, 0.15, 0.18);
      glTexCoord2f(0.5,0.0); glVertex3f(0.1, 0.03, 0.18);
      glTexCoord2f(0.5,1.0); glVertex3f(0.1, 0.03, -0.18);
      glTexCoord2f(0.0,1.0); glVertex3f(0.1, 0.15, -0.18);
      glEnd();
   }

   //Lights (taillights or headlights)
   float emColor[4];
   if(m == 1) {
      emColor[0] = 1.0 * emission;
      emColor[1] = 1.0 * emission;
      emColor[2] = 0.8 * emission;
      emColor[3] = 1.0 * emission;
      glColor3f(1, 1, 0.8);
   } else {
      emColor[0] = 0.8 * emission;
      emColor[1] = 0.0 * emission;
      emColor[2] = 0.0 * emission;
      emColor[3] = 1.0 * emission;
      glColor3f(.8, 0, 0);
   }

   glMaterialf(GL_FRONT,GL_SHININESS,0);
   glMaterialfv(GL_FRONT,GL_SPECULAR,emColor);
   glMaterialfv(GL_FRONT,GL_EMISSION,emColor);

   glBindTexture(GL_TEXTURE_2D,_textureHeadLamp);

   glBegin(GL_TRIANGLE_FAN);
   glNormal3f(1, 0, 0);
   glTexCoord2f(0.5,0.5); glVertex3f(0.1, 0.13, -0.25);
   for (th=0;th<=360;th+=10)
   {
      double ph = 3-90;
      glTexCoord2f(0.5*Cos(th)+0.5,0.5*Sin(th)+0.5);
      glVertex3d(0.1, 0.13+(Cos(th)*Cos(ph)), -0.25+(Sin(th)*Cos(ph)));
   }
   glEnd();

   glBegin(GL_TRIANGLE_FAN);
   glNormal3f(1, 0, 0);
   glTexCoord2f(0.5,0.5); glVertex3f(0.1, 0.13, 0.25);
   for (th=0;th<=360;th+=10)
   {
      double ph = 3-90;
      glTexCoord2f(0.5*Cos(th)+0.5,0.5*Sin(th)+0.5);
      glVertex3d(0.1, 0.13+(Cos(th)*Cos(ph)), 0.25+(Sin(th)*Cos(ph)));
   }
   glEnd();

   glEnable(GL_POLYGON_OFFSET_FILL);

   glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS,shiny);
   glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,white);
   glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,black);

   //Undo transformations
   glPopMatrix();  
}

static void policeCar(double x,double y,double z,
                     double dx,double dy,double dz,
                     double th)
{
   //  Set specular color to white
   float white[] = {1,1,1,1};
   float black[] = {0,0,0,1};
   glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS,shiny);
   glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,white);
   glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,black);

   float cr = 0.1;
   float cb = 0.1;
   float cg = 0.1;

   //  Save transformation
   glPushMatrix();
   //  Offset
   glTranslated(x,y,z);
   glRotated(th,0,1,0);
   glScaled(dx,dy,dz);

   glPolygonOffset(1,1);

   wheel(0.6,0,0.4, 1,1,1, 0, 8, 10);
   wheel(-0.6,0,-0.4, 1,1,1, 0, 8, 10);
   wheel(0.6,0,-0.4, 1,1,1, 0, 8, 10);
   wheel(-0.6,0,0.4, 1,1,1, 0, 8, 10);

   glColor3f(cr, cb, cg);

   //Lower Body
   body(0,0.1,0, 0.8,0.1,0.4, 0, 0);
   //Cabin
//   glColor3f(cr, cb, cg);
   body(0,0.3,0, 0.2,0.1,0.35, 0, 1);
//   glColor3f(cr, cb, cg);
   body(-0.4,0.3,0, 0.2,0.1,0.35, 0, 1);

   texScale = 1.0;

   glColor3f(cr, cb, cg);

   //Front Bumper
   bumper(0.8,0,0, 1,1,1, 0, 1);

   glColor3f(cr, cb, cg);

   //Rear Bumper
   bumper(-0.8,0,0, 1,1,1, 180, 0);

   //  Set specular color to white
   glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS,shiny);
   glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,white);
   glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,black);
   
   glEnable(GL_POLYGON_OFFSET_FILL);

   glColor3f(cr, cb, cg);

   glBindTexture(GL_TEXTURE_2D,_textureBasicMetal);

   //Texture repitition values
   float texRepX = 1.0;
   float texRepY = 1.0;

   //Hood and upper side pannels
   texRepX = dx/texScale;
   texRepY = dz/texScale;
   glBegin(GL_QUADS);
   glNormal3f(0, 0.707107, 0.707107);
   glTexCoord2f(0.0,0.0); glVertex3f(-0.8, 0.25, 0.35);
   glTexCoord2f(texRepX,0.0); glVertex3f(-0.8, 0.2, 0.4);
   glTexCoord2f(texRepX,texRepY); glVertex3f(0.8, 0.2, 0.4);
   glTexCoord2f(0.0,texRepY); glVertex3f(0.8, 0.25, 0.35);

   glNormal3f(0, 1, 0);
   glTexCoord2f(0.0,0.0); glVertex3f(0.4, 0.25, 0.35);
   glTexCoord2f(texRepX,0.0); glVertex3f(0.8, 0.25, 0.35);
   glTexCoord2f(texRepX,texRepY); glVertex3f(0.8, 0.25, -0.35);
   glTexCoord2f(0.0,texRepY); glVertex3f(0.4, 0.25, -0.35);
   
   glNormal3f(0, 0.707107, -0.707107);
   glTexCoord2f(0.0,0.0); glVertex3f(-0.8, 0.2, -0.4);
   glTexCoord2f(texRepX,0.0); glVertex3f(-0.8, 0.25, -0.35);
   glTexCoord2f(texRepX,texRepY); glVertex3f(0.8, 0.25, -0.35);
   glTexCoord2f(0.0,texRepY); glVertex3f(0.8, 0.2, -0.4);
   glEnd();

   glBindTexture(GL_TEXTURE_2D,_textureGlass);

   glColor3f(0.8, 0.8, 1);

   //Windshield
   texRepX = 1.0;
   texRepY = 1.0;
   glBegin(GL_QUADS);
   glNormal3f(0.6, 0.8, 0);
   glTexCoord2f(0.0,0.0); glVertex3f(0.4,0.25,0.35);
   glTexCoord2f(texRepX,0.0); glVertex3f(0.4,0.25,-0.35);
   glTexCoord2f(texRepX,texRepY); glVertex3f(0.2,0.4,-0.35);
   glTexCoord2f(0.0,texRepY); glVertex3f(0.2,0.4,0.35);
   glEnd();

   glBegin(GL_TRIANGLES);
   glNormal3f(0,0,1);
   glTexCoord2f(0.0,0.0); glVertex3f(0.2,0.4,0.35);
   glTexCoord2f(texRepX, 0.0); glVertex3f(0.2,0.25,0.35);
   glTexCoord2f(texRepX, texRepY); glVertex3f(0.4,0.25,0.35);

   glNormal3f(0,0,-1);
   glTexCoord2f(0.0,0.0); glVertex3f(0.4,0.25,-0.35);
   glTexCoord2f(texRepX, 0.0); glVertex3f(0.2,0.25,-0.35);
   glTexCoord2f(texRepX, texRepY); glVertex3f(0.2,0.4,-0.35);
   glEnd();

   //Rear Window
   texRepX = 1.0;
   texRepY = 1.0;
   glBegin(GL_QUADS);
   glNormal3f(-0.6, 0.8, 0);
   glTexCoord2f(0.0,0.0); glVertex3f(-0.8,0.25,-0.35);
   glTexCoord2f(texRepX,0.0); glVertex3f(-0.8,0.25,0.35);
   glTexCoord2f(texRepX,texRepY); glVertex3f(-0.6,0.4,0.35);
   glTexCoord2f(0.0,texRepY); glVertex3f(-0.6,0.4,-0.35);
   glEnd();

   glBegin(GL_TRIANGLES);
   glNormal3f(0,0,1);
   glTexCoord2f(0.0,0.0); glVertex3f(-0.8,0.25,0.35);
   glTexCoord2f(texRepX, 0.0); glVertex3f(-0.6,0.25,0.35);
   glTexCoord2f(texRepX, texRepY); glVertex3f(-0.6,0.4,0.35);
   
   glNormal3f(0,0,-1);
   glTexCoord2f(0.0,0.0); glVertex3f(-0.6,0.4,-0.35);
   glTexCoord2f(texRepX, 0.0); glVertex3f(-0.6,0.25,-0.35);
   glTexCoord2f(texRepX, texRepY); glVertex3f(-0.8,0.25,-0.35);
   glEnd();

   float t = glutGet(GLUT_ELAPSED_TIME)/1000.0;

   //Light bar

   glBindTexture(GL_TEXTURE_2D,_textureGlass);
   //Mid section
   glColor3f(0.1, 0.1, 0.1);
   cube(-0.2,0.42,0, 0.07,0.02,0.05, 0);

   //  Enable light 1 - Police Light
   glEnable(GL_LIGHT1);
   
   if ((int)(t/0.5) % 2 == 0) {
      float amb[4] = {0,0,0,0};
      float dif[4] = {0.8,0,0,1};
      float spec[4] = {0,0,0,1};
      float pos[4] = {-0.2,0.26,-0.15,1.0};
      //Red Light
      glLightfv(GL_LIGHT1,GL_AMBIENT ,amb);
      glLightfv(GL_LIGHT1,GL_DIFFUSE ,dif);
      glLightfv(GL_LIGHT1,GL_SPECULAR,spec);
      glLightfv(GL_LIGHT1,GL_POSITION,pos);

      glLightf(GL_LIGHT1,GL_CONSTANT_ATTENUATION ,at0/100.0);
      glLightf(GL_LIGHT1,GL_LINEAR_ATTENUATION   ,at1/100.0);
      glLightf(GL_LIGHT1,GL_QUADRATIC_ATTENUATION,at2/100.0);

      //Red Light
      float redEm[4] = {0.8, 0, 0, 1.0};
      glMaterialf(GL_FRONT,GL_SHININESS,0);
      glMaterialfv(GL_FRONT,GL_SPECULAR,redEm);
      glMaterialfv(GL_FRONT,GL_EMISSION,redEm);
      glColor3f(0.5, 0, 0);
      cube(-0.2,0.42,-0.15, 0.07,0.02,0.1, 0);
      //Blue Light
      glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS,shiny);
      glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,white);
      glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,black);
      glColor3f(0, 0, 0.5);
      cube(-0.2,0.42,0.15, 0.07,0.02,0.1, 0);
   } else {
      float amb[4] = {0,0,0,0};
      float dif[4] = {0,0,0.8,1};
      float spec[4] = {0,0,0,1};
      float pos[4] = {-0.2,0.26,0.15,1.0};
      //Blue Light
      glLightfv(GL_LIGHT1,GL_AMBIENT ,amb);
      glLightfv(GL_LIGHT1,GL_DIFFUSE ,dif);
      glLightfv(GL_LIGHT1,GL_SPECULAR,spec);
      glLightfv(GL_LIGHT1,GL_POSITION,pos);

      glLightf(GL_LIGHT1,GL_CONSTANT_ATTENUATION ,at0/100.0);
      glLightf(GL_LIGHT1,GL_LINEAR_ATTENUATION   ,at1/100.0);
      glLightf(GL_LIGHT1,GL_QUADRATIC_ATTENUATION,at2/100.0);

      //Red Light
      glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS,shiny);
      glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,white);
      glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,black);
      glColor3f(0.5, 0, 0);
      cube(-0.2,0.42,-0.15, 0.07,0.02,0.1, 0);
      //Blue Light
      float blueEm[4] = {0, 0, 0.8, 1.0};
      glMaterialf(GL_FRONT,GL_SHININESS,0);
      glMaterialfv(GL_FRONT,GL_SPECULAR,blueEm);
      glMaterialfv(GL_FRONT,GL_EMISSION,blueEm);
      glColor3f(0, 0, 0.5);
      cube(-0.2,0.42,0.15, 0.07,0.02,0.1, 0);
   }

   glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS,shiny);
   glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,white);
   glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,black);

   //Undo transformations
   glPopMatrix();
}

static void car(double x,double y,double z,
                 double dx,double dy,double dz,
                 double th)
{
   //  Set specular color to white
   float white[] = {1,1,1,1};
   float black[] = {0,0,0,1};
//      float cr = 0.1;
//   float cb = 0.1;
//   float cg = 0.1;
   glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS,shiny);
   glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,white);
   glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,black);

   //  Save transformation
   glPushMatrix();
   //  Offset
   glTranslated(x,y,z);
   glRotated(th,0,1,0);
   glScaled(dx,dy,dz);

   glPolygonOffset(1,1);

   wheel(0.6,0,0.4, 1,1,1, 0, 8, 10);
   wheel(-0.6,0,-0.4, 1,1,1, 0, 8, 10);
   wheel(0.6,0,-0.4, 1,1,1, 0, 8, 10);
   wheel(-0.6,0,0.4, 1,1,1, 0, 8, 10);

//   glColor3f(cr, cb, cg);

   //Lower Body
   body(0,0.1,0, 0.8,0.1,0.4, 0, 0);
   //Cabin
   body(-0.1,0.3,0, 0.3,0.1,0.35, 0, 1);

   texScale = 1.0;

//   glColor3f(cr, cb, cg);

   //Front Bumper
   bumper(0.8,0,0, 1,1,1, 0, 1);

//   glColor3f(cr, cb, cg);

   //Rear Bumper
   bumper(-0.8,0,0, 1,1,1, 180, 0);

   //  Set specular color to white
   glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS,shiny);
   glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,white);
   glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,black);
   
   glEnable(GL_POLYGON_OFFSET_FILL);

//   glColor3f(cr, cb, cg);

   glBindTexture(GL_TEXTURE_2D,_textureBasicMetal);

   //Texture repitition values
   float texRepX = 1.0;
   float texRepY = 1.0;

   //Hood and upper side pannels
   texRepX = dx/texScale;
   texRepY = dz/texScale;
   glBegin(GL_QUADS);
   glNormal3f(0, 0.707107, 0.707107);
   glTexCoord2f(0.0,0.0); glVertex3f(-0.8, 0.25, 0.35);
   glTexCoord2f(texRepX,0.0); glVertex3f(-0.8, 0.2, 0.4);
   glTexCoord2f(texRepX,texRepY); glVertex3f(0.8, 0.2, 0.4);
   glTexCoord2f(0.0,texRepY); glVertex3f(0.8, 0.25, 0.35);

   glNormal3f(0, 1, 0);
   glTexCoord2f(0.0,0.0); glVertex3f(0.4, 0.25, 0.35);
   glTexCoord2f(texRepX,0.0); glVertex3f(0.8, 0.25, 0.35);
   glTexCoord2f(texRepX,texRepY); glVertex3f(0.8, 0.25, -0.35);
   glTexCoord2f(0.0,texRepY); glVertex3f(0.4, 0.25, -0.35);
   
   glNormal3f(0, 0.707107, -0.707107);
   glTexCoord2f(0.0,0.0); glVertex3f(-0.8, 0.2, -0.4);
   glTexCoord2f(texRepX,0.0); glVertex3f(-0.8, 0.25, -0.35);
   glTexCoord2f(texRepX,texRepY); glVertex3f(0.8, 0.25, -0.35);
   glTexCoord2f(0.0,texRepY); glVertex3f(0.8, 0.2, -0.4);
   glEnd();

   //Trunk
   texRepX = dx/texScale;
   texRepY = dz/texScale;
   glBegin(GL_QUADS);
   glNormal3f(0,1,0);
   glTexCoord2f(0.0,0.0); glVertex3f(-0.8, 0.25, -0.35);
   glTexCoord2f(texRepX,0.0); glVertex3f(-0.8, 0.25, 0.35);
   glTexCoord2f(texRepX,texRepY); glVertex3f(-0.6, 0.25, 0.35);
   glTexCoord2f(0.0,texRepY); glVertex3f(-0.6, 0.25, -0.35);
   glEnd();

   glBindTexture(GL_TEXTURE_2D,_textureGlass);

   glColor3f(0.8, 0.8, 1);

   //Windshield
   texRepX = 1.0;
   texRepY = 1.0;
   glBegin(GL_QUADS);
   glNormal3f(0.6, 0.8, 0);
   glTexCoord2f(0.0,0.0); glVertex3f(0.4,0.25,0.35);
   glTexCoord2f(texRepX,0.0); glVertex3f(0.4,0.25,-0.35);
   glTexCoord2f(texRepX,texRepY); glVertex3f(0.2,0.4,-0.35);
   glTexCoord2f(0.0,texRepY); glVertex3f(0.2,0.4,0.35);
   glEnd();

   glBegin(GL_TRIANGLES);
   glNormal3f(0,0,1);
   glTexCoord2f(0.0,0.0); glVertex3f(0.2,0.4,0.35);
   glTexCoord2f(texRepX, 0.0); glVertex3f(0.2,0.25,0.35);
   glTexCoord2f(texRepX, texRepY); glVertex3f(0.4,0.25,0.35);

   glNormal3f(0,0,-1);
   glTexCoord2f(0.0,0.0); glVertex3f(0.4,0.25,-0.35);
   glTexCoord2f(texRepX, 0.0); glVertex3f(0.2,0.25,-0.35);
   glTexCoord2f(texRepX, texRepY); glVertex3f(0.2,0.4,-0.35);
   glEnd();

   //Rear Window
   texRepX = 1.0;
   texRepY = 1.0;
   glBegin(GL_QUADS);
   glNormal3f(-0.6, 0.8, 0);
   glTexCoord2f(0.0,0.0); glVertex3f(-0.6,0.25,-0.35);
   glTexCoord2f(texRepX,0.0); glVertex3f(-0.6,0.25,0.35);
   glTexCoord2f(texRepX,texRepY); glVertex3f(-0.4,0.4,0.35);
   glTexCoord2f(0.0,texRepY); glVertex3f(-0.4,0.4,-0.35);
   glEnd();

   glBegin(GL_TRIANGLES);
   glNormal3f(0,0,1);
   glTexCoord2f(0.0,0.0); glVertex3f(-0.6,0.25,0.35);
   glTexCoord2f(texRepX, 0.0); glVertex3f(-0.4,0.25,0.35);
   glTexCoord2f(texRepX, texRepY); glVertex3f(-0.4,0.4,0.35);
   
   glNormal3f(0,0,-1);
   glTexCoord2f(0.0,0.0); glVertex3f(-0.4,0.4,-0.35);
   glTexCoord2f(texRepX, 0.0); glVertex3f(-0.4,0.25,-0.35);
   glTexCoord2f(texRepX, texRepY); glVertex3f(-0.6,0.25,-0.35);
   glEnd();

   glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS,shiny);
   glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,white);
   glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,black);

   glBindTexture(GL_TEXTURE_2D,_textureCarbonFiber);

   //Spoiler
   glColor3f(0.3,0.3,0.3);
   cube(-0.75,0.28,0.3, 0.02,0.03,0.02, 0);
   cube(-0.75,0.28,-0.3, 0.02,0.03,0.02, 0);

   texRepX = 5.0;
   texRepY = 1.0;

   glBegin(GL_QUADS);
   glNormal3f(0, -1, 0);
   glTexCoord2f(0.0,0.0); glVertex3f(-0.7,0.31,-0.35);
   glTexCoord2f(texRepX,0.0); glVertex3f(-0.7,0.31,0.35);
   glTexCoord2f(texRepX,texRepY); glVertex3f(-0.8,0.31,0.35);
   glTexCoord2f(0.0,texRepY); glVertex3f(-0.8,0.31,-0.35);
   
   glNormal3f(0.196116, 0.980581, 0);
   glTexCoord2f(0.0,0.0); glVertex3f(-0.8,0.33,-0.35);
   glTexCoord2f(texRepX,0.0); glVertex3f(-0.8,0.33,0.35);
   glTexCoord2f(texRepX,texRepY); glVertex3f(-0.7,0.31,0.35);
   glTexCoord2f(0.0,texRepY); glVertex3f(-0.7,0.31,-0.35);

   texRepX = 5.0;
   texRepY = 0.2;
   glNormal3f(-1, 0, 0);
   glTexCoord2f(0.0,0.0); glVertex3f(-0.8,0.33,0.35);
   glTexCoord2f(texRepX,0.0); glVertex3f(-0.8,0.33,-0.35);
   glTexCoord2f(texRepX,texRepY); glVertex3f(-0.8,0.31,-0.35);
   glTexCoord2f(0.0,texRepY); glVertex3f(-0.8,0.31,0.35);
   
   glEnd();

   glBindTexture(GL_TEXTURE_2D,_textureBasicMetal);
//   glColor3f(cr, cb, cg);

   //Spoiler Fins
   texRepX = dx/texScale;
   texRepY = dy/texScale;
   glBegin(GL_TRIANGLES);
   glNormal3f(0,0,-1);
   glTexCoord2f(0.0,0.0); glVertex3f(-0.68,0.31,-0.35);
   glTexCoord2f(texRepX, 0.0); glVertex3f(-0.82,0.31,-0.35);
   glTexCoord2f(texRepX, texRepY); glVertex3f(-0.82,0.35,-0.35);

   glNormal3f(0,0,1);
   glTexCoord2f(0.0,0.0); glVertex3f(-0.82,0.35,0.35);
   glTexCoord2f(texRepX, 0.0); glVertex3f(-0.82,0.31,0.35);
   glTexCoord2f(texRepX, texRepY); glVertex3f(-0.68,0.31,0.35);
   
   //Duplicate to draw both sides (with inverted normals) when face culling is on
   glNormal3f(0,0,1);
   glTexCoord2f(0.0,0.0); glVertex3f(-0.68,0.31,-0.35);
   glTexCoord2f(texRepX, 0.0); glVertex3f(-0.82,0.31,-0.35);
   glTexCoord2f(texRepX, texRepY); glVertex3f(-0.82,0.35,-0.35);

   glNormal3f(0,0,-1);
   glTexCoord2f(0.0,0.0); glVertex3f(-0.82,0.35,0.35);
   glTexCoord2f(texRepX, 0.0); glVertex3f(-0.82,0.31,0.35);
   glTexCoord2f(texRepX, texRepY); glVertex3f(-0.68,0.31,0.35);
   
   glEnd();

   //Undo transformations
   glPopMatrix();
}

static void workshop(double x, double z, double th)
{
   //  Save transformation
   glPushMatrix();
   //  Offset
   glTranslated(x,0,z);
   glRotated(th,0,1,0);

   //Building - Brown Workshop
   texScale = 0.5;
   cube(0,1.6,0, 2,0.4,1, 0); //Top
   cube(-1.75,0.65,0, 0.25,0.55,1, 0); //Left
   cube(1.75,0.65,0, 0.25,0.55,1, 0); //Right
   cube(0,0.65,0, 0.2,0.55,1, 0); //Middle

   //"Floor" for brown workshop
   glColor3f(0.7, 0.7, 0.7);
   glBindTexture(GL_TEXTURE_2D,_textureSidewalk);
   texScale = 1;
   cube(0,-0.05,0.9, 2,0.15,0.1, 0);
   
   //Garage Doors
   glColor3f(0.5, 0.5, 0.5);
   glBindTexture(GL_TEXTURE_2D,_textureGarageDoor);
   glBegin(GL_QUADS);
   float texRepX = 1.0;
   float texRepY = 1.0;
   //Left
   glNormal3f(0, 0, 1);
   glTexCoord2f(0.0,0.0); glVertex3f(-1.5, 0.1, 0.9);
   glTexCoord2f(texRepX,0.0); glVertex3f(-0.2, 0.1, 0.9);
   glTexCoord2f(texRepX,texRepY); glVertex3f(-0.2, 1.2, 0.9);
   glTexCoord2f(0.0,texRepY); glVertex3f(-1.5, 1.2, 0.9);
   //Right
   glNormal3f(0, 0, 1);
   glTexCoord2f(0.0,0.0); glVertex3f(0.2, 0.1, 0.9);
   glTexCoord2f(texRepX,0.0); glVertex3f(1.5, 0.1, 0.9);
   glTexCoord2f(texRepX,texRepY); glVertex3f(1.5, 1.2, 0.9);
   glTexCoord2f(0.0,texRepY); glVertex3f(0.2, 1.2, 0.9);
   glEnd();

   //Undo transformations
   glPopMatrix();
}

static void greyHouse(double x, double z, double th) {
   //  Save transformation
   glPushMatrix();
   //  Offset
   glTranslated(x,0,z);
   glRotated(th,0,1,0);

   glPolygonOffset(1,1);
   glEnable(GL_POLYGON_OFFSET_FILL);

   float white[] = {1,1,1,1};
   float black[] = {0,0,0,1};
   glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS,shiny);
   glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,white);
   glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,black);

   //Walkway (to house)
   glColor3f(0.7, 0.7, 0.7);
   glBindTexture(GL_TEXTURE_2D,_textureWalkway);
   texScale = 0.4;
   cube(0,-0.05,-1.5, 0.5,0.15,0.4, 90); // Between hedges

   //Hedges
   glColor3f(0, 0.3, 0.1);
   glBindTexture(GL_TEXTURE_2D,_textureHedge);
   texScale = 0.25;
   cube(1.23,0.3,-1.2, 0.77,0.2,0.17, 0);
   cube(-1.23,0.3,-1.2, 0.77,0.2,0.17, 0);

   //Grass
   glColor3f(0.7, 0.7, 0.7);
   glBindTexture(GL_TEXTURE_2D,_textureGrass);
   texScale = 0.5;
   cube(-1.2,-0.05,-1.5, 0.8,0.15,0.5, 0);
   cube(1.2,-0.05,-1.5, 0.8,0.15,0.5, 0);

   //Building - Grey House
   glColor3f(0.7, 0.7, 0.7);
   glBindTexture(GL_TEXTURE_2D,_textureGreyBrick);
   texScale = 0.5;
   cube(0,0.9,-3, 2,0.8,1, 0);

   //Door Frame
   glColor3f(0.5, 0.5, 0.5);
   glBindTexture(GL_TEXTURE_2D,_textureWoodBeam);
   texScale = 0.5;
   cube(-0.4,0.65,-1.95, 0.1,0.55,0.05, 0);
   cube(0.4,0.65,-1.95, 0.1,0.55,0.05, 0);
   cube(0,1.25,-1.95, 0.5,0.05,0.05, 0);

   //Window Sills
   cube(-1.3,0.65,-1.95, 0.45,0.05,0.05, 0); //Left
   cube(1.3,0.65,-1.95, 0.45,0.05,0.05, 0); //Right

   glDisable(GL_POLYGON_OFFSET_FILL);

   //Door
   glColor3f(0.5, 0.5, 0.5);
   glBindTexture(GL_TEXTURE_2D,_textureFrontDoor2);
   glBegin(GL_QUADS);
   double texRepX = 1.0;
   double texRepY = 1.0;
   glNormal3f(0, 0, 1);
   glTexCoord2f(0.0,0.0); glVertex3f(-0.31, 0.1, -2);
   glTexCoord2f(texRepX,0.0); glVertex3f(0.31, 0.1, -2);
   glTexCoord2f(texRepX,texRepY); glVertex3f(0.31, 1.21, -2);
   glTexCoord2f(0.0,texRepY); glVertex3f(-0.31, 1.21, -2);
   glEnd();

   //Windows
   glColor3f(0.7, 0.7, 0.7);

   glBindTexture(GL_TEXTURE_2D,_textureWindow1);
   glBegin(GL_QUADS);
   texRepX = 1.0;
   texRepY = 2.0;

   //Left Window
   glNormal3f(0, 0, 1);
   glTexCoord2f(0.0,0.0); glVertex3f(-1.7, 0.7, -2);
   glTexCoord2f(texRepX,0.0); glVertex3f(-0.9, 0.7, -2);
   glTexCoord2f(texRepX,texRepY); glVertex3f(-0.9, 1.3, -2);
   glTexCoord2f(0.0,texRepY); glVertex3f(-1.7, 1.3, -2);

   //Right Window
   glNormal3f(0, 0, 1);
   glTexCoord2f(0.0,0.0); glVertex3f(0.9, 0.7, -2);
   glTexCoord2f(texRepX,0.0); glVertex3f(1.7, 0.7, -2);
   glTexCoord2f(texRepX,texRepY); glVertex3f(1.7, 1.3, -2);
   glTexCoord2f(0.0,texRepY); glVertex3f(0.9, 1.3, -2);

   glEnd();

   //Undo transformations
   glPopMatrix();
}

static void skybox(float dim) {
   glDisable(GL_POLYGON_OFFSET_FILL);

   //  Set specular color to white
   float white[] = {1,1,1,1};
   float black[] = {0,0,0,1};
   glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS,0);
   glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,white);
   glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,black);

   glColor3f(0.7, 0.7, 0.7);
   glBindTexture(GL_TEXTURE_2D,_textureSkyboxFront);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
   glBegin(GL_QUADS);
   glNormal3f(0, 0, -1);
   glTexCoord2f(1.0,0.0); glVertex3f(+dim,-dim, dim);
   glTexCoord2f(0.0,0.0); glVertex3f(-dim,-dim, dim);
   glTexCoord2f(0.0,1.0); glVertex3f(-dim,+dim, dim);
   glTexCoord2f(1.0,1.0); glVertex3f(+dim,+dim, dim);
   glEnd();

   glBindTexture(GL_TEXTURE_2D,_textureSkyboxBack);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
   glBegin(GL_QUADS);
   glNormal3f(0, 0, 1);
   glTexCoord2f(1.0,0.0); glVertex3f(-dim,-dim, -dim);
   glTexCoord2f(0.0,0.0); glVertex3f(+dim,-dim, -dim);
   glTexCoord2f(0.0,1.0); glVertex3f(+dim,+dim, -dim);
   glTexCoord2f(1.0,1.0); glVertex3f(-dim,+dim, -dim);
   glEnd();

   glBindTexture(GL_TEXTURE_2D,_textureSkyboxRight);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
   glBegin(GL_QUADS);
   glNormal3f(-1, 0, 0);
   glTexCoord2f(1.0,0.0); glVertex3f(dim,-dim, -dim);
   glTexCoord2f(0.0,0.0); glVertex3f(dim,-dim, +dim);
   glTexCoord2f(0.0,1.0); glVertex3f(dim,+dim, +dim);
   glTexCoord2f(1.0,1.0); glVertex3f(dim,+dim, -dim);
   glEnd();

   glBindTexture(GL_TEXTURE_2D,_textureSkyboxLeft);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
   glBegin(GL_QUADS);
   glNormal3f(1, 0, 0);
   glTexCoord2f(1.0,0.0); glVertex3f(-dim,-dim, +dim);
   glTexCoord2f(0.0,0.0); glVertex3f(-dim,-dim, -dim);
   glTexCoord2f(0.0,1.0); glVertex3f(-dim,+dim, -dim);
   glTexCoord2f(1.0,1.0); glVertex3f(-dim,+dim, +dim);
   glEnd();

   glBindTexture(GL_TEXTURE_2D,_textureSkyboxTop);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
   glBegin(GL_QUADS);
   glNormal3f(0, -1, 0);
   glTexCoord2f(1.0,0.0); glVertex3f(+dim,dim, +dim);
   glTexCoord2f(0.0,0.0); glVertex3f(-dim,dim, +dim);
   glTexCoord2f(0.0,1.0); glVertex3f(-dim,dim, -dim);
   glTexCoord2f(1.0,1.0); glVertex3f(+dim,dim, -dim);
   glEnd();
}

static void lampPost(double x,double y,double z,
                     double dx,double dy,double dz,
                     double th)
{
   //  Save transformation
   glPushMatrix();
   //  Offset
   glTranslated(x,y,z);
   glRotated(th,0,1,0);

   //  Set specular color to white
   float white[] = {1,1,1,1};
   float black[] = {0,0,0,1};
   glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS,shiny);
   glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,white);
   glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,black);

   float radScale = 0.03;
   float baseRadScale = 0.05;
   float capRadScale = 0.01;
   float outerRadScale = 0.08;

   //Pole
   glColor3f(0.4, 0.4, 0.4);
   glBindTexture(GL_TEXTURE_2D,_textureBasicMetal);   
   glBegin(GL_QUADS);
   int i;
   for(i=45; i <= 360; i += 45) {
      //Base
      glNormal3f(Cos(i-22.5),0,Sin(i-22.5));
      glTexCoord2f(Cos(i), 0); glVertex3f(baseRadScale*Cos(i), 0, baseRadScale*Sin(i));
      glTexCoord2f(Cos(i), 0); glVertex3f(baseRadScale*Cos(i-45), 0, baseRadScale*Sin(i-45));
      glTexCoord2f(Cos(i), 1); glVertex3f(radScale*Cos(i-45), 0.1, radScale*Sin(i-45));
      glTexCoord2f(Cos(i), 1); glVertex3f(radScale*Cos(i), 0.1, radScale*Sin(i));
      //Pole Lower
      glNormal3f(Cos(i-22.5),0,Sin(i-22.5));
      glTexCoord2f(Cos(i), 0); glVertex3f(radScale*Cos(i), 0.1, radScale*Sin(i));
      glTexCoord2f(Cos(i), 0); glVertex3f(radScale*Cos(i-45), 0.1, radScale*Sin(i-45));
      glTexCoord2f(Cos(i), 2); glVertex3f(radScale*Cos(i-45), 0.5, radScale*Sin(i-45));
      glTexCoord2f(Cos(i), 2); glVertex3f(radScale*Cos(i), 0.5, radScale*Sin(i));
      //Pole Upper
      glNormal3f(Cos(i-22.5),0,Sin(i-22.5));
      glTexCoord2f(Cos(i), 0); glVertex3f(radScale*Cos(i), 0.5, radScale*Sin(i));
      glTexCoord2f(Cos(i), 0); glVertex3f(radScale*Cos(i-45), 0.5, radScale*Sin(i-45));
      glTexCoord2f(Cos(i), 2); glVertex3f(radScale*Cos(i-45), 1, radScale*Sin(i-45));
      glTexCoord2f(Cos(i), 2); glVertex3f(radScale*Cos(i), 1, radScale*Sin(i));
//      //Light Cap
      glNormal3f(Cos(i-22.5),0,Sin(i-22.5));
      glTexCoord2f(Cos(i), 0); glVertex3f(radScale*Cos(i), 1.15, radScale*Sin(i));
      glTexCoord2f(Cos(i), 0); glVertex3f(radScale*Cos(i-45), 1.15, radScale*Sin(i-45));
      glTexCoord2f(Cos(i), 1); glVertex3f(capRadScale*Cos(i-45), 1.17, capRadScale*Sin(i-45));
      glTexCoord2f(Cos(i), 1); glVertex3f(capRadScale*Cos(i), 1.17, capRadScale*Sin(i));
   }
   glEnd();

   //Light
   float em[4] = {0.8, 0.8, 0.1, 1.0};
   glMaterialf(GL_FRONT,GL_SHININESS,0);
   glMaterialfv(GL_FRONT,GL_SPECULAR,em);
   glMaterialfv(GL_FRONT,GL_EMISSION,em);
   glColor3f(0.8, 0.8, 0.1);
   glBindTexture(GL_TEXTURE_2D,_textureGlass);   
   glBegin(GL_QUADS);
   i = 45;
   for(i = 45; i <= 360; i += 45) {
//      Lower
      glNormal3f(Cos(i-22.5),0,Sin(i-22.5));
      glTexCoord2f(Cos(i), 0); glVertex3f(radScale*Cos(i), 1, radScale*Sin(i));
      glTexCoord2f(Cos(i), 0); glVertex3f(radScale*Cos(i-45), 1, radScale*Sin(i-45));
      glTexCoord2f(Cos(i), 1); glVertex3f(outerRadScale*Cos(i-45), 1.1, outerRadScale*Sin(i-45));
      glTexCoord2f(Cos(i), 1); glVertex3f(outerRadScale*Cos(i), 1.1, outerRadScale*Sin(i));
//      Upper
      glNormal3f(Cos(i-22.5),0,Sin(i-22.5));
      glTexCoord2f(Cos(i), 0); glVertex3f(outerRadScale*Cos(i), 1.1, outerRadScale*Sin(i));
      glTexCoord2f(Cos(i), 0); glVertex3f(outerRadScale*Cos(i-45), 1.1, outerRadScale*Sin(i-45));
      glTexCoord2f(Cos(i), 1); glVertex3f(radScale*Cos(i-45), 1.15, radScale*Sin(i-45));
      glTexCoord2f(Cos(i), 1); glVertex3f(radScale*Cos(i), 1.15, radScale*Sin(i));
   }
   glEnd();

   glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS,shiny);
   glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,white);
   glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,black);

   //Undo transformations
   glPopMatrix();
}


void track_street(){
	
	float sizeBlock = 4;
	float x = -2;
	float y = 0.1;
	float z = 2;
	float dx = 0.5 * sizeBlock; // biar simetris
	float dy = 0.1;
	float dz = 0.5 * sizeBlock;
	float pengkolan = dx*4/sizeBlock;
   texScale = dx;
	int i;
	glBindTexture(GL_TEXTURE_2D,_textureAsphalt);


//jalan 1
   for(i = 0; i < 8; i++){
      cube(x,y,z, dx,dy,dz, 90);
      z -= sizeBlock;
   }
   

   //pengkolan1
   glBindTexture(GL_TEXTURE_2D,_textureBend);
   circle(x+(sizeBlock/2), y, z+(sizeBlock/2), dx,dy, dz, 180, pengkolan);

   
 //jalan 2  
 glBindTexture(GL_TEXTURE_2D,_textureAsphalt);
   x+= sizeBlock;
   for(i = 0; i < 2; i++){
	  cube(x,y,z, dx,dy,dz, 0);
      x += sizeBlock;
   }

    //pengkolan2
    glBindTexture(GL_TEXTURE_2D,_textureBend);
   circle(x-(sizeBlock/2), y, z+(sizeBlock/2), dx,dy, dz, 90, pengkolan);
   

//   jalan 3
   glBindTexture(GL_TEXTURE_2D,_textureAsphalt);
   z+=sizeBlock;
   for(i = 0; i < 8; i++){
	  cube(x,y,z, dx,dy,dz, 90);
      z += sizeBlock;
   }   
   
  
   //pengkolan3
   glBindTexture(GL_TEXTURE_2D,_textureBend);
   circle(x+(sizeBlock/2), y, z-(sizeBlock/2), dx,dy, dz, 270, pengkolan);

// jalan 4  
 
   glBindTexture(GL_TEXTURE_2D,_textureAsphalt);
   x+=sizeBlock;
   for(i = 0; i < 4; i++){
	  cube(x,y,z, dx,dy,dz, 0);
      x += sizeBlock;
   }   

   
   //pengkolan4
   glBindTexture(GL_TEXTURE_2D,_textureBend);
   circle(x-(sizeBlock/2), y, z+(sizeBlock/2), dx,dy, dz, 90, pengkolan);


// jalan 5
glBindTexture(GL_TEXTURE_2D,_textureAsphalt);
   z+=sizeBlock;
   for(i = 0; i < 2; i++){
	  cube(x,y,z, dx,dy,dz, 90);
      z += sizeBlock;
   } 
   

   
      //pengkolan5
      glBindTexture(GL_TEXTURE_2D,_textureBend);
   circle(x-(sizeBlock/2), y, z-(sizeBlock/2), dx,dy, dz, 0, pengkolan);
   

//   Jalan 6
   glBindTexture(GL_TEXTURE_2D,_textureAsphalt);
	x-=sizeBlock;
   for(i = 0; i < 12; i++){ 
	  cube(x,y,z, dx,dy,dz, 0);
      x -= sizeBlock;
   } 

   
      //pengkolan6
      glBindTexture(GL_TEXTURE_2D,_textureBend);
   circle(x+(sizeBlock/2), y, z-(sizeBlock/2), dx,dy, dz, 270, pengkolan);

//   jalan 7
   glBindTexture(GL_TEXTURE_2D,_textureAsphalt);
   	z-=sizeBlock;
   for(i = 0; i < 2; i++){
	  cube(x,y,z, dx,dy,dz, 90);
      z -= sizeBlock;
   }
   

   
   //pengkolan7
   glBindTexture(GL_TEXTURE_2D,_textureBend);
   circle(x+(sizeBlock/2), y, z+(sizeBlock/2), dx,dy, dz, 180, pengkolan);
   

//   jalan 8
   glBindTexture(GL_TEXTURE_2D,_textureAsphalt);
    x+=sizeBlock;
   for(i = 0; i < 4; i++){
	  cube(x,y,z, dx,dy,dz, 0);
      x += sizeBlock;
   }
   

   
       //pengkolan8
       glBindTexture(GL_TEXTURE_2D,_textureBend);
   circle(x-(sizeBlock/2), y, z-(sizeBlock/2), dx,dy, dz, 0, pengkolan);
}

void pager(){
	
	float sizeBlock = 4;
	float x = -2;
	float y = 0;
	float z = 2;
	float pos = sizeBlock/2;
	int i;
	
	float thickness = 0.1;
	float width = 0.5 * sizeBlock;
	float height = 0.5 * width; //1
	
	glBindTexture(GL_TEXTURE_2D,_textureGarageDoor);


//jalan 1
   for(i = 0; i < 8; i++){
	cube(x+pos,y,z, thickness,height,width, 0);
	cube(x-pos,y,z, thickness,height,width, 0);
	z -= sizeBlock;
   }
   
 
 
 //jalan 2  
   x+= sizeBlock;
   for(i = 0; i < 2; i++){
	cube(x,y,z+pos, thickness,height,width, 90);
	cube(x,y,z-pos, thickness,height,width, 90);
      x += sizeBlock;
   }

//jalan 3
   z+=sizeBlock;
   for(i = 0; i < 8; i++){
	cube(x+pos,y,z, thickness,height,width, 0);
	cube(x-pos,y,z, thickness,height,width, 0);
      z += sizeBlock;
   }   
   
//jalan 4
   x+=sizeBlock;
   for(i = 0; i < 4; i++){
	cube(x,y,z+pos, thickness,height,width, 90);
	cube(x,y,z-pos, thickness,height,width, 90);
      x += sizeBlock;
   }   
      
// jalan 5
   z+=sizeBlock;
   for(i = 0; i < 2; i++){
	cube(x+pos,y,z, thickness,height,width, 0);
	cube(x-pos,y,z, thickness,height,width, 0);
      z += sizeBlock;
   } 
     
// jalan 6
	x-=sizeBlock;
   for(i = 0; i < 12; i++){ //harusnya 14
	cube(x,y,z+pos, thickness,height,width, 90);
	cube(x,y,z-pos, thickness,height,width, 90);
      x -= sizeBlock;
   } 

// jalan 7
   z-=sizeBlock;
   for(i = 0; i < 2; i++){
	cube(x+pos,y,z, thickness,height,width, 0);
	cube(x-pos,y,z, thickness,height,width, 0);
      z -= sizeBlock;
   }

// jalan 8
    x+=sizeBlock;
   for(i = 0; i < 4; i++){
	cube(x,y,z+pos, thickness,height,width, 90);
	cube(x,y,z-pos, thickness,height,width, 90);
      x += sizeBlock;
   }


}

void gapura(){
		float sizeBlock = 4;
	float x = -2;
	float y = 0.1;
	float z = 2;
	float dx = 0.5 * sizeBlock; // biar simetris
	float dy = 0.2;
	float dz = 0.5 * sizeBlock;
	float pengkolan = dx*4/sizeBlock;
   texScale = dx;
//	int i;
	
	
	glBindTexture(GL_TEXTURE_2D,_textureStartLine);
	cube(-2.000000, 3.00000, 2.000000,dx, dy, dz/4, 0); //atas
	cube(0.000000, 1.00000, 2.000000,dy, dx, dz/4, 0); //kiri
	cube(-4.000000, 1.00000, 2.000000,dy, dx, dz/4, 0); //kanan
}

void tribun(){
	
	glBindTexture(GL_TEXTURE_2D,_textureSidewalk);
	cube(16,0,-14, 1,2,15,0);
	cube(17,0,-14, 1,3,15,0);
	cube(18,0,-14, 1,4,15,0);
	
	glBindTexture(GL_TEXTURE_2D,_textureBasicMetal);
	cube(19.1,0,-14, 0.1,5,15,0); //tiang
	cube(17,5,-14, 2.2,0.1,15,0); //atap
	
                     
	glBindTexture(GL_TEXTURE_2D,_textureSidewalk);
	cube(-8,0,-14, 1,2,15,0);
	cube(-9,0,-14, 1,3,15,0);
	cube(-10,0,-14, 1,4,15,0);
	
	glBindTexture(GL_TEXTURE_2D,_textureBasicMetal);
	cube(-11.1,0,-14, 0.1,5,15,0); //tiang
	cube(-9,5,-14, 2.2,0.1,15,0); //atap          
                     
                     

	
	
}

mainland(){

	
	
	glBindTexture(GL_TEXTURE_2D,_textureGrass);
   
	cube(0,0.0000001,0,35,0.001,35,0);
   
	glBindTexture(GL_TEXTURE_2D,_textureSand);
   
	cube(0,0.00001,28, 35,0.01,10,0);
	
	
}

setlampu(){
		int i;
	float lampu = -24;
	   for(i = 0; i < 22; i++){
	lampPost(lampu,0,18, 25,0,25,0);
      lampu -= 0.5;
   } 
	
	lampu = 34;
	for(i = 0; i < 6; i++){
	lampPost(lampu,0,18, 25,0,25,0);
      lampu -= 0.5;
  }
  
}

parkiran(){
	 int i;
	   float ws = 28;
   for(i=0; i<12; i++){
   	
	   workshop(ws, 28, 0);
   		ws -= 5;
   		
   }
}

sponsor(){

	glBindTexture(GL_TEXTURE_2D,_textureUltraJaya);
	cube(-21,1,-23, 2,1,2,270);
	
	glBindTexture(GL_TEXTURE_2D,_textureKeepSmile);
	cube(-21,1,-19, 2,1,2,270);
	
	glBindTexture(GL_TEXTURE_2D,_textureOishi);
	cube(-21,1,-15, 2,1,2,270);
	
	glBindTexture(GL_TEXTURE_2D,_textureLemonia);
	cube(-21,1,-11, 2,1,2,270);
	
	glBindTexture(GL_TEXTURE_2D,_textureChocodot);
	cube(-25,1,-23, 2,1,2,270);
	
	glBindTexture(GL_TEXTURE_2D,_textureFresTea);
	cube(-25,1,-19, 2,1,2,270);
	
	glBindTexture(GL_TEXTURE_2D,_textureChocodot);
	cube(-25,1,-15, 2,1,2,270);
	
	glBindTexture(GL_TEXTURE_2D,_textureBurgerKing);
	cube(-25,1,-11, 2,1,2,270);
	
	glBindTexture(GL_TEXTURE_2D,_textureUsb);
	cube(-21,1,-7, 2,1,2,270);
	
	glBindTexture(GL_TEXTURE_2D,_textureNikitaNabila);
	cube(-25,1,-7, 2,1,2,270);
	
}


/*
 *  OpenGL (GLUT) calls this routine to display the scene
 */
void display()
{
	
	int i=0;
   const double len=2.0;  //  Length of axes
   //  Erase the window and the depth buffer
   glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
   //  Enable Z-buffering in OpenGL
   glEnable(GL_DEPTH_TEST);
   //Enable Face Culling
   glEnable(GL_CULL_FACE);
   //Enable Textures
   glEnable(GL_TEXTURE_2D);
   glTexEnvi(GL_TEXTURE_ENV , GL_TEXTURE_ENV_MODE , tMode?GL_REPLACE:GL_MODULATE);

//nambah ini

glEnable(GL_CULL_FACE); 
glDisable(GL_CULL_FACE); //

	

   //  Undo previous transformations
   glLoadIdentity();
//   printf("%d\n", mode);
   
   //  Perspective - set eye position
   if (mode==1)
   {
      double Ex = -2*dim;
      double Ey = +2*dim*Sin(th);
      double Ez = +2*dim*Cos(th);
      gluLookAt(Ex,Ey,Ez , 0,10,0, 90,0,30 );
   }
   //  First Person
  else if (mode==2)
   {
   	 refX = (dim * Sin(th)) + positionCar[i].x;
      refY = (dim * Sin(ph));
      refZ = (dim * -Cos(th)) + positionCar[i].z;
      gluLookAt(positionCar[i].x,positionCar[i].y+1,positionCar[i].z, refX,refY,refZ, 0,5,0);
   }
   
     else if (mode==3)
   {
      double Ex = dim;
      double Ey = dim       ;
      double Ez = dim*Cos(th)*Cos(ph);
      gluLookAt(Ex,Ey,Ez, 11.000000, 0.300000, 5.000000, 90,0,120 );
   }
   
   else
   {
      refX = (dim * Sin(th)) + fpX;
      refY = (dim * Sin(ph));
      refZ = (dim * -Cos(th)) + fpZ;
      gluLookAt(fpX,fpY,fpZ, refX,refY,refZ, 0,1,0);
   }
      
   

   //  Flat or smooth shading
   glShadeModel(smooth ? GL_SMOOTH : GL_FLAT);

   //  Light position
   float Position[]  = {distance*Cos(zh),distance*Sin(zh),0,1.0};
   // float MoonPosition[]  = {-(distance*Cos(zh)),-(distance*Sin(zh)),0,1.0};


   //  OpenGL should normalize normal vectors
   glEnable(GL_NORMALIZE);
   //  Enable lighting
   glEnable(GL_LIGHTING);
   //  Location of viewer for specular calculations
   glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER,local);
   //  glColor sets ambient and diffuse color materials
   glColorMaterial(GL_FRONT_AND_BACK,GL_AMBIENT_AND_DIFFUSE);
   glEnable(GL_COLOR_MATERIAL);

   //  Enable light 0 - Sun
   glEnable(GL_LIGHT0);
   //  Set ambient, diffuse, specular components and position of light 0 (Sun)
   glLightfv(GL_LIGHT0,GL_AMBIENT ,Ambient);
   glLightfv(GL_LIGHT0,GL_DIFFUSE ,Diffuse);
   glLightfv(GL_LIGHT0,GL_SPECULAR,Specular);
   glLightfv(GL_LIGHT0,GL_POSITION,Position);
   

   //Inital values for texture repitition
   double texRepX = 1.0;
   double texRepY = 1.0;

   //Draw scene

   //Skybox
   skybox(64);

   //  Set specular color to white
   float white[] = {1,1,1,1};
   float black[] = {0,0,0,1};
   glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS,shiny);
   glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,white);
   glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,black);

   //Enable offset for windows, doors, and other decals
   glPolygonOffset(1,1);
   glEnable(GL_POLYGON_OFFSET_FILL);


   glColor3f(0.4, 0.4, 0.4);

	float sizeBlock = 4;
	float x = -2;
	float y = 0.1;
	float z = 2;
	float dx = 0.5 * sizeBlock; // biar simetris
	float dy = 0.1;
	float dz = 0.5 * sizeBlock;
	float pengkolan = dx*4/sizeBlock;
   texScale = dx;
//	int i;
	
	glBindTexture(GL_TEXTURE_2D,_textureBrownBrick);
	
	gapura();
	tribun();
	
	
//	car(positionCar.x, positionCar.y, positionCar.z-0.5, dx/2,0.25/2, dz/2, positionCar.degree);
//	policeCar(positionCar.x+1, positionCar.y, positionCar.z+1, dx/2,0.25/2, dz/2, positionCar.degree);
//	car(positionCar.x-1, positionCar.y, positionCar.z-1, dx/2,0.25/2, dz/2, positionCar.degree);

	track_street();
	pager();
	
	mainland();
   sponsor();
   
   setlampu();
//	parkiran();

for(i=0; i<CAR_SUM; i++){
     car(positionCar[i].x, positionCar[i].y, positionCar[i].z-0.5, dx/2,0.75, dz/2, positionCar[i].degree);	
     }
     
   
//   	car(positionCar.x, positionCar.y, positionCar.z-0.5, dx/2,0.25/2, dz/2, positionCar.degree);
//   	car(positionCar[i].x+1, positionCar.y, positionCar.z+1, dx/2,0.25/2, dz/2, positionCar.degree);

   //  Display parameters
  // glWindowPos2i(5,5);f

   Print("Angle=%d,%d  Dim=%.1f  FOV=%d  Projection=%s",th,ph,dim,fov,mode?"Perpective":"FP");

//	//x
//	glColor3f(1,0,0);
//	cube(0,0,0, 1,1,1,0);
//	cube(1,0,0, 1,1,1,0);
//	cube(2,0,0, 1,1,1,0);
//	cube(-1,0,0, 1,1,1,0);
//	
//	//y
//	glColor3f(0,1,0);
//	cube(0,0,0, 1,1,1,0);
//	cube(0,1,0, 1,1,1,0);
//	cube(0,2,0, 1,1,1,0);
//	cube(0,-1,0, 1,1,1,0);
//	//z
//	glColor3f(0,0,1);
//	cube(0,0,0, 1,1,1,0);
//	cube(0,0,1, 1,1,1,0);
//	cube(0,0,2, 1,1,1,0);
//	cube(0,0,-1, 1,1,1,0);

   //  Render the scene and make it visible
   ErrCheck("display");
   glFlush();
   glutSwapBuffers();
}

static void setEnvLighting()
{
   float ambScale = 0.8;

   if(Sin(zh) >= 0.2) {
      Ambient[0] = (255 / 255) * ambScale;
      Ambient[1] = (255 / 255) * ambScale;
      Ambient[2] = (255 / 255) * ambScale;

      Diffuse[0] = 1.0;
      Diffuse[1] = 1.0;
      Diffuse[2] = 1.0;
   } else if (Sin(zh) < 0.4 && Sin(zh) >= 0) {
      Ambient[0] = (255 / 255) * ambScale;
      Ambient[1] = ((60 + (195*Sin(zh)*2.5)) / 255) * ambScale;
      Ambient[2] = ((60 + (195*Sin(zh)*2.5)) / 255) * ambScale;

      Diffuse[0] = Sin(zh)*2.5;
      Diffuse[1] = Sin(zh)*2.5;
      Diffuse[2] = Sin(zh)*2.5;
   } else if (Sin(zh) < 0 && Sin(zh) >= -0.4){
      Ambient[0] = ((255 - (235*Sin(zh)*-2.5)) / 255) * ambScale;
      Ambient[1] = (60 / 255) * ambScale;
      Ambient[2] = ((60 + (120*Sin(zh)*-2.5)) / 255) * ambScale;

      Diffuse[0] = 0;
      Diffuse[1] = 0;
      Diffuse[2] = 0;
   } else {
      Ambient[0] = (20 / 255) * ambScale;
      Ambient[1] = (60 / 255) * ambScale;
      Ambient[2] = (180 / 255) * ambScale;

      Diffuse[0] = 0;
      Diffuse[1] = 0;
      Diffuse[2] = 0;
   }
}

/*
 *  GLUT calls this routine when the window is resized
 */

void idle()
{
   //  Elapsed time in seconds
   double t = glutGet(GLUT_ELAPSED_TIME)/1000.0;
   zh = fmod(10*t,360.0);
   int i=0;

   setEnvLighting();


//ini bikin buat animasi car
// printf("%f", x);

for(i=0; i<CAR_SUM; i++)
{
		if (carAnimation[i][carArrayPosition[i]].increment)
		{ // jika posisi increment, kanan
			if(carAnimation[i][carArrayPosition[i]].x)
			{
				positionCar[i].x += speedCar[i];
				positionCar[i].degree = 180;
			}
			if(carAnimation[i][carArrayPosition[i]].y)
			{
				positionCar[i].y += speedCar[i]; //gaada juga gaapa
			
			}
			if(carAnimation[i][carArrayPosition[i]].z)
			{
				positionCar[i].z += speedCar[i];
				positionCar[i].degree = 270;
			}
		} else 
		{
			if(carAnimation[i][carArrayPosition[i]].x)
			{
				positionCar[i].x += -speedCar[i];
				positionCar[i].degree = 0;
			}
			if(carAnimation[i][carArrayPosition[i]].y)
			{
				positionCar[i].y += -speedCar[i];
			}
			if(carAnimation[i][carArrayPosition[i]].z)
			{
				positionCar[i].z += -speedCar[i];
				positionCar[i].degree = 90;
			}
		}
	

	
	if(compareFloat(positionCar[i].x, carAnimation[i][carArrayPosition[i]].end.x) && compareFloat(positionCar[i].y, carAnimation[i][carArrayPosition[i]].end.y) && compareFloat(positionCar[i].z, carAnimation[i][carArrayPosition[i]].end.z))
	{
		if (carArrayPosition[i] == 7)
		{
			carArrayPosition[i] = 0;
			positionCar[i] = carAnimation[i][carArrayPosition[i]].start;		
		} else
		
		{
			carArrayPosition[i] ++;
			positionCar[i] = carAnimation[i][carArrayPosition[i]].start;
		}
	}
	
}
glutPostRedisplay();
}

/*
 *  GLUT calls this routine when an arrow key is pressed
 */
void special(int key,int x,int y)
{
   //  Right arrow key - increase angle by 5 degrees
   if (key == GLUT_KEY_RIGHT)
      th += 5;
   //  Left arrow key - decrease angle by 5 degrees
   else if (key == GLUT_KEY_LEFT)
      th -= 5;
   //  Up arrow key - increase elevation by 5 degrees
   else if (key == GLUT_KEY_UP)
      ph += 5;
   //  Down arrow key - decrease elevation by 5 degrees
   else if (key == GLUT_KEY_DOWN)
      ph -= 5;
   //  PageUp key - increase dim
   else if (key == GLUT_KEY_PAGE_DOWN)
      dim += 0.1;
   //  PageDown key - decrease dim
   else if (key == GLUT_KEY_PAGE_UP && dim>1)
      dim -= 0.1;
   th %= 360;
   ph %= 360;
   //  Update projection
   Project(fov,asp,dim);
   //  Tell GLUT it is necessary to redisplay the scene
   glutPostRedisplay();
}

/*
 *  GLUT calls this routine when a key is pressed
 */
void key(unsigned char ch,int x,int y)
{
   //  Exit on ESC
   if (ch == 27)
      exit(0);
   //  Reset view angle
   else if (ch == '0')
      th = ph = 0;
   //  Toggle axes
   else if (ch == 'x' || ch == 'X')
      axes = 1-axes;
   //  Switch projection mode
   else if (ch == '1')
      mode = 0;
	else if (ch == '2')
      mode = 1;
   //  Toggle light movement
   else if (ch == 'm' || ch == 'M')
      move = 1-move;
   //  Change field of view angle
   else if (ch == '-' && ch>1)
      fov--;
   else if (ch == '+' && ch<179)
      fov++;
   //  Move
   else if (ch == 'a' || ch == 'A') //kanan
      fpX++;
    //  Move
   else if (ch == 'd' || ch == 'D') //kiri
      fpX--;
   //  Move
   else if (ch == 'w' || ch == 'W') //atas
      fpY++;
    //  Move
   else if (ch == 's' || ch == 'S') //bawah
      fpY--;
    //  Move
   else if (ch == 'q' || ch == 'Q') 
      fpZ++;
    //  Move
   else if (ch == 'e' || ch == 'E')
      fpZ--;
    else if (ch == '3')
      mode=2;
    else if (ch == '4')
      mode=3;
   //  Reproject
   Project(fov,asp,dim);
   //  Animate if requested
   glutIdleFunc(move?idle:NULL);
   //  Tell GLUT it is necessary to redisplay the scene
   glutPostRedisplay();
}
/*
 *  GLUT calls this routine when the window is resized
 */
void reshape(int width,int height)
{
   //  Ratio of the width to the height of the window
   asp = (height>0) ? (double)width/height : 1;
   //  Set the viewport to the entire window
   glViewport(0,0, width,height);
   //  Set projection
   Project(fov,asp,dim);
}


/*
 * Init Texture BMP
 */
void init(){
	_textureSkyboxBack = LoadTexBMP("skybox-back.bmp");
	_textureSkyboxFront = LoadTexBMP("skybox-front.bmp");
	_textureSkyboxLeft = LoadTexBMP("skybox-left.bmp");
	_textureSkyboxRight = LoadTexBMP("skybox-right.bmp");
	_textureSkyboxTop = LoadTexBMP("skybox-top.bmp");
	_textureGlass = LoadTexBMP("glass.bmp");
	_textureWoodFence = LoadTexBMP("wood-fence.bmp");
	_textureAsphalt = LoadTexBMP("asphalt.bmp");
	_textureBasicMetal = LoadTexBMP("basic-metal.bmp"); 
	_textureBrownBrick = LoadTexBMP("brown-brick.bmp");
	_textureCarWheel = LoadTexBMP("car-wheel.bmp");
	_textureCinderBlock = LoadTexBMP("cinder-block.bmp");
	_textureGarageDoor = LoadTexBMP("garage-door.bmp");
	_textureGrass = LoadTexBMP("grass.bmp");
	_textureGreyBrick = LoadTexBMP("grey-brick.bmp");
	_textureHedge = LoadTexBMP("hedge.bmp");
	_textureSidewalk = LoadTexBMP("sidewalk.bmp");
	_textureWalkway = LoadTexBMP("walkway.bmp");
	_textureWhiteBrick = LoadTexBMP("white-brick.bmp");
	_textureCarGrill = LoadTexBMP("car-grill.bmp");
	_textureFrontDoor2 = LoadTexBMP("front-door-2.bmp");
	_textureWindow1 = LoadTexBMP("window-1.bmp");
	_textureMetalRoof = LoadTexBMP("metal-roof.bmp");
	_textureTireTread = LoadTexBMP("tire-tread.bmp");
	_textureWarehouseWindow = LoadTexBMP("warehouse-window.bmp");
	_textureHeadLamp = LoadTexBMP("headlamp.bmp");
	_textureCarbonFiber = LoadTexBMP("carbon-fiber.bmp");
	_textureWoodBeam = LoadTexBMP("wood-beam.bmp");
	_textureStartLine = LoadTexBMP("start-line.bmp");
	_textureBend = LoadTexBMP("bend.bmp");
	_textureSand = LoadTexBMP("sand.bmp");
	_textureUltraJaya = LoadTexBMP("ultra-jaya.bmp");
	_textureOishi = LoadTexBMP("oishi.bmp");
	_textureLemonia = LoadTexBMP("lemonia.bmp");
	_textureKeepSmile = LoadTexBMP("keep-smile.bmp");
	_textureFresTea = LoadTexBMP("fres-tea.bmp");
	_textureChocodot = LoadTexBMP("chocodot.bmp");
	_textureChitato = LoadTexBMP("chitato.bmp");
	_textureBurgerKing = LoadTexBMP("burger-king.bmp");
	_textureUsb = LoadTexBMP("usb.bmp");
	_textureNikitaNabila = LoadTexBMP("nikita-nabila.bmp");
}
/*
 *  Start up GLUT and tell it what to do
 */
int main(int argc,char* argv[])
{
   //  Initialize GLUT
   glutInit(&argc,argv);
   //  Request double buffered, true color window with Z buffering at 600x600
   glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);
   glutInitWindowSize(800,600);
   glutCreateWindow("Final Project");
   //  Set callbacks
   init();
//   initCarPosition();
   glutDisplayFunc(display);
   glutReshapeFunc(reshape);
   glutSpecialFunc(special);
   glutKeyboardFunc(key);
   glutIdleFunc(idle);
   //  Pass control to GLUT so it can interact with the user
   ErrCheck("init");
   glutMainLoop();
   return 0;
}
