#include "Common.h"


const VS::DatabaseOptions _dbOpt = {
	.chunkSizeX = 64,
	.chunkSizeY = 16,
	.chunkSizeZ = 64,
	.maxBlockID = 200,
	.fullyLoadedChunkBufferSize = 64 * 64 + 10,
	.modifiedVoxelPriorityValue = 34,
	.queryedVoxelPriorityValue = 10
};

VS::VoxelDatabase vd(_dbOpt);

int main() {
	randomAccesBenchMark();
	//weightedBenchmark();

}