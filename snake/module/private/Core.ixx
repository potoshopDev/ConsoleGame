// Copyright © 2022 Oleg Belekhov. All Right Reserved.
// Licensed under the CC0 License

//module Event, Gameplay, Input

export module Core;
import Event;
import <memory>;



namespace core
{


	export
		class EventManager
	{
	public:

		static inline void subscribe(emodule::lambda_null_or_one_arg func__, emodule::event_type et__) noexcept
		{
			if (event_manag.get())
			{
				event_manag.get()->subscribe(func__, et__);
			}
		}
		static inline void create_event(emodule::event_type et__) noexcept
		{
			if (event_manag.get())
			{
				event_manag.get()->create_event(et__);
			}
		}
		static inline void create_event(emodule::event_type et__, int arg) noexcept
		{
			if (event_manag.get())
			{
				event_manag.get()->create_event(et__, arg);
			}
		}
		static inline void call() noexcept
		{
			if (event_manag.get())
			{
				event_manag.get()->call();
			}
		}


	private:

		static std::unique_ptr< emodule::IEvent>event_manag;
		
		explicit EventManager() = delete;
		explicit EventManager(const EventManager&) = delete;
		explicit EventManager(const EventManager&&) = delete;
		EventManager& operator=(const EventManager& other) = delete;
		EventManager& operator=(const EventManager&& other) = delete;

	};
	std::unique_ptr< emodule::IEvent> EventManager::event_manag = static_cast< std::unique_ptr<emodule::IEvent>>(new emodule::Event());
}
