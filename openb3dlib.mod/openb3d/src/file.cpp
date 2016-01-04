/*
 *  file.mm
 *  iminib3d
 *
 *  Created by Simon Harrison.
 *  Copyright Si Design. All rights reserved.
 *
 */

// Note from Thomas
// not much that make sense in this file.

#include "file.h"
#include "string_helper.h"
//#include "misc.h"

#include <string>
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <algorithm>

#if defined(BLITZMAX_DEBUG)
#include "bmaxdebug.h"
#endif

using namespace std;

static string filepath_in_use;

/*string File::DocsDir(){

    NSArray *paths = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES);
    CFStringRef documentsDirectory = (CFStringRef)[paths objectAtIndex:0];
    if (!documentsDirectory) {
        NSLog(@"Documents directory not found!");
        return NO;
    }

	const char* c_docs_dir=CFStringGetCStringPtr(documentsDirectory,kCFStringEncodingMacRoman);

	int success=0;
	char localBuffer[300];

	if(c_docs_dir==NULL){
		success=CFStringGetCString(documentsDirectory,localBuffer,300,kCFStringEncodingMacRoman);
	}

	string docs_dir="";

	if(success){
		docs_dir=localBuffer;
	}else{
		docs_dir="";
	}

	return docs_dir;

}*/

#if defined(OLD_FILE_SYS)
inline int filelen(FILE *pFile){
	int len,p=ftell(pFile);
	fseek(pFile,0,SEEK_END);
	len=ftell(pFile);
	fseek(pFile,p,SEEK_SET);
	return len;
}

inline void readfile(File *file,FILE *pFile){
	int len=filelen(pFile);
	file->buf=new unsigned char[len];
	file->pos=file->buf;
	file->end=file->buf+len;
	fread(file->buf,1,len,pFile);
	fclose(pFile);
}

string File::ResourceFilePath(string filename){

	std::replace(filename.begin(), filename.end(), '\\', '/');

	File* stream;

	stream=File::ReadFile(filename);

	if (stream!=0) {
		stream->CloseFile();
		return filename;
	}

	string::size_type idx=filename.rfind("/");

	if(idx!=string::npos){
		filename=filename.substr(idx+1);
	}
	if (filepath_in_use.length() != 0) {
		filename=filepath_in_use+"/"+filename;
	}

	stream=File::ReadFile(filename);

	if (stream!=0) {
		stream->CloseFile();
		return filename;
	}

	return "";

}

File* File::ReadResourceFile(string filename){

	std::replace(filename.begin(), filename.end(), '\\', '/');

	string::size_type idx=filename.rfind("/");

	if(idx!=string::npos){
		filepath_in_use=filename.substr(0,idx);
	}

	//string filename2=ResourceFilePath(filename);
	if(filename==""){
		cout << "Error: No Filename: " << filename << endl;
	}

	const char* c_filename=filename.c_str(); // WHY?

	FILE* pFile=fopen(c_filename,"rb");

	if(pFile==NULL){
		cout << "Error: Cannot Find Resource File: " << filename << endl;
		return NULL;
	}

	File* file=new File();
#if 0
	file->pFile=pFile;
#else
	readfile(file,pFile);
#endif

	return file;
}

File* File::ReadFile(string filename){
	//string filename2=filename; // WHY?

	FILE* pFile=fopen(filename.c_str(),"rb");

	if(pFile==NULL){
		cout << "Error: Can't Find Document File '"+filename+"'" << endl;
		return NULL;
	}

	File* file=new File();
#if 0
	file->pFile=pFile;
#else
	readfile(file,pFile);
#endif

	return file;
}

File* File::WriteFile(string filename){

/*	if(filename==""){
		RuntimeError("Error: No Filename");
	}*/

	string filename2=filename;

	FILE* pFile=fopen(filename2.c_str(), "wb" );

	if(pFile==NULL){
		cout << "Error: Can't Write File '"+filename+"'" << endl;
		return NULL;
	}

	File* file=new File();
	file->pFile=pFile;

	return file;

}

