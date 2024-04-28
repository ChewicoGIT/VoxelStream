#pragma once
#include "InternalDefinitions.h"
#include <vector>

namespace VS {
	class Chunk
	{
	public:
		Chunk() = default;
		Chunk(Chunk& _chnk) = delete;
		Chunk(ARRAY_POINTER _priorityPosition, OptimizedChunk* _optimizedChunk);
		~Chunk();

		VOXEL_TYPE getVoxel(unsigned short voxelID);
		void modifyVoxel(unsigned short voxelID, VOXEL_TYPE _voxel);

		ChunkSaveState saveState = ChunkSaveState::None;
		// We don't know which array it is, only if we search for the save state
		ARRAY_POINTER priorityPosition;

		void initAsOptimizedChunk(OptimizedChunk* _optimizedChunk);

		void useFullyLoadedChunk(FullyLoadedChunk* _fullyLoadedChunk, ARRAY_POINTER _priorityPosition);
		void useOptimizedChunk(ARRAY_POINTER _priorityPosition);

		void convertToOptimizedChunk(ARRAY_POINTER _priorityPosition);
		void convertToFullyLoadedChunk(ARRAY_POINTER _priorityPosition, FullyLoadedChunk* data);

		// Only in case it is a optimizedChunk
		OptimizedChunk* optimizedChunk;
		// Only in case it is a fully loaded chunk
		FullyLoadedChunk* fullyLoadedChunk;

	private:

		// Only in case it is a Same Type Chunk (all ids are the same like air)
		VOXEL_TYPE chunkSameTypeID = 0;


	};
}

