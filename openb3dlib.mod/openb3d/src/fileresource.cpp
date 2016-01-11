#include "fileresource.h"

void FreeUnzipHandle(UnzipHandle *file){
	if(file) unzClose(file);
}

// ==========================================================================================================

vector<FileZipResource> FileZipResource::resources;

FileResource *FileZipResource::Create(const string& filename){
	UnzipHandle *handle=unzOpen(filename.c_str());
	if(!handle) return NULL;

	resources.push_back(FileZipResource(UnzipHandlePtr(handle,FreeUnzipHandle),filename));
	return &resources.back();
}

void FileZipResource::Free(){
	resources.clear();
}

FileZipResource::FileZipResource(UnzipHandlePtr handle,const string& name):handle(handle),name(name){
}

FilePtr FileZipResource::ReadFile(const string& filename) const{
	if(unzLocateFile(handle.get(),filename.c_str(),true)!=UNZ_OK) return NULL;
	// This is a quick fix for a bug.
	// It might be possible that we're unable to stream small packet of data
	// directly from the zlib stream. Doubtful that it'll be worth to fix.
#if 0
	return FilePtr(new FileUnz(handle),FreeFilePtr);
#else // Test fix for crash
	unsigned char *buffer;
	int len;

	FileUnz unz(handle);
	len=unz.ReadBuffer(&buffer);

	if(len<=0) return FilePtr();

	FileMem *file=new FileMem(buffer,len);
	return FilePtr(file,FreeFilePtr);
#endif
}

bool FileZipResource::FileExist(const string& filename) const{
	return (unzLocateFile(handle.get(),filename.c_str(),true)==UNZ_OK);
}

const char *FileZipResource::GetName() const{
	return name.c_str();
}

void FileZipResource::FreeResource(){
	vector<FileZipResource>::iterator i,b,e;

	b=resources.begin();
	e=resources.end();

	for(i=b;i!=e;i++){
		if(this==&*i) resources.erase(i,i);
	}
}
