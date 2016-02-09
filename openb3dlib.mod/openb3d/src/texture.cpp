/*
 *  texture.mm
 *  iminib3d
 *
 *  Created by Simon Harrison.
 *  Copyright Si Design. All rights reserved.
 *
 */

#include <GL/glew.h>

#include "texture.h"
//#include "stb_image.h"
#include "image.h"

#include "texture_filter.h"
#include "string_helper.h"
#include "file.h"
#include "global.h"
#include "shadow.h"
#include "dds.h"

#include <string.h>
#include <algorithm>

#if defined(BLITZMAX_DEBUG)
#include "bmaxdebug.h"
#endif

void CopyPixels (unsigned char *src, unsigned int srcWidth, unsigned int srcHeight, unsigned int srcX, unsigned int srcY, 
	unsigned char *dst, unsigned int dstWidth, unsigned int dstHeight, unsigned int bytesPerPixel);

static int gl_cube_faces[]={
	GL_TEXTURE_CUBE_MAP_NEGATIVE_X,
	GL_TEXTURE_CUBE_MAP_POSITIVE_Z,
	GL_TEXTURE_CUBE_MAP_POSITIVE_X,
	GL_TEXTURE_CUBE_MAP_NEGATIVE_Z,
	GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,
	GL_TEXTURE_CUBE_MAP_POSITIVE_Y,
};

inline void TextureDeleteFrames(unsigned int *frames){
	if(frames){
		for(unsigned int *i=frames; *i; i++){
			glDeleteTextures(1,&*i);
		}
		delete[] frames;
#if defined(BLITZMAX_DEBUG)
	DebugLog("TextureDeleteFrames");
#endif
	}
}

inline void GL_DeleteTextures(unsigned int* name){
	if(name){
		glDeleteTextures(1,name);
		if(Texture::name_list.size()) Texture::name_list.remove(*name);
#if defined(BLITZMAX_DEBUG)
		DebugLog("GL_DeleteTextures [%i]",Texture::name_list.size());
#endif
	}
}

inline void GL_GenTextures(Texture *tex){
	unsigned int id;

	glGenTextures(1,&id);
	Texture::name_list.push_back(id);

	tex->texture_ref=tex_name_t(&Texture::name_list.back(),GL_DeleteTextures);
	tex->texture=*tex->texture_ref;
}

// ==========================================================================================================

inline void GL_TexImage2D(GLenum target,int width,int height,unsigned char *buffer){
	if(!buffer) return;

	if(Global::gl_sgis_generate_mipmap){
		glTexParameteri(target,GL_GENERATE_MIPMAP,GL_TRUE);
	}

	glTexImage2D(target,0,GL_RGBA,width,height,0,GL_RGBA,GL_UNSIGNED_BYTE,buffer);

	if(Global::gl_ext_framebuffer_object){
		glGenerateMipmap(target);
	}
	if(Global::glu_build_mipmaps){ // FIXME: Build custom mipmap generator
		gluBuild2DMipmaps(target,GL_RGBA,width,height,GL_RGBA,GL_UNSIGNED_BYTE,buffer);
	}
}

inline void GL_CopyTexImage2D(GLenum target,int x,int y,int width,int height){
	if(Global::gl_sgis_generate_mipmap){
		glTexParameteri(target,GL_GENERATE_MIPMAP,GL_TRUE);
	}	
	glCopyTexImage2D(target,0,GL_RGBA,x,y,width,height,0);
	if(Global::gl_ext_framebuffer_object){
		glGenerateMipmap(target);
	}
}

// ==========================================================================================================

// A bloated way to minimize number of allocations when using dstbuffer (See animtexture and cubemap)
// But I seriously dont know if I will use animated textures at all.

// FIXME: Remove this. Not point in using it (yet)
struct TempImageBuffer{
	unsigned char *buf;
	int size;
};

static TempImageBuffer destbuffer={NULL,0};

#define DEST_BUFFER_GRANULARITY	16384

