// Copyright © 2022 Oleg Belekhov. All Right Reserved.
// Licensed under the CC0 License
// c++ 20 need

export module Source;

namespace emodule
{
	export	template<class... Ts> struct overload : Ts... { using Ts::operator()...; };
}
