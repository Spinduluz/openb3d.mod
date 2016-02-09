#ifndef LOAD3DS_H
#define LOAD3DS_H

namespace load3ds{
	Mesh* Load3ds(FilePtr file,Entity* parent_ent);
	Mesh* Load3ds(const string& path,Entity* parent_ent);
}

#endif // LOAD3DS_H
