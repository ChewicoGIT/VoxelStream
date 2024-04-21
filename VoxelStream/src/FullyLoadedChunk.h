#pragma once
#include "OptimizedChunk.h"
#include "InternalDefinitions.h"

namespace VS {

	struct FullyLoadedChunk {

		bool InUse = false;
		VOXEL_TYPE voxelData[VOXEL_X * VOXEL_Y * VOXEL_Z];
		
	};

}