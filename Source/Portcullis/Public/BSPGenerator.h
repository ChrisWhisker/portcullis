#pragma once

#include "CoreMinimal.h"
#include "DungeonGenerator.h"
#include "Engine/World.h"
#include "FRect.h"
#include <vector>

/**
 *
 */
class PORTCULLIS_API BSPGenerator final : public DungeonGenerator
{
public:
	virtual void Initialize(UWorld* InWorld) override;
	virtual void Generate() override;

private:
	static void DivideSpace(std::vector<FRect>& Spaces, int Depth, int MaxDepth);
	void CreateRooms(const std::vector<FRect>& Spaces);

	std::vector<FRect> Rooms;
};
