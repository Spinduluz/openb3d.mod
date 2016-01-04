#include "dds.h"


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
