// Fill out your copyright notice in the Description page of Project Settings.


#include "DungeonGenerator.h"

DungeonGenerator::~DungeonGenerator()
{
}

void DungeonGenerator::DrawDebugRect(const FRect& Rect) const
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
	const FColor Color = FColor::MakeRandomColor();
	DrawLine(TopLeft, TopRight, Color);
	DrawLine(TopRight, BottomRight, Color);
	DrawLine(BottomRight, BottomLeft, Color);
	DrawLine(BottomLeft, TopLeft, Color);
}

void DungeonGenerator::DrawLine(const FVector& Start, const FVector& End, const FColor Color) const
{
	DrawDebugLine(
		World,
		Start,
		End,
		Color,
		false, 10, 0,
		10.0f
	);
}
