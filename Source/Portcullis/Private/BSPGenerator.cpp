#include "BSPGenerator.h"
#include "DrawDebugHelpers.h"

void BSPGenerator::Initialize(UWorld* InWorld)
{
	if (!World)
	{
		World = InWorld;
	}
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
	constexpr FRect InitialSpace = {0, 0, 10000, 10000};
	std::vector<FRect> Spaces = {InitialSpace}; // Use initializer list to initialize the vector

	// Recursively divide the space to a certain depth
	constexpr int MaxDepth = 4;
	DivideSpace(Spaces, 0, MaxDepth);

	// Output the resulting spaces and draw debug shapes
	for (const auto& Rect : Spaces)
	{
		UE_LOG(LogTemp, Log, TEXT("FRect: x=%d, y=%d, width=%d, height=%d"), Rect.X, Rect.Y, Rect.Width, Rect.Height);
		DrawDebugRect(Rect); // Draw debug shape for each rectangle
	}
}

void BSPGenerator::DivideSpace(std::vector<FRect>& Spaces, const int Depth, const int MaxDepth)
{
	if (Depth >= MaxDepth) return;

	std::vector<FRect> NewSpaces;
	for (const auto& Rect : Spaces)
	{
		if (static_cast<bool>(std::rand() % 2)) // Divide horizontally
		{
			int Split = 1; // Default split value to avoid division by zero
			if (Rect.Height > 1)
			{
				Split = Rect.Y + std::rand() % (Rect.Height - 1) + 1;
			}
			FRect Top = {Rect.X, Rect.Y, Rect.Width, Split - Rect.Y};
			FRect Bottom = {Rect.X, Split, Rect.Width, Rect.Y + Rect.Height - Split};
			NewSpaces.push_back(Top);
			NewSpaces.push_back(Bottom);
		}
		else // Divide vertically
		{
			int Split = 1; // Default split value to avoid division by zero
			if (Rect.Width > 1)
			{
				Split = Rect.X + std::rand() % (Rect.Width - 1) + 1;
			}
			FRect Left = {Rect.X, Rect.Y, Split - Rect.X, Rect.Height};
			FRect Right = {Split, Rect.Y, Rect.X + Rect.Width - Split, Rect.Height};
			NewSpaces.push_back(Left);
			NewSpaces.push_back(Right);
		}
	}
	Spaces = NewSpaces;

	DivideSpace(Spaces, Depth + 1, MaxDepth);
}

void BSPGenerator::DrawDebugRect(const FRect& Rect) const
{
	if (!World) return;

	const FVector Center(Rect.X + Rect.Width * 0.5f, Rect.Y + Rect.Height * 0.5f, 0.f);
	const FVector Extent(Rect.Width * 0.5f, Rect.Height * 0.5f, 0.f);

	// Calculate the corners of the rectangle
	const FVector TopLeft = Center + FVector(-Extent.X, -Extent.Y, 0.f);
	const FVector BottomLeft = Center + FVector(-Extent.X, Extent.Y, 0.f);
	const FVector TopRight = Center + FVector(Extent.X, -Extent.Y, 0.f);
	const FVector BottomRight = Center + FVector(Extent.X, Extent.Y, 0.f);

	// Draw lines to represent the edges of the rectangle
	DrawLine(TopLeft, TopRight);
	DrawLine(TopRight, BottomRight);
	DrawLine(BottomRight, BottomLeft);
	DrawLine(BottomLeft, TopLeft);
}

void BSPGenerator::DrawLine(const FVector& Start, const FVector& End) const
{
	DrawDebugLine(
		World,
		Start,
		End,
		FColor::Red,
		false, 10, 0,
		10.0f
	);
}
