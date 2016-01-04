/*
 *  texture.mm
 *  iminib3d
 *
 *  Created by Simon Harrison.
 *  Copyright Si Design. All rights reserved.
 *
 */

#include "glew.h"

#include "texture.h"
#include "stb_image.h"

#include "texture_filter.h"
#include "string_helper.h"
#include "file.h"
#include "global.h"
#include "shadow.h"

#include <string.h>

#if defined(BLITZMAX_DEBUG)
#include "bmaxdebug.h"
#endif

list<Texture*> Texture::tex_list;

void CopyPixels (unsigned char *src, unsigned int srcWidth, unsigned int srcHeight, unsigned int srcX, unsigned int srcY, unsigned char *dst, unsigned int dstWidth, unsigned int dstHeight, unsigned int bytesPerPixel);
#if 1
static int gl_cube_faces[]={
	GL_TEXTURE_CUBE_MAP_NEGATIVE_X,
	GL_TEXTURE_CUBE_MAP_POSITIVE_Z,
	GL_TEXTURE_CUBE_MAP_POSITIVE_X,
	GL_TEXTURE_CUBE_MAP_NEGATIVE_Z,
	GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,
	GL_TEXTURE_CUBE_MAP_POSITIVE_Y,
};
#else
static int gl_cube_faces[]={
	GL_TEXTURE_CUBE_MAP_NEGATIVE_X,
	GL_TEXTURE_CUBE_MAP_NEGATIVE_Z,
	GL_TEXTURE_CUBE_MAP_POSITIVE_X,
	GL_TEXTURE_CUBE_MAP_POSITIVE_Z,
	GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,
	GL_TEXTURE_CUBE_MAP_POSITIVE_Y,
};
#endif

// FIXME: Cleanup.... I get a headache from my own code
inline void GL_TexImage2D(GLenum target,int width,int height,unsigned char *buffer){
	if(Global::gl_sgis_generate_mipmap && (!Global::gl_ext_framebuffer_object || !buffer)){
		glTexParameteri(target,GL_GENERATE_MIPMAP,GL_TRUE);
	}
	// Create texture
	glTexImage2D(target,0,GL_RGBA,width,height,0,GL_RGBA,GL_UNSIGNED_BYTE,buffer);
	// 
	if(Global::gl_ext_framebuffer_object && buffer){
		glGenerateMipmap(target);
	}else if(!Global::gl_sgis_generate_mipmap && buffer){
		gluBuild2DMipmaps(target,GL_RGBA,width,height,GL_RGBA,GL_UNSIGNED_BYTE,buffer);
	}
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

	io.eof=_io_file_eof;
	io.read=_io_file_read;
	io.skip=_io_file_skip;

	File *file=File::ReadFile(filename);
	return stbi_load_from_callbacks(&io,file,width,height,0,4);
	//return stbi_load(filename,width,height,0,4);
}

void stbi_free_pixbuf(unsigned char *buf){
	stbi_image_free(buf);
}

// ==========================================================================================================

Texture::LoadPixbuf Texture::loadpixbuf = stbi_load_pixbuf;
Texture::FreePixbuf Texture::freepixbuf = stbi_free_pixbuf;

// ==========================================================================================================

Texture::Texture()
:texture(0)
,file_name()
,file_abs()
,file_hash(0)
,frames(NULL)
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
,glTexEnv_count(0)
,glTexEnv() {
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
		
	glTexEnv_count=0;//
#endif
};

