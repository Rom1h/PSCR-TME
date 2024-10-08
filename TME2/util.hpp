#include <iostream>
#include <fstream>
#include <regex>
#include <chrono>
#include <vector>
#include <iterator>
#include <forward_list>


namespace util{

	template<typename iterator>
	size_t count(iterator begin,iterator end){
		size_t count = 0;
		for(;begin!=end; ++begin){
			++count;
		}
		return count;
	}
	template<typename iterator, typename T>
	size_t count_if_equal(iterator begin,iterator end,const T  val ){
		size_t count = 0;
		for(;begin!=end;++begin){
			if(*begin == val){
				++count;
			}
		}
		return count;
	}
};
