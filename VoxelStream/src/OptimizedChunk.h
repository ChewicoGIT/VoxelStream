#pragma once
#include <vector>
#include "InternalDefinitions.h"

namespace VS {
	struct Node {
		VoxelData voxel;
		unsigned short repetition;
	};

	class OptimizedChunk {
	public:
		std::vector<Node> nodes;

		OptimizedChunk();
		OptimizedChunk(FullyLoadedChunk& _fullyLoadedChunk);
		OptimizedChunk(OptimizedChunk& _optimizedChunk);

		VoxelData getVoxel(ARRAY_POINTER position);
		void setVoxel(unsigned short position, VoxelData _voxel);
	};

}