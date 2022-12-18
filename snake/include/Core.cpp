// Copyright © 2022 Oleg Belekhov. All Right Reserved.
// Licensed under the CC0 License
#include "Core.h"

namespace core
{

	std::unique_ptr< emodule::IEvent> core::EventManager::event_manag = static_cast<std::unique_ptr<emodule::IEvent>>(new emodule::Event);




}
