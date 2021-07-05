/* This file contains the (header)methods for controlling/determining
 * the frames per second (fps).
 */

#ifndef FPSCOUNTER_H
#define FPSCOUNTER_H

extern int actual_fps;

double get_tick_count(void);
char* calc_frame_rate();

#endif