unsigned char *AllocDestBuffer(int size){
	size=size+DEST_BUFFER_GRANULARITY-(size%DEST_BUFFER_GRANULARITY);

	if(!destbuffer.buf){
		destbuffer.buf=new unsigned char[size];
		destbuffer.size=size;
	}
	if(size>destbuffer.size){
		delete[] destbuffer.buf;
		destbuffer.buf=new unsigned char[size];
		destbuffer.size=size;
	}
	return destbuffer.buf;
}

// ==========================================================================================================

int _io_file_eof(void *ptr){
	File *file=(File*)ptr;
	return file->Eof();
}

int _io_file_read(void *ptr,char *b, int len){
	File *file=(File*)ptr;
	return file->Read(b,1,len);
}

void _io_file_skip(void *ptr,unsigned int n){
	File *file=(File*)ptr;
	int pos=file->FilePos();
	file->SeekFile(pos+n);
}

unsigned char *stbi_load_pixbuf(const char *filename,int *width,int *height){
	stbi_io_callbacks io;
	unsigned char *pixbuf;

	io.eof=_io_file_eof;
	io.read=_io_file_read;
	io.skip=_io_file_skip;

	FilePtr file=File::ReadFile(filename);
	pixbuf=stbi_load_from_callbacks(&io,file.get(),width,height,0,4);

	return pixbuf;
	//return stbi_load(filename,width,height,0,4);
}

void stbi_free_pixbuf(unsigned char *buf){
	stbi_image_free(buf);
}

// ==========================================================================================================
CLASS_ALLOCATOR_IMPL(Texture);

list<Texture*> Texture::tex_list;
list<unsigned int> Texture::name_list;

// FIXME: Remove these
Texture::LoadPixbuf Texture::loadpixbuf = stbi_load_pixbuf;
Texture::FreePixbuf Texture::freepixbuf = stbi_free_pixbuf;

// ==========================================================================================================

Texture::Texture()
:texture(0)
,texture_ref(NULL,GL_DeleteTextures)
,file_name()
,file_abs()
,file_hash(0)
,frames(NULL)
,frames_ref(NULL,TextureDeleteFrames)
,flags(0)
,blend(2)
,coords(0)
,format(0)
,u_scale(1)
,v_scale(1)
,u_pos(0)
,angle(0)
,width(0)
,height(0)
,no_frames(1)
,framebuffer(NULL)
,cube_face(0)
,cube_mode(1)
,texenv_count(0)
,texenv() {
#if 0
	//texture=NULL;
	file_name="";
	flags=0,blend=2,coords=0;
	u_scale=1.0,v_scale=1.0,u_pos=0.0,v_pos=0.0,angle=0.0;
	string file_abs="";
	width=0,height=0; // returned by Name/Width/Height commands
	no_frames=1;
	framebuffer=0;
	cube_face=0,cube_mode=1;
		
	texenv_count=0;//
#endif
};

Texture::~Texture(){
	if(!tex_list.size()) return;
	tex_list.remove(this);
	// Delete the temp buffer when all the textures
	// are released
#if 0 // Scrap this
	if(!tex_list.size() && destbuffer.buf){
		delete[] destbuffer.buf;
		destbuffer.buf=NULL;
		destbuffer.size=0;
	}
#endif
}

