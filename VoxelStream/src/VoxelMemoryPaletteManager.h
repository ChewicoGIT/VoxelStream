#pragma once
#include <vector>
#include "VoxelStream/DataStructures.h"
#include "InternalDefinitions.h"

namespace VS {

	class VoxelMemoryPaletteManager
	{
	public:
		VoxelMemoryPaletteManager(DatabaseOptions dbOpt);
		VOXEL_TYPE getVoxelID(VoxelData voxel);
		VoxelData& getVoxelData(VOXEL_TYPE voxelID);

		const DatabaseOptions dbOpt;
		std::vector<VoxelData> paletteData;
		// At the end a voxel type is a direction to the palette
		std::vector<VOXEL_TYPE>* dataPalettePointer;
		
	};

}
