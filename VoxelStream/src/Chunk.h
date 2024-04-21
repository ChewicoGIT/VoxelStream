#pragma once
#include "VoxelStream/Definitions.h"
#include "InternalDefinitions.h"
#include "OptimizedChunk.h"

namespace VS {
	class Chunk
	{
	public:
		//We don't know which array is, only if we search for the save state;
		ARRAY_POINTER priorityPosition;

		ChunkSaveState saveState;
		//Only in case it is a optimizedChunk
		OptimizedChunk* optimizedChunk;
		//Only in case it is a Same Type Chunk (all ids are the same like air)
		VOXEL_TYPE chunkSameTypeID;

	};
}

