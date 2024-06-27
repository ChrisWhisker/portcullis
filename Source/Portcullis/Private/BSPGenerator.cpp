#include "BSPGenerator.h"
#include <cstdlib>
#include <ctime>
#include "DrawDebugHelpers.h"

void FBSPGenerator::Initialize(UWorld* InWorld)
{
	FDungeonGenerator::Initialize(InWorld); // Call base class Initialize to set the World
}

void FBSPGenerator::Generate()
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

	for (const auto& SubSpace : SubSpaces)
	{
		DrawDebugRect(SubSpace, FColor::Silver);
	}

	// Create rooms in the divided spaces
	CreateRooms(SubSpaces);
}

void FBSPGenerator::DivideSpace(std::vector<FRect>& SubSpaces, const int CurrentDepth, const int MaxDepth)
{
	if (CurrentDepth >= MaxDepth) return;

	std::vector<FRect> NewSubSpaces;

	for (const auto& SubSpace : SubSpaces)
	{
		constexpr int MinSize = 300;
		if (static_cast<bool>(std::rand() % 2)) // Divide horizontally
		{
			if (SubSpace.Height > MinSize * 2) // Skip division if resulting spaces would be too small
			{
				const int SplitLine = SubSpace.Y + MinSize + std::rand() % (SubSpace.Height - MinSize * 2);
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
				const int SplitLine = SubSpace.X + MinSize + std::rand() % (SubSpace.Width - MinSize * 2);
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

void FBSPGenerator::CreateRooms(const std::vector<FRect>& SubSpaces)
{
	for (const auto& SubSpace : SubSpaces)
	{
		constexpr int32 RoomMargin = 10;
		const int32 MaxWidth = FMath::Min(SubSpace.Width - RoomMargin, FMath::RandRange(1500, 5000));
		const int32 MaxHeight = FMath::Min(SubSpace.Height - RoomMargin, FMath::RandRange(1500, 5000));

		const int32 RoomWidth = FMath::RandRange(MaxWidth / 2, MaxWidth);
		const int32 RoomHeight = FMath::RandRange(MaxHeight / 2, MaxHeight);

		const int32 RoomX = SubSpace.X + FMath::RandRange(0, SubSpace.Width - RoomWidth);
		const int32 RoomY = SubSpace.Y + FMath::RandRange(0, SubSpace.Height - RoomHeight);

		FRect Room(RoomX, RoomY, RoomWidth, RoomHeight);
		Rooms.push_back(Room);

		DrawDebugRect(Room, FColor::Magenta, 30);
	}
}
