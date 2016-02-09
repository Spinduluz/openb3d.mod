/*
 *  string_helper.h
 *  iminib3d
 *
 *  Created by Simon Harrison.
 *  Copyright Si Design. All rights reserved.
 *
 */

// Updated. Reference parameters instead of copy parameters

#ifndef STRING_HELPER_H
#define STRING_HELPER_H

#include <string>
using namespace std;

// Not available in C++ 11?
inline int strncasecmp(const char *s1,const char *s2,int n) {
	while(1){
		int c1=::tolower(*s1++);
		int c2=::tolower(*s2++);

		if(!n--) return 0;
		if(c1!=c2) return c1-c2;
		if(!c1) return 0;
	}
	return 0;
}

inline int strcasecmp(const char *s1,const char *s2){
	while(1){
		int c1=::tolower(*s1++);
		int c2=::tolower(*s2++);

		if(c1!=c2) return c1-c2;
		if(!c1) return 0;
	}
}
#if 0
inline int strcmp(const char *s1,const char *s2){
	while(1){
		int c1=*s1++;
		int c2=*s2++;

		if(c1!=c2) return c1-c2;
		if(!c1) return 0;
	}
}
#endif

#define MAX_TEMP_STRING		8
#define MAX_TEMP_STRING_LEN	8192

inline char* GetTempString(){
	static char temp[MAX_TEMP_STRING][MAX_TEMP_STRING_LEN];
	static int index=0;
	
	char *tmp=temp[index++ & (MAX_TEMP_STRING-1)];
	tmp[0]=0;

	return tmp;
}

inline char *RequestTempString(int maxsize){
	if(maxsize>=MAX_TEMP_STRING_LEN) return new char[maxsize];
	return GetTempString();
}

inline void HandleRequestedTempString(char *tmp,int maxsize){
	if(maxsize>=MAX_TEMP_STRING_LEN) delete[] tmp;
}

inline bool CheckExtension(const string& filename,const string& ext){
	size_t p=filename.find_last_of('.');
	if(p==string::npos) return false;
	return (strncasecmp(filename.c_str()+p,ext.c_str(),ext.length())==0);
}
 
// FIXME: Inline this crap or replace it....
string Left(const string& s,int length);
string Right(const string& s,unsigned int length);
string Mid(const string& s,int offset,int characters=0);
string Replace(string s,const string& find,const string& replace);
int Instr(const string& s1,const string& s2,int offset=0);
string Upper(const string& s);
string Lower(const string& s);
string Trim(string s);
string Chr(int asc);
int Asc(const string& s);
int Len(const string& s);
string Split(string s,const string& splitter,int count);
#if 0
inline size_t StringHash(const string& s){
	hash<string> stringhash;
	return stringhash(s);
}
#else
size_t StringHash(const string& s,bool casesensitive=true);
#endif

#endif
