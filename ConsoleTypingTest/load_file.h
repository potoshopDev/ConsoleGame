#pragma once
#include <variant>
#include <string>
#include <fstream>
#include <vector>

namespace console
{
	class  load_file
	{
	public:
		load_file() {};
		static [[nodiscard]] std::variant<int, std::wstring> get_words(int count = 10)
		{
			std::string file{ "word.txt" };
			std::ifstream is{file};
			if (is.eof())
			{
				return 0;//error
			}

			std::istream_iterator<std::string> str{ is };
			std::istream_iterator<std::string> eos;

			std::vector<std::string> tmp_vector{ str, eos };

			srand(static_cast<unsigned>(time(0LL)));
			std::string tmp_string;

			for (auto i = 0; i < count; ++i) // select <count> random words
			{
				tmp_string += tmp_vector.at(rand() % (tmp_vector.size() - 1)) + " ";
			}

			std::wstring out_string{ tmp_string.begin(), tmp_string.end() };


			is.close();
			return std::move(out_string);
		}

	};
}
