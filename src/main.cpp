#ifdef _WIN32
	#include <windows.h>
	#include <gl/gl.h>
	#define GLUT_DISABLE_ATEXIT_HACK
#endif

#include <GL/glut.h>
#include <iostream>
#include <math.h>
#include <string>
#include <cstring>
#include <time.h> 
#include <fstream>

#include "test.hpp"

// GLUT-functions
void InitGL(void);//Funktion zur festlegung der Startbedinungen
void Render(void);
void Resize(int width, int height);
void SpecialKey(int key, int x, int y); //special keys (F1, F2, ... )
void keyPressed (unsigned char key, int x, int y); // (keys a .. z, esc ...)
// GLUT-functions

// window size preferences
int wnd_width  = 1024; // opengl window width
int wnd_height = 768;  // opengl window height
// window size preferences 

// mouse rotate
float rotY = 1;
float rotZ = 1;
float deltaAngle = 0.01;
float angle = 0;
bool mouse_down = false;
float cache_x = 0;
float cache_y = 0;
bool test = 0;
bool window1;
float dx, dy;
// mouse rotate

float mousezoom = 1;

// solar elevation calculation variables
float hours = 0;
float minutes = 0;
float seconds = 0;

float day = 1; // 1 - 29 (-> day_reset = day + 1)
float month = 1;
float year = 2016;

int months_list[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};	// no leap yrs
int time_increment_seconds = 1;
int time_increment_minutes = 0;
int time_increment_hours = 0;
int time_increment_days = 0;
int time_increment_months = 0;
int time_increment_years = 0;
int time_increment_global_seconds = 1;

float sun_traj_x2 [2000];
float sun_traj_y2 [2000];
float sun_traj_z2 [2000];

int sun_traj_index = 0;
int sun_reset = 0;
int day_reset = day + 1;
// solar elevation calculation variables

// screen rotation
float rot = 0;
// screen rotation

int afps = 0;

void display3dText( float x, float y, float z, int r, int g, int b, char *print_text )
{
	glColor3f(r, g, b);

	glRasterPos3f(x, y, z); // (breite, höhe, tiefe)

	for(unsigned int i = 0; i < (unsigned)strlen(print_text); i++)
		glutBitmapCharacter(GLUT_BITMAP_8_BY_13, print_text[i]);
}

void display2dText( float x, float y, int r, int g, int b, char *print_text )
{
	glColor3f(r, g, b);

	glRasterPos2f(x, y); // (breite, höhe, tiefe)

	for(unsigned int i = 0; i < (unsigned)strlen(print_text); i++)
		glutBitmapCharacter(GLUT_BITMAP_8_BY_13, print_text[i]);
}

double GetTickCount(void) 
{
	struct timespec now;

	if (clock_gettime(CLOCK_MONOTONIC, &now))
	return 0;

	return now.tv_sec * 1000.0 + now.tv_nsec / 1000000.0;
}

char* CalculateFrameRate()
{
	static float lastTime = 0.0f;                // This will hold the time from the last frame
	float currentTime = GetTickCount() * 0.001f;    
	static float framesPerSecond = 0.0f;         // This will store our fps

	++framesPerSecond;

	if(currentTime - lastTime > 1.0f)
	{
		afps = framesPerSecond;
		lastTime = currentTime;

		char bufferf [5];
		sprintf (bufferf, "%i", afps);

		framesPerSecond = 0;
	}
	else
	{
		char bufferf [5];
		sprintf (bufferf, "%i", afps);
	}

	char* return_fps = new char;
	sprintf (return_fps, "FPS: %i", afps);

	return return_fps;
}
// FPS

// MOUSE (wheel)
void mouse(int button, int state, int x, int y)
{
   // button 3 = scroll up; button 4 = scroll down
	if ((button == 3) || (button == 4)) // It's a wheel event
	{
		if (state == GLUT_UP)
			return; // Disregard redundant GLUT_UP events

		if (button == 4) // scroll out
		{
			mousezoom = mousezoom * 0.9;
		}
		else // scroll in (button == 3)
		{
			mousezoom = mousezoom * 1.1;
		}
	}
	else // normal button event
	{
		if (button == GLUT_LEFT_BUTTON) // when the button is released
		{
			if (state == GLUT_UP)
			{
				angle += deltaAngle;
				mouse_down = false;
			}
			else // state = GLUT_DOWN -> mouse button pressed DOWN
			{
				mouse_down = true;
				test = 0;
			}
		}
   }

	// suppress "unused parameter ‘.’" warnings
	(void)x;
	(void)y;
}
// MOUSE (wheel)

