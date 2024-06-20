#pragma once

#include "CoreMinimal.h"
#include "DungeonGenerator.h"
#include "Engine/World.h"

/**
 *
 */
class PORTCULLIS_API BSPGenerator final : DungeonGenerator
{
public:
	void Initialize(UWorld* InWorld);

	virtual void Generate() override;

private:
	static void DivideSpace(std::vector<FRect>& Spaces, int Depth, int MaxDepth);
};
