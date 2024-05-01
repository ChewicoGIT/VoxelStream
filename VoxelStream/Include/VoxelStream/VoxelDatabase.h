#pragma once
#include "DataStructures.h"

namespace VS {

	class VoxelDatabase
	{
	public:
		VoxelDatabase(DatabaseOptions opt);
		VoxelDatabase(const char* fileLoaction);
		// To avoid errors, delete copy constructor
		VoxelDatabase(const VoxelDatabase& _copy) = delete;
		~VoxelDatabase();

		void SetVoxel(unsigned int x, unsigned int y, unsigned int z, VoxelData voxelData);
		/// Gets the voxel data of a specific space
		/// Be careful because it does not have bound check limit and could cause error
		VoxelData GetVoxel(unsigned int x, unsigned int y, unsigned int z);

		void saveData(const char* fileLocation);

		int getTransformations();
		int objectWasFullyLoaded();

	private:
		DatabaseOptions dbOpt;
		ChunkMemoryManager* chunkMemory;
		int _objectWasFullyLoaded = 0;

	};

}

