#pragma once
#include "VoxelStream/DataStructures.h"

#define VOXEL_X 32
#define VOXEL_Y 32
#define VOXEL_Z 32

#define ARRAY_POINTER unsigned short

#define VOXEL_ID(x, y, z) x + y * VOXEL_X + z * VOXEL_X * VOXEL_Y 

namespace VS {
	class Chunk;
	class FullyLoadedChunk;
	class OptimizedChunk;
	
	enum class ChunkSaveState
	{
		None,
		FullyLoadedChunk,
		OptimizedChunk
	}; 

	//This is a lik between a chunk and its fully loaded chunk data
	struct FullyLoadedChunkLink {
		//In case it is not assigned the _chunk will be nullptr
		Chunk* _chunk = nullptr;
		FullyLoadedChunk* _chunkData;
	};

}