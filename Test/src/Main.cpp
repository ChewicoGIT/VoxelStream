#include "Mode.h"
#include "Common.h"

const VS::DatabaseOptions _dbOpt = {
	.chunkSizeX = 64,
	.chunkSizeY = 16,
	.chunkSizeZ = 64,
	.maxBlockID = 300,
	.fullyLoadedChunkBufferSize = 64 * 1 * 64 * 1 + 20,
	.modifiedVoxelPriorityValue = 64,
	.queryedVoxelPriorityValue = 2
};

VS::VoxelDatabase* vd;

int main() {

	saveExample();
	loadExample();
	 
	//randomAccesBenchMark();
	//weightedBenchmark();

}