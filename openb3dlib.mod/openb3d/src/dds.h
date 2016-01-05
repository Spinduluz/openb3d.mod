#ifndef DDS_H
#define DDS_H

#include <memory.h>
#include <vector>
#include <iostream>

using namespace std;

class DirectDrawSurface{
public:
	unsigned char *buf;
	vector<DirectDrawSurface> mipmaps;

	int width;
	int height;
	int depth;
	int mipmapcount;
	int pitch;
	int size;

	unsigned char *dxt;
	unsigned int format;
	unsigned int target;

	static DirectDrawSurface *LoadSurface(const string& filename);
};

#endif // DDS_H

