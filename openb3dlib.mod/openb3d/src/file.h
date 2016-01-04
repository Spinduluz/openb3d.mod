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
#include <stdio.h>
#include <memory.h>
#include "unzip/unzip.h"

#if defined(BLITZMAX_BUILD)
#include <pub.mod/mapstream.mod/bbstream.h>
#endif

using namespace std;

#if defined(OLD_FILE_SYS)
class File{
 // This class should really be "abstracted".
// But I can't really be arsed tbh
public:
	File()
	:pFile(NULL)
#if defined(BLITZMAX_BUILD)
	,bbStream(NULL)
#endif
	,buf(NULL)
	,pos(NULL)
	,end(NULL){};
	
	virtual ~File(){
		//if(pFile) delete pFile;
	}
	
	FILE * pFile;

#if defined(BLITZMAX_BUILD)
	bbStreamIO *bbStream;
#endif

	unsigned char *buf;
	unsigned char *pos;
	unsigned char *end;

//	static string DocsDir();
	static string ResourceFilePath(string filename);
	static File* ReadResourceFile(string filename);
	static File* ReadFile(string filename);
	static File* WriteFile(string filename);

	static File* ReadBuffer(const void *buffer,int len);
#if defined(BLITZMAX_BUILD)
	static File* ReadStreamBB(bbStreamIO *stream);
#endif
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
	
	virtual void SeekFile(int pos);
	virtual int FilePos();
	virtual int FileSize();
	
	virtual int Eof();

	virtual size_t Write(const void *buffer,size_t size,size_t count);
	virtual size_t Read(void *buffer,size_t size,size_t count);
	virtual char *Gets(char *str,int num);
	int Seek(long int offset,int origin);
};
#else
class File{
public:
	File(){
	};
	
	virtual ~File(){
	}

//	static string DocsDir();
	static string ResourceFilePath(const string& filename);
	static File* ReadResourceFile(const string& filename);
	static File* ReadFile(const string& filename);
	static File* WriteFile(const string& filename);

	static File* ReadBuffer(const void *buffer,int len);
#if defined(BLITZMAX_BUILD)
	static File* ReadStreamBB(bbStreamIO *stream);
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

class FileUnz : public File{
public:
	unz_file_info info;
	unzFile *file;
	int bytes_read;

	FileUnz(unzFile *file=NULL);
	~FileUnz();

	void SeekFile(int pos);
	int FilePos();
	int FileSize();
	int Eof();

	size_t Read(void *buffer,size_t size,size_t count);
};

// ==========================================================================================================

#if defined(BLITZMAX_BUILD)
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

#endif // OLD_FILE_SYS
 
#endif
