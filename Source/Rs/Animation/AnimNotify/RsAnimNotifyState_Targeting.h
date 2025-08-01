﻿// Copyright 2024 Team BH.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "Rs/Targeting/RsTargetingTypes.h"
#include "RsAnimNotifyState_Targeting.generated.h"

/**
 * 
 */
UCLASS(Abstract)
class RS_API URsAnimNotifyState_Targeting : public UAnimNotifyState
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

	UFUNCTION(BlueprintPure, Category = "Targeting")
	TArray<AActor*> GetTargets() { return Targets;};
	
protected:
	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference) override;

	UFUNCTION(BlueprintCallable, Category = "Targeting")
	bool PerformTargeting(USkeletalMeshComponent* MeshComp);
	
	UPROPERTY()
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
