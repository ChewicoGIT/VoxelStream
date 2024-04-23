#include <iostream>
#include "VoxelStream/VoxelDatabase.h"

const VS::DatabaseOptions _dbOpt = {
	.chunkSizeX = 10,
	.chunkSizeY = 10,
	.chunkSizeZ = 10,
	.maxBlockID = 20,
	.fullyLoadedChunkBufferSize = 10 * 10,
	.modifiedVoxelPriorityValue = 10,
	.queryedVoxelPriorityValue = 1
};

VS::VoxelDatabase vd(_dbOpt);

int main() {

	while (true)
	{
		std::cout << vd.GetVoxel(10, 10, 10).blockID << "\n";
		getchar();
		vd.AddVoxel(10, 10, 10, VS::VoxelData{
			.blockID = 18,
			.blockState = 0
			});

		vd.debugData();
		_sleep(100);
	}

}