// Copyright 2024 Team BH.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "Rs/Targeting/RsTargetingTypes.h"
#include "RsAnimNotify_Targeting.generated.h"

/**
 * 
 */
UCLASS(Abstract)
class RS_API URsAnimNotify_Targeting : public UAnimNotify
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Targeting|Source", meta = (GetOptions="GetSocketNames"))
	FName SocketName = FName("root");

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Targeting|Source")
	FVector PositionOffset = FVector::ZeroVector;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Targeting|Source")
	FRotator RotationOffset = FRotator::ZeroRotator;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Targeting")
	FRsTargetingShape Shape;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Targeting")
	FRsTargetingCollision Collision;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Targeting")
	FRsTargetingFilter Filter;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Targeting")
	FRsTargetingSorter Sorter;

	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;
	
protected:
	bool PerformTargeting(USkeletalMeshComponent* MeshComp);
	
	UPROPERTY(BlueprintReadOnly)
	TArray<AActor*> Targets;

#if WITH_EDITOR
	UFUNCTION()
	TArray<FName> GetSocketNames() const { return SocketNames; }
#endif // WITH_EDITOR

#if WITH_EDITORONLY_DATA
	UPROPERTY()
	TArray<FName> SocketNames;
#endif // WITH_EDITORONLY_DATA
};