Texture* Texture::LoadTexture(string filename,int flags){
	filename=File::ResourceFilePath(filename);
	if(filename.empty()) return NULL;

	// Try to load DDS file first
	//if(is_dds(filename)){
	if(CheckExtension(filename,".dds")){
		// Check this stuff here to avoid unnecessary allocations/deallocations
		size_t hash=StringHash(filename);
		FilterFlags(filename,flags);

		Texture *old=TexInList(hash,flags);
		if(old) return old;

		DirectDrawSurface *dds=DirectDrawSurface::LoadSurface(filename,false);
		if(!dds) return NULL;

		Texture* tex=new Texture();
		tex->file_name=filename;
		tex->file_hash=hash;
		tex->flags=flags;
		tex_list.push_back(tex);

		GL_GenTextures(tex);
		glBindTexture(dds->target,tex->texture);
		dds->UploadTexture(tex);
		dds->FreeDirectDrawSurface();

		return tex;
	}

	// Other images.
	if (flags&128) {
		// Check this stuff here to avoid unnecessary allocations/deallocations
		size_t hash=StringHash(filename);
		FilterFlags(filename,flags);

		Texture *old=TexInList(hash,flags);
		if(old) return old;

		Texture* tex=new Texture();
		tex->file_name=filename;
		tex->file_hash=hash;
		tex->format=GL_RGBA;
		tex->flags=flags;
		tex_list.push_back(tex);

		unsigned char* buffer;
		buffer=loadpixbuf(filename.c_str(),&tex->width,&tex->height);

		unsigned int name;
		tex->no_frames=1;
		tex->width=tex->width/6;
		//tex->frames=new unsigned int[6];
		// FIXME:
		unsigned char* dstbuffer=new unsigned char[tex->width*tex->height*4];
		GL_GenTextures(tex);
		glBindTexture(GL_TEXTURE_CUBE_MAP,name);

		//tex.gltex=tex.gltex[..tex.no_frames]
		for (int i=0;i<6;i++){
			CopyPixels(buffer,tex->width*6,tex->height,tex->width*i,0,dstbuffer,tex->width,tex->height, 4);
			GL_TexImage2D(gl_cube_faces[i],tex->width,tex->height,dstbuffer);
		}
		delete dstbuffer;
		freepixbuf(buffer);

		tex->texture=name;
		return tex;
	}
	else
	{
	  return Texture::LoadAnimTexture(filename,flags,0,0,0,1);
	}
}

// FIXME: Allow loading top-down animated textures as well as left-right.
// Top down would eliminate the need for a temporary buffer
Texture* Texture::LoadAnimTexture(string filename,int flags, int frame_width,int frame_height,int first_frame,int frame_count){
	filename=File::ResourceFilePath(filename);
	if(filename.empty()) return NULL;

	// Check this stuff here to avoid unnecessary allocations/deallocations
	size_t hash=StringHash(filename);
	FilterFlags(filename,flags);

	Texture *old=TexInList(hash,flags);
	if(old) return old;

	Texture* tex=new Texture();
	tex->file_name=filename;
	tex->file_hash=hash;
	tex->format=GL_RGBA;
	tex->flags=flags;
	tex_list.push_back(tex);

	unsigned char* buffer=loadpixbuf(filename.c_str(),&tex->width,&tex->height);

	if (frame_count<=2){
		GL_GenTextures(tex);
		glBindTexture(GL_TEXTURE_2D,tex->texture);
		GL_TexImage2D(GL_TEXTURE_2D,tex->width,tex->height,buffer);
	}else{
		//
		// FIXME: DO NOT FORGET
		//	Add reference to opengl textures..
		//
		//
		//
		tex->no_frames=frame_count;
		tex->frames=new unsigned int[frame_count+1]; // Allocate an extra unsigned int so we know where the array ends
		// FIXME:
		unsigned char* dstbuffer=new unsigned char[frame_width*frame_height*4];
		//tex.gltex=tex.gltex[..tex.no_frames]
		int frames_in_row=tex->width/frame_width;

		for (int i=0;i<frame_count;i++){
			unsigned int id;
			CopyPixels(buffer,tex->width,tex->height,frame_width*(i%frames_in_row),frame_height*(i/frames_in_row),dstbuffer,frame_width,frame_height,4);

			glGenTextures(1,&id);
			glBindTexture(GL_TEXTURE_2D,id);
			glTexParameteri(GL_TEXTURE_2D,GL_GENERATE_MIPMAP,GL_TRUE);
			GL_TexImage2D(GL_TEXTURE_2D,frame_width,frame_height,dstbuffer);

			tex->frames[i]=id;
		}
		tex->texture=tex->frames[0];
		name_list.push_back(tex->texture);
		tex->texture_ref=tex_name_t(&name_list.back(),GL_DeleteTextures);

		tex->width=frame_width;
		tex->height=frame_height;

		tex->frames[frame_count]=0;
		tex->frames_ref=tex_frames_t(tex->frames,TextureDeleteFrames);

		delete dstbuffer;
	}
	
	freepixbuf(buffer);

	return tex;

}