Texture* Texture::LoadTexture(string filename,int flags){
	if (flags&128) {
		filename=File::ResourceFilePath(filename);

		Texture* tex=new Texture();
		tex->file_name=filename;
		tex->file_hash=StringHash(filename);
		tex->format=GL_RGBA;

		// set tex.flags before TexInList
		tex->flags=flags;
		tex->FilterFlags();

		// check to see if texture with same properties exists already, if so return existing texture
		Texture* old_tex=tex->TexInList();
		if(old_tex){
			delete tex; // No memory leak...
			return old_tex;
		}else{
			tex_list.push_back(tex);
		}

		string filename_left=Left(filename,Len(filename)-4);
		string filename_right=Right(filename,3);

		unsigned char* buffer;
		buffer=loadpixbuf(filename.c_str(),&tex->width,&tex->height);

		unsigned int name;
		tex->no_frames=1;
		tex->width=tex->width/6;
		tex->frames=new unsigned int[6];

		unsigned char* dstbuffer=new unsigned char[tex->width*tex->height*4];
		glGenTextures(1,&name);
		glBindTexture(GL_TEXTURE_CUBE_MAP,name);

		//tex.gltex=tex.gltex[..tex.no_frames]
		for (int i=0;i<6;i++){
			CopyPixels (buffer,tex->width*6,tex->height,tex->width*i,0,dstbuffer,tex->width,tex->height, 4);
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

Texture* Texture::LoadAnimTexture(string filename,int flags, int frame_width,int frame_height,int first_frame,int frame_count){
	filename=File::ResourceFilePath(filename);

	Texture* tex=new Texture();
	tex->file_name=filename;
	tex->file_hash=StringHash(filename);
	tex->format=GL_RGBA;

	// set tex.flags before TexInList
	tex->flags=flags;
	tex->FilterFlags();

	// check to see if texture with same properties exists already, if so return existing texture
	Texture* old_tex=tex->TexInList();
	if(old_tex){
		delete tex; // No memory leak.
		return old_tex;
	}else{
		tex_list.push_back(tex);
	}
#if 0
	string filename_left=Left(filename,Len(filename)-4);
	string filename_right=Right(filename,3);
#endif

	unsigned char* buffer=loadpixbuf(filename.c_str(),&tex->width,&tex->height);

	unsigned int id;
	if (frame_count<=2){
		glGenTextures(1,&id);
		glBindTexture(GL_TEXTURE_2D,id);
		//glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA8,tex->width,tex->height,0,GL_RGBA, GL_UNSIGNED_BYTE,buffer);
		GL_TexImage2D(GL_TEXTURE_2D,tex->width,tex->height,buffer);

		tex->texture=id;
	} else {
		tex->no_frames=frame_count;
		tex->frames=new unsigned int[frame_count];

		unsigned char* dstbuffer=new unsigned char[frame_width*frame_height*4];
		//tex.gltex=tex.gltex[..tex.no_frames]
		int frames_in_row=tex->width/frame_width;

		for (int i=0;i<frame_count;i++){
			CopyPixels(buffer,tex->width,tex->height,frame_width*(i%frames_in_row),frame_height*(i/frames_in_row),
				dstbuffer,frame_width,frame_height,4);

			glGenTextures(1,&id);
			glBindTexture(GL_TEXTURE_2D,id);
			glTexParameteri(GL_TEXTURE_2D,GL_GENERATE_MIPMAP,GL_TRUE);
			//glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA8,frame_width,frame_height,0,GL_RGBA,GL_UNSIGNED_BYTE,dstbuffer);
			GL_TexImage2D(GL_TEXTURE_2D,frame_width,frame_height,dstbuffer);
			//gluBuild2DMipmaps(GL_TEXTURE_2D,GL_RGBA,frame_width, frame_height,GL_RGBA,GL_UNSIGNED_BYTE,dstbuffer);

			tex->frames[i]=id;
		}
		tex->texture=tex->frames[0];
		tex->width=frame_width;
		tex->height=frame_height;
		delete dstbuffer;
	}
	
	freepixbuf(buffer);

	return tex;

}

Texture* Texture::CreateTexture(int width,int height,int flags, int frames,string fname){
	Texture* tex=new Texture();

	tex->flags=flags;
	tex->FilterFlags();
	tex->width=width;
	tex->height=height;
	tex->file_name=fname;
	tex->file_hash=StringHash(fname);
	tex->format=GL_RGBA;

	int target=GL_TEXTURE_2D;
	if(flags&128) target=GL_TEXTURE_CUBE_MAP;

	unsigned int id;
	glGenTextures(1,&id);
	/*glBindTexture(target,id);
	GL_TexImage2D(target,width,height,NULL);*/
	tex->texture=id;

	return tex;

}

void Texture::FreeTexture(){
	tex_list.remove(this);
	delete this;
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
	TextureFilter* filter=new TextureFilter(); // Why even make this a pointer??
	filter->text_match=text_match;
	filter->flags=flags;
	TextureFilter::tex_filter_list.push_back(filter);
}

Texture* Texture::TexInList(){
	// check if tex already exists in list and if so return it
	list<Texture*>::iterator it;
	for(it=tex_list.begin();it!=tex_list.end();it++){
		Texture* tex=*it;
#if 0
		if(!tex->file_hash || !file_hash){ // Assume we dont have a hash.
			if(file_name==tex->file_name && flags==tex->flags){// && blend==tex->blend){
			//if(u_scale==tex->u_scale && v_scale==tex->v_scale && u_pos==tex->u_pos && v_pos==tex->v_pos && angle==tex->angle){
				return tex;
			//}
			}
		} else {
			if(file_hash==tex->file_hash){
				return tex;
			}
		}
#else
		if(file_hash==tex->file_hash && flags==tex->flags){
			return tex;
		}
#endif
	}
	return NULL;
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

inline void GL_CopyTexImage2D(GLenum target,int x,int y,int width,int height){
	if(Global::gl_sgis_generate_mipmap && !Global::gl_ext_framebuffer_object){
		glTexParameteri(target,GL_GENERATE_MIPMAP,GL_TRUE);
	}	
	glCopyTexImage2D(target,0,GL_RGBA,x,y,width,height,0);
	if(Global::gl_ext_framebuffer_object){
		glGenerateMipmap(target);
	}
}

void Texture::BackBufferToTex(int frame){
	if(flags&128){
		glBindTexture(GL_TEXTURE_CUBE_MAP,texture);
		GL_CopyTexImage2D(gl_cube_faces[cube_face],0,0,width,height);
		//glCopyTexImage2D(gl_cube_faces[cube_face],0,GL_RGBA,0,/*Global::height-height*/0,width,height,0);
		//glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
	}else{
		glBindTexture(GL_TEXTURE_2D,texture);
		glCopyTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,0,/*Global::height-height*/0,width,height,0);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
}

void Texture::CameraToTex(Camera* cam, int frame){

	GLenum target;

	Global::camera_in_use=cam;

	if(flags&128){
		target=GL_TEXTURE_CUBE_MAP;
	}else{
		target=GL_TEXTURE_2D;
	}
	
	glBindTexture(target, texture);

	if (!framebuffer){
		framebuffer=new unsigned int[2];
		glGenFramebuffers(1, &framebuffer[0]);
		glGenRenderbuffers(1, &framebuffer[1]);
		if(flags&128){
			for (int i=0;i<6;i++){
				//GL_TexImage2D(gl_cube_faces[i],width,height,NULL);
				glTexImage2D(gl_cube_faces[i],0,GL_RGBA,width,height,0,GL_RGBA,GL_UNSIGNED_BYTE,NULL);
			}
		}else{
			GL_TexImage2D(GL_TEXTURE_2D,width,height,NULL);
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

	glBindTexture (target, texture);//
	glGenerateMipmap(target);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
}


void Texture::TexToBuffer(unsigned char* buffer, int frame){
	glBindTexture (GL_TEXTURE_2D,texture);
	if(flags&128){
		glGetTexImage(gl_cube_faces[cube_face], 0, GL_RGBA, GL_UNSIGNED_BYTE, buffer);
	}else{
		glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_BYTE, buffer);
	}
}


void Texture::DepthBufferToTex(Camera* cam=0 ){

	glBindTexture(GL_TEXTURE_2D,texture);
	if (cam==0){
		glCopyTexImage2D(GL_TEXTURE_2D,0,GL_DEPTH_COMPONENT,0,Global::height-height,width,height,0);
		glTexParameteri(GL_TEXTURE_2D,GL_GENERATE_MIPMAP_SGIS,GL_TRUE);
	}else{
		Global::camera_in_use=cam;
		
		if (framebuffer==0){
			framebuffer=new unsigned int[1];
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


void CopyPixels (unsigned char *src, unsigned int srcWidth, unsigned int srcHeight, unsigned int srcX, unsigned int srcY, unsigned char *dst, unsigned int dstWidth, unsigned int dstHeight, unsigned int bytesPerPixel) {
	// Copy image data line by line
	unsigned int y;
	for (y = 0; y < dstHeight; y++)
		memcpy (dst + y * dstWidth * bytesPerPixel, src + ((y + srcY) * srcWidth + srcX) * bytesPerPixel, dstWidth * bytesPerPixel);
}

