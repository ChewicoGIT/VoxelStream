#pragma once
#include "Common.h"

void randomAccesBenchMark() {

	time_t randomSeed = time(NULL);
	for (int x = 0; x < 5; x++) {

		srand(5);
		auto start = std::chrono::high_resolution_clock::now();
		for (int x = 0; x < MAX_ITIRENATIONS; x++) {

			int randomX = rand() % (64 * 32);
			int randomY = rand() % (3 * 32);
			int randomZ = rand() % (64 * 32);

			unsigned short randomVoxel = rand() % 200;
			unsigned short randomStat = rand() % 10;

			vd.SetVoxel(randomX, randomY, randomZ, VS::VoxelData() = {
				.blockID = randomVoxel,
				.blockState = randomStat
				});



		}
		auto end = std::chrono::high_resolution_clock::now();
		auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
		std::cout << "Finished writing \n";
		std::cout << "Transformations: " << vd.getTransformations() << "\n";
		std::cout << "Time taken: " << duration.count() << " milliseconds" << std::endl;

	}

	srand(5);

	int collisionOrError = 0;
	auto _start = std::chrono::high_resolution_clock::now();
	for (int x = 0; x < MAX_ITIRENATIONS; x++) {

		int randomX = rand() % (5 * 32);
		int randomY = rand() % (5 * 32);
		int randomZ = rand() % (5 * 32);

		unsigned short randomVoxel = rand() % 200;
		unsigned short randomStat = rand() % 10;

		VS::VoxelData voxel = vd.GetVoxel(randomX, randomY, randomZ);

		if (voxel.blockID != randomVoxel || voxel.blockState != randomStat) {
			collisionOrError++;
		}
	}
	auto _end = std::chrono::high_resolution_clock::now();
	auto _duration = std::chrono::duration_cast<std::chrono::milliseconds>(_end - _start);
	std::cout << "Finished reading \n";
	std::cout << "Time taken: " << _duration.count() << " milliseconds" << std::endl;

	std::cout << "Collision errors : " << collisionOrError;

}