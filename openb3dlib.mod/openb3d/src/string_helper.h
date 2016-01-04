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
 
// FIXME: Inline this crap or replace it....
string Left(const string& s,int length);
string Right(const string& s,unsigned int length);
string Mid(const string& s,int offset,int characters=0);
string Replace(const string& s,const string& find,const string& replace);
int Instr(const string& s1,const string& s2,int offset=0);
string Upper(const string& s);
string Lower(const string& s);
string Trim(const string& s);
string Chr(int asc);
int Asc(const string& s);
int Len(const string& s);
string Split(const string& s,const string& splitter,int count);
// Simple string "hasher"
size_t StringHash(const string& s);

#endif
