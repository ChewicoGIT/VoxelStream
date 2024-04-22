#pragma once
#include <vector>
#include "VoxelStream/DataStructures.h"
#include "InternalDefinitions.h"

namespace VS {

	// This is the data to keep track the voxels with this data
	struct VoxelPaletteData {
		VoxelData voxelData;
	};

	class VoxelMemoryPaletteManager
	{
	public:
		VoxelMemoryPaletteManager(DatabaseOptions dbOpt);
		VOXEL_TYPE getVoxelID(VoxelData voxel);
		VoxelData& getVoxelData(VOXEL_TYPE voxelID);

	private:

		DatabaseOptions dbOpt;
		std::vector<VoxelPaletteData> paletteData;
		// At the end a voxel type is a direction to the palette
		std::vector<VOXEL_TYPE>* dataPalettePointer;
		
	};

}
