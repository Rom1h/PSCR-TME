/*
 * String.h
 *
 *  Created on: Sep 21, 2024
 *      Author: romain
 */

#ifndef STRING_H_
#define STRING_H_
#include <string>
#include <iostream>
#include <cstring>
#include "util.h"

class String {
public:
	const char * str;
	String(const char * s);
	~String();
	int length();
	String& operator=(const String& other);


};

std::ostream & operator<< (std::ostream & os, const String & vec) ;
#endif /* STRING_H_ */
