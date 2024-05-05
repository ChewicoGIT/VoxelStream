#include "Mode.h"
#include "Common.h"

const vs::DatabaseOptions _dbOpt = {
	.chunkSizeX = 64,
	.chunkSizeY = 16,
	.chunkSizeZ = 64,
	.fullyLoadedChunkBufferSize = 64 * 1 * 64 * 1 + 20,
	.modifiedVoxelPriorityValue = 100,
	.queryedVoxelPriorityValue = 10
};

vs::VoxelDatabase* vd;

int main() {

	saveExample();
	
	loadExample();
	 
	//randomAccesBenchMark();
	//weightedBenchmark();

}