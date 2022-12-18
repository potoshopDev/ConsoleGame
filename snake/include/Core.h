// Copyright © 2022 Oleg Belekhov. All Right Reserved.
// Licensed under the CC0 License

//module Event, Game play, Input
#pragma once

#include <memory>
#include "Event.h"


namespace core
{

	class EventManager
	{
	public:
		EventManager()
		{}
		//EventManager()
		//{
		//	//event_manag = static_cast<std::unique_ptr<emodule::IEvent>>(new emodule::Event());
		//	//std::unique_ptr< emodule::IEvent> EventManager::event_manag = static_cast<std::unique_ptr<emodule::IEvent>>(new emodule::Event());
		//}
		static int subscribe(emodule::lambda_null_or_one_arg func__, emodule::event_type et__) noexcept
		{
			if (event_manag.get())
			{
				return event_manag.get()->subscribe(func__, et__);
			}
			return -1; //#error, event_manag not valid!!!
		}
		static void create_event(emodule::event_type et__) noexcept
		{
			if (event_manag.get())
			{
				event_manag.get()->create_event(et__);
			}
		}
		static void create_event(emodule::event_type et__, int arg) noexcept
		{
			if (event_manag.get())
			{
				event_manag.get()->create_event(et__, arg);
			}
		}
		static void call() noexcept
		{
			if (event_manag.get())
			{
				event_manag.get()->call();
			}
		}
		static void unsubscribe(int key) noexcept
		{
			if (event_manag.get())
			{
				event_manag.get()->unsubscribe(key);
			}
		}


	private:


		static std::unique_ptr< emodule::IEvent>event_manag;
		explicit EventManager(const EventManager&) = delete;
		explicit EventManager(const EventManager&&) = delete;
		EventManager& operator=(const EventManager& other) = delete;
		EventManager& operator=(const EventManager&& other) = delete;

	};

}

