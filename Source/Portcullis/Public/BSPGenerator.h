// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DungeonGenerator.h"

struct Rect
{
	int x, y, width, height;
};

/**
 * 
 */
class PORTCULLIS_API BSPGenerator : DungeonGenerator
{
public:
	BSPGenerator();
	virtual ~BSPGenerator() override;

	virtual void Generate() override;

private:
	void divideSpace(std::vector<Rect>& spaces, int depth, int maxDepth);
};
