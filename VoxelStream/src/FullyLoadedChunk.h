#pragma once
#include "InternalDefinitions.h"

namespace vs {

	class FullyLoadedChunk {
	public:
		VoxelData voxelData[VOXEL_X * VOXEL_Y * VOXEL_Z];

		FullyLoadedChunk();
		void load(OptimizedChunk* optimizedChunk);
		
	};

}