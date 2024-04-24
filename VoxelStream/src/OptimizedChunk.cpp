#include "OptimizedChunk.h"
#include "FullyLoadedChunk.h"
#include <iostream>

VOXEL_TYPE VS::OptimizedChunk::getVoxel(unsigned short position)
{
	int currentID = 0;

	for (const Node& _node : nodes) {
		if (position >= currentID && position < currentID + _node.repetition)
			return _node.voxel;

		currentID += _node.repetition;

	}

	return 0;
}

VS::OptimizedChunk::OptimizedChunk()
{
	nodes.push_back(Node() = { .voxel = 0, .repetition = VOXEL_X * VOXEL_Y * VOXEL_Z });
}

VS::OptimizedChunk::OptimizedChunk(FullyLoadedChunk* _fullyLoadedChunk)
{

	Node workingNode = {
		.voxel = _fullyLoadedChunk->voxelData[0],
		.repetition = 1
	};

	nodes = std::vector<Node>(256);

	for (int x = 1; x < VOXEL_X * VOXEL_Y * VOXEL_Z; x++) {
		if (_fullyLoadedChunk->voxelData[x] == workingNode.voxel)
			workingNode.repetition++;

		nodes.push_back(workingNode);
		workingNode.voxel = _fullyLoadedChunk->voxelData[x];
		workingNode.repetition = 1;

	}
	nodes.push_back(workingNode);

}

void VS::OptimizedChunk::setVoxel(unsigned short position, VOXEL_TYPE _voxel)
{

	int listID = 0;
	int currentID = 0;
	for (Node& _node : nodes) {
		if (position >= currentID + _node.repetition || position < currentID || _node.repetition == 0) {
			currentID += _node.repetition;
			listID++;
			continue;
		}

		if (nodes.size() > 300) {
			listID++;
		}

		// If it is the same type we do nothing
		if (_node.voxel == _voxel)
			return;

		// If the current node is a single node we can safely swap the id to change the node
		if (_node.repetition == 1) {

			_node.voxel = _voxel;
			return;
		}

		// If the position of the voxel is at the start of the node ...
		if (position == currentID) {
			// If the position of the node is at the start of the array
			if (listID != 0) {
				// If the node at the back is the same voxel type then we can increase it and decrease us
				if (nodes[listID - 1].voxel == _voxel) {
					nodes[listID - 1].repetition++;
					_node.repetition--;
					return;
				}
			}
			else {
				// We can safely add a node to the start
				nodes.insert(nodes.begin(), Node() = { .voxel = _voxel, .repetition = 1 });
				return;
			}

		}

		// If the position of the voxel is at the end of the node ...
		if (position == currentID + _node.repetition) {
			// If the position of the node is at the end of the array
			if (listID != nodes.size() - 1) {
				// If the front node is the same voxel type then we can increase it and decrease us
				if (nodes[listID + 1].voxel == _voxel) {
					nodes[listID + 1].repetition++;
					_node.repetition--;
					return;
				}
			}
			else {
				// We can safely add a node to the end
				nodes.push_back(Node() = { .voxel = _voxel, .repetition = 1 });
				return;
			}

		}

		// In this case the node is 2 repetitions we can split it in 2
		if (_node.repetition == 2) {
			if (currentID == position){
				
				nodes.insert(nodes.begin() + listID, Node() = { .voxel = _voxel, .repetition = 1 });
				_node.repetition--;
			}
			else {
				if(position == _node.repetition + currentID)
					nodes.push_back(Node() = { .voxel = _voxel, .repetition = 1 });
				else
					nodes.insert(nodes.begin() + listID + 1, Node() = { .voxel = _voxel, .repetition = 1 });
				_node.repetition--;
			}
			return;
		}

		// In this case we have to split the node in 3
		// The relative position tell us the offset between 
		// the voxel position to the start of the node
		short relativePosition = position - currentID;
		unsigned short _startNodeSize = relativePosition;
		unsigned short _endNodeSize = _node.repetition - relativePosition - 1;

		_node.repetition = relativePosition;
		nodes.insert(nodes.begin() + listID + 1, Node() = { .voxel = _voxel, .repetition = 1 });
		nodes.insert(nodes.begin() + listID + 2, Node() = { .voxel = _node.voxel, .repetition = _endNodeSize });

		return;
	}



}
