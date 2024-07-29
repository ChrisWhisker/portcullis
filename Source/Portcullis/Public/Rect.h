#pragma once

#include "CoreMinimal.h"
#include "Engine/World.h"

struct PORTCULLIS_API FRect
{
public:
	float X, Y, Width, Height;

	FRect(const float InX, const float InY, const float InWidth, const float InHeight)
		: X(InX), Y(InY), Width(InWidth), Height(InHeight)
	{
	}

	void Draw(const UWorld* World, FColor Color = FColor::Transparent, float LineThickness = 10.f) const;
	void Carve(UWorld* World) const;

private:
	static void DrawLine(const UWorld* World, const FVector& Start, const FVector& End, FColor Color,
	                     float Thickness = 10.f);
};
