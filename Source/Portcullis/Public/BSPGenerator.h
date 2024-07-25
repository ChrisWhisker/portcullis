#pragma once

#include <vector>
#include "CoreMinimal.h"
#include "DungeonGenerator.h"
#include "GeneratorSettings.h"
#include "Rect.h"
#include "Engine/World.h"

/**
 * Generates a randomized dungeon using the Binary Space Partitioning (BSP) algorithm.
 */
class PORTCULLIS_API FBSPGenerator final : public FDungeonGenerator
{
public:
	virtual void Generate(const FGeneratorSettings Settings) override;

private:
	std::vector<FRect> Rooms;
	std::vector<FRect> Corridors;
	static void DivideSpace(std::vector<FRect>& SubSpaces, int CurrentDepth, int MaxDepth);
	void CreateRooms(const std::vector<FRect>& SubSpaces, const FGeneratorSettings Settings);
	void ConnectRooms();
};
