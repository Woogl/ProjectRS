// Copyright 2025 Team BH.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "RsRagdollComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class RS_API URsRagdollComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	URsRagdollComponent();
	
	UFUNCTION(BlueprintCallable, Category = "RS")
	void StartRagdoll_Replicated();
	
	UFUNCTION(BlueprintCallable, Category = "RS")
	void StartRagdoll_Local();

protected:
	virtual void BeginPlay() override;
	
	UPROPERTY(Transient)
	TObjectPtr<ACharacter> CharacterOwner;
	
	UPROPERTY(Transient)
	TObjectPtr<USkeletalMeshComponent> MeshComponent;

private:
	UFUNCTION(Server, Reliable)
	void StartRagdoll_Server();

	UFUNCTION(NetMulticast, Reliable)
	void StartRagdoll_Multicast();
};
