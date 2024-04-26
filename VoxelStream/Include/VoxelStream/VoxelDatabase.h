#pragma once
#include "DataStructures.h"

namespace VS {

	class VoxelDatabase
	{
	public:
		VoxelDatabase(DatabaseOptions opt);
		~VoxelDatabase();

		void SetVoxel(unsigned int x, unsigned int y, unsigned int z, VoxelData voxelData);
		/// Gets the voxel data of a specific space
		/// Be careful because it does not have bound check limit and could cause error
		VoxelData GetVoxel(unsigned int x, unsigned int y, unsigned int z);

		int getTransformations();

	private:
		DatabaseOptions dbOpt;
		ChunkMemoryManager* chunkMemory;
		VoxelMemoryPaletteManager* voxelPalette;


	};

}

