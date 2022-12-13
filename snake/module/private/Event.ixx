// Copyright © 2022 Oleg Belekhov. All Right Reserved.
// Licensed under the CC0 License

export module Event;
import Source;
import <functional>;
import <vector>;
import <algorithm>;
import <iterator>;
import <variant>;

namespace emodule
{
	
	export
		enum class event_type
	{
		EVENT_NONE = 0,
		EVENT_START = 1,
		EVENT_STOP 

	};

	export using func_null_args = std::function<void()>;
	export using func_one_arg = std::function<void(int)>;
	export using func_null_or_one_arg = std::variant<func_null_args, func_one_arg>;
	using QueueSubs = std::vector < std::pair<event_type, func_null_or_one_arg>>;
	using QueueEvent = std::vector<std::pair<emodule::event_type, std::variant<bool, int>>>;

	export
		class IEvent
	{
	public:
		virtual ~IEvent() {};

	public:

		virtual void subscribe(func_null_or_one_arg func__, event_type et__) = 0;
		virtual void create_event(event_type et__) = 0;
		virtual void create_event(event_type et__, int arg__) = 0;
		virtual void call() = 0;

	};

export
	class Event : public IEvent
	{
	public:
		virtual void subscribe(func_null_or_one_arg func__, event_type et__) override;
		virtual void create_event(event_type et__) override;
		virtual void create_event(event_type et__, int arg__) override;
		virtual void call() override;

	private:

		QueueSubs queue_subs;
		QueueEvent queue_event;
	};

	void Event::subscribe(func_null_or_one_arg func__, event_type et__)
	{
		queue_subs.push_back(std::pair(et__, func__));
	}

	void Event::create_event(event_type et__)
	{
		queue_event.push_back(std::pair(et__, false));
	}

	void Event::create_event(event_type et__, int arg__)
	{
		queue_event.push_back(std::pair(et__, arg__));
	}

	void Event::call()
	{
		for (auto& e : queue_event)
			for (auto& s : queue_subs)
				if ((s.first == e.first))
				{
					bool have_arg = true;
					int arg = 0;
					overload o{
						[&have_arg](bool& b) {have_arg = false; },
						[&arg](int& i) {arg = i; }
					};
					std::visit(o, e.second);

					overload o2{
						[](func_null_args& f) { f(); },
						[&have_arg, &arg](func_one_arg& f) { have_arg ? f(arg) : f(0); }
					};

					std::visit(o2, s.second);

				}

		queue_event.clear();
	}

}
