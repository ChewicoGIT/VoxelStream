#pragma once
#include "InternalDefinitions.h"
#include <vector>

namespace VS {
	class Chunk
	{
	public:
		Chunk() = default;
		Chunk(Chunk& _chnk) = delete;
		Chunk(BIG_ARRAY_POINTER _priorityPosition, OptimizedChunk* _optimizedChunk);
		~Chunk();

		VOXEL_TYPE getVoxel(unsigned short voxelID);
		void modifyVoxel(unsigned short voxelID, VOXEL_TYPE _voxel);

		ChunkSaveState saveState = ChunkSaveState::None;
		// We don't know which array it is, only if we search for the save state
		BIG_ARRAY_POINTER priorityPosition;

		void initAsOptimizedChunk(BIG_ARRAY_POINTER _priorityPosition, OptimizedChunk* _optimizedChunk);

		void useFullyLoadedChunk(FullyLoadedChunk* _fullyLoadedChunk, BIG_ARRAY_POINTER _priorityPosition);
		void useOptimizedChunk(BIG_ARRAY_POINTER _priorityPosition);

		void convertToOptimizedChunk(BIG_ARRAY_POINTER _priorityPosition);
		void convertToFullyLoadedChunk(BIG_ARRAY_POINTER _priorityPosition, FullyLoadedChunk* data);

		// Only in case it is a optimizedChunk
		OptimizedChunk* optimizedChunk;
		// Only in case it is a fully loaded chunk
		FullyLoadedChunk* fullyLoadedChunk;

	private:

		// Only in case it is a Same Type Chunk (all ids are the same like air)
		VOXEL_TYPE chunkSameTypeID = 0;


	};
}

