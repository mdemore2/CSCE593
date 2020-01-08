#pragma once

#include <chrono>
#include <vector>
#include <tuple>
#include <iostream>
#include <sstream> 

namespace Aftr
{
	class Tester
	{
	public:

		static Tester* New();

		void onCreate();

		std::tuple<int, int, float> params_new_test();

		bool time_has_passed(std::chrono::system_clock::time_point);//take in time

		bool test_on();

		void turn_test_on();

		void set_current_time(std::chrono::system_clock::time_point);


	private:
		Tester();
		const int max_time = 12;
		const float max_rotation = 2.0;
		const int max_missiles = 4;
		std::vector<std::tuple<int, int, float>> all_permutations;
		bool testing = false;
		float time = 0;
		std::chrono::system_clock::time_point current_time;
	};

}
