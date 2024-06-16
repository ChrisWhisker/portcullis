// Fill out your copyright notice in the Description page of Project Settings.


#include "BSPGenerator.h"

#include <iostream>
#include <random>

BSPGenerator::BSPGenerator()
{
}

BSPGenerator::~BSPGenerator()
{
}

void BSPGenerator::Generate()
{
	std::srand(static_cast<unsigned int>(std::time(nullptr)));

	// Define the initial space
	Rect initialSpace = {0, 0, 100, 100};
	std::vector<Rect> spaces = {initialSpace}; // Use initializer list to initialize the vector

	// Recursively divide the space to a certain depth
	int maxDepth = 4;
	divideSpace(spaces, 0, maxDepth);

	// Output the resulting spaces
	for (const auto& space : spaces)
	{
		UE_LOG(LogTemp, Log, TEXT("Rect: x=%d, y=%d, width=%d, height=%d"), space.x, space.y, space.width, space.height);
		std::cout << "Rect: x=" << space.x << ", y=" << space.y
			<< ", width=" << space.width << ", height=" << space.height << std::endl;
	}
}

void BSPGenerator::divideSpace(std::vector<Rect>& spaces, int depth, int maxDepth)
{
	if (depth >= maxDepth) return;

	std::vector<Rect> newSpaces;
	for (const auto& space : spaces) {
		bool divideHorizontally = static_cast<bool>(std::rand() % 2);

		if (divideHorizontally) {
			int split = 1; // Default split value to avoid division by zero
			if (space.height > 1) {
				split = space.y + std::rand() % (space.height - 1) + 1;
			}
			Rect top = {space.x, space.y, space.width, split - space.y};
			Rect bottom = {space.x, split, space.width, space.y + space.height - split};
			newSpaces.push_back(top);
			newSpaces.push_back(bottom);
		} else {
			int split = 1; // Default split value to avoid division by zero
			if (space.width > 1) {
				split = space.x + std::rand() % (space.width - 1) + 1;
			}
			Rect left = {space.x, space.y, split - space.x, space.height};
			Rect right = {split, space.y, space.x + space.width - split, space.height};
			newSpaces.push_back(left);
			newSpaces.push_back(right);
		}
	}
	spaces = newSpaces;

	divideSpace(spaces, depth + 1, maxDepth);

}
