/*
  wallbot.cpp - Implements wall/string bot kinemetics
  Part of Grbl_ESP32

  Copyright (c) 2019 Barton Dring @buildlog


  Grbl is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  Grbl is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with Grbl.  If not, see <http://www.gnu.org/licenses/>.

	
*/
#include "grbl.h"

#ifdef USE_KINEMATICS

/*
 Apply inverse kinematics for a polar system

 float target: 							The desired target location in machine space
 plan_line_data_t *pl_data: Plan information like feed rate, etc
 float *position:						The previous "from" location of the move

 Note: It is assumed only the radius axis is homed and only X and Z have offsets


*/
void inverse_kinematics(float *target, plan_line_data_t *pl_data, float *position)
{
	

	float dx, dy, dz; // distances in each axis
	float p_dx, p_dy, p_dz; // distances in each polar axis

	float dist; // the distances in both systems...used to determine feed rate
	float new_feedrate;  //

	uint32_t segment_count;  // number of segments the move will be broken in to.
	float seg_target[N_AXIS]; // The target of the current segment

	float converted[N_AXIS]; // target location in new coordinate system
	
	float half_width = (fabs(home_travel[X_AXIS]) + settings.homing_pulloff + BELT_LENGTH_CAL) / 2.0;
	float height = - settings.max_travel[Y_AXIS];

	float x_offset = gc_state.coord_system[X_AXIS]+gc_state.coord_offset[X_AXIS]; // offset from machine coordinate system
	float z_offset = gc_state.coord_system[Z_AXIS]+gc_state.coord_offset[Z_AXIS]; // offset from machine coordinate system
	
	//grbl_sendf(CLIENT_SERIAL, "[MSG: half_width:%4.3f height:%4.3f]\r\n", half_width, height);

	// calculate move distance for each axis in the cartesian system
	dx = target[X_AXIS] - position[X_AXIS];
	dy = target[Y_AXIS] - position[Y_AXIS];
	dz = target[Z_AXIS] - position[Z_AXIS];

	// calculate the total 2 axis move distance, so we know how many small sements we need
	// Z axis is the same in both coord systems
	dist = sqrt( (dx * dx) + (dy * dy) + (dz * dz));

	if (pl_data->condition & PL_COND_FLAG_RAPID_MOTION) {
		segment_count = 1;    // rapid G0 motion is not used to draw, so skip the segmentation
	} else {
		segment_count = ceil(dist / SEGMENT_LENGTH);    // determine the number of segments we need	... round up so there is at least 1
	}

	dist /= segment_count;  //

	//grbl_sendf(CLIENT_SERIAL, "[MSG: Segments:%d]\r\n", segment_count);

	for(uint32_t segment = 1; segment <= segment_count; segment++) {
		
		float dist_cart, dist_conv;  // used to comapre distance in cartesian to the new converted system for feed rate comp 
		
		// determine this segment's target
		seg_target[X_AXIS] = position[X_AXIS] + (dx / float(segment_count) * segment) - x_offset;
		seg_target[Y_AXIS] = position[Y_AXIS] + (dy / float(segment_count) * segment);
		seg_target[Z_AXIS] = position[Z_AXIS] + (dz / float(segment_count) * segment) - z_offset;
		
		dist_cart = hypot_f(seg_target[X_AXIS], seg_target[Y_AXIS]);

		
		converted[X_AXIS] = hypot_f(half_width + seg_target[X_AXIS], height - seg_target[Y_AXIS]) + settings.max_travel[Z_AXIS]; //- ARM_LENGTH_CAL;
		converted[Y_AXIS] = hypot_f(half_width - seg_target[X_AXIS], height - seg_target[Y_AXIS]) + settings.max_travel[Z_AXIS]; // - ARM_LENGTH_CAL;
		converted[Z_AXIS] = seg_target[Z_AXIS];
		
		dist_cart = hypot_f(seg_target[X_AXIS], seg_target[Y_AXIS]); // distance of this segment in cartesian system
		dist_conv = hypot_f(converted[X_AXIS], converted[Y_AXIS]);  // distance of this segment in converted system
		pl_data->feed_rate *= dist_conv / dist_cart;  // apply the distance ratio between coord systems to the feed rate		
		
		//grbl_sendf(CLIENT_SERIAL, "[MSG: X %4.3f %4.3f  Y %4.3f %4.3f]\r\n", seg_target[X_AXIS], converted[X_AXIS], seg_target[Y_AXIS], converted[Y_AXIS]);

		mc_line(converted, pl_data);
	}
}

#endif