Texture* Texture::CreateTexture(int width,int height,int flags,int frames,string fname){
	Texture* tex=new Texture();

	tex->flags=flags;
	tex->FilterFlags();
	tex->width=width;
	tex->height=height;
	tex->file_name=fname;
	tex->file_hash=StringHash(fname);
	tex->format=GL_RGBA;

	//unsigned int id;
	//glGenTextures(1,&id);
	//tex->texture=id;
	GL_GenTextures(tex);

	return tex;

}

void Texture::FreeTexture(){
	DestroyRef();
}

void Texture::DrawTexture(int x,int y){
  //	[texture drawAtPoint:CGPointMake(x,y)];
}

void Texture::TextureBlend(int blend_no){
	blend=blend_no;
}

void Texture::TextureCoords(int coords_no){
	coords=coords_no;
}

void Texture::ScaleTexture(float u_s,float v_s){
	u_scale=1.0/u_s;
	v_scale=1.0/v_s;
}

void Texture::PositionTexture(float u_p,float v_p){
	u_pos=-u_p;
	v_pos=-v_p;
}

void Texture::RotateTexture(float ang){
	angle=ang;
}

string Texture::TextureName(){
	return file_name;
}

void Texture::SetTextureName(string name){
	file_name = name;
	file_hash = StringHash(name);
}

void Texture::ClearTextureFilters(){
	TextureFilter::tex_filter_list.clear();
}

void Texture::AddTextureFilter(string text_match,int flags){
	TextureFilter* filter=new TextureFilter();
	filter->text_match=text_match;
	filter->flags=flags;
	TextureFilter::tex_filter_list.push_back(filter);
}

Texture* Texture::TexInList(size_t hash,int flags){
	// I though about not checking the flags but decided against it since
	// someone might want to implement a material system (like Quake3).

	// So right now we'll just load the texture again if flags differ.
	// It would probably be a good idea to reuse texturename though.

	// check if tex already exists in list and if so increase ref count and return it

	// Not flag 2048 forces the creation of a new class instance but keeps the underlying
	// gl object

	// FIXME: See model.cpp
	for(Texture* tex : tex_list){
		if(hash==tex->file_hash){
			if(flags==tex->flags && !(tex->flags & 2048)){
				tex->AddRef();
				return tex;
			}

			// If we've flagged the texture as unique or
			// the texture flags differ then we create a new class
			// but continue to use the underlying gl object
			flags &= ~2048;

			Texture *newtex=new Texture;
			newtex->flags=tex->flags;
			newtex->FilterFlags();
			newtex->blend=tex->blend;
			newtex->coords=tex->coords;
			newtex->u_pos=tex->u_pos;
			newtex->v_pos=tex->v_pos;
			newtex->u_scale=tex->u_scale;
			newtex->v_scale=tex->v_scale;
			newtex->angle=tex->angle;

			newtex->texture_ref=tex->texture_ref;
			newtex->texture=*newtex->texture_ref;
			newtex->frames=tex->frames;
			newtex->frames_ref=tex->frames_ref;

			return newtex;
		}
	}
	return NULL;
}

