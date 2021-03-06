/*
 *  string_helper.mm
 *  iminib3d
 *
 *  Created by Simon Harrison.
 *  Copyright Si Design. All rights reserved.
 *
 */

#include "string_helper.h"

#include <string>
#include <algorithm>

using namespace std;

#if defined(BLITZMAX_DEBUG)
#include "bmaxdebug.h"
#endif

string Left(const string& s,int length){
	if (length <= 0) return ""; // SK: fix out of bounds.
	return s.substr(0,length);
}

string Right(const string& s,unsigned int length){
	if (length > s.size()) return ""; // SK: fix out of bounds.
	return s.substr(s.length()-length);
}

// offset starts at 1
// characters is optional. default is length of string-(offset-1)
string Mid(const string& s,int offset,int characters){
		if(offset<1) offset=1; // prevent offset being < 1 - causes error
		if(characters==0) characters=Len(s)-(offset-1);

		return s.substr(offset-1,characters);
}

string Replace( string s,const string& find_s,const string& replace_s){
	size_t i;
	for(;;){
		i=s.find(find_s);

		if(i!=string::npos){
			s.replace(i,find_s.length(),replace_s);
		}else{
			return s;
		}
	
	}
	return s;
}

int Instr(const string& s1,const string& s2,int offset){
	size_t i=s1.find(s2,offset);

	if(i!=string::npos){
		return int(i)+1;
	}else{
		return 0;	
	}	
}

string Upper(const string& s){
	string r;
	transform(s.begin(),s.end(),r.begin(),::toupper);
	return r;
}

string Lower(const string& s){
	string r;
	transform(s.begin(),s.end(),r.begin(),::tolower);
	return r;
}

string Trim(string s){
    size_t startpos=s.find_first_not_of(" \t");
    size_t endpos=s.find_last_not_of(" \t");
  
    if(string::npos==startpos||string::npos==endpos){
        s="";  
    }else{
        s=s.substr(startpos,endpos-startpos+1);
	}	
	return s;
}

string Chr(int asc){	
	string s="";
	s=s+char(asc);

	return s;
}

int Asc(const string& s){
	return s[0];
}

int Len(const string& s){
	return s.length();
}

// not in Blitz - returns the part of the string that lies in between the nth (specified by count) pair of splitters (specified by splitter). if the string has no splitter at front or end of string, they're added internally before splitting
string Split(string s,const string& splitter,int count){
	if(count<1) return "";

	if(Left(s,Len(splitter))!=splitter) s=splitter+s; // add splitter to start of string if not already there to make splitting easier
	if(Right(s,Len(splitter))!=splitter) s=s+splitter; // add splitter to end of string if not already there to make splitting easier
	int pos=Instr(s,splitter,0); // get position of first splitter

	for(int i=1;i<=count;i++){
	
		int new_pos=Instr(s,splitter,pos);
		if(new_pos!=0){
			pos=pos+Len(splitter);
			string split_string=Mid(s,pos,new_pos-pos);
			pos=new_pos;
			if(i==count){
				return split_string;
			}
		}else{
			return "";
		}
	}
	return "";
}

#if 1
size_t StringHash(const string& s,bool casesensitive){
	size_t hash=0;
	for(int c : s){
		if(!casesensitive) c=::tolower(c);
		// sdbm algorithm
		hash=c+(hash<<6)+(hash<<16)-hash;
	}
	return hash;
}
#endif


