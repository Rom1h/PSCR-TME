/*
 * util.cpp
 *
 *  Created on: Sep 22, 2024
 *      Author: romain
 */

#include "util.h"

int util::length(const char * s){
	int count = 0;
	for(const char * tmp = s;*tmp; ++tmp){
		++count;
	}
	return count;
}
char * util::newCopy(const char * s){
	char * res = new char[length(s)+1];
	char *cp = res;
	do{
		*cp++=*s;

	}while(*s++);
	return res;

}
int util::compare(const char* s1, const char* s2){
	int s1L = util::length(s1);
	int s2L = util::length(s2);
	if(s1L<s2L){
		return -1;
	}
	else if(s1L>s2L){
		return 1;
	}
	for(int i = 0;i<s1L;i++){

		if(s1[i]<s2[i])return -1;
		if(s1[i]>s2[i])return 1;

	}
	return 0;
}
