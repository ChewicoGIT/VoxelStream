#pragma once
#include "Common.h"

// ------------------------------------------- WORKING... -------------------------------------------

// You will have 5 category with each one a weight
int categoryWeight[5] = {
	16, 8, 4, 2, 1
};

int sumCategories = 1 + 2 + 4 + 8 + 16; // this would be = 2 ^ Category number - 1

int getRandomCategory() {
	int randomNumber = rand() % (sumCategories + 1);
	int _catSum = 0;

	for (int x = 0; x < 5; x++) {
		_catSum += categoryWeight[x];
		if (randomNumber < _catSum)
			return x;
	}

	return 4;
}

void weightedBenchmark() {


	// Set the random seed
	srand(5);
	int ap[5] = { 0, 1, 2, 3, 4 };
	for (int x = 0; x < MAX_ITIRENATIONS; x++) {

		ap[getRandomCategory()]++;

		int minChunk = 0;
		int maxChunk = 0;

		int randomX = rand() % (64 * 32);
		int randomY = rand() % (16 * 32);
		int randomZ = rand() % (64 * 32);
		
		unsigned short randomVoxel = rand() % 200;
		unsigned short randomStat = rand() % 10;
		
		vd.SetVoxel(randomX, randomY, randomZ, VS::VoxelData() = {
			.blockID = randomVoxel,
			.blockState = randomStat
			});



	}
	std::cout << "0: " << ap[0] << "\n";
	std::cout << "1: " << ap[1] << "\n";
	std::cout << "2: " << ap[2] << "\n";
	std::cout << "3: " << ap[3] << "\n";
	std::cout << "4: " << ap[4] << "\n";


}