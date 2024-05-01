#pragma once
#include "InternalDefinitions.h"
#include <vector>

namespace VS {
	class Chunk
	{
	public:
		ChunkSaveState saveState = ChunkSaveState::None;
		// This is the priority position depending on the save state
		ARRAY_POINTER priorityPosition;

		// Only in case it is a optimizedChunk
		OptimizedChunk* optimizedChunk;
		// Only in case it is a fully loaded chunk
		FullyLoadedChunk* fullyLoadedChunk;

		Chunk() = default;
		Chunk(Chunk& _chnk) = delete;
		~Chunk();

		VoxelData getVoxel(unsigned short voxelID);
		void setVoxel(unsigned short voxelID, VoxelData _voxel);

		void initFullyLoadedChunk(ARRAY_POINTER _priorityPosition, FullyLoadedChunk* _fullyLoadedChunk);
		void initFullyLoadedChunk(ARRAY_POINTER _priorityPosition, FullyLoadedChunk* _fullyLoadedChunk, OptimizedChunk* _optimizedChunk);
		void initOptimizedChunk(ARRAY_POINTER _priorityPosition, OptimizedChunk* _optimizedChunk);
		void initOptimizedChunk(ARRAY_POINTER _priorityPosition);

		void convertToOptimizedChunk(ARRAY_POINTER _priorityPosition);
		void convertToFullyLoadedChunk(ARRAY_POINTER _priorityPosition, FullyLoadedChunk* data);

	};
}