// MOUSE MOVE
void mouseMove(int x, int y)
{
	if (mouse_down == true)
	{
		if (test == 0)
		{
			test = 1;
			cache_x = x;
			cache_y = y;
		}
		else
		{
			test = 0;
			dx = x - cache_x;
			dy = y - cache_y;

			cache_x = 0;
			cache_y = 0;

			if ((rotY < 35 && dy < 0) || (rotY > -35 && dy > 0))
				rotY -= dy*0.5;

			rotZ += dx*0.5;
		}
	}

	// suppress "unused parameter ‘.’" warnings
	(void)x;
	(void)y;
}
// MOUSE MOVE

// MOUSE-tracking without keys pressed
void mousepassivemove(int x, int y)
{
//		std::cout << "pass" << std::endl;
	// suppress "unused parameter ‘.’" warnings
	(void)x;
	(void)y;
}
// MOUSE-tracking without keys pressed

// GLUT idle function
void myIdleFunc()
{
	float rotate_scene = 0.1;
	bool rotate = 0;

	if (rotate == 1)
		rot += rotate_scene;

	glutPostRedisplay();
}
// GLUT idle function

// main function
int main(int argc, char **argv)
{

	test_function(42);

	glutInit(&argc, argv);							// Initialsierung der GLUT-Bibliothek
	glutInitDisplayMode(  GLUT_DOUBLE |			// doppelter Puffer
                         GLUT_DEPTH  |			// Tiefen Puffer vorhanden
                         GLUT_RGB    |       // Farbpuffer mit Rot,Grün und Blau
								 GLUT_MULTISAMPLE);  // 
   glutInitWindowSize(wnd_width, wnd_height);	// windowdimentions (width, height)
   glutInitWindowPosition(500, 100);			// windowposition (vertical, horizontal)

	glEnable( GL_BLEND ); 
	glDisable( GL_DEPTH_TEST );
	glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA ); 
	glHint( GL_LINE_SMOOTH_HINT, GL_NICEST ); 
	glHint( GL_POLYGON_SMOOTH_HINT, GL_NICEST ); 
	glDisable( GL_LINE_SMOOTH ); 
	glDisable( GL_POLYGON_SMOOTH ); 
	glDisable( GL_MULTISAMPLE );

   window1 = glutCreateWindow("opengl-cpp-template");	// windowtitle
 
   InitGL();

   glutDisplayFunc(&Render); // Funktion zum zeichnen des Bildes
   glutReshapeFunc(&Resize); // Funktion die auf Größenänderungen des Fensers eingeht
	glutSpecialFunc(&SpecialKey); // Funktion zur Behandlung von Sondertasten z.B.: Pfeiltasten
	glutKeyboardFunc(keyPressed); // Tell GLUT to use the method "keyPressed" for key presses  

	//MOUSE
	glutMouseFunc(mouse); // mousewheel and buttons
	glutIdleFunc(myIdleFunc);
	//MOUSE

	// transparency
	glEnable (GL_BLEND);
	glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	// transparency

   glutMainLoop();
}
// main function

// initialize OPENGL
void InitGL(void)
{
	glClearColor(0.0, 0.0, 0.0, 0.0); // backgroundcolor
	glEnable(GL_DEPTH_TEST); // set the depthbuffer
}
// initialize OPENGL

void SpecialKey(int key, int x, int y)
{
	switch (key)
	{
		case GLUT_KEY_F1: // F1 -> reset the program
			rot = 0;
			rotY = 0;
			rotZ = 0;
			mousezoom = 1;
      	break;

		case GLUT_KEY_F12: // F12 -> reset rotation
			std::cout << "F12" << std::endl;
			break;
	}

	// suppress "unused parameter ‘.’" warnings
	(void)x;
	(void)y;

   glutPostRedisplay();
}

void keyPressed (unsigned char key, int x, int y)
{  
  switch (key)
  {
    case 27: // Escape key
      glutDestroyWindow (window1);
      exit (0);
      break;

	case '-':
		if (time_increment_global_seconds == 2)
			time_increment_global_seconds = 1;
		else if (time_increment_global_seconds > 2)
			time_increment_global_seconds = time_increment_global_seconds / 2;

		time_increment_minutes = time_increment_global_seconds / 60;
		time_increment_seconds = time_increment_global_seconds % 60;
		time_increment_hours = time_increment_minutes / 60;
		time_increment_minutes = time_increment_minutes % 60;
      break;

	case '+':
		if (time_increment_global_seconds == 1)
			time_increment_global_seconds = 2;
		else if (time_increment_global_seconds < 32768)
			time_increment_global_seconds = time_increment_global_seconds * 2;

		time_increment_minutes = time_increment_global_seconds / 60;
		time_increment_seconds = time_increment_global_seconds % 60;
		time_increment_hours = time_increment_minutes / 60;
		time_increment_minutes = time_increment_minutes % 60;
      break;
  }

	// suppress "unused parameter ‘.’" warnings
	(void)x;
	(void)y;

  glutPostRedisplay();
} 

