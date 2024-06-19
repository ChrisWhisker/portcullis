#pragma once

#include "CoreMinimal.h"
#include "DungeonGenerator.h"
#include "Engine/World.h"

struct FRect
{
	int X, Y, Width, Height;
};

/**
 *
 */
class PORTCULLIS_API BSPGenerator final : DungeonGenerator
{
public:
	void Initialize(UWorld* InWorld);

	virtual void Generate() override;

private:
	UWorld* World = nullptr; // Member variable to hold UWorld pointer

	static void DivideSpace(std::vector<FRect>& Spaces, int Depth, int MaxDepth);

	void DrawDebugRect(const FRect& Rect) const; // Updated function signature

	void DrawLine(const FVector& Start, const FVector& End) const;
};
