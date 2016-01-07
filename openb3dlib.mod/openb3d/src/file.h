/*
 *  file.h
 *  iminib3d
 *
 *  Created by Simon Harrison.
 *  Copyright Si Design. All rights reserved.
 *
 */
 
#ifndef FILE_H
#define FILE_H
 
//#import <UIKit/UIKit.h>

#include <string>
#include <memory>
#include <stdio.h>
#include <memory.h>
#include <vector>
#include "unzip.h"

#if defined(BLITZMAX_BUILD) && defined(BLITZMAX_TSTREAM)
#include <pub.mod/mapstream.mod/bbstream.h>
#endif

#if defined(BLITZMAX_DEBUG)
#include "bmaxdebug.h"
#endif

using namespace std;

class FileResource;
class File;
typedef shared_ptr<File> FilePtr;

void FreeFilePtr(File *file);

class File{
public:
	File();
	virtual ~File();

//	static string DocsDir();
	static vector<FileResource*> resources;

	static bool AddFileResource(const string& filename,int reserved=0);
	static void CloseFileResource(const char *name);

	static string ResourceFilePath(string filename);
	static FilePtr ReadResourceFile(string filename);
	static FilePtr ReadFile(const string& filename);
	static FilePtr WriteFile(const string& filename);

	static FilePtr ReadBuffer(const void *buffer,int len);
#if defined(BLITZMAX_BUILD) && defined(BLITZMAX_TSTREAM)
	static FilePtr ReadStreamBB(bbStreamIO *stream);
#endif
	static bool Exists(const string& filename);

	void CloseFile();
	char ReadByte();
	short ReadShort();
	int ReadInt();
	long ReadLong();
	float ReadFloat();
	string ReadString();
	string ReadLine();
	void WriteByte(char c);
	void WriteShort(short s);
	void WriteInt(int i);
	void WriteLong(long l);
	void WriteFloat(float f);
	void WriteString(string s);
	void WriteLine(string s);

	int ReadBuffer(vector<unsigned char>& buf);
	int ReadBuffer(unsigned char **buffer);
	
	virtual void SeekFile(int pos);
	virtual int FilePos();
	virtual int FileSize();
	
	virtual int Eof();

	virtual size_t Write(const void *buffer,size_t size,size_t count);
	virtual size_t Read(void *buffer,size_t size,size_t count);
	virtual char *Gets(char *str,int num);
};

// ==========================================================================================================

class FileStdc : public File {
public:
	FILE *file;
	bool noclose;

	FileStdc(FILE *file=NULL,bool noclose=false);
	FileStdc(const string& filename,const string& mode);
	~FileStdc();

	void SeekFile(int pos);
	int FilePos();
	int FileSize();
	int Eof();

	size_t Write(const void *buffer,size_t size,size_t count);
	size_t Read(void *buffer,size_t size,size_t count);
	char *Gets(char *str,int num);
};

// ==========================================================================================================

class FileMem : public File{
public:
	unsigned char *buf;
	unsigned char *pos;
	unsigned char *end;

	FileMem(unsigned char *buf=NULL,int len=0);
	~FileMem();

	void SeekFile(int pos);
	int FilePos();
	int FileSize();
	int Eof();

	size_t Read(void *buffer,size_t size,size_t count);

	bool ReadFile(FILE *f);
	bool ReadFile(const string& filename);
};

// ==========================================================================================================

typedef void UnzipHandle;
typedef shared_ptr<UnzipHandle> UnzipHandlePtr;

class FileUnz : public File{
public:
	UnzipHandlePtr file;
	unz_file_info info;
	size_t bytes_read;

	FileUnz(UnzipHandlePtr file=UnzipHandlePtr());
	~FileUnz();

	void SeekFile(int pos);
	int FilePos();
	int FileSize();
	int Eof();

	size_t Read(void *buffer,size_t size,size_t count);
};

// ==========================================================================================================

#if defined(BLITZMAX_BUILD) && defined(BLITZMAX_TSTREAM)
class FileBB : public File {
public:
	bbStreamIO *stream;

	FileBB(bbStreamIO *stream=NULL);
	~FileBB();

	void SeekFile(int pos);
	int FilePos();
	int FileSize();
	int Eof();

	size_t Read(void *buffer,size_t size,size_t count);
};
#endif // BLITZMAX_BUILD

// ==========================================================================================================
 
#endif