File* File::ReadBuffer(const void *buffer,int len){
	if(!buffer || !len)
		return NULL;

	File *file = new File;
	file->buf = (unsigned char*)buffer;
	file->pos = file->buf;
	file->end = file->buf+len;

	return file;
}
#if defined(BLITZMAX_BUILD)
File* File::ReadStreamBB(bbStreamIO *stream){
	if(!stream)
		return NULL;

	File *file = new File;
	file->bbStream = stream;

	return file;
}
#endif

void File::CloseFile(){

	if(pFile) fclose(pFile);
	if(buf) delete[] buf;

	delete this;

}

char File::ReadByte(){
	char c;
	Read(&c,1,1);
	return c;
}

short File::ReadShort(){
	short s;
	Read(&s,1,2);
	return s;
}

int File::ReadInt(){
	int i;
	Read(&i,1,4);
	return i;
}

long File::ReadLong(){
	long l;
	Read(&l,1,8);
	return l;
}

float File::ReadFloat(){
	float f;
	Read(&f,1,4);
	return f;
}

string File::ReadString(){
	int length=ReadInt();
#if 0 // Old crap
	char* c=new char[length+1];
	fgets(c,length+1,pFile);

	string s=c;

	return s;
#else
	string s;
	s.reserve(length+1); // I assume we're expecting a zero terminated string? I have no idea to be honest
	Read(&s[0],1,length+1);

	return s;
#endif
}

string File::ReadLine(){

	string s="";
	char c=ReadByte();

	// get string up to first new line character of end of file
	while(c!=13 && c!=10 && Eof()!=true){
		if(c!=0){
			s+=c;
		}
		c=ReadByte();
	}

	int pos=-1;

	// pass possible remaining new line character
	if(Eof()!=true){
		pos=FilePos();
		c=ReadByte();
		if(c!=13 && c!=10) SeekFile(pos);
	}

	return s;
}

void File::WriteByte(char c){
	fwrite(&c,1,1,pFile);
}

void File::WriteShort(short s){
	fwrite(&s,1,2,pFile);
}

void File::WriteInt(int i){
	fwrite(&i,1,4,pFile);
}

void File::WriteLong(long l){
	fwrite(&l,1,8,pFile);
}

void File::WriteFloat(float f){
	fwrite(&f,1,4,pFile);
}

void File::WriteString(string s){
#if 0 // Old crap
	const char* cs=s.c_str();
	fputs(cs,pFile);
#else
	fputs(s.c_str(),pFile);
#endif
}

void File::WriteLine(string s){
#if 0 // Old crap
	for(unsigned int i=0;i<s.length();i++){

		string sc=&s[i];
		const char* c=sc.c_str();

		WriteByte(*c);
	}

	char c13=13;
	char c10=10;

	WriteByte(c13);
	WriteByte(c10);
#else
#ifdef _WIN32
	char lf[3]="\r\n";
#else
	char lf[2]="\n";
#endif
	fwrite(s.c_str(),1,s.length(),pFile);
	fwrite(lf,1,sizeof(lf)-1,pFile);
#endif
}

void File::SeekFile(int pos){
	if(pFile) fseek(pFile,pos,SEEK_SET);
	if(buf) Seek(pos,SEEK_SET);
#if defined(BLITZMAX_BUILD)
	if(bbStream) bbStream->Seek(pos);
#endif
}

int File::FilePos(){
	if(pFile) return ftell(pFile);
	if(buf) return (int)(pos-buf);
#if defined(BLITZMAX_BUILD)
	if(bbStream) return bbStream->Pos();
#endif
	return 0;
}

int File::FileSize(){
	if(pFile) return filelen(pFile);
	if(buf) return (int)(end-buf);
#if defined(BLITZMAX_BUILD)
	if(bbStream) return bbStream->Size();
#endif
}

