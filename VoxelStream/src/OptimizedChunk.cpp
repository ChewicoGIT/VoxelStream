#include "OptimizedChunk.h"
#include "FullyLoadedChunk.h"

VS::VoxelData VS::OptimizedChunk::getVoxel(ARRAY_POINTER position)
{
	int currentID = 0;

	for (const Node& _node : nodes) {
		if (position >= currentID && position < currentID + _node.repetition)
			return _node.voxel;

		currentID += _node.repetition;

	}

	return VoxelData();
}

VS::OptimizedChunk::OptimizedChunk()
{
	nodes.push_back(Node() = { .voxel = 0, .repetition = VOXEL_X * VOXEL_Y * VOXEL_Z });
}

VS::OptimizedChunk::OptimizedChunk(FullyLoadedChunk& _fullyLoadedChunk)
{

	Node workingNode = {
		.voxel = _fullyLoadedChunk.voxelData[0],
		.repetition = 1 };

	nodes = std::vector<Node>();

	for (int x = 1; x < VOXEL_X * VOXEL_Y * VOXEL_Z; x++) {
		if (_fullyLoadedChunk.voxelData[x] == workingNode.voxel) {
			workingNode.repetition++;
			continue;
		}

		nodes.push_back(workingNode);

		workingNode.voxel = _fullyLoadedChunk.voxelData[x];
		workingNode.repetition = 1;

	}
	nodes.push_back(workingNode);

}

VS::OptimizedChunk::OptimizedChunk(OptimizedChunk& _optimizedChunk)
{

	Node workingNode = {
		.voxel = _optimizedChunk.nodes[0].voxel,
		.repetition = _optimizedChunk.nodes[0].repetition};


	nodes = std::vector<Node>();

	for (int x = 1; x < _optimizedChunk.nodes.size(); x++) {

		if (_optimizedChunk.nodes[x].repetition == 0)
			continue;

		if (_optimizedChunk.nodes[x].voxel == workingNode.voxel) {
			workingNode.repetition += _optimizedChunk.nodes[x].repetition;
			continue;
		}

		nodes.push_back(workingNode);

		workingNode.voxel = _optimizedChunk.nodes[x].voxel;
		workingNode.repetition = _optimizedChunk.nodes[x].repetition;

	}

	nodes.push_back(workingNode);

}

void VS::OptimizedChunk::setVoxel(unsigned short position, VoxelData _voxel)
{

	int listID = 0;
	int currentID = 0;

	for (Node& _node : nodes) {


		if (position >= currentID + _node.repetition || position < currentID || _node.repetition == 0) {
			currentID += _node.repetition;
			listID++;
			continue;
		}

		if (_node.voxel == _voxel)
			return;

		if (_node.repetition == 1) {
			_node.voxel = _voxel;
			return;
		}


		// If the position of the voxel is at the start of the node ...
		// If the position of the node is not at the start of the array
		if (position == currentID && listID != 0) {
			// If the node at the back has 0 repetition it is free to use
			if (nodes[listID - 1].repetition == 0) {
				_node.repetition--;
				nodes[listID - 1].repetition = 1;
				nodes[listID - 1].voxel = _voxel;

				return;
			}

			// If the node at the back is the same voxel type then we can increase it and decrease us
			if (nodes[listID - 1].voxel == _voxel) {
				_node.repetition--;
				nodes[listID - 1].repetition++;

				return;
			}
		}

		// If the position of the voxel is at the end of the node ...
		// If the position of the node is not at the end of the array
		if (position == currentID + _node.repetition - 1 && listID != nodes.size() - 1) {
			// If the node at the front has 0 repetition it is free to use
			if (nodes[listID + 1].repetition == 0) {
				_node.repetition--;
				nodes[listID + 1].repetition = 1;
				nodes[listID + 1].voxel = _voxel;

				return;
			}

			// If the node at the back is the same voxel type then we can increase it and decrease us
			if (nodes[listID + 1].voxel == _voxel) {
				_node.repetition--;
				nodes[listID + 1].repetition++;

				return;
			}
		}

		// If it is at the start of the array we can add one at the start
		if (listID == 0 && position == currentID) {
			_node.repetition--;
			nodes.insert(nodes.begin() ,
				Node() = { .voxel = _voxel, .repetition = 1 });

			return;
		}


		// If it is at the start of the array we can add one at the start
		if (listID == nodes.size() - 1 && position == currentID + _node.repetition - 1) {
			_node.repetition--;
			nodes.push_back(Node() = { .voxel = _voxel, .repetition = 1 });

			return;
		}

		// If the repetition is 2 we can split it into 2
		if (_node.repetition == 2) {
			_node.repetition--;
			nodes.insert(nodes.begin() + listID + ((currentID == position) ? 0 : 1),
				Node() = { .voxel = _voxel, .repetition = 1 });

			return;
		}

 		// In this case we have to split the node in 3
 		// The relative position tell us the offset between 
 		// the voxel position to the start of the node
 		short relativePosition = position - currentID;
 		unsigned short _startNodeSize = relativePosition;
 		unsigned short _endNodeSize = _node.repetition - relativePosition - 1;

		// Ok so this line cost me 3h of debuggin because instead of caching and then asign .voxel = _tempVoxel
		// i used Node{ .voxel = _node.voxel, .repetition = _endNodeSize } witch is incorrect because at the moment
		// you insert a node in the vector it invalidates any reference to the itirenator so giving random numbers.
		VoxelData _tempVoxel = _node.voxel;
 
 		_node.repetition = relativePosition;
 		nodes.insert(nodes.begin() + listID + 1, Node{ .voxel = _voxel, .repetition = 1 });
 		nodes.insert(nodes.begin() + listID + 2, Node{ .voxel = _tempVoxel, .repetition = _endNodeSize });

 		return;
	}



}
