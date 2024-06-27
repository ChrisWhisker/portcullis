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
			if (SubSpace.Height <= MinSize * 2) // Skip division if resulting spaces would be too small
			{
				NewSubSpaces.push_back(SubSpace);
				continue;
			}
			const int SplitLine = SubSpace.Y + MinSize + std::rand() % (SubSpace.Height - MinSize * 2);
			FRect TopSpace = {SubSpace.X, SubSpace.Y, SubSpace.Width, SplitLine - SubSpace.Y};
			FRect BottomSpace = {SubSpace.X, SplitLine, SubSpace.Width, SubSpace.Y + SubSpace.Height - SplitLine};
			NewSubSpaces.push_back(TopSpace);
			NewSubSpaces.push_back(BottomSpace);
		}
		else // Divide vertically
		{
			if (SubSpace.Width <= MinSize * 2) // Skip division if resulting spaces would be too small
			{
				NewSubSpaces.push_back(SubSpace);
				continue;
			}
			const int SplitLine = SubSpace.X + MinSize + std::rand() % (SubSpace.Width - MinSize * 2);
			FRect LeftSpace = {SubSpace.X, SubSpace.Y, SplitLine - SubSpace.X, SubSpace.Height};
			FRect RightSpace = {SplitLine, SubSpace.Y, SubSpace.X + SubSpace.Width - SplitLine, SubSpace.Height};
			NewSubSpaces.push_back(LeftSpace);
			NewSubSpaces.push_back(RightSpace);
		}
	}
	SubSpaces = NewSubSpaces;

	DivideSpace(SubSpaces, CurrentDepth + 1, MaxDepth);
}

void FBSPGenerator::CreateRooms(const std::vector<FRect>& SubSpaces)
{
	for (const auto& SubSpace : SubSpaces)
	{
		// Randomize the room size with a margin and maximum size constraints
		constexpr int32 RoomMargin = 10;
		constexpr int32 MaxRoomSize = 1500;

		const int32 RoomWidth = FMath::RandRange(FMath::Min(SubSpace.Width / 2, MaxRoomSize - RoomMargin),
		                                         FMath::Min(SubSpace.Width - RoomMargin, MaxRoomSize));
		const int32 RoomHeight = FMath::RandRange(FMath::Min(SubSpace.Height / 2, MaxRoomSize - RoomMargin),
		                                          FMath::Min(SubSpace.Height - RoomMargin, MaxRoomSize));

		const int32 RoomX = SubSpace.X + FMath::RandRange(0, SubSpace.Width - RoomWidth);
		const int32 RoomY = SubSpace.Y + FMath::RandRange(0, SubSpace.Height - RoomHeight);

		FRect Room(RoomX, RoomY, RoomWidth, RoomHeight);
		Rooms.push_back(Room);

		DrawDebugRect(Room, FColor::Magenta, 30);
	}
}
