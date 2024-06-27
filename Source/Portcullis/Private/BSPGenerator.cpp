#include "BSPGenerator.h"
#include <cstdlib>
#include <ctime>
#include "DrawDebugHelpers.h"

void BSPGenerator::Initialize(UWorld* InWorld)
{
	DungeonGenerator::Initialize(InWorld); // Call base class Initialize to set the World
}

void BSPGenerator::Generate()
{
	if (!World)
	{
		UE_LOG(LogTemp, Error, TEXT("No valid world context set for BSPGenerator. Call Initialize() first."));
		return;
	}

	std::srand(static_cast<unsigned int>(std::time(nullptr)));

	// Define the initial space
	const FRect InitialSpace = {0, 0, 10000, 10000};
	std::vector<FRect> Spaces = {InitialSpace}; // Use initializer list to initialize the vector

	// Recursively divide the space to a certain depth
	FlushPersistentDebugLines(World);
	constexpr int MaxDepth = 4;
	DivideSpace(Spaces, 0, MaxDepth);

	for (const auto& Space : Spaces)
	{
		DrawDebugRect(Space, FColor::Silver);
	}

	// Create rooms in the divided spaces
	CreateRooms(Spaces);

	// Output the resulting rooms
	for (const auto& Room : Rooms)
	{
		UE_LOG(LogTemp, Log, TEXT("Room: x=%d, y=%d, width=%d, height=%d"), Room.X, Room.Y, Room.Width, Room.Height);
	}
}

void BSPGenerator::DivideSpace(std::vector<FRect>& Spaces, const int Depth, const int MaxDepth)
{
	if (Depth >= MaxDepth) return;

	std::vector<FRect> NewSpaces;

	for (const auto& Rect : Spaces)
	{
		constexpr int MinSize = 300;
		if (static_cast<bool>(std::rand() % 2)) // Divide horizontally
		{
			if (Rect.Height <= MinSize * 2) // Skip division if resulting spaces would be too small
			{
				NewSpaces.push_back(Rect);
				continue;
			}
			const int Split = Rect.Y + MinSize + std::rand() % (Rect.Height - MinSize * 2);
			FRect Top = {Rect.X, Rect.Y, Rect.Width, Split - Rect.Y};
			FRect Bottom = {Rect.X, Split, Rect.Width, Rect.Y + Rect.Height - Split};
			NewSpaces.push_back(Top);
			NewSpaces.push_back(Bottom);
		}
		else // Divide vertically
		{
			if (Rect.Width <= MinSize * 2) // Skip division if resulting spaces would be too small
			{
				NewSpaces.push_back(Rect);
				continue;
			}
			const int Split = Rect.X + MinSize + std::rand() % (Rect.Width - MinSize * 2);
			FRect Left = {Rect.X, Rect.Y, Split - Rect.X, Rect.Height};
			FRect Right = {Split, Rect.Y, Rect.X + Rect.Width - Split, Rect.Height};
			NewSpaces.push_back(Left);
			NewSpaces.push_back(Right);
		}
	}
	Spaces = NewSpaces;

	DivideSpace(Spaces, Depth + 1, MaxDepth);
}

void BSPGenerator::CreateRooms(const std::vector<FRect>& Spaces)
{
	for (const auto& Space : Spaces)
	{
		// Randomize the room size with a margin and maximum size constraints
		constexpr int32 RoomMargin = 10;
		const int32 MaxRoomWidth = FMath::RandRange(1500,5000);
		const int32 MaxRoomHeight = FMath::RandRange(1500,5000);

		const int32 RoomWidth = FMath::RandRange(FMath::Min(Space.Width / 2, MaxRoomWidth - RoomMargin),
		                                         FMath::Min(Space.Width - RoomMargin, MaxRoomWidth));
		const int32 RoomHeight = FMath::RandRange(FMath::Min(Space.Height / 2, MaxRoomHeight - RoomMargin),
		                                          FMath::Min(Space.Height - RoomMargin, MaxRoomHeight));

		const int32 RoomX = Space.X + FMath::RandRange(0, Space.Width - RoomWidth);
		const int32 RoomY = Space.Y + FMath::RandRange(0, Space.Height - RoomHeight);

		FRect Room(RoomX, RoomY, RoomWidth, RoomHeight);
		Rooms.push_back(Room);

		DrawDebugRect(Room, FColor::Magenta, 30);
	}
}
