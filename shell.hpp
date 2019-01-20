#ifndef SHELL_H
#define SHELL_H
#include <vector>
#include <functional>
#include <thread>
#include <algorithm>
#include <queue>
#include "ctpl_stl.h"
class Shell{
	template <class T, class Cmp = std::less<T>>
	void thFuncNP (std::vector<T> &seq, size_t start, size_t end, size_t d,
				 Cmp cmp = Cmp()){
		for (size_t i = start; i < end; i++){
			for (size_t j = i; j < seq.size(); j += d){
				for (size_t k = j; k >= d && cmp(seq[k], seq[k - d]); k -= d){
					std::swap(seq[k], seq[k - d]);
				}
			}
		}
	}

public:
	template <class T, class Cmp = std::less<T>>
	void single (std::vector<T> &seq, Cmp cmp = Cmp()){
		for (size_t d = seq.size() / 2; d; d >>= 1){
			for (size_t i = d; i < seq.size(); i++){
				for (size_t j = i; j >= d && cmp(seq[j], seq[j - d]); j -= d){
					std::swap(seq[j], seq[j - d]);
				}
			}
		}
	}

	template <class T, class Cmp = std::less<T>>
	void multipleP (std::vector<T> &seq,
		size_t maxTh = std::max(std::thread::hardware_concurrency(), 2U),
		Cmp cmp = Cmp()){
		ctpl::thread_pool pool(maxTh);
		auto thFunc = [](int id, std::vector<T> &seq, size_t start, size_t end,
						 size_t d, Cmp cmp = Cmp()){
			for (size_t i = start; i < end; i++){
				for (size_t j = i; j < seq.size(); j += d){
					for (size_t k = j; k >= d && cmp(seq[k], seq[k - d]);
					 													k -= d){
						std::swap(seq[k], seq[k - d]);
					}
				}
			}
		};
		for (size_t d = seq.size() / 2; d; d >>= 1){
			std::vector<std::future<void>> results (maxTh);
			size_t offset = d / maxTh + 1;
			for (size_t i = 0; i < maxTh; i++){
				size_t start = i * offset;
				size_t end = start + offset;
				results[i] = pool.push(thFunc, std::ref(seq),
															start, end, d, cmp);
			}
			for (size_t i = 0; i < maxTh; i++){
				results[i].get();
			}
		}
	}

	template <class T, class Cmp = std::less<T>>
	void multipleNPGood (std::vector<T> &seq,
		size_t maxTh = std::max(std::thread::hardware_concurrency(), 2U),
		Cmp cmp = Cmp()){
		for (size_t d = seq.size() / 2; d; d >>= 1){
			size_t offset = d / maxTh + 1;
			std::vector<std::thread> threads;
			for (size_t i = 0; i < maxTh; i++){
				size_t start = i * offset;
				size_t end = start + offset;
				threads.push_back(std::thread(&Shell::thFuncNP<T, Cmp>, this,
					std::ref(seq), start, end, d, cmp));
			}
			for (size_t i = 0; i < maxTh; i++){
				threads[i].join();
			}
		}
	}

	template <class T, class Cmp = std::less<T>>
	void multipleNPBad (std::vector<T> &seq, size_t maxTh = 4, Cmp cmp = Cmp()){
		for (size_t d = seq.size() / 2; d; d >>= 1){
			size_t tasks = d;
			size_t start = 0;
			while (tasks){
				std::vector<std::thread> threads;
				for (size_t i = 0; i < maxTh && i < tasks; i++, start++){
					threads.push_back(std::thread(&Shell::thFuncNP<T, Cmp>,
								this, std::ref(seq), start, start + 1, d, cmp));
				}
				for (size_t i = 0; i < threads.size(); i++){
					threads[i].join();
				}
				tasks -= threads.size();
			}
		}
	}
};
#endif
