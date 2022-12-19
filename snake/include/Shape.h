// Copyright © 2022 Oleg Belekhov. All Right Reserved.
// Licensed under the CC0 License
#pragma once

#include <vector>

#define WIN32_LEAN_AND_MEAN
#include<windows.h>

namespace emodule
{
	struct spoint
	{
		short x = 0;
		short y = 0;
	};


	using str_end_pos = std::pair<spoint, spoint>;
	struct shape_data
	{
		std::vector<str_end_pos> position;
		std::vector<spoint> vector_velocity;
		std::vector<CHAR_INFO> char_buffer;
		std::vector<short> color;
	};

	void update_empty(shape_data* sd__, size_t ID, COORD buf_suz) ;
	void update_linear(shape_data* sd__, size_t ID, COORD buf_suz);
	//	void update_blink(shape_data* sd__, size_t ID, COORD buf_suz);

	using update_func = void(*) (shape_data*, size_t, COORD);

	class IShape
	{
	public:
		virtual inline ~IShape() {};

		virtual inline void Initilize(shape_data* sd__, str_end_pos pos__) noexcept = 0;
		virtual inline size_t GetID() noexcept = 0;
		virtual inline void Update_Draw(update_func uf__) noexcept = 0;
		virtual inline void SetColor(short col__) noexcept = 0;


	};

	class StaticShape : public IShape
	{
	public:
		virtual inline void Initilize(shape_data* sd__, str_end_pos pos__) noexcept override;
		virtual inline size_t GetID() noexcept override;
		virtual inline void Update_Draw(update_func uf__) noexcept override;
		virtual inline void SetColor(short col__) noexcept override;

	protected:
		[[nodiscard]] inline shape_data* GetData() const noexcept
		{
			return data;
		}
	private:
		shape_data* data = nullptr;
		size_t ID = -1;
		short color = BACKGROUND_RED | BACKGROUND_GREEN | FOREGROUND_INTENSITY;

	};

	class Wall final: public StaticShape
	{
	public:
		Wall(shape_data* sd__, str_end_pos pos__)
		{
			StaticShape::Initilize(sd__, pos__);
		}

	};

	class DynamicShape : public StaticShape
	{
	public:
		virtual inline void Initilize(shape_data* sd__, str_end_pos pos__) noexcept override;
	private:
		const spoint default_vel = spoint(1i16, 0i16); //default move right
	};

	class Snake final: public DynamicShape
	{
	public:
		Snake(shape_data* sd__, str_end_pos pos__)
		{
			DynamicShape::Initilize(sd__, pos__);
		}
	};

	void StaticShape::Initilize(shape_data* sd__, str_end_pos pos__) noexcept
	{
		if (!data && sd__)
		{
			data = sd__;
			data->position.push_back(pos__);
			data->vector_velocity.push_back(spoint(0i16, 0i16));
			data->color.push_back(color);

			ID = data->position.size() - 1;
			auto ID_vel = data->vector_velocity.size() - 1;
			auto ID_col = data->color.size() - 1;

			if (ID != ID_col && ID != ID_vel)
			{
				ID = -1;//core check this id and create event_error
			}
		}
	}

	size_t StaticShape::GetID() noexcept
	{
		return ID;
	}

	void StaticShape::Update_Draw(update_func uf__) noexcept
	{
		auto ref_data = GetData();
		auto id = GetID();
		if (!ref_data || id < 0) return;

		uf__(ref_data, id, COORD{100, 25}); //TODO: refactoring 
	}

	void StaticShape::SetColor(short col__) noexcept
	{
		if (data)
		{
			color = col__;
			data->color.at(ID) = color;
		}
	}

	void DynamicShape::Initilize(shape_data* sd__, str_end_pos pos__) noexcept
	{
		StaticShape::Initilize(sd__, pos__);
		auto ref_data = GetData();
		auto id = GetID();

		if (id >= 0 && ref_data)
		{
			SetColor(FOREGROUND_GREEN);
			ref_data->vector_velocity.at(id) = default_vel;
			
		}
	}

}
