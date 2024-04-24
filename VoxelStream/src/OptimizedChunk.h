#pragma once
#include <vector>
#include "InternalDefinitions.h"

namespace VS {
	struct Node {
		VOXEL_TYPE voxel;
		unsigned short repetition;
	};

	class OptimizedChunk {

	public:
		OptimizedChunk();
		OptimizedChunk(FullyLoadedChunk* _fullyLoadedChunk);

		VOXEL_TYPE getVoxel(unsigned short position);
		void setVoxel(unsigned short position, VOXEL_TYPE _voxel);

		std::vector<Node> nodes;

	};

}