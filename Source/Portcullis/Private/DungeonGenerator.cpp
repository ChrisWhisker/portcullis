#include "DungeonGenerator.h"
#include "DrawDebugHelpers.h"

void FDungeonGenerator::Initialize(UWorld* InWorld)
{
	World = InWorld;
}

void FDungeonGenerator::DrawDebugRect(const FRect& Rect, FColor Color = FColor::Transparent,
                                      const float LineThickness) const
{
	if (!World)
	{
		UE_LOG(LogTemp, Error, TEXT("Can't execute DrawDebugRect(). Call Initialize() first."));
		return;
	}

	const FVector RectCenter(Rect.X + Rect.Width * 0.5f, Rect.Y + Rect.Height * 0.5f, 0.f);
	const FVector RectExtent(Rect.Width * 0.5f, Rect.Height * 0.5f, 0.f);

	// Calculate the corners of the rectangle
	const FVector TopLeftCorner = RectCenter + FVector(-RectExtent.X, -RectExtent.Y, 0.f);
	const FVector BottomLeftCorner = RectCenter + FVector(-RectExtent.X, RectExtent.Y, 0.f);
	const FVector TopRightCorner = RectCenter + FVector(RectExtent.X, -RectExtent.Y, 0.f);
	const FVector BottomRightCorner = RectCenter + FVector(RectExtent.X, RectExtent.Y, 0.f);

	if (Color == FColor::Transparent)
	{
		Color = Color.MakeRandomColor();
	}
	// Draw lines to represent the edges of the rectangle
	DrawLine(TopLeftCorner, TopRightCorner, Color, LineThickness);
	DrawLine(TopRightCorner, BottomRightCorner, Color, LineThickness);
	DrawLine(BottomRightCorner, BottomLeftCorner, Color, LineThickness);
	DrawLine(BottomLeftCorner, TopLeftCorner, Color, LineThickness);
}

void FDungeonGenerator::DrawLine(const FVector& Start, const FVector& End, const FColor Color,
                                 const float Thickness) const
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
