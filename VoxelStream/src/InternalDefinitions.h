#pragma once

#define VOXEL_X 32
#define VOXEL_Y 32
#define VOXEL_Z 32

#define VOXEL_TYPE unsigned short
#define ARRAY_POINTER unsigned short

#define VOXEL_ID(x, y, z) x + y * VOXEL_X + z * VOXEL_X * VOXELY 

namespace VS {
	class Chunk;
	class FullyLoadedChunk;

	enum class ChunkSaveState
	{
		SameType,
		FullyLoadedChunk,
		CompressedChunk
	}; 


	struct FullyLoadedChunkLink {
		//In case it is not assigned the _chunk will be nullptr
		Chunk* _chunk;
		FullyLoadedChunk* _chunkData;
	};

}