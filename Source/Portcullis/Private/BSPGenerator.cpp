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

	// TODO Create additive brush for the walls

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

	// Connect rooms with corridors
	ConnectRooms();

	if (Settings.bDrawRooms)
	{
		for (const auto& Room : Rooms)
		{
			Room.Draw(World, FColor::Magenta, 30.0f);
		}

		for (const auto& Corridor : Corridors)
		{
			Corridor.Draw(World, FColor::Magenta, 30.0f);
		}
	}
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

		Room.Carve(World);
	}
}

void FBSPGenerator::ConnectRooms()
{
	// Define a constant for corridor width
	const float CorridorWidth = FMath::RandRange(150.0f, 300.0f);

	// Use a simple Minimum Spanning Tree (MST) approach to connect rooms
	std::vector<bool> Connected(Rooms.size(), false);
	Connected[0] = true; // Start with the first room connected

	for (size_t i = 0; i < Rooms.size() - 1; ++i)
	{
		float MinDistance = FLT_MAX;
		size_t RoomAIndex = 0;
		size_t RoomBIndex = 0;

		// Find the closest pair of connected and unconnected rooms
		for (size_t j = 0; j < Rooms.size(); ++j)
		{
			if (Connected[j])
			{
				for (size_t k = 0; k < Rooms.size(); ++k)
				{
					if (!Connected[k])
					{
						FVector2D CenterA(Rooms[j].X + Rooms[j].Width / 2.0f, Rooms[j].Y + Rooms[j].Height / 2.0f);
						FVector2D CenterB(Rooms[k].X + Rooms[k].Width / 2.0f, Rooms[k].Y + Rooms[k].Height / 2.0f);
						const float Distance = FVector2D::Distance(CenterA, CenterB);

						if (Distance < MinDistance)
						{
							MinDistance = Distance;
							RoomAIndex = j;
							RoomBIndex = k;
						}
					}
				}
			}
		}

		// Connect the closest unconnected room to the connected network
		const FRect RoomA = Rooms[RoomAIndex];
		const FRect RoomB = Rooms[RoomBIndex];
		Connected[RoomBIndex] = true;

		// Create corridors connecting RoomA and RoomB
		const FVector2D Start = FVector2D(RoomA.X + RoomA.Width / 2.0f, RoomA.Y + RoomA.Height / 2.0f);
		const FVector2D End = FVector2D(RoomB.X + RoomB.Width / 2.0f, RoomB.Y + RoomB.Height / 2.0f);

		if (FMath::RandBool())
		{
			// Horizontal first, then vertical
			const float CorridorY = Start.Y - CorridorWidth / 2.0f;
			const float CorridorHeight = CorridorWidth;
			const float CorridorLength = FMath::Abs(End.X - Start.X);

			FRect Horizontal = FRect(FMath::Min(Start.X, End.X), CorridorY, CorridorLength, CorridorHeight);
			Corridors.push_back(Horizontal);
			Horizontal.Carve(World);

			const float CorridorX = End.X - CorridorWidth / 2.0f;
			const float CorridorWidthVertical = CorridorWidth;
			const float CorridorHeightVertical = FMath::Abs(End.Y - Start.Y);

			FRect Vertical = FRect(CorridorX, FMath::Min(Start.Y, End.Y), CorridorWidthVertical,
			                       CorridorHeightVertical);
			Corridors.push_back(Vertical);
			Vertical.Carve(World);
		}
		else
		{
			// Vertical first, then horizontal
			const float CorridorX = Start.X - CorridorWidth / 2.0f;
			const float CorridorWidthVertical = CorridorWidth;
			const float CorridorHeightVertical = FMath::Abs(End.Y - Start.Y);

			FRect Vertical = FRect(CorridorX, FMath::Min(Start.Y, End.Y), CorridorWidthVertical,
			                       CorridorHeightVertical);
			Corridors.push_back(Vertical);
			Vertical.Carve(World);

			const float CorridorY = End.Y - CorridorWidth / 2.0f;
			const float CorridorHeight = CorridorWidth;
			const float CorridorLength = FMath::Abs(End.X - Start.X);

			FRect Horizontal = FRect(FMath::Min(Start.X, End.X), CorridorY, CorridorLength, CorridorHeight);
			Corridors.push_back(Horizontal);
			Horizontal.Carve(World);
		}
	}
}
