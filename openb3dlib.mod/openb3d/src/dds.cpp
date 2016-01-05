#include "file.h"
#include "dds.h"
#include "glew.h"


struct DXTColorBlock{
	unsigned short col0;
	unsigned short col1;
	unsigned char row[4];
};

struct DXT3AlphaBlock {
	unsigned short row[4];
};

struct DXT5AlphaBlock {
	unsigned char alpha0;
	unsigned char alpha1;
	unsigned char row[6];
};

typedef void (*FlipDXT)(DXTColorBlock *line,unsigned int num);

#define DXT_MASK	0x00000007

inline void DXT_FlipAlphaDXT5(DXT5AlphaBlock *block){
	unsigned char bits[4][4];
	unsigned int bit, *p;

	bit=0;
	memcpy(&bit,&block->row[0],3);

	bits[0][0]=(unsigned char)(bit & DXT_MASK);
	bit>>=3;
	bits[0][1]=(unsigned char)(bit & DXT_MASK);
	bit>>=3;
	bits[0][2]=(unsigned char)(bit & DXT_MASK);
	bit>>=3;
	bits[0][3]=(unsigned char)(bit & DXT_MASK);
	bit>>=3;
	bits[1][0]=(unsigned char)(bit & DXT_MASK);
	bit>>=3;
	bits[1][1]=(unsigned char)(bit & DXT_MASK);
	bit>>=3;
	bits[1][2]=(unsigned char)(bit & DXT_MASK);
	bit>>=3;
	bits[1][3]=(unsigned char)(bit & DXT_MASK);

	bit=0;
	memcpy(&bit,&block->row[3],3);

	bits[2][0]=(unsigned char)(bit & DXT_MASK);
	bit>>=3;
	bits[2][1]=(unsigned char)(bit & DXT_MASK);
	bit>>=3;
	bits[2][2]=(unsigned char)(bit & DXT_MASK);
	bit>>=3;
	bits[2][3]=(unsigned char)(bit & DXT_MASK);
	bit>>=3;
	bits[3][0]=(unsigned char)(bit & DXT_MASK);
	bit>>=3;
	bits[3][1]=(unsigned char)(bit & DXT_MASK);
	bit>>=3;
	bits[3][2]=(unsigned char)(bit & DXT_MASK);
	bit>>=3;
	bits[3][3]=(unsigned char)(bit & DXT_MASK);
	
	p=((unsigned int*)&(block->row[0]));

	*p=*p | (bits[3][0]<<0);
	*p=*p | (bits[3][1]<<3);
	*p=*p | (bits[3][2]<<6);
	*p=*p | (bits[3][3]<<9);

	*p=*p | (bits[2][0]<<12);
	*p=*p | (bits[2][1]<<15);
	*p=*p | (bits[2][2]<<18);
	*p=*p | (bits[2][3]<<21);

	p=((unsigned int*)&(block->row[3]));

	*p=*p | (bits[1][0]<<0);
	*p=*p | (bits[1][1]<<3);
	*p=*p | (bits[1][2]<<6);
	*p=*p | (bits[1][3]<<9);

	*p &= 0xff000000;

	*p=*p | (bits[0][0]<<12);
	*p=*p | (bits[0][1]<<15);
	*p=*p | (bits[0][2]<<18);
	*p=*p | (bits[0][3]<<21);
}

inline void DXT_SwapPointer(void *ptr1,void *ptr2,unsigned int size){
	static vector<unsigned char> tmp;
	void *ptr;

	// FIXME:
	//	Use shrink to fit?
	//	Max size to avoid possible overflows?
	if (tmp.size() < size)
		tmp.reserve(size);

	ptr=&tmp[0];
	memcpy(ptr,ptr1,size);
	memcpy(ptr1,ptr2,size);
	memcpy(ptr2,ptr,size);
}

template<short size>
inline void DXT_SwapPointer(void *ptr1,void *ptr2){
	unsigned char temp[size];

	memcpy(temp,ptr1,size);
	memcpy(ptr1,ptr2,size);
	memcpy(ptr2,temp,size);
}

inline void DXT_FlipBlocksDXTC1(DXTColorBlock *line,unsigned int num){
	DXTColorBlock *current;
	unsigned int i;

	current=line;
	for(i=0; i<num; i++){
		DXT_SwapPointer(&current->row[0],&current->row[3],1);
		DXT_SwapPointer(&current->row[1],&current->row[2],1);

		current++;
	}

}

