#pragma once
#include "InternalDefinitions.h"

namespace VS {
	class Chunk
	{
	public:
		// We don't know which array it is, only if we search for the save state
		ARRAY_POINTER priorityPosition;

		ChunkSaveState saveState = ChunkSaveState::SameType;

		// Only in case it is a optimizedChunk
		OptimizedChunk* optimizedChunk;
		// Only in case it is a fully loaded chunk
		FullyLoadedChunk* fullyLoadedChunk;

		// Only in case it is a Same Type Chunk (all ids are the same like air)
		VOXEL_TYPE chunkSameTypeID = 0;

		VOXEL_TYPE getVoxel(unsigned short voxelID);



	};
}

