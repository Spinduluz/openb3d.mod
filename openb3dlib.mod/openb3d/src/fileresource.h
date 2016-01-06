#ifndef FILERESOURCE_H
#define FILERESOURCE_H

#include "file.h"
#include "unzip.h"

#include <memory>
#include <iostream>
#include <vector>
#include <array>

#if defined(BLITZMAX_DEBUG)
#include "bmaxdebug.h"
#endif

using namespace std;

class FileResource{
public:
	virtual FilePtr ReadFile(const string& filename) const=0;
	virtual bool FileExist(const string& filename) const=0;
	virtual const char *GetName() const=0;

	virtual void FreeResource()=0;
};

class FileZipResource : public FileResource{
public:
	static vector<FileZipResource> resources;

	UnzipHandlePtr handle;
	string name;

	static FileResource *Create(const string& filename);
	static void Free();

	FileZipResource(UnzipHandlePtr handle=UnzipHandlePtr(), const string& name="");

	FilePtr ReadFile(const string& filename) const;
	bool FileExist(const string& filename) const;
	const char *GetName() const;

	void FreeResource();
};

#endif // FILERESOURCE_H

