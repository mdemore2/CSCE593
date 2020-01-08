#include "Tester.h"
#include <vector>
#include <tuple>
#include <iostream> 
#include <sstream> 

namespace Aftr
{
	Tester* Tester::New() {
		Tester* test = new Tester();
		test->onCreate();
		return test;
	}

	void Tester::onCreate() {
		for (float k = 1.0; k < max_rotation; k += .5) {
			for (int j = 1; j <= max_missiles; j++) {
				for (int i = 4; i <= max_time; i = i+2) {
					all_permutations.push_back(std::make_tuple( i, j, k ));
				}
			}
		}
		all_permutations.push_back(std::make_tuple(0, 0, 0));
	}

	Tester::Tester() {
	}


	std::tuple<int, int, float> Tester::params_new_test() {
		static int curr_permutation = -1;

		curr_permutation++;
		return all_permutations[curr_permutation];
	}

	bool Tester::time_has_passed(std::chrono::system_clock::time_point time) {
		std::chrono::duration< double, std::micro > seconds = time - this->current_time;
		double deltaTSec = seconds.count() / 1'000'000;
		if (deltaTSec >= 14)
			return true;
		else
			return false;
	}

	void Tester::set_current_time(std::chrono::system_clock::time_point current) {
		current_time = current;
	}

	bool Tester::test_on() {
		return testing;
	}

	void Tester::turn_test_on() {
		testing = true;
	}
}