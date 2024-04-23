#pragma once
#include "InternalDefinitions.h"

namespace VS {
	class Chunk
	{
	public:
		Chunk();
		~Chunk();

		VOXEL_TYPE getVoxel(unsigned short voxelID);
		void modifyVoxel(unsigned short voxelID, VOXEL_TYPE _voxel);

		ChunkSaveState saveState = ChunkSaveState::CompressedChunk;
		// We don't know which array it is, only if we search for the save state
		ARRAY_POINTER priorityPosition;
	private:

		// Only in case it is a optimizedChunk
		OptimizedChunk* optimizedChunk;
		// Only in case it is a fully loaded chunk
		FullyLoadedChunk* fullyLoadedChunk;

		// Only in case it is a Same Type Chunk (all ids are the same like air)
		VOXEL_TYPE chunkSameTypeID = 0;


	};
}

