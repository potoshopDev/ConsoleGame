// Copyright © 2022 Oleg Belekhov. All Right Reserved.
// Licensed under the CC0 License
// c++ 20 need
#pragma once

namespace emodule
{
		template<class... Ts> struct overload : Ts... { using Ts::operator()...; };
}