inline void DXT_FlipBlocksDXTC3(DXTColorBlock *line,unsigned int num){
	DXT3AlphaBlock	*block;
	DXTColorBlock	*current;
	unsigned int i;

	current=line;
	for(i=0; i<num; i++){
		block=(DXT3AlphaBlock *)current;

		DXT_SwapPointer(&block->row[0],&block->row[3],2);
		DXT_SwapPointer(&block->row[1],&block->row[2],2);

		current++;

		DXT_SwapPointer(&current->row[0],&current->row[3],1);
		DXT_SwapPointer(&current->row[1],&current->row[2],1);

		current++;
	}
}

inline void DXT_FlipBlocksDXTC5(DXTColorBlock *line,unsigned int num){
	DXT5AlphaBlock	*Block;
	DXTColorBlock	*current;
	unsigned int i;

	current=line;
	for(i=0; i<num; i++){
		Block=(DXT5AlphaBlock*)current;

		DXT_FlipAlphaDXT5(Block);

		current++;

		DXT_SwapPointer(&current->row[0],&current->row[3],1);
		DXT_SwapPointer(&current->row[1],&current->row[2],1);

		current++;
	}
}

// ==========================================================================================================

#define DDSD_CAPS					0x00000001
#define DDSD_HEIGHT					0x00000002
#define DDSD_WIDTH					0x00000004
#define DDSD_PITCH					0x00000008
#define DDSD_PIXELFORMAT			0x00001000
#define DDSD_MIPMAPCOUNT			0x00020000
#define DDSD_LINEARSIZE				0x00080000
#define DDSD_DEPTH					0x00800000

#define DDPF_ALPHAPIXELS			0x00000001
#define DDPF_FOURCC					0x00000004
#define DDPF_RGB					0x00000040
#define DDPF_RGBA					0x00000041

#define DDSCAPS_COMPLEX				0x00000008
#define DDSCAPS_TEXTURE				0x00001000
#define DDSCAPS_MIPMAP				0x00400000

#define DDSCAPS2_CUBEMAP			0x00000200
#define DDSCAPS2_CUBEMAP_POSITIVEX	0x00000400
#define DDSCAPS2_CUBEMAP_NEGATIVEX	0x00000800
#define DDSCAPS2_CUBEMAP_POSITIVEY	0x00001000
#define DDSCAPS2_CUBEMAP_NEGATIVEY	0x00002000
#define DDSCAPS2_CUBEMAP_POSITIVEZ	0x00004000
#define DDSCAPS2_CUBEMAP_NEGATIVEZ	0x00008000
#define DDSCAPS2_CUBEMAP_ALL_FACES	0x0000FC00
#define DDSCAPS2_VOLUME				0x00200000

#define FOURCC_DXT1					0x31545844
#define FOURCC_DXT3					0x33545844
#define FOURCC_DXT5					0x35545844

#pragma pack(push,packed)
#pragma pack(1)

struct DDSPixelFormat {
	unsigned int	size;
	unsigned int	flags;
	unsigned int	fourcc;
	unsigned int	rgbbitcount;
	unsigned int	rbitmask;
	unsigned int	gbitmask;
	unsigned int	bbitmask;
	unsigned int	abitmask;
};

struct DDSHeader {
	unsigned int	size;
	unsigned int	flags;
	unsigned int	height;
	unsigned int	width;
	unsigned int	pitchoflinearsize;
	unsigned int	depth;
	unsigned int	mipmapcount;
	unsigned int	reserved1[11];
	DDSPixelFormat	pixelformat;
	unsigned int	caps1;
	unsigned int	caps2;
	unsigned int	reserved2[3];
};

#pragma pack(pop,packed)

inline int DDS_Clamp(int a) { return a<0?1:a; }

