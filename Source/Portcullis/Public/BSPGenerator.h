#pragma once

#include <vector>
#include "CoreMinimal.h"
#include "DungeonGenerator.h"
#include "Rect.h"
#include "Engine/World.h"

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
