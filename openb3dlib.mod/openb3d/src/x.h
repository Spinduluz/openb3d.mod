#ifndef LOAD_X
#define LOAD_X

namespace loadX{
	Mesh* LoadX(File* Stream,Entity* parent_ent);
	Mesh* LoadX(string URL,Entity* parent_ent);
}

#endif // LOAD_X
