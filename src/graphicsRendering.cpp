/* This file contains the functions used to render graphics on the screen.
 * 
 * These are mainly glut- and OpenGL functions, responsible for drawing
 * on the screen as well as handling the user input (mouse and keyboard).
 */

#ifdef _WIN32
	#include <windows.h>
	#include <gl/gl.h>
	#define GLUT_DISABLE_ATEXIT_HACK
#endif

#include <GL/glut.h>
#include <iostream>
#include <cstring>

#include "graphicsRendering.hpp"
#include "fpsCounter.hpp"

/**************/
/** variables */
/**************/

// TODO: move these variables into a class?!
// mouse rotate
float scene_rotate_y = 1;
float scene_rotate_z = 1;
bool mouse_pressed_down = false;
float cache_x_pos = 0.0;
float cache_y_pos = 0.0;
bool mouse_move_flag = false;
int window_id;

float auto_rotate_z = 0;	// (automatically) screen rotation
int actual_fps = 0;

/**************/
/** functions */
/**************/

// height and width of the window
int window_width  = 1024;
int window_height = 768;

float scale_geometry = 1;	// changed via the mousewheel action

void initGL(void)
{
	glClearColor(0.0, 0.0, 0.0, 0.0); // backgroundcolor
	glEnable(GL_DEPTH_TEST); // set the depthbuffer
}


void render(void)
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(100.0, 2.0, 2.0, 200.0);
	gluLookAt(10.0, 0.0, -10.0, 0.0, 0.0, 0.0, 0.0, 0.0, -1.0);
	glScalef(scale_geometry, scale_geometry, scale_geometry);
	glMatrixMode(GL_MODELVIEW);

	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

	// set the rotation of the scenery
	glRotatef(scene_rotate_y, 0.0, 1.0, 0.0);
	glRotatef(scene_rotate_z + auto_rotate_z, 0.0, 0.0, -1.0);

	// draw the coordinate system
	glBegin(GL_LINES);
		glColor4f(1.0f, 0.0f, 0.0f, 1.0f);	// red
		glVertex3f(0.0f, 0.0f, 0.0f);
		glVertex3f(2.0f, 0.0f, 0.0f);

		glColor4f(0.0f, 1.0f, 0.0f, 1.0f);	// green
		glVertex3f(0.0f, 0.0f, 0.0f);
		glVertex3f(0.0f,  2.0f, 0.0f);

		glColor4f(0.0f, 0.0f, 1.0f, 1.0f);	// blue
		glVertex3f(0.0f, 0.0f, 0.0f);
		glVertex3f(0.0f,  0.0f, 2.0f);
	glEnd();

	// (grey) surface plane
	glColor4f(0.5f, 0.5f, 0.5f, 0.5f); // grey, half transparent
	int dim_plane = 20;
	glBegin(GL_QUADS);
		glVertex3f(dim_plane, dim_plane, 0.0f);
		glVertex3f(dim_plane, -dim_plane, 0.0f);
		glVertex3f(-dim_plane, -dim_plane, 0.0f);
		glVertex3f(-dim_plane, dim_plane, 0.0f);
	glEnd();

	// draw the HUD
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();

	glOrtho(0.0, window_width, window_height, 0.0, 0.0, 1.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glDisable(GL_CULL_FACE);

	glClear(GL_DEPTH_BUFFER_BIT);

	float spacing_hud_text = 12;
	float spacing_hud_offet_y = 15;

	char* fetch_fps = calc_frame_rate();
	render_2d_text(5, spacing_hud_offet_y, 1, 1, 1, fetch_fps);

	char buffer[15];
	sprintf(buffer, "R_y: %.1f", scene_rotate_z);
	char* scene_rotate_z_print = buffer;
	render_2d_text(5, spacing_hud_offet_y + spacing_hud_text, 1, 1, 1, scene_rotate_z_print);

	char buffer2[15];
	sprintf(buffer2, "R_x: %.1f", scene_rotate_y);
	char* scene_rotate_y_print  = buffer2;
	render_2d_text(5, spacing_hud_offet_y + 2 * spacing_hud_text, 1, 1, 1, scene_rotate_y_print);

	glutPassiveMotionFunc(mouse_passive_move);
	glutMotionFunc(mouse_motion);

	glutSwapBuffers(); // copy back buffer into the front buffer
}