int File::Eof(){
#if 0 // Old crap
	int endof=0;
	int pos=ftell(pFile);
	char c;
	fread(&c,1,1,pFile);
	endof=feof(pFile);
	fseek(pFile,pos,SEEK_SET);
	return endof;
#else
	if(pFile) return feof(pFile);
	if(buf) return (pos>=end);
#if defined(BLITZMAX_BUILD)
	if (bbStream) return bbStream->Eof();
#endif
	return 1;
#endif
}

size_t File::Read(void *buffer,size_t size,size_t count){
	if(pFile) return fread(buffer,size,count,pFile);
#if defined(BLITZMAX_BUILD)
	if(bbStream) return bbStream->Read(buffer,size*count);
#endif

	size_t r=end-pos;
	size_t s=size*count;

	/*BBString *str=bbStringFromCString( "From OpenB3D" );
	bbOnDebugLog( str );*/

	if(r>s) r=s;
	memcpy(buffer,pos,r);
	pos+=r;
	return r;
}

char *File::Gets(char *str,int num){
	if(pFile) return fgets(str,num,pFile);
	if(buf) Read(str,1,num);
#if defined(BLITZMAX_BUILD)
	if(bbStream) bbStream->Read(str,num);
#endif
	return str;
}

int File::Seek(long int offset,int origin){
	unsigned char *p;

	switch(origin){
	case SEEK_SET:
		p=buf+offset;
		break;
	case SEEK_CUR:
		p=pos+offset;
		break;
	case SEEK_END:
		p=end+offset;
		break;
	}

	if (p<buf) p=buf;
	if (p>end) p=end;

	pos=p;

	return 0;
}
#else
string File::ResourceFilePath(const string& filename){
#if 0
	std::replace(filename.begin(), filename.end(), '\\', '/');

	File* stream;

	stream=File::ReadFile(filename);

	if (stream!=0) {
		stream->CloseFile();
		return filename;
	}

	string::size_type idx=filename.rfind("/");

	if(idx!=string::npos){
		filename=filename.substr(idx+1);
	}
	if (filepath_in_use.length() != 0) {
		filename=filepath_in_use+"/"+filename;
	}

	stream=File::ReadFile(filename);

	if (stream!=0) {
		stream->CloseFile();
		return filename;
	}

	return "";
#else
	string name=filename;
	std::replace(name.begin(), name.end(), '\\', '/');

	FILE* stream;

	stream=fopen(name.c_str(),"rb");

	if (stream) {
		fclose(stream);
		return name;
	}

	string::size_type idx=name.rfind("/");

	if(idx!=string::npos){
		name=name.substr(idx+1);
	}
	if (filepath_in_use.length() != 0) {
		name=filepath_in_use+"/"+name;
	}

	stream=fopen(name.c_str(),"rb");

	if (stream) {
		fclose(stream);
		return name;
	}

	return "";
#endif
}

File* File::ReadResourceFile(const string& filename){
	string name=filename;

	std::replace(name.begin(), name.end(), '\\', '/');

	string::size_type idx=name.rfind("/");

	if(idx!=string::npos){
		filepath_in_use=name.substr(0,idx);
	}

	//string filename2=ResourceFilePath(filename);
	if(name==""){
#if defined(BLITZMAX_DEBUG)
		DebugLog("Error: No Filename");
		//cout << "Error: No Filename: " << filename << endl;
#endif
		return NULL;
	}

	FILE* f=fopen(name.c_str(),"rb");

	if(!f){
#if defined(BLITZMAX_DEBUG)
		DebugLog("Error: Cannot Find Resource File: %s",filename.c_str());
		//cout << "Error: Cannot Find Resource File: " << filename << endl;
#endif
		return NULL;
	}

	FileMem* file=new FileMem();
	file->ReadFile(f);
	fclose(f);

	return file;
}

File* File::ReadFile(const string& filename){
	FILE* f=fopen(filename.c_str(),"rb");

	if(!f){
#if defined(BLITZMAX_DEBUG)
		DebugLog("Error: Can't Find Document File '%s'",filename.c_str());
		//cout << "Error: Can't Find Document File '"+filename+"'" << endl;
#endif
		return NULL;
	}

	FileMem* file=new FileMem();
	file->ReadFile(f);
	fclose(f);

	return file;
}

