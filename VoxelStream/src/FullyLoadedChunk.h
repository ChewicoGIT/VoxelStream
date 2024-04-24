#pragma once
#include "InternalDefinitions.h"

namespace VS {

	class FullyLoadedChunk {
	
	public:
		VOXEL_TYPE voxelData[VOXEL_X * VOXEL_Y * VOXEL_Z];
		FullyLoadedChunk();

		void load(OptimizedChunk* optimizedChunk);
		
		
	};

}