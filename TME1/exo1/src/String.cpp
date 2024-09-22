/*
 * String.cpp
 *
 *  Created on: Sep 21, 2024
 *      Author: romain
 */

#include "String.h"
#include <string>
#include <iostream>
#include <cstring>

String::String(const char * s ) {
	str=util::newCopy(s);
}
String::~String(){
	delete [] str;
}

int String::length(){
	return util::length(str);
}
String& String::operator=(const String& other){
	if (this != &other) {  // Vérifier l'auto-affectation
	        delete[] str;  // Libérer la mémoire existante pour éviter les fuites
	        str = util::newCopy(other.str);
	  }
	  return *this;

}
std::ostream & operator<< (std::ostream & os, const String & vec){
	os<<vec.str;
	return os;
}

bool operator==(const String &a,const String &b){
	int val = util::compare(a.str,b.str);
	return val == 0;
}
bool operator<(const String &a,const String &b){
	int val = util::compare(a.str,b.str);
	return val < 0;
}