File* File::WriteFile(const string& filename){
	FILE* f=fopen(filename.c_str(), "wb" );

	if(!f){
#if defined(BLITZMAX_DEBUG)
		DebugLog("Error: Can't Write File '%s'",filename.c_str());
		//cout << "Error: Can't Write File '"+filename+"'" << endl;
#endif
		return NULL;
	}

	FileStdc* file=new FileStdc(f);
	return file;

}

File* File::ReadBuffer(const void *buffer,int len){
	if(!buffer || !len)
		return NULL;

	File *file = new FileMem((unsigned char*)buffer,len);
	return file;
}

#if defined(BLITZMAX_BUILD)
File* File::ReadStreamBB(bbStreamIO *stream){
	if(!stream)
		return NULL;

	FileBB *file = new FileBB(stream);

	return file;
}
#endif

bool File::Exists(const string& filename){
	FILE *f=fopen(filename.c_str(),"rb");
	bool exists=(f!=NULL);
	
	fclose(f);
	return exists;
}

void File::CloseFile(){
	delete this;
}

char File::ReadByte(){
	char c;
	Read(&c,1,1);
	return c;
}

short File::ReadShort(){
	short s;
	Read(&s,1,2);
	return s;
}

int File::ReadInt(){
	int i;
	Read(&i,1,4);
	return i;
}

long File::ReadLong(){
	long l;
	Read(&l,1,8);
	return l;
}

float File::ReadFloat(){
	float f;
	Read(&f,1,4);
	return f;
}

string File::ReadString(){
	int length=ReadInt();
	string s;

	s.reserve(length+1); // I assume we're expecting a zero terminated string? I have no idea to be honest
	Read(&s[0],1,length+1);

	return s;
}

string File::ReadLine(){

	string s="";
	char c=ReadByte();

	// get string up to first new line character of end of file
	while(c!=13 && c!=10 && !Eof()){
		if(c!=0){
			s+=c;
		}
		c=ReadByte();
	}

	int pos=-1;

	// pass possible remaining new line character
	if(!Eof()){
		pos=FilePos();
		c=ReadByte();
		if(c!=13 && c!=10) SeekFile(pos);
	}

	return s;
}

void File::WriteByte(char c){
	Write(&c,1,1);
}

void File::WriteShort(short s){
	Write(&s,1,2);
}

void File::WriteInt(int i){
	Write(&i,1,4);
}

void File::WriteLong(long l){
	Write(&l,1,8);
}

void File::WriteFloat(float f){
	Write(&f,1,4);
}

void File::WriteString(string s){
	Write(&s[0],1,s.length());
}

void File::WriteLine(string s){
#ifdef _WIN32
	char lf[3]="\r\n";
#else
	char lf[2]="\n";
#endif
	Write(s.c_str(),1,s.length());
	Write(lf,1,sizeof(lf)-1);
}

void File::SeekFile(int pos){
}

int File::FilePos(){
	return 0;
}

int File::FileSize(){
	return 0;
}

int File::Eof(){
	return 1;
}

size_t File::Write(const void *buffer,size_t size,size_t count){
	return 0;
}

size_t File::Read(void *buffer,size_t size,size_t count){
	return 0;
}

char *File::Gets(char *str,int num){
	Read(str,1,num);
	return str;
}

// ==========================================================================================================

FileStdc::FileStdc(FILE *file,bool noclose):file(file),noclose(noclose){
}

FileStdc::FileStdc(const string& filename,const string& mode):file(NULL){
	file=fopen(filename.c_str(),mode.c_str());
}

FileStdc::~FileStdc() {
	if(file && !noclose) fclose(file);
}

void FileStdc::SeekFile(int pos){
	fseek(file,pos,SEEK_SET);
}

int FileStdc::FilePos(){
	return ftell(file);
}

int FileStdc::FileSize(){
	int l,p;
	p=ftell(file);
	fseek(file,0,SEEK_END);
	l=ftell(file);
	fseek(file,p,SEEK_SET);
	return l;
}

