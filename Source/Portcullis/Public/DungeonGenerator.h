#pragma once

#include "CoreMinimal.h"
#include "Rect.h"
#include "Engine/World.h"

/**
 *
 */
class PORTCULLIS_API FDungeonGenerator
{
public:
	virtual ~FDungeonGenerator() = default;
	virtual void Initialize(UWorld* InWorld);
	virtual void Generate() = 0;

protected:
	UWorld* World = nullptr;
	void DrawDebugRect(const FRect& Rect, FColor Color, float LineThickness = 10) const;
	void DrawLine(const FVector& Start, const FVector& End, const FColor Color, const float Thickness = 10) const;
};
