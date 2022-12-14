// Copyright © 2022 Oleg Belekhov. All Right Reserved.
// Licensed under the CC0 License

//module Core, shape, scoreboard

export module Engine;
import <iostream>;
import Core;


namespace core
{

	export
		class Engine
	{
	public:
		Engine()
		{
			core::EventManager::create_event(emodule::event_type::EVENT_START, 12);
			auto ref_test([this](int i) {Test(i); });
			core::EventManager::subscribe(ref_test, emodule::event_type::EVENT_START);
		}
		void Test(int i)
		{
			std::cout << "Start " << i << std::endl;
		}
		void Test2()
		{
			core::EventManager::call();
		}
	private:
		void Test3()
		{
			std::cout << "Start2" << std::endl;
		}
	};
}