Texture* Texture::TexInList(size_t hash,int flags,int blend,int coords,float u_pos,float v_pos,float u_scale,float v_scale,float angle){
	for(Texture* tex : tex_list){
		if(hash==tex->file_hash){
			if(tex->flags==flags && tex->blend==blend && tex->coords==coords && tex->u_pos==u_pos && tex->v_pos==v_pos && tex->u_scale==u_scale && tex->v_scale==v_scale && tex->angle==angle){
				tex->AddRef();
				return tex;
			}

			Texture *newtex=new Texture;
			newtex->flags=flags;
			newtex->FilterFlags();
			newtex->blend=blend;
			newtex->coords=coords;
			newtex->u_pos=u_pos;
			newtex->v_pos=v_pos;
			newtex->u_scale=u_scale;
			newtex->v_scale=v_scale;
			newtex->angle=angle;

			newtex->texture_ref=tex->texture_ref;
			newtex->texture=*newtex->texture_ref;
			newtex->frames=tex->frames;
			newtex->frames_ref=tex->frames_ref;

			return newtex;
		}
	}
	return NULL;
}

Texture* Texture::TexInList(){
	for(Texture* tex : tex_list){
		if(file_hash==tex->file_hash){
			if(flags==tex->flags && !(flags & 2048)){
				tex->AddRef();
				return tex;
			}

			// If we've flagged the texture as unique or
			// the texture flags differ then we create a new class
			// but continue to use the underlying gl object
			flags &= ~2048;

			Texture *newtex=new Texture;
			newtex->flags=tex->flags;
			newtex->FilterFlags();
			newtex->blend=tex->blend;
			newtex->coords=tex->coords;
			newtex->u_pos=tex->u_pos;
			newtex->v_pos=tex->v_pos;
			newtex->u_scale=tex->u_scale;
			newtex->v_scale=tex->v_scale;
			newtex->angle=tex->angle;

			newtex->texture_ref=tex->texture_ref;
			newtex->texture=*newtex->texture_ref;
			newtex->frames=tex->frames;
			newtex->frames_ref=tex->frames_ref;

			return newtex;
		}
	}
	return NULL;
}

void Texture::FilterFlags(const string& filename,int& flags){
	for(TextureFilter* filter : TextureFilter::tex_filter_list){
		if(Instr(filename,filter->text_match)) flags|=filter->flags;
	}
}

void Texture::FilterFlags(){
	// combine specifieds flag with texture filter flags
	list<TextureFilter*>::iterator it;
	for(it=TextureFilter::tex_filter_list.begin();it!=TextureFilter::tex_filter_list.end();it++){
		TextureFilter* filter=*it;
		// Have to check what the hell this is doing...
		if(Instr(file_name,filter->text_match)) flags=flags|filter->flags;
	}
}

void Texture::BufferToTex(unsigned char* buffer, int frame){
	if(flags&128){
		glBindTexture (GL_TEXTURE_CUBE_MAP,texture);
		GL_TexImage2D(gl_cube_faces[cube_face],width,height,buffer);
	}else{
		glBindTexture (GL_TEXTURE_2D,texture);
		GL_TexImage2D(GL_TEXTURE_2D,width,height,buffer);
	}

}

void Texture::BackBufferToTex(int frame){
	if(flags&128){
		glBindTexture(GL_TEXTURE_CUBE_MAP,texture);
		GL_CopyTexImage2D(gl_cube_faces[cube_face],0,0,width,height);
	}else{
		glBindTexture(GL_TEXTURE_2D,texture);
		GL_CopyTexImage2D(GL_TEXTURE_2D,0,0,width,height);
	}
}

