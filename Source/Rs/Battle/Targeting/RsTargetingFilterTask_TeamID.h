// Copyright 2024 Team BH.

#pragma once

#include "CoreMinimal.h"
#include "Tasks/TargetingFilterTask_BasicFilterTemplate.h"
#include "RsTargetingFilterTask_TeamID.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class RS_API URsTargetingFilterTask_TeamID : public UTargetingFilterTask_BasicFilterTemplate
{
	GENERATED_BODY()

public:
	virtual bool ShouldFilterTarget(const FTargetingRequestHandle& TargetingHandle, const FTargetingDefaultResultData& TargetData) const override;

protected:
	/** The set of required team IDs (must be one of these IDs to not be filtered out) */
	UPROPERTY(EditAnywhere, Category = "Targeting Filter | Team ID")
	TSet<uint8> RequiredTeamIDs;

	/** The set of ignored team IDs (must NOT be one of these IDs) */
	UPROPERTY(EditAnywhere, Category = "Targeting Filter | Team ID")
	TSet<uint8> IgnoredTeamIDs;

	/** Add source actor's team ID to "IgnoredTeamIDs" */
	UPROPERTY(EditAnywhere, Category = "Targeting Filter | Team ID")
	bool bIgnoreSourceTeamID = true;
};
