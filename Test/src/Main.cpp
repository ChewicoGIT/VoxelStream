#include <iostream>
#include <cstdlib>
#include <stdlib.h>
#include <time.h>
#include <chrono>

#include "VoxelStream/DataStructures.h"
#include "VoxelStream/VoxelDatabase.h"

#define MAX_ITIRENATIONS 1000000

const VS::DatabaseOptions _dbOpt = {
	.chunkSizeX = 64,
	.chunkSizeY = 16,
	.chunkSizeZ = 64,
	.maxBlockID = 200,
	.fullyLoadedChunkBufferSize = 64 * 64 + 10,
	.modifiedVoxelPriorityValue = 10,
	.queryedVoxelPriorityValue = 2
};

VS::VoxelDatabase vd(_dbOpt);

int main() {



	time_t randomSeed = time(NULL);
	srand(5);

	auto start = std::chrono::high_resolution_clock::now();
	for (int x = 0; x < MAX_ITIRENATIONS; x++) {

		int randomX = rand() % (64 * 32);// + 30 * 32;
		int randomY = rand() % (16 * 32);// + 10 * 32;
		int randomZ = rand() % (64 * 32);// + 30 * 32;

		unsigned short randomVoxel = rand() % 2;
		unsigned short randomStat = rand() % 1;

		vd.SetVoxel(randomX, randomY, randomZ, VS::VoxelData() = {
			.blockID = randomVoxel,
			.blockState = randomStat
		});



	}
	auto end = std::chrono::high_resolution_clock::now();
	auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
	std::cout << "Finished writing \n";
	std::cout << "Time taken: " << duration.count() << " milliseconds" << std::endl;

	srand(5);

	int collisionOrError = 0;
	start = std::chrono::high_resolution_clock::now();
	for (int x = 0; x < MAX_ITIRENATIONS; x++) {

		int randomX = rand() % (64 * 32);
		int randomY = rand() % (16 * 32);
		int randomZ = rand() % (64 * 32);

		unsigned short randomVoxel = rand() % 200;
		unsigned short randomStat = rand() % 10;

		VS::VoxelData voxel = vd.GetVoxel(randomX, randomY, randomZ);

		if (voxel.blockID != randomVoxel || voxel.blockState != randomStat) {
			collisionOrError++;
		}
	}
	end = std::chrono::high_resolution_clock::now();
	duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
	std::cout << "Finished reading \n";
	std::cout << "Time taken: " << duration.count() << " milliseconds" << std::endl;

	std::cout << "Collision errors : " << collisionOrError;

}