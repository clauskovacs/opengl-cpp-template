#ifdef _WIN32
	#include <windows.h>
	#include <gl/gl.h>
	#define GLUT_DISABLE_ATEXIT_HACK
#endif

#include <GL/glut.h>

#include "graphicsRendering.hpp"

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(	GLUT_DOUBLE	|
							GLUT_DEPTH	|
							GLUT_RGB	|
							GLUT_MULTISAMPLE);

	glutInitWindowSize(window_width, window_height);
	glutInitWindowPosition(500, 100);	// set the windowposition (vertical, horizontal)

	glEnable(GL_BLEND);
	glDisable(GL_DEPTH_TEST);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
	glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);
	glDisable(GL_LINE_SMOOTH);
	glDisable(GL_POLYGON_SMOOTH);
	glDisable(GL_MULTISAMPLE);

	window_id = glutCreateWindow("opengl-cpp-template");

	initGL();

	glutDisplayFunc(&render);
	glutReshapeFunc(&resize);
	glutSpecialFunc(&glut_special_key_event);
	glutKeyboardFunc(glut_normal_key_event); 

	//MOUSE
	glutMouseFunc(mouse_callback);
	glutIdleFunc(glut_idle_function);

	// transparency
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// start the main loop
	glutMainLoop();
}
