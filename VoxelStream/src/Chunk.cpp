#include "Chunk.h"
#include "FullyLoadedChunk.h"
#include "OptimizedChunk.h"

VOXEL_TYPE VS::Chunk::getVoxel(unsigned short voxelID)
{

	switch (saveState)
	{
		// If it is the same type is easy to retrive
	case VS::ChunkSaveState::SameType:
		return chunkSameTypeID;

		// If it is a fully loaded chunk is easy to retrive to
	case VS::ChunkSaveState::FullyLoadedChunk:
		return fullyLoadedChunk->voxelData[voxelID];

	case VS::ChunkSaveState::CompressedChunk:
		return optimizedChunk->getVoxel(voxelID);

	}

	return 0;
}
