#include "FullyLoadedChunk.h"
#include "OptimizedChunk.h"


VS::FullyLoadedChunk::FullyLoadedChunk()
{
	for (int x = 0; x < VOXEL_X * VOXEL_Y * VOXEL_Z; x++)
		voxelData[x] = VoxelData();

}

void VS::FullyLoadedChunk::load(OptimizedChunk* optimizedChunk)
{
	int di = 0;
	int index = 0;
	for (const Node& _node : optimizedChunk->nodes) {

		//This function is very easy
		for (int x = 0; x < _node.repetition; x++) {
			voxelData[index] = _node.voxel;
			index++;
		}
		di++;
	}
}
