// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
class PORTCULLIS_API DungeonGenerator
{
public:
	virtual ~DungeonGenerator();

	virtual void Generate() = 0;
};
