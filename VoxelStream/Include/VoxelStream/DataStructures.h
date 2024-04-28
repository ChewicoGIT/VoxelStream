#pragma once

namespace VS {
	
	class ChunkMemoryManager;
	class VoxelMemoryPaletteManager;

	struct DatabaseOptions {
		
		/// Chunk sizes defines the amount of voxels
		/// in each direction. Keep in mind that a chunk
		/// has 32 voxels in each direction so for example
		/// a database 64 voxels in x direction will
		/// need 2 in chunk size x.
		unsigned int chunkSizeX = 20;
		unsigned int chunkSizeY = 5;
		unsigned int chunkSizeZ = 20;

		/// This is a very important value and it specifies
		/// the max id of a block id, if you try to acces a 
		/// block id bigger than this it will give an error
		unsigned int maxBlockID = 2;

		/// This parameter tell the buffer size of voxel raw Data.
		/// This data is stored raw so consumes more memory
		/// A convenient value would be the chunkSizeX * chunkSizeZ
		unsigned int fullyLoadedChunkBufferSize = 20 * 20;

		/// This is the amount of numbers that a chunk fowards
		/// when a voxel is modified
		unsigned int modifiedVoxelPriorityValue = 10;
		/// This is the amount of numbers that a chunk fowards
		/// when a voxel is obtained
		unsigned int queryedVoxelPriorityValue = 1;

	};

	/// This is the data that is in a voxel, to change the data
	/// that can be store you have to build the library
	struct VoxelData {
		unsigned short blockID;
		unsigned short blockState;

		inline bool operator == (const VoxelData& _b) const {
			return _b.blockID == this->blockID &&
				_b.blockState == this->blockState;
		}

	};
}
