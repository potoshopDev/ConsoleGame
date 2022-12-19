// Copyright © 2022 Oleg Belekhov. All Right Reserved.
// Licensed under the CC0 License
#pragma once

#include <functional>
#include <vector>
#include <variant>
#include <map>
#include "Source.h"

namespace emodule
{
	enum class event_type
	{
		EVENT_NONE = 0,
		EVENT_START = 1,
		EVENT_STOP,
		EVENT_ERROR,
		EVENT_UPDATE,
		EVENT_INPUT,


	};


	enum error_code : int
	{
		ERROR_SYNC = 1,
		ERROR_INIT,
		ERROR_OUT_OF_RANGE,
	};

	using lambda_null_args = std::function<void()>;
	using lambda_one_arg = std::function<void(int)>;
	using lambda_null_or_one_arg = std::variant<lambda_null_args, lambda_one_arg>;
	using QueueSubs = std::map <int, std::pair<event_type, lambda_null_or_one_arg>>;
	using QueueEvent = std::vector<std::pair<emodule::event_type, std::variant<bool, int>>>;


	class IEvent
	{
	public:
		virtual inline ~IEvent() {};

		virtual inline int subscribe(lambda_null_or_one_arg func__, event_type et__) noexcept = 0;
		virtual inline void create_event(event_type et__) noexcept = 0;
		virtual inline void create_event(event_type et__, int arg__) noexcept = 0;
		virtual inline void call() noexcept = 0;
		virtual inline void unsubscribe(int key) noexcept = 0;

	};


	class Event final : public IEvent
	{
	public:

		virtual inline int subscribe(lambda_null_or_one_arg func__, event_type et__) noexcept override;
		virtual inline void create_event(event_type et__) noexcept override;
		virtual inline void create_event(event_type et__, int arg__) noexcept override;
		virtual inline void call() noexcept override;
		virtual inline void unsubscribe(int key) noexcept override;

	private:

		QueueSubs queue_subs;
		QueueEvent queue_event;

	};


	void Event::unsubscribe(int key) noexcept
	{
		auto it_elem = queue_subs.find(key);
		if (it_elem != queue_subs.end())
		{
			queue_subs.erase(it_elem);
		}

	}
	int Event::subscribe(lambda_null_or_one_arg func__, event_type et__) noexcept
	{
		auto key = 0;
		if (!queue_subs.empty())
		{
			key = queue_subs.rbegin()->first;
			++key;
		}
		queue_subs[key] = std::pair(et__, func__);
		return key;
	}

	void Event::create_event(event_type et__) noexcept
	{
		queue_event.push_back(std::pair(et__, false));
	}

	void Event::create_event(event_type et__, int arg__) noexcept
	{
		queue_event.push_back(std::pair(et__, arg__));
	}

	void Event::call() noexcept
	{
		auto qe = std::move(queue_event);
		for (auto& e : qe)
			for (auto& s : queue_subs)
				if ((s.second.first == e.first))
				{
					bool have_arg = true;
					int arg = 0;
					overload o{
						[&have_arg](bool& b) noexcept {have_arg = false; }, // event haven't arg
						[&arg](int& i) noexcept {arg = i; } //ok, event have arg
					};
					std::visit(o, e.second);

					overload o2{
						[](lambda_null_args& f) noexcept { f(); }, // call lambda no param
						[&have_arg, &arg](lambda_one_arg& f) noexcept { have_arg ? f(arg) : f(0); } // call lambda one param
					};
					 std::visit(o2, s.second.second);
				}

		qe.clear();
	}
}
