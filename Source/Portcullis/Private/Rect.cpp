#include "Rect.h"
#include "DrawDebugHelpers.h"
#include "Builders/CubeBuilder.h"
#include "Editor/EditorEngine.h"
#include "Engine/Brush.h"
#include "Engine/World.h"

void FRect::Draw(const UWorld* World, FColor Color, const float LineThickness) const
{
	if (!World)
	{
		UE_LOG(LogTemp, Error, TEXT("Can't draw rectangle without a world context object."));
		return;
	}

	const FVector RectCenter(X + Width * 0.5f, Y + Height * 0.5f, 0.f);
	const FVector RectExtent(Width * 0.5f, Height * 0.5f, 0.f);

	// Calculate the corners of the rectangle
	const FVector TopLeftCorner = RectCenter + FVector(-RectExtent.X, -RectExtent.Y, 0.f);
	const FVector BottomLeftCorner = RectCenter + FVector(-RectExtent.X, RectExtent.Y, 0.f);
	const FVector TopRightCorner = RectCenter + FVector(RectExtent.X, -RectExtent.Y, 0.f);
	const FVector BottomRightCorner = RectCenter + FVector(RectExtent.X, RectExtent.Y, 0.f);

	if (Color == FColor::Transparent)
	{
		Color = Color.MakeRandomColor();
	}

	// Draw lines to represent the edges of the rectangle
	DrawLine(World, TopLeftCorner, TopRightCorner, Color, LineThickness);
	DrawLine(World, TopRightCorner, BottomRightCorner, Color, LineThickness);
	DrawLine(World, BottomRightCorner, BottomLeftCorner, Color, LineThickness);
	DrawLine(World, BottomLeftCorner, TopLeftCorner, Color, LineThickness);
}

void FRect::DrawLine(const UWorld* World, const FVector& Start, const FVector& End, const FColor Color,
                     const float Thickness)
{
	DrawDebugLine(
		World,
		Start,
		End,
		Color,
		true, 0, 0,
		Thickness
	);
}

void FRect::Carve(UWorld* World) const
{
	// Check if the World context object is valid
	if (!World)
	{
		UE_LOG(LogTemp, Error, TEXT("Can't carve rectangle without a world context object."));
		return;
	}

	// Check if the function is executed within the Unreal Editor
	if (!GEditor)
	{
		UE_LOG(LogTemp, Error, TEXT("Carving can only be performed in the editor."));
		return;
	}

	// Spawn a new brush actor
	ABrush* NewBrush = World->SpawnBrush();

	// Create a cube builder object for defining the shape of the brush
	UCubeBuilder* CubeBuilder = NewObject<UCubeBuilder>(NewBrush, UCubeBuilder::StaticClass(), NAME_None,
	                                                    RF_Transactional);
	CubeBuilder->X = Width;
	CubeBuilder->Y = Height;
	CubeBuilder->Z = 1000.f; // Set the default depth

	// Assign the cube builder to the brush and configure it to subtractive mode
	NewBrush->BrushBuilder = CubeBuilder;
	NewBrush->BrushType = Brush_Subtract;

	// Create a new model for the brush and initialize it
	NewBrush->Brush = NewObject<UModel>(NewBrush, NAME_None, RF_Transactional);
	NewBrush->Brush->Initialize(NewBrush, false);
	NewBrush->SetActorLocation(FVector(X + Width * 0.5f, Y + Height * 0.5f, 0.f));

	// Build the brush using the brush builder and rebuild BSPs
	NewBrush->BrushBuilder->Build(NewBrush->GetWorld(), NewBrush);
	NewBrush->SetNeedRebuild(NewBrush->GetLevel());
	GEditor->RebuildAlteredBSP();
}
