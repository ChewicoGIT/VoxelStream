#pragma once
#include <vector>
#include "InternalDefinitions.h"

namespace VS {
	struct Node {
		VOXEL_TYPE voxelID;
		unsigned short repetition;
	};

	struct OptimizedChunk {
		std::vector<Node> nodes;

		// Get the size in bytes ocupied for debug
		unsigned int getSize() {
			return nodes.max_size() * sizeof(Node);
		}

		VOXEL_TYPE getVoxel(unsigned char position) {
			int currentID = 0;

			for (const Node& _node : nodes) {
				if (position >= currentID && position < currentID + _node.repetition)
					return _node.voxelID;

				currentID += _node.repetition;

			}

			return 0;

		}
	};

}