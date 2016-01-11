#ifndef IMAGE_H
#define IMAGE_H

#include <string>
#include <stb_image.h>
#include <memory>

#include "file.h"

using namespace std;

typedef shared_ptr<unsigned char> ImagePtr;

inline int stbi_cb_eof(void* p){
	File* f=(File*)p;
	return f->Eof();
}

inline int stbi_cb_read(void* p,char* b,int s){
	File* f=(File*)p;
	return f->Read(b,1,s);
}

inline void stbi_cb_skip(void* p,unsigned int n){
	File* f=(File*)p;
	int pos=f->FilePos()+n;
	f->SeekFile(pos);
}

inline ImagePtr LoadImage(const string& filename,int* width,int* height,int comp){
	FilePtr file=File::ReadFile(filename);
	if(!file) return NULL;

	stbi_io_callbacks cb;
	cb.eof=stbi_cb_eof;
	cb.read=stbi_cb_read;
	cb.skip=stbi_cb_skip;

	return ImagePtr(stbi_load_from_callbacks(&cb,file.get(),width,height,NULL,comp),stbi_image_free);
}

#endif // IMAGE_H

