#include "VoxelStream/VoxelDatabase.h"
#include "InternalDefinitions.h"
#include "Chunk.h"
#include "FullyLoadedChunk.h"
#include "ChunkMemoryManager.h"


VS::VoxelDatabase::VoxelDatabase(DatabaseOptions opt)
{
	dbOpt = opt;
	chunkMemory = new ChunkMemoryManager(opt);

}

VS::VoxelDatabase::~VoxelDatabase()
{
	delete chunkMemory;

}