void render_3d_text(float x, float y, float z, int r, int g, int b, char* print_text)
{
	glColor3f(r, g, b);
	glRasterPos3f(x, y, z); // set the 3d position: (x, y, z) - coordinates

	for (unsigned int i = 0; i < (unsigned)strlen(print_text); i++)
	{
		glutBitmapCharacter(GLUT_BITMAP_8_BY_13, print_text[i]);
	}
}

void render_2d_text(float x, float y, int r, int g, int b, char* print_text)
{
	glColor3f(r, g, b);
	glRasterPos2f(x, y); // set the 2d position: (x, y) - coordinates

	for (unsigned int i = 0; i < (unsigned)strlen(print_text); i++)
	{
		glutBitmapCharacter(GLUT_BITMAP_8_BY_13, print_text[i]);
	}
}


void mouse_motion(int mouse_x_pos, int mouse_y_pos)
{
	float delta_x, delta_y;

	// change the rotatation of the scenery if the mouse is pressed down
	if (mouse_pressed_down == true)
	{
		if (mouse_move_flag == false)
		{
			mouse_move_flag = true;
			cache_x_pos = mouse_x_pos;
			cache_y_pos = mouse_y_pos;
		}
		else
		{
			mouse_move_flag = false;
			delta_x = mouse_x_pos - cache_x_pos;
			delta_y = mouse_y_pos - cache_y_pos;

			cache_x_pos = 0;
			cache_y_pos = 0;

			if ((scene_rotate_y < 35 && delta_y < 0) || (scene_rotate_y > -35 && delta_y > 0))
			{
				scene_rotate_y -= delta_y * 0.5;
			}

			scene_rotate_z += delta_x * 0.5;
		}
	}

	// suppress "unused parameter ‘...’" warnings
	(void)mouse_x_pos;
	(void)mouse_y_pos;
}

void mouse_passive_move(int mouse_x_pos, int mouse_y_pos)
{
	// suppress "unused parameter ‘...’" warnings
	(void)mouse_x_pos;
	(void)mouse_y_pos;
}


void resize(int new_width, int new_height)
{
	glViewport(0, 0, (GLint)new_width, (GLint)new_height);
	window_width = new_width;
	window_height= new_height;
}


void glut_special_key_event(int key_pressed, int mouse_pos_x, int mouse_pos_y)
{
	switch (key_pressed)
	{
		case GLUT_KEY_F1: // F1 -> reset the program
			auto_rotate_z = 0;
			scene_rotate_y = 0;
			scene_rotate_z = 0;
			scale_geometry = 1;
			break;
	}

	// suppress "unused parameter ‘...’" warnings
	(void)mouse_pos_x;
	(void)mouse_pos_y;

	glutPostRedisplay();
}


void glut_normal_key_event(unsigned char key_pressed, int mouse_pos_x, int mouse_pos_y)
{
	switch (key_pressed)
	{
		case 27: // Escape key
			glutDestroyWindow(window_id);
			exit (0);
			break;

		case '-':
			break;

		case '+':
			break;
	}

	// suppress "unused parameter ‘...’" warnings
	(void)mouse_pos_x;
	(void)mouse_pos_y;

	glutPostRedisplay();
}


void mouse_callback(int button_pressed, int mouse_state, int mouse_x_pos, int mouse_y_pos)
{
	// button 3 = scroll up; button 4 = scroll down
	if ((button_pressed == 3) || (button_pressed == 4)) // mousewheel event
	{
		if (mouse_state == GLUT_UP)
		{
			return; // Disregard redundant GLUT_UP events
		}

		if (button_pressed == 4) // scroll out
		{
			scale_geometry = scale_geometry * 0.9;
		}
		else // scroll in (button == 3)
		{
			scale_geometry = scale_geometry * 1.1;
		}
	}
	else // normal button event
	{
		if (button_pressed == GLUT_LEFT_BUTTON)
		{
			if (mouse_state == GLUT_UP)
			{
				mouse_pressed_down = false;
			}
			else // mouse_state = GLUT_DOWN -> mouse button pressed DOWN
			{
				mouse_pressed_down = true;
				mouse_move_flag = false;
			}
		}
	}

	// suppress "unused parameter ‘...’" warnings
	(void)mouse_x_pos;
	(void)mouse_y_pos;
}

void glut_idle_function()
{
	// automatically rotate the scenery
	bool auto_rotate = false;
	float rotate_scene = 0.1;

	if (auto_rotate)
	{
		auto_rotate_z += rotate_scene;
	}

	glutPostRedisplay();
}
