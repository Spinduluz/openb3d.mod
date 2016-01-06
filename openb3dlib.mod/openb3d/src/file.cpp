/*
 *  file.mm
 *  iminib3d
 *
 *  Created by Simon Harrison.
 *  Copyright Si Design. All rights reserved.
 *
 */

#include "file.h"
#include "string_helper.h"
#include "fileresource.h"
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
void FreeFilePtr(File *file){
	if(file) file->CloseFile();
}

vector<FileResource*> File::resources;

bool File::AddFileResource(const string& filename,int reserved){
	FileResource *r=FileZipResource::Create(filename);
	if(r) resources.push_back(r);
	return (r!=NULL);
}

void File::CloseFileResource(const char *name){
	vector<FileResource*>::iterator i,b,e;

	b=resources.begin();
	e=resources.end();

	for(i=b; i!=e; i++){
		if(!strcmp(name,(*i)->GetName())){
			(*i)->FreeResource();
			resources.erase(i,i);
		}
	}
}

// I should take some time and look more closely at this
// method some day. Is this really useful?
string File::ResourceFilePath(string filename){
	std::replace(filename.begin(), filename.end(), '\\', '/');
	string rname=filename; // Save name for archive

	FILE* stream;

	stream=fopen(filename.c_str(),"rb");

	if (stream) {
		fclose(stream);
		return filename;
	}

	string::size_type idx=filename.rfind("/");

	if(idx!=string::npos){
		filename=filename.substr(idx+1);
	}
	if (filepath_in_use.length() != 0) {
		filename=filepath_in_use+"/"+filename;
	}

	stream=fopen(filename.c_str(),"rb");

	if (stream) {
		fclose(stream);
		return filename;
	}

	for(FileResource *r : resources){
		if(r->FileExist(rname)) return rname; 
	}

	return "";
}

FilePtr File::ReadResourceFile(string filename){
	std::replace(filename.begin(), filename.end(), '\\', '/');

	string::size_type idx=filename.rfind("/");

	if(idx!=string::npos){
		filepath_in_use=filename.substr(0,idx);
	}

	//string filename2=ResourceFilePath(filename);
	if(filename==""){
#if defined(BLITZMAX_DEBUG)
		DebugLog("Error: No Filename");
		//cout << "Error: No Filename: " << filename << endl;
#endif
		return NULL;
	}

	FILE* f=fopen(filename.c_str(),"rb");

	if(!f){
		// Not found on file system... check resources
		for(FileResource *r : resources){
			if(r->FileExist(filename)) return r->ReadFile(filename);
		}

#if defined(BLITZMAX_DEBUG)
		DebugLog("Error: Cannot Find Resource File: %s",filename.c_str());
		//cout << "Error: Cannot Find Resource File: " << filename << endl;
#endif
		return NULL;
	}

	FileMem* file=new FileMem();
	file->ReadFile(f);
	fclose(f);

	return FilePtr(file,FreeFilePtr);
}

FilePtr File::ReadFile(const string& filename){
	FILE* f=fopen(filename.c_str(),"rb");

	if(!f){
		// Not found on file system... check resources
		for(FileResource *r : resources){
			if(r->FileExist(filename)) return r->ReadFile(filename);
		}

#if defined(BLITZMAX_DEBUG)
		DebugLog("Error: Can't Find Document File '%s'",filename.c_str());
#endif
		return NULL;
	}

	FileMem* file=new FileMem();
	file->ReadFile(f);
	fclose(f);

	return FilePtr(file,FreeFilePtr);
}

FilePtr File::WriteFile(const string& filename){
	FILE* f=fopen(filename.c_str(), "wb" );

	if(!f){
#if defined(BLITZMAX_DEBUG)
		DebugLog("Error: Can't Write File '%s'",filename.c_str());
#endif
		return NULL;
	}

	FileStdc* file=new FileStdc(f);
	return FilePtr(file,FreeFilePtr);

}

FilePtr File::ReadBuffer(const void *buffer,int len){
	if(!buffer || !len)
		return NULL;

	File *file = new FileMem((unsigned char*)buffer,len);
	return FilePtr(file,FreeFilePtr);
}

#if defined(BLITZMAX_BUILD) && defined(BLITZMAX_TSTREAM)
FilePtr File::ReadStreamBB(bbStreamIO *stream){
	if(!stream)
		return NULL;

	FileBB *file = new FileBB(stream);

	return FilePtr(file,FreeFilePtr);
}
#endif

File::File(){
};
	
File::~File(){
}

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
// FIXME: Pass by reference
void File::WriteLine(string s){
#ifdef _WIN32
	char linefeed[]="\r\n";
#else
	char linefeed[]="\n";
#endif
	Write(s.c_str(),1,s.length());
	Write(linefeed,1,sizeof(linefeed)-1);
}

int File::ReadBuffer(vector<unsigned char>& buf){
	int len=FileSize();
	if(!len) return -1;

	buf.reserve(len);
	return Read(&buf[0],1,len);
}

int File::ReadBuffer(unsigned char **buffer){
	int len=FileSize();
	if(!len) return -1;

	unsigned char *buf=new unsigned char[len];
	if(!buf) return -1;

	Read(buf,1,len);
	*buffer=buf;
	return len;
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
	int size;

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

FileUnz::FileUnz(UnzipHandlePtr file):file(file),info(),bytes_read(0){
	if(file){
		unzOpenCurrentFile(file.get());
		unzGetCurrentFileInfo(file.get(),&info,NULL,0,NULL,0,NULL,0);
	}
}

FileUnz::~FileUnz(){
	if(file) unzCloseCurrentFile(file.get());
}

void FileUnz::SeekFile(int pos){
	unsigned char b[ UNZ_SEEK_BUFFER ];
	int p=0;

	if(pos>FileSize()) pos=FileSize();
	// Close and reopen the file
	unzCloseCurrentFile(file.get());
	unzOpenCurrentFile(file.get());

	while(p<pos){
		int s=pos-p;
		if(s>UNZ_SEEK_BUFFER) s=UNZ_SEEK_BUFFER;
		p+=unzReadCurrentFile(file.get(),b,s);
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
	int r=unzReadCurrentFile(file.get(),buffer,size*count);
	bytes_read+=r;
	return r;
}

#undef UNZ_SEEK_BUFFER

// ==========================================================================================================

#if defined(BLITZMAX_BUILD) && defined(BLITZMAX_TSTREAM)
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
