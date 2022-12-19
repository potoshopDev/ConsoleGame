// Copyright © 2022 Oleg Belekhov. All Right Reserved.
// Licensed under the CC0 License

//module Core, shape, scoreboard
#pragma once


#include <iostream>
#include <memory>
#include "include/Core.h"
#include "include/Shape.h"
#include "Input.h"

namespace core
{
	using event_man = core::EventManager;
	using event_t = emodule::event_type;

	constexpr short g_WinSizeX = 100;
	constexpr short g_WinSizeY = 25;
	constexpr short g_FontSirze = 8;



	class Engine final
	{
	public:
		Engine()
		{
			//subscribe handle error
			auto tmp_em = [this](int i) {Engine::ErrorManager(i); };
			core::EventManager::subscribe(tmp_em, emodule::event_type::EVENT_ERROR);

			if (!InitConsole())
			{
				core::EventManager::create_event(emodule::event_type::EVENT_ERROR, static_cast<int>(emodule::error_code::ERROR_INIT));
			}

			//create main start_event
			core::EventManager::create_event(emodule::event_type::EVENT_START);

			const short snake_size = 3i16;
			const auto snake_pos = emodule::str_end_pos({ g_WinSizeX / 2 - snake_size, g_WinSizeY / 2 }, { g_WinSizeX / 2, g_WinSizeY / 2 });
			shapes__.push_back(static_cast<std::unique_ptr < emodule::IShape>>(new emodule::Snake(&data__, snake_pos)));

			auto tmp_update = [this]() { shapes__[0]->Update_Draw(emodule::update_linear); };
			core::EventManager::subscribe(tmp_update, emodule::event_type::EVENT_UPDATE);

			const auto wall_pos1 = emodule::str_end_pos({ 0, 0 }, { g_WinSizeX, 0 });
			const auto wall_pos2 = emodule::str_end_pos({ 0, 1 }, { 0, g_WinSizeY });
			const auto wall_pos3 = emodule::str_end_pos({ g_WinSizeX, 1 }, { g_WinSizeX, g_WinSizeY });
			const auto wall_pos4 = emodule::str_end_pos({ 1, g_WinSizeY }, { g_WinSizeX - 1, g_WinSizeY - 1 });

			shapes__.push_back(static_cast<std::unique_ptr < emodule::IShape>>(new emodule::Wall(&data__, wall_pos1)));
			shapes__.push_back(static_cast<std::unique_ptr < emodule::IShape>>(new emodule::Wall(&data__, wall_pos2)));
			shapes__.push_back(static_cast<std::unique_ptr < emodule::IShape>>(new emodule::Wall(&data__, wall_pos3)));
			shapes__.push_back(static_cast<std::unique_ptr < emodule::IShape>>(new emodule::Wall(&data__, wall_pos4)));

			auto tmp_input = [this]() {InputConsole->GetFocus(); InputConsole->GetPressedKey(); };
			core::EventManager::subscribe(tmp_input, emodule::event_type::EVENT_UPDATE);

			auto tmp_input_manag = [this](int i) {InputManager(i); };
			core::EventManager::subscribe(tmp_input_manag, emodule::event_type::EVENT_INPUT);
		}

		bool InitConsole() noexcept;
		//error handle
		void ErrorManager(int code_error);

		void InputManager(int code_key);

		void Update_Draw() noexcept;
		void Draw() noexcept;

	private:
		std::vector<std::unique_ptr<emodule::IShape>> shapes__;
		emodule::shape_data data__;

		COORD buffer_size{ g_WinSizeX, g_WinSizeY };

		SMALL_RECT full_rect{ 0, 0, g_WinSizeX - 1, g_WinSizeY - 1 };
		HANDLE console_handle{ nullptr };


		std::unique_ptr<emodule::IInput> InputConsole = static_cast<std::unique_ptr<emodule::IInput>>(new emodule::ConsoleInput);

		bool work_correct = true;
	};
}
