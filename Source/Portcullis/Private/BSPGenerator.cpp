// Fill out your copyright notice in the Description page of Project Settings.


#include "BSPGenerator.h"

void BSPGenerator::Generate()
{
	std::srand(static_cast<unsigned int>(std::time(nullptr)));

	// Define the initial space
	constexpr FRect InitialSpace = {0, 0, 100, 100};
	std::vector<FRect> Spaces = {InitialSpace}; // Use initializer list to initialize the vector

	// Recursively divide the space to a certain depth
	constexpr int MaxDepth = 4;
	DivideSpace(Spaces, 0, MaxDepth);

	// Output the resulting spaces
	for (const auto& [X, Y, Width, Height] : Spaces)
	{
		UE_LOG(LogTemp, Log, TEXT("FRect: x=%d, y=%d, width=%d, height=%d"), X, Y, Width,Height);
	}
}

void BSPGenerator::DivideSpace(std::vector<FRect>& Spaces, int Depth, int MaxDepth)
{
	if (Depth >= MaxDepth) return;

	std::vector<FRect> NewSpaces;
	for (const auto& [x, y, width, height] : Spaces)
	{
		if (static_cast<bool>(std::rand() % 2)) // Divide horizontally
		{
			int Split = 1; // Default split value to avoid division by zero
			if (height > 1)
			{
				Split = y + std::rand() % (height - 1) + 1;
			}
			FRect Top = {x, y, width, Split - y};
			FRect Bottom = {x, Split, width, y + height - Split};
			NewSpaces.push_back(Top);
			NewSpaces.push_back(Bottom);
		}
		else
		{
			int Split = 1; // Default split value to avoid division by zero
			if (width > 1)
			{
				Split = x + std::rand() % (width - 1) + 1;
			}
			FRect Left = {x, y, Split - x, height};
			FRect Right = {Split, y, x + width - Split, height};
			NewSpaces.push_back(Left);
			NewSpaces.push_back(Right);
		}
	}
	Spaces = NewSpaces;

	DivideSpace(Spaces, Depth + 1, MaxDepth);
}
