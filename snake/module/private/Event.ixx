// Copyright � 2022 Oleg Belekhov. All Right Reserved.
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

	export using lambda_null_args = std::function<void()>;
	export using lambda_one_arg = std::function<void(int)>;
	export using lambda_null_or_one_arg = std::variant<lambda_null_args, lambda_one_arg>;
	using QueueSubs = std::vector < std::pair<event_type, lambda_null_or_one_arg>>;
	using QueueEvent = std::vector<std::pair<emodule::event_type, std::variant<bool, int>>>;

	export
		class IEvent
	{
	public:
		virtual ~IEvent() {};

	public:

		virtual void subscribe(lambda_null_or_one_arg func__, event_type et__) = 0;
		virtual void create_event(event_type et__) = 0;
		virtual void create_event(event_type et__, int arg__) = 0;
		virtual void call() = 0;

	};

export
	class Event : public IEvent
	{
	public:
		virtual void subscribe(lambda_null_or_one_arg func__, event_type et__) override;
		virtual void create_event(event_type et__) override;
		virtual void create_event(event_type et__, int arg__) override;
		virtual void call() override;

	private:

		QueueSubs queue_subs;
		QueueEvent queue_event;
	};

	void Event::subscribe(lambda_null_or_one_arg func__, event_type et__)
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
						[&have_arg](bool& b) {have_arg = false; }, // event haven't arg
						[&arg](int& i) {arg = i; } //ok, event have arg
					};
					std::visit(o, e.second);

					overload o2{
						[](lambda_null_args& f) { f(); }, // call lymbda no param
						[&have_arg, &arg](lambda_one_arg& f) { have_arg ? f(arg) : f(0); } // call lymda one param
					};

					std::visit(o2, s.second);

				}

		queue_event.clear();
	}

}