#include "BSPGenerator.h"
#include <cstdlib>
#include <ctime>
#include "DrawDebugHelpers.h"
#include "GeneratorSettings.h"

void FBSPGenerator::Generate(const FGeneratorSettings Settings)
{
	if (!World)
	{
		UE_LOG(LogTemp, Error, TEXT("No valid world context set for BSPGenerator. Call Initialize() first."));
		return;
	}

	std::srand(static_cast<unsigned int>(std::time(nullptr)));

	// Define the initial space
	const FRect InitialSpace = {0, 0, 10000, 10000};
	std::vector<FRect> SubSpaces = {InitialSpace}; // Use initializer list to initialize the vector

	// Recursively divide the space to a certain depth
	FlushPersistentDebugLines(World);
	constexpr int MaxDepth = 4;
	DivideSpace(SubSpaces, 0, MaxDepth);

	if (Settings.bDrawPartitions)
	{
		for (const auto& SubSpace : SubSpaces)
		{
			SubSpace.Draw(World, FColor::Silver);
		}
	}

	// Create rooms in the divided spaces
	CreateRooms(SubSpaces, Settings);
}

void FBSPGenerator::DivideSpace(std::vector<FRect>& SubSpaces, const int CurrentDepth, const int MaxDepth)
{
	if (CurrentDepth >= MaxDepth) return;

	std::vector<FRect> NewSubSpaces;

	for (const auto& SubSpace : SubSpaces)
	{
		constexpr float MinSize = 300.f;
		if (static_cast<bool>(std::rand() % 2)) // Divide horizontally
		{
			if (SubSpace.Height > MinSize * 2) // Skip division if resulting spaces would be too small
			{
				const int IntHeight = static_cast<int>(SubSpace.Height - MinSize * 2);
				const float SplitLine = SubSpace.Y + MinSize + std::rand() % IntHeight;
				NewSubSpaces.push_back({SubSpace.X, SubSpace.Y, SubSpace.Width, SplitLine - SubSpace.Y});
				NewSubSpaces.push_back(
					{SubSpace.X, SplitLine, SubSpace.Width, SubSpace.Y + SubSpace.Height - SplitLine});
			}
			else
			{
				NewSubSpaces.push_back(SubSpace);
			}
		}
		else // Divide vertically
		{
			if (SubSpace.Width > MinSize * 2) // Skip division if resulting spaces would be too small
			{
				const int IntWidth = static_cast<int>(SubSpace.Width - MinSize * 2);
				const float SplitLine = SubSpace.X + MinSize + std::rand() % IntWidth;
				NewSubSpaces.push_back({SubSpace.X, SubSpace.Y, SplitLine - SubSpace.X, SubSpace.Height});
				NewSubSpaces.push_back(
					{SplitLine, SubSpace.Y, SubSpace.X + SubSpace.Width - SplitLine, SubSpace.Height});
			}
			else
			{
				NewSubSpaces.push_back(SubSpace);
			}
		}
	}
	SubSpaces = std::move(NewSubSpaces); // Move instead of copy for efficiency

	DivideSpace(SubSpaces, CurrentDepth + 1, MaxDepth);
}


void FBSPGenerator::CreateRooms(const std::vector<FRect>& SubSpaces, const FGeneratorSettings Settings)
{
	for (const auto& SubSpace : SubSpaces)
	{
		constexpr float RoomMargin = 10;
		const float MaxWidth = FMath::Min(SubSpace.Width - RoomMargin, FMath::RandRange(1500.0f, 5000.0f));
		const float MaxHeight = FMath::Min(SubSpace.Height - RoomMargin, FMath::RandRange(1500.0f, 5000.0f));

		const float RoomWidth = FMath::RandRange(MaxWidth / 2.0f, MaxWidth);
		const float RoomHeight = FMath::RandRange(MaxHeight / 2.0f, MaxHeight);

		const float RoomX = SubSpace.X + FMath::RandRange(0.0f, SubSpace.Width - RoomWidth);
		const float RoomY = SubSpace.Y + FMath::RandRange(0.0f, SubSpace.Height - RoomHeight);

		FRect Room(RoomX, RoomY, RoomWidth, RoomHeight);
		Rooms.push_back(Room);

		if (Settings.bDrawRooms)
		{
			Room.Draw(World, FColor::Magenta, 30.0f);
		}
	}
}
