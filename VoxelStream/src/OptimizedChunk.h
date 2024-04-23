#pragma once
#include <vector>
#include "InternalDefinitions.h"

namespace VS {
	struct Node {
		VOXEL_TYPE voxel;
		unsigned short repetition;
	};

	class OptimizedChunk {
	private:
		std::vector<Node> nodes;


	public:
		OptimizedChunk();

		VOXEL_TYPE getVoxel(unsigned char position);
		void setVoxel(unsigned char position, VOXEL_TYPE _voxel);

	};

}