/*
 * main2.cpp
 *
 *  Created on: Sep 22, 2024
 *      Author: romain
 */
#include <cstring>
#include <iostream>
#include "String.h"
#include "util.h"
#include <string>
int main(){
	const char * s1 = "Hello";
	std::cout<< s1<< std::endl;
	const char * s2 = util::newCopy(s1);
	std::cout<< s2<< std::endl;
	std::cout<< util::length(s1)<< std::endl;

	String s("bonjour");
	std::cout<<s.length()<<std::endl;
	std::cout<<s<<std::endl;
	String test = s;
	//delete [] s2;
	return 1;

}



