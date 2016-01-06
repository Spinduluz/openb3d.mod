#ifndef LOADMD2_H
#define LOADMD2_H

namespace loadMD2{
  Mesh* LoadMD2(string URL, Entity* parent_ent);
  Mesh* LoadMD2(FilePtr Stream, Entity* parent_ent);
}

#endif // LOADMD2_H
