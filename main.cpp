#include <iostream>
#include <string>
#include <cstdlib>
#include <chrono>
#include "shell.hpp"

int main (int argc, char **argv){
	Shell foo;

	std::vector<double> res (12);
	size_t times = argc < 3 ? 1 : atoi(argv[2]);
	bool fast = false;
	for (int i = 1; i < argc; i++){
		if (std::string(argv[i]) == "-f"){
			fast = true;
			break;
		}
	}

	for (size_t t = 0; t < times; t++){
		std::vector<int> a;
		std::srand(std::time(0));
		for (int i = 0; i < atoi(argv[1]); i++){
			a.push_back(std::rand());
		}
		std::vector<int> b(a);
		std::vector<int> c(a);
		std::vector<int> d(a);

		//single
		auto begin = std::chrono::high_resolution_clock::now();
		foo.single(a);
		auto end = std::chrono::high_resolution_clock::now();
		auto time = std::chrono::duration_cast<std::chrono::milliseconds>(
														end - begin).count();
		if (t){
			res[0] = std::max(res[0], (double)time);
			res[1] = std::min(res[1], (double)time);
			res[2] = (res[2] * t + (double)time) / (t + 1);
		}
		else{
			res[0] = time;
			res[1] = time;
			res[2] = time;
		}


		//multipleNPBad
		if (!fast){
			begin = std::chrono::high_resolution_clock::now();
			foo.multipleNPBad(b);
			end = std::chrono::high_resolution_clock::now();
			time = std::chrono::duration_cast<std::chrono::milliseconds>(
														end - begin).count();
			if (t){
				res[3] = std::max(res[3], (double)time);
				res[4] = std::min(res[4], (double)time);
				res[5] = (res[5] * t + (double)time) / (t + 1);
			}
			else{
				res[3] = time;
				res[4] = time;
				res[5] = time;
			}
		}


		//multipleNPGood
		begin = std::chrono::high_resolution_clock::now();
		foo.multipleNPGood(c);
		end = std::chrono::high_resolution_clock::now();
		time = std::chrono::duration_cast<std::chrono::milliseconds>(
														end - begin).count();
		if (t){
			res[6] = std::max(res[6], (double)time);
			res[7] = std::min(res[7], (double)time);
			res[8] = (res[8] * t + (double)time) / (t + 1);
		}
		else{
			res[6] = time;
			res[7] = time;
			res[8] = time;
		}

		//multipleP
		begin = std::chrono::high_resolution_clock::now();
		foo.multipleP(d);
		end = std::chrono::high_resolution_clock::now();
		time = std::chrono::duration_cast<std::chrono::milliseconds>(
														end - begin).count();
		if (t){
			res[9] = std::max(res[9], (double)time);
			res[10] = std::min(res[10], (double)time);
			res[11] = (res[11] * t + (double)time) / (t + 1);
		}
		else{
			res[9] = time;
			res[10] = time;
			res[11] = time;
		}
	}

	std::cout << "single:" << std::endl;
	std::cout << "Worst: " << res[0] << std::endl;
	std::cout << "Best: " << res[1] << std::endl;
	std::cout << "Average: " << res[2] << std::endl << std::endl;

	if (!fast){
		std::cout << "multipleNPBad:" << std::endl;
		std::cout << "Worst: " << res[3] << std::endl;
		std::cout << "Best: " << res[4] << std::endl;
		std::cout << "Average: " << res[5] << std::endl << std::endl;
	}

	std::cout << "multipleNPGood:" << std::endl;
	std::cout << "Worst: " << res[6] << std::endl;
	std::cout << "Best: " << res[7] << std::endl;
	std::cout << "Average: " << res[8] << std::endl << std::endl;

	std::cout << "multipleP:" << std::endl;
	std::cout << "Worst: " << res[9] << std::endl;
	std::cout << "Best: " << res[10] << std::endl;
	std::cout << "Average: " << res[11] << std::endl;
}
