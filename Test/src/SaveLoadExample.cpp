#include "Common.h"

void saveExample() {

	vd = new VS::VoxelDatabase(_dbOpt);

	time_t randomSeed = time(NULL);

	srand(5);

	auto start = std::chrono::high_resolution_clock::now();
	for (int x = 0; x < MAX_ITIRENATIONS; x++) {

		int randomX = rand() % (64 * 1 * 32);
		int randomY = rand() % (16 * 1 * 32);
		int randomZ = rand() % (64 * 1 * 32);

		unsigned short randomVoxel = rand() % 200;
		unsigned short randomStat = rand() % 200;

		vd->SetVoxel(randomX, randomY, randomZ, VS::VoxelData() = {
			.blockID = randomVoxel,
			.blockState = randomStat
			});



	}
	auto end = std::chrono::high_resolution_clock::now();
	auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
	std::cout << "Finished writing \n";
	std::cout << "Transformations: " << vd->getTransformations() << "\n";
	std::cout << "Time taken: " << duration.count() << " milliseconds" << "\n";
	std::cout << "\n";

	vd->saveData("voxels.bin");
	std::cout << "Saved in voxels.bin" << "\n";
	delete vd;
	getchar();

}


#include "Common.h"

void loadExample() {


	vd = new VS::VoxelDatabase("voxels.bin");

	srand(5);

	int collisionOrError = 0;
	auto _start = std::chrono::high_resolution_clock::now();
	for (int x = 0; x < MAX_ITIRENATIONS; x++) {

		int randomX = rand() % (64 * 1 * 32);
		int randomY = rand() % (16 * 1 * 32);
		int randomZ = rand() % (64 * 1 * 32);

		unsigned short randomVoxel = rand() % 200;
		unsigned short randomStat = rand() % 200;

		VS::VoxelData voxel = vd->GetVoxel(randomX, randomY, randomZ);

		if (voxel.blockID != randomVoxel || voxel.blockState != randomStat) {
			collisionOrError++;
		}
	}

	auto _end = std::chrono::high_resolution_clock::now();
	auto _duration = std::chrono::duration_cast<std::chrono::milliseconds>(_end - _start);
	std::cout << "Finished reading \n";
	std::cout << "Time taken: " << _duration.count() << " milliseconds" << std::endl;

	std::cout << "Collision errors : " << collisionOrError;
	delete vd;
	getchar();

}