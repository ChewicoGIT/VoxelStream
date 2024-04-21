#pragma once
#include "InternalDefinitions.h"

namespace VS {
	struct Node {
		VOXEL_TYPE voxelID;
		unsigned char repetition;
	};

	struct OptimizedChunk {
		int nodeAmount = 0;
		Node nodes[];
	};

}