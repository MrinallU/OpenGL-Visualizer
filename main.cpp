#include <GL/glut.h>
#include <vecmath.h>

#include <cmath>
#include <fstream>
#include <iostream>
#include <iterator>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

// Globals
int colorId = 0;
double lightXPos = 1, lightYPos = 1; 
// This is the list of points (3D vectors)
vector<Vector3f> vecv;

// This is the list of normals (also 3D vectors)
vector<Vector3f> vecn;

// This is the list of faces (indices into vecv and vecn)
vector<vector<unsigned> > vecf;


// You will need more global variables to implement color and position changes


// These are convenience functions which allow us to call OpenGL 
// methods on Vec3d objects
inline void glVertex(const Vector3f &a) 
{ glVertex3fv(a); }

inline void glNormal(const Vector3f &a) 
{ glNormal3fv(a); }

void drawCustomObj(){
    for (const auto &face : vecf) {
        auto a = face[0];
        auto c = face[1];
        auto d = face[2];
        auto f = face[3];
        auto g = face[4];
        auto i = face[5];

        glBegin(GL_TRIANGLES);
        glNormal(vecn[c - 1]);
        glVertex(vecv[a - 1]);
        glNormal(vecn[f - 1]);
        glVertex(vecv[d - 1]);
        glNormal(vecn[i - 1]);
        glVertex(vecv[g - 1]);
        glEnd();
    }
}

// This function is called whenever a "Normal" key press is received.
void keyboardFunc( unsigned char key, int x, int y )
{
    switch ( key )
    {
    case 27: // Escape key
        exit(0);
        break;
    case 'c':
        // add code to change color here
    colorId++; 
    if(colorId==4) colorId = 0; 
		cout << "Changed Color" << "." << endl; 
        break;
    default:
        cout << "Unhandled key press " << key << "." << endl;        
    }

	// this will refresh the screen so that the user sees the color change
    glutPostRedisplay();
}

// This function is called whenever a "Special" key press is received.
// Right now, it's handling the arrow keys.
void specialFunc( int key, int x, int y )
{
    switch ( key )
    {
    case GLUT_KEY_UP:
        // add code to change light position
		lightYPos += 0.5; 
		break;
    case GLUT_KEY_DOWN:
        // add code to change light position
		lightYPos -= 0.5; 
		break;
    case GLUT_KEY_LEFT:
        // add code to change light position
		lightXPos -= 0.5; 
		break;
    case GLUT_KEY_RIGHT:
        // add code to change light position
		lightXPos += 0.5; 
		break;
    }

	// this will refresh the screen so that the user sees the light position
    glutPostRedisplay();
}

void loadInput()
{
	// load the OBJ file here
  int MAX_BUFFER_SIZE = 1048576; 
  char buffer[MAX_BUFFER_SIZE];


  cin.getline(buffer, MAX_BUFFER_SIZE);
  cin.getline(buffer, MAX_BUFFER_SIZE);
  cin.getline(buffer, MAX_BUFFER_SIZE);
  cin.getline(buffer, MAX_BUFFER_SIZE);
  cin.getline(buffer, MAX_BUFFER_SIZE);
 
  while ((buffer[0]=='f')||(buffer[0]=='v')||(buffer[0]=='s')||(buffer[0]=='g')||(buffer[0]=='u')) {
    stringstream ss(buffer); 
    string s; 
    ss >> s; 
    
    if(s=="f"){

      int a,b,c,d,e,f,g,h,i; 
      char s2; 
      ss>>a>>s2>>b >>s2>>c>>d>>s2>>e>>s2>>f>>g>>s2>>h>>s2>>i; 
      vecf.push_back({a, c, d, f, g, i});

    }else if(s=="v" || s=="vn"){

      Vector3f v; 
      ss >> v[0] >> v[1] >> v[2]; 
      if(s=="v"){
        vecv.push_back(v); 
      }else{
        vecn.push_back(v); 
      }
      cout << v[0] << " " << v[1] << " " << v[2] << endl; 
    }

    cin.getline(buffer, MAX_BUFFER_SIZE); 
  }

}


// This function is responsible for displaying the object.
void drawScene(void)
{
    int i;

    // Clear the rendering window
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Rotate the image
    glMatrixMode( GL_MODELVIEW );  // Current matrix affects objects positions
    glLoadIdentity();              // Initialize to the identity

    // Position the camera at [0,0,5], looking at [0,0,0],
    // with [0,1,0] as the up direction.
    gluLookAt(0.0, 0.0, 5.0,
              0.0, 0.0, 0.0,
              0.0, 1.0, 0.0);

    // Set material properties of object

	// Here are some colors you might use - feel free to add more
    GLfloat diffColors[4][4] = { {0.5, 0.5, 0.9, 1.0},
                                 {0.9, 0.5, 0.5, 1.0},
                                 {0.5, 0.9, 0.3, 1.0},
                                 {0.3, 0.8, 0.9, 1.0} };
    
	// Here we use the first color entry as the diffuse color
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, diffColors[colorId]);

	// Define specular color and shininess
    GLfloat specColor[] = {1.0, 1.0, 1.0, 1.0};
    GLfloat shininess[] = {100.0};

	// Note that the specular color and shininess can stay constant
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specColor);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, shininess);
  
    // Set light properties

    // Light color (RGBA)
    GLfloat Lt0diff[] = {1.0,1.0,1.0,1.0};
    // Light position
	GLfloat Lt0pos[] = {lightXPos, lightYPos, 5.0f, 1.0f};

    glLightfv(GL_LIGHT0, GL_DIFFUSE, Lt0diff);
    glLightfv(GL_LIGHT0, GL_POSITION, Lt0pos);

	// This GLUT method draws a teapot.  You should replace
	// it with code which draws the object you loaded.
	//glutSolidTeapot(1.0);
   drawCustomObj();   
    // Dup the image to the screen.
    glutSwapBuffers();


}

// Initialize OpenGL's rendering modes
void initRendering()
{
    glEnable(GL_DEPTH_TEST);   // Depth testing must be turned on
    glEnable(GL_LIGHTING);     // Enable lighting calculations
    glEnable(GL_LIGHT0);       // Turn on light #0.
}

// Called when the window is resized
// w, h - width and height of the window in pixels.
void reshapeFunc(int w, int h)
{
    // Always use the largest square viewport possible
    if (w > h) {
        glViewport((w - h) / 2, 0, h, h);
    } else {
        glViewport(0, (h - w) / 2, w, w);
    }

    // Set up a perspective view, with square aspect ratio
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    // 50 degree fov, uniform aspect ratio, near = 1, far = 100
    gluPerspective(50.0, 1.0, 1.0, 100.0);
}

// Main routine.
// Set up OpenGL, define the callbacks and start the main loop
int main( int argc, char** argv )
{
  
    loadInput();

    glutInit(&argc,argv);

    // We're going to animate it, so double buffer 
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH );

    // Initial parameters for window position and size
    glutInitWindowPosition( 60, 60 );
    glutInitWindowSize( 360, 360 );
    glutCreateWindow("Assignment 0");

    // Initialize OpenGL parameters.
    initRendering();

    // Set up callback functions for key presses
    glutKeyboardFunc(keyboardFunc); // Handles "normal" ascii symbols
    glutSpecialFunc(specialFunc);   // Handles "special" keyboard keys

     // Set up the callback function for resizing windows
    glutReshapeFunc( reshapeFunc );

    // Call this whenever window needs redrawing
    glutDisplayFunc( drawScene );

    // Start the main loop.  glutMainLoop never returns.
    glutMainLoop( );

    return 0;	// This line is never reached.
}



