#pragma once

namespace VS {
	
	class Chunk;
	class MemoryManager;

	struct FullyLoadedChunk;
	//This is a lik between a chunk and its fully loaded chunk data
	struct FullyLoadedChunkLink;

	struct DatabaseOptions {
		
		/// Chunk sizes defines the amount of voxels
		/// in each direction. Keep in mind that a chunk
		/// has 32 voxels in each direction so for example
		/// a database 64 voxels in x direction will
		/// need 2 in chunk size x.
		unsigned int chunkSizeX = 10;
		unsigned int chunkSizeY = 5;
		unsigned int chunkSizeZ = 10;
		
		/// This parameter tell the buffer size of voxel raw Data.
		/// This data is stored raw so consumes more memory
		unsigned int fullyLoadedChunkBufferSize = 100;


	};



}