int FileStdc::Eof(){
	return feof(file);
}

size_t FileStdc::Write(const void *buffer,size_t size,size_t count){
	return fwrite(buffer,size,count,file);
}

size_t FileStdc::Read(void *buffer,size_t size,size_t count){
	return fread(buffer,size,count,file);
}

char *FileStdc::Gets(char *str,int num){
	return fgets(str,num,file);
}

// ==========================================================================================================

FileMem::FileMem(unsigned char *buf,int len):buf(buf),pos(buf),end(buf?buf+len:NULL){
}

FileMem::~FileMem() {
	if(buf) delete[] buf;
	buf=NULL;
	pos=NULL;
	end=NULL;
}

void FileMem::SeekFile(int filepos){
	unsigned char *p=buf+filepos;
	if(p>end) p=end;
	pos=p;
}

int FileMem::FilePos(){
	return (int)(pos-buf);
}

int FileMem::FileSize(){
	return (int)(end-buf);
}

int FileMem::Eof(){
	return (pos==end);
}

size_t FileMem::Read(void *buffer,size_t size,size_t count){
	if(Eof()) return 0;
	if(!size || !count) return 0;

	size_t r=end-pos;
	size_t s=count*size;

	if(r>s) r=s;

	memcpy(buffer,pos,r);
	pos+=r;

	return r / size;
}

bool FileMem::ReadFile(FILE *f){
	int size,p;

	fseek(f,0,SEEK_END);
	size=ftell(f);
	fseek(f,0,SEEK_SET);

	buf=new unsigned char[size+1];
	pos=buf;
	end=buf+size;
	return (fread(buf,1,size,f)==size);
}

bool FileMem::ReadFile(const string& filename){
	FILE *f=fopen(filename.c_str(),"rb");
	if(!f) return false;
	bool r=ReadFile(f);
	fclose(f);
	return r;
}


// ==========================================================================================================

#define UNZ_SEEK_BUFFER 16384

FileUnz::FileUnz(unzFile *file):info(),file(NULL),bytes_read(0){
	if(file){
		unzGetCurrentFileInfo(file,&info,NULL,0,NULL,0,NULL,0);
		unzOpenCurrentFile(file);
	}
}

FileUnz::~FileUnz(){
	if(file) unzCloseCurrentFile(file);
}

void FileUnz::SeekFile(int pos){
	unsigned char b[ UNZ_SEEK_BUFFER ];
	int p=0;

	if(pos>FileSize()) pos=FileSize();
	// Close and reopen the file
	unzCloseCurrentFile(file);
	unzOpenCurrentFile(file);

	while(p<pos){
		int s=pos-p;
		if(s>UNZ_SEEK_BUFFER) s=UNZ_SEEK_BUFFER;
		p+=unzReadCurrentFile(file,b,s);
	}
	bytes_read=pos;
}

int FileUnz::FilePos(){
	return bytes_read;
}

int FileUnz::FileSize(){
	return info.uncompressed_size;
}

int FileUnz::Eof() {
	return (bytes_read==info.uncompressed_size);
}

size_t FileUnz::Read(void *buffer,size_t size,size_t count){
	int r=unzReadCurrentFile(file,buffer,size*count);
	bytes_read+=r;
	return r;
}

#undef UNZ_SEEK_BUFFER

// ==========================================================================================================

#if defined(BLITZMAX_BUILD)
FileBB::FileBB(bbStreamIO *stream):stream(stream){
}

FileBB::~FileBB() {
}

void FileBB::SeekFile(int pos){
	stream->Seek(pos);
}

int FileBB::FilePos(){
	return stream->Pos();
}

int FileBB::FileSize(){
	return stream->Size();
}

int FileBB::Eof(){
	return stream->Eof();
}

size_t FileBB::Read(void *buffer,size_t size,size_t count){
	return stream->Read(buffer,size*count);
}
#endif // BLITZMAX_BUILD

// ==========================================================================================================

#endif