inline bool DDS_GetInfo(DDSHeader *header,unsigned int& format,int& components,unsigned int& type){
	type=GL_TEXTURE_2D;
	if(header->caps2 & DDSCAPS2_CUBEMAP){
		type=GL_TEXTURE_CUBE_MAP;
	}
	// Bitmap3D not implemented?
	if(header->caps2 & DDSCAPS2_VOLUME){
		return false;
	}

	if(header->pixelformat.flags & DDPF_FOURCC){
		switch(header->pixelformat.fourcc)
		{
		case FOURCC_DXT1:
			format=GL_COMPRESSED_RGB_S3TC_DXT1_EXT;
			components=3;
			break;

		case FOURCC_DXT3:
			format=GL_COMPRESSED_RGBA_S3TC_DXT3_EXT;
			components=4;
			break;

		case FOURCC_DXT5:
			format=GL_COMPRESSED_RGBA_S3TC_DXT5_EXT;
			components=4;
			break;
		}
	}else if(header->pixelformat.flags & DDPF_RGBA && header->pixelformat.rgbbitcount==32){
		format=GL_RGBA;
	}else if(header->pixelformat.flags & DDPF_RGB && header->pixelformat.rgbbitcount==32){
		format=GL_RGBA;
	}else if(header->pixelformat.flags & DDPF_RGB && header->pixelformat.rgbbitcount==24){
		format=GL_RGB;
	}else if(header->pixelformat.rgbbitcount==8){ // Luminance?
		return false;
	}else{
		return false;
	}
}

inline int DDS_GetSizeBytes(int width,int height,int format){
	if(format==GL_COMPRESSED_RGB_S3TC_DXT1_EXT || format==GL_COMPRESSED_RGBA_S3TC_DXT3_EXT || format==GL_COMPRESSED_RGBA_S3TC_DXT5_EXT){
		return ((width+3)/4) * ((height+3)/4) * (format==GL_COMPRESSED_RGB_S3TC_DXT1_EXT?8:16);
	}
	return width * height * (format==GL_RGB?3:4);
}

inline int DDS_GetPitch(int width,unsigned int format){
	if(format==GL_COMPRESSED_RGB_S3TC_DXT1_EXT || format==GL_COMPRESSED_RGBA_S3TC_DXT3_EXT || format==GL_COMPRESSED_RGBA_S3TC_DXT5_EXT){
		return ((width+3)/4) * ((format==GL_COMPRESSED_RGB_S3TC_DXT1_EXT?8:16));
	}
}

#if defined(_MSC_VER)
#define strncasecmp _strnicmp
#endif

DirectDrawSurface *DirectDrawSurface::LoadSurface(const string& filename){
	unsigned char *buffer,*buf;
	DDSHeader *dds;
	File *file;
	int len;

	file=File::ReadFile(filename);
	len=file->FileSize();

	buffer=new unsigned char[len];
	file->Read(buffer,1,len);

	buf=buffer;
	if(strncasecmp((char*)buf,"DDS ",4)){
		return NULL;
	}
	buf+=4;
	dds=(DDSHeader*)buf;
	buf+=sizeof(DDSHeader);

	unsigned int format,type;
	int components;

	if(!DDS_GetInfo(dds,format,components,type)){
		return NULL;
	}

	DirectDrawSurface *surf=new DirectDrawSurface;

	int mipmaps;

	surf->format = format;
	surf->target = type;
	surf->width=dds->width;
	surf->height=dds->height;
	surf->depth=DDS_Clamp(dds->depth);
	mipmaps=surf->mipmapcount=dds->mipmapcount;

	if(mipmaps!=0) mipmaps--;

	for(int i=0; i< (type==GL_TEXTURE_CUBE_MAP?6:1); i++){
		DirectDrawSurface *s;

		surf->mipmaps.push_back(DirectDrawSurface());
		s=&surf->mipmaps.back();

		s->format=format;
		s->target=type;
		s->dxt=buf;
		s->width=surf->width;
		s->height=surf->height;
		s->depth=surf->depth;
		s->pitch=DDS_GetPitch(s->width,format);
		s->size=DDS_GetSizeBytes(s->width,s->height,format) * s->depth;

		buf+=s->size;

		int w,h,d;

		w=DDS_Clamp(s->width>>1);
		h=DDS_Clamp(s->height>>1);
		d=DDS_Clamp(s->depth>>1);

		for(int j=0; j<mipmaps && ( w || h ); j++){
			DirectDrawSurface *mip;
			s->mipmaps.push_back(DirectDrawSurface());
			mip=&s->mipmaps.back();

			mip->format=format;
			mip->target=type;
			mip->dxt=buf;
			mip->width=w;
			mip->height=h;
			mip->depth=d;
			mip->pitch=DDS_GetPitch(w,format);
			mip->size=DDS_GetSizeBytes(w,h,format) * d;

			buf+=mip->size;

			w=DDS_Clamp(s->width>>1);
			h=DDS_Clamp(s->height>>1);
			d=DDS_Clamp(s->depth>>1);
		}
	}

	return surf;
}