void Texture::CameraToTex(Camera* cam, int frame){

	Global::camera_in_use=cam;

	GLenum target=(flags&128)?GL_TEXTURE_CUBE_MAP:GL_TEXTURE_2D;
	glBindTexture(target, texture);

	if (!framebuffer){
		framebuffer=new unsigned int[2];
		glGenFramebuffers(1, &framebuffer[0]);
		glGenRenderbuffers(1, &framebuffer[1]);
		if(flags&128){
			for (int i=0;i<6;i++){
				glTexImage2D(gl_cube_faces[i],0,GL_RGBA,width,height,0,GL_RGBA,GL_UNSIGNED_BYTE,NULL);
			}
		}else{
			glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,width,height,0,GL_RGBA,GL_UNSIGNED_BYTE,NULL);
		}

	}

	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer[0]);

	if(flags&128){
		glFramebufferTexture2D(GL_FRAMEBUFFER,GL_COLOR_ATTACHMENT0,gl_cube_faces[cube_face],texture,0);
	}else{
		glFramebufferTexture2D(GL_FRAMEBUFFER,GL_COLOR_ATTACHMENT0,GL_TEXTURE_2D,texture,0);
	}

	//Depth buffer
	glBindRenderbuffer(GL_RENDERBUFFER,framebuffer[1]);
	glRenderbufferStorage(GL_RENDERBUFFER,GL_DEPTH_STENCIL,width,height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER,GL_DEPTH_ATTACHMENT,GL_RENDERBUFFER,framebuffer[1]); 
	glFramebufferRenderbuffer(GL_FRAMEBUFFER,GL_STENCIL_ATTACHMENT,GL_RENDERBUFFER,framebuffer[1]); 

	cam->Render();

	if (Global::Shadows_enabled==true)
		ShadowObject::Update(cam);

	//glFramebufferRenderbuffer( GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_RENDERBUFFER, 0); 

	glBindTexture(target, texture);//
	glGenerateMipmap(target);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
}


void Texture::TexToBuffer(unsigned char* buffer, int frame){
	// FIXME: Compressed textures
	glBindTexture (GL_TEXTURE_2D,texture);
	if(flags&128){
		glGetTexImage(gl_cube_faces[cube_face], 0, GL_RGBA, GL_UNSIGNED_BYTE, buffer);
	}else{
		glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_BYTE, buffer);
	}
}


void Texture::DepthBufferToTex(Camera* cam=0 ){

	glBindTexture(GL_TEXTURE_2D,texture);
	if (!cam){
		//glCopyTexImage2D(GL_TEXTURE_2D,0,GL_DEPTH_COMPONENT,0,Global::height-height,width,height,0);
		//glTexParameteri(GL_TEXTURE_2D,GL_GENERATE_MIPMAP_SGIS,GL_TRUE);
		GL_CopyTexImage2D(GL_TEXTURE_2D,0,0,width,height);
	}else{
		Global::camera_in_use=cam;
		
		if (!framebuffer){
			framebuffer=new unsigned int[2];
			glGenFramebuffers(1, &framebuffer[0]);
			glGenRenderbuffers(1, &framebuffer[1]);
			//glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT24, width, height, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, 0);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);//
		}
		
		glBindFramebuffer(GL_FRAMEBUFFER, framebuffer[0]);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture, 0);//
		
		glBindRenderbuffer(GL_RENDERBUFFER, framebuffer[1]);
		glRenderbufferStorage( GL_RENDERBUFFER, GL_DEPTH_STENCIL, width, height);//
		//glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, texture, 0);
		glFramebufferRenderbuffer( GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, framebuffer[1]);//
		glFramebufferRenderbuffer( GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_RENDERBUFFER, framebuffer[1]);//

		cam->Render();
		
		//glFramebufferRenderbuffer( GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_RENDERBUFFER, 0); 

		glBindTexture(GL_TEXTURE_2D,texture);//
		glGenerateMipmap(GL_TEXTURE_2D);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glBindRenderbuffer(GL_RENDERBUFFER, 0);
	}
}


void CopyPixels (unsigned char *src, unsigned int srcWidth, unsigned int srcHeight, unsigned int srcX, unsigned int srcY, 
	unsigned char *dst, unsigned int dstWidth, unsigned int dstHeight, unsigned int bytesPerPixel) {
	// Copy image data line by line
	unsigned int y;

	for(y=0; y<dstHeight; y++)
		memcpy (dst+y*dstWidth*bytesPerPixel,src+((y+srcY)*srcWidth+srcX)*bytesPerPixel,dstWidth*bytesPerPixel);
}

