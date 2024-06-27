#include "../Public/Rect.h"

void FRect::Draw(const UWorld* World, FColor Color, const float LineThickness) const
{
	if (!World)
	{
		UE_LOG(LogTemp, Error, TEXT("Can't draw rectangle without a world context object."));
		return;
	}

	const FVector RectCenter(X + Width * 0.5f, Y + Height * 0.5f, 0.f);
	const FVector RectExtent(Width * 0.5f, Height * 0.5f, 0.f);

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
	DrawLine(World, TopLeftCorner, TopRightCorner, Color, LineThickness);
	DrawLine(World, TopRightCorner, BottomRightCorner, Color, LineThickness);
	DrawLine(World, BottomRightCorner, BottomLeftCorner, Color, LineThickness);
	DrawLine(World, BottomLeftCorner, TopLeftCorner, Color, LineThickness);
}

void FRect::DrawLine(const UWorld* World, const FVector& Start, const FVector& End, const FColor Color,
                     const float Thickness)
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
