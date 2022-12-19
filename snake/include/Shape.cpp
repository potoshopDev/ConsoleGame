// Copyright © 2022 Oleg Belekhov. All Right Reserved.
// Licensed under the CC0 License
#include "Shape.h"
#include "Core.h"

namespace emodule
{

	void update_empty(shape_data* sd__, size_t ID, COORD buf_suz)
	{

	}

	short convert_coor(short x, short y, short maxx)
	{
		return maxx * y + x;
	}

	void update_linear(shape_data* sd__, size_t ID, COORD buf_sz)
	{
		//read var
		const auto [bx, by] = buf_sz;
		auto & buf = sd__->char_buffer;
		const auto [str_p, end_p] = sd__->position[ID];
		const auto [sx, sy] = str_p;
		const auto [ex, ey] = end_p;
		const auto [vec_x, vec_y] = sd__->vector_velocity[ID];

		const auto nx = vec_x + sx;
		const auto ny = vec_y + sy;

		if (nx < bx && ny < by && nx >= 0 && ny >= 0)
		{
			buf[convert_coor(nx, ny, bx)].Char.UnicodeChar = L't';
			buf[convert_coor(nx, ny, bx)].Attributes = FOREGROUND_GREEN;
			buf[convert_coor(ex, ey, bx)].Char.UnicodeChar = 0;
			//TODO: what next last coor?
			sd__->position[ID] = emodule::str_end_pos(spoint(nx, ny), spoint(ex, ey));
		}
		else
		{
			core::EventManager::create_event(event_type::EVENT_ERROR, emodule::error_code::ERROR_OUT_OF_RANGE);
		}

	}


}
