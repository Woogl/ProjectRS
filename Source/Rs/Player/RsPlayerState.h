// Copyright 2024 Team BH.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "RsPlayerState.generated.h"

/**
 * 
 */
UCLASS()
class RS_API ARsPlayerState : public APlayerState
{
	GENERATED_BODY()

public:
	ARsPlayerState();
	
	virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const override;
	
	uint32 GetUserId() const;
	void SetUserId(uint32 NewUserId);
	
private:
	UPROPERTY(ReplicatedUsing = OnRep_UserId)
	uint32 UserId = 0;
	
	UFUNCTION()
	virtual void OnRep_UserId();
};
