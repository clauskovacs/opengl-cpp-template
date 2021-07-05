/* This file contains the (header)functions used to render graphics on the screen.
 * 
 * These are mainly glut- and OpenGL functions, responsible for drawing
 * on the screen as well as handling the user input (mouse and keyboard).
 */

#ifndef GRAPHICSRENDERING_H
#define GRAPHICSRENDERING_H

/**************/
/** variables */
/**************/
// TODO: move these variables into a class?!

// mouse rotate
extern float scene_rotate_y;
extern float scene_rotate_z;
extern bool mouse_pressed_down;
extern float cache_x_pos;
extern float cache_y_pos;
extern bool mouse_move_flag;
extern int window_id;

extern float auto_rotate_z;	// (automatically) screen rotation

// height and width of the window
extern int window_width;
extern int window_height;

extern float scale_geometry;	// changed via the mousewheel action

/**************/
/** functions */
/**************/
void initGL(void);
void render(void);
void render_3d_text(float x, float y, float z, int r, int g, int b, char* print_text);
void render_2d_text(float x, float y, int r, int g, int b, char* print_text);
void mouse_motion(int mouse_x_pos, int mouse_y_pos);
void mouse_passive_move(int mouse_x_pos, int mouse_y_pos);
void resize(int new_width, int new_height);
void glut_special_key_event(int key_pressed, int mouse_pos_x, int mouse_pos_y); //special keys (F1, F2, ... )
void glut_normal_key_event(unsigned char key_pressed, int mouse_pos_x, int mouse_pos_y); // (keys a .. z, esc ...)
void mouse_callback(int button_pressed, int mouse_state, int mouse_x_pos, int mouse_y_pos);
void glut_idle_function();

#endif
