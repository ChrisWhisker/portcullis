#include "DungeonGenerator.h"
#include "DrawDebugHelpers.h"

void DungeonGenerator::Initialize(UWorld* InWorld)
{
	World = InWorld;
}

void DungeonGenerator::DrawDebugRect(const FRect& Rect, FColor Color = FColor::Transparent, const float LineThickness) const
{
	if (!World)
	{
		UE_LOG(LogTemp, Error, TEXT("Can't execute DrawDebugRect(). Call Initialize() first."));
		return;
	}

	const FVector Center(Rect.X + Rect.Width * 0.5f, Rect.Y + Rect.Height * 0.5f, 0.f);
	const FVector Extent(Rect.Width * 0.5f, Rect.Height * 0.5f, 0.f);

	// Calculate the corners of the rectangle
	const FVector TopLeft = Center + FVector(-Extent.X, -Extent.Y, 0.f);
	const FVector BottomLeft = Center + FVector(-Extent.X, Extent.Y, 0.f);
	const FVector TopRight = Center + FVector(Extent.X, -Extent.Y, 0.f);
	const FVector BottomRight = Center + FVector(Extent.X, Extent.Y, 0.f);

	// Draw lines to represent the edges of the rectangle
	if (Color == FColor::Transparent)
	{
		Color = Color.MakeRandomColor();
	}
	DrawLine(TopLeft, TopRight, Color, LineThickness);
	DrawLine(TopRight, BottomRight, Color, LineThickness);
	DrawLine(BottomRight, BottomLeft, Color, LineThickness);
	DrawLine(BottomLeft, TopLeft, Color, LineThickness);
}

void DungeonGenerator::DrawLine(const FVector& Start, const FVector& End, const FColor Color, const float Thickness) const
{
	DrawDebugLine(
		World,
		Start,
		End,
		Color,
		true, 0, 0,
		Thickness
	);
}
