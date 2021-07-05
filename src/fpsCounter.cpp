/* This file contains the methods for controlling/determining
 * the frames per second (fps).
 */

#include <iostream>
#include <time.h>

#include <fpsCounter.hpp>

double get_tick_count(void) 
{
	struct timespec now;

	if (clock_gettime(CLOCK_MONOTONIC, &now))
	{
		return 0;
	}

	return now.tv_sec * 1000.0 + now.tv_nsec / 1000000.0;
}

char* calc_frame_rate()
{
	static float last_frame_time = 0.0f;	// This will hold the time from the last frame
	float current_time = get_tick_count() * 0.001f;
	static float frames_per_second = 0.0f;	// This will store our fps

	++frames_per_second;

	if (current_time - last_frame_time > 1.0f)
	{
		actual_fps = frames_per_second;
		last_frame_time = current_time;

		char temp_buffer[5];
		sprintf(temp_buffer, "%i", actual_fps);

		frames_per_second = 0;
	}
	else
	{
		char temp_buffer[5];
		sprintf(temp_buffer, "%i", actual_fps);
	}

	char* return_fps = new char;
	sprintf(return_fps, "FPS: %i", actual_fps);

	return return_fps;
}
