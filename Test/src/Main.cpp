#include <iostream>
#include "VoxelStream/VoxelDatabase.h"

const VS::DatabaseOptions _dbOpt = {
	.chunkSizeX = 5,
	.chunkSizeY = 5,
	.chunkSizeZ = 5,
	.maxBlockID = 300,
	.fullyLoadedChunkBufferSize = 5 * 5 + 1,
	.modifiedVoxelPriorityValue = 5,
	.queryedVoxelPriorityValue = 1
};

VS::VoxelDatabase vd(_dbOpt);

int main() {

	while (true)
	{
		std::cout << vd.GetVoxel(10, 10, 10).blockID << "\n";
		for(int x = 0; x < 1; x++)
			vd.AddVoxel(3 * 32 + 1, 3 * 32 + 1, 3 * 32 + 1, VS::VoxelData{
				.blockID = 18,
				.blockState = 0
				});

		vd.debugData();
		_sleep(100);
		getchar();

	}

}