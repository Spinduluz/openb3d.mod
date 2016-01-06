#ifndef LOAD3DS_H
#define LOAD3DS_H

namespace load3ds{
	Mesh* Load3ds(FilePtr Stream,Entity* parent_ent);
	Mesh* Load3ds(string URL,Entity* parent_ent);
}

#endif // LOAD3DS_H