// resize window (OPENGL)
void Resize(int width, int height)
{
	glViewport(0, 0, (GLint)width, (GLint)height); 
	wnd_width = width;
	wnd_height= height;
}
// resize window (OPENGL)

void Render(void)
{
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   gluPerspective(100.0, 2.0, 2.0, 200.0);     
	gluLookAt(10, 0, -10, 0, 0, 0, 0, 0, -1);
	glScalef( mousezoom, mousezoom, mousezoom );
   glMatrixMode(GL_MODELVIEW);

   glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

	// POST ROTATION
   glRotatef(rotY, 0.0, 1.0, 0.0);			// mouserotation
   glRotatef(rotZ + rot, 0.0, 0.0, -1.0); // mouserotation

	// coordinate system
	glBegin(GL_LINES);
		glColor4f(1.0f, 0.0f, 0.0f, 1.0f);     // red
	   glVertex3f(0.0f, 0.0f, 0.0f);
	   glVertex3f(2.0f,  0.0f, 0.0f);

		glColor4f(0.0f, 1.0f, 0.0f, 1.0f);     // green
	   glVertex3f(0.0f, 0.0f, 0.0f);
	   glVertex3f(0.0f,  2.0f, 0.0f);

		glColor4f(0.0f, 0.0f, 1.0f, 1.0f);     // blue
	   glVertex3f(0.0f, 0.0f, 0.0f);
	   glVertex3f(0.0f,  0.0f, 2.0f);
	glEnd();
	// coordinate system


	// surface plane
	glColor4f(0.5f, 0.5f, 0.5f, 0.5f); // grey -> half transparent
	int dim_plane = 20;
	glBegin(GL_QUADS);
		glVertex3f( dim_plane, dim_plane, 0.0f);
		glVertex3f( dim_plane, -dim_plane, 0.0f);
		glVertex3f( -dim_plane, -dim_plane, 0.0f);
		glVertex3f( -dim_plane, dim_plane, 0.0f);
	glEnd();    
	// surface plane

	// increment time
	seconds += time_increment_seconds;

	if (seconds > 59 or time_increment_minutes > 0)
	{
		seconds = fabs(60 - seconds);
		minutes += time_increment_minutes + 1;
	}

	if (minutes > 59 or time_increment_hours > 0)
	{
		minutes = fabs(60 - minutes);
		hours += time_increment_hours + 1;
	}

	if (hours > 23 or time_increment_days > 0)
	{
		hours = fabs(23 - hours);
		day += time_increment_days + 1;
	}

	if (day > months_list[(int)(month - 1)])
	{
		if (month == 12)
		{
			month = 1;
			year += time_increment_years + 1;
		}
		else
			month += time_increment_months + 1;

		day = 1;
		day_reset = 1;
	}
	// increment time

	// HUD
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();

	glOrtho(0.0, wnd_width, wnd_height, 0.0, 0.0, 1.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glDisable(GL_CULL_FACE);

	glClear(GL_DEPTH_BUFFER_BIT);

	float spacing_hud_text = 12;
	float spacing_hud_offet_y = 15;

	char * fetch_fps = CalculateFrameRate();
	display2dText(5, spacing_hud_offet_y, 1, 1, 1, fetch_fps);

	char buffer [15];
	sprintf (buffer, "R_y: %.1f", rotZ);
	char *rotZ_print = buffer;
	display2dText(5, spacing_hud_offet_y + spacing_hud_text, 1, 1, 1, rotZ_print);

	char buffer2 [15];
	sprintf (buffer2, "R_x: %.1f", rotY);
	char *rotY_print  = buffer2;
	display2dText(5, spacing_hud_offet_y + 2 * spacing_hud_text, 1, 1, 1, rotY_print);

	char buffer3 [50];
	sprintf (buffer3, "Time: %.0f-%02.0f-%02.0f  %02.0f:%02.0f:%02.0f", year, month, day, hours, minutes, seconds);
	char *date_print  = buffer3;
	display2dText(5, spacing_hud_offet_y + 3 * spacing_hud_text, 1, 1, 1, date_print);

	char buffer4 [25];
	sprintf (buffer4, "+dt: %i:%i:%i", time_increment_hours, time_increment_minutes, time_increment_seconds);
	char *increment_time = buffer4;
	display2dText(5, spacing_hud_offet_y + 4 * spacing_hud_text, 1, 1, 1, increment_time); 
	// HUD


	glutPassiveMotionFunc(mousepassivemove);
	glutMotionFunc(mouseMove);
   
   glutSwapBuffers(); // copy back-buffer into the front-buffer
}
