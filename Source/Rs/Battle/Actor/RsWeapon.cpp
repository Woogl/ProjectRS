// Copyright 2024 Team BH.


#include "RsWeapon.h"

#include "Components/BoxComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SphereComponent.h"
#include "Rs/RsLogChannels.h"

const FName ARsWeapon::DefaultWeaponID(TEXT("Weapon_Default"));

ARsWeapon::ARsWeapon()
{
	// Avoid ticking if possible.
	//PrimaryActorTick.bCanEverTick = false;

	bReplicates = true;
	bNetUseOwnerRelevancy = true;
	
	WeaponID = DefaultWeaponID;
}

FGenericTeamId ARsWeapon::GetGenericTeamId() const
{
	if (AActor* WeaponOwner = GetAttachParentActor())
	{
		if (IGenericTeamAgentInterface* TeamInterface = Cast<IGenericTeamAgentInterface>(WeaponOwner))
		{
			return TeamInterface->GetGenericTeamId();
		}
	}
	return FGenericTeamId::NoTeam;
}

FName ARsWeapon::GetWeaponID() const
{
	return WeaponID;
}

UPrimitiveComponent* ARsWeapon::GetRootPrimitive() const
{
	return Cast<UPrimitiveComponent>(RootComponent);
}

FRsTargetingShape ARsWeapon::GetWeaponShape() const
{
	if (UPrimitiveComponent* WeaponRoot = GetRootPrimitive())
	{
		FRsTargetingShape Shape;
		if (WeaponRoot->IsA(UBoxComponent::StaticClass()))
		{
			Shape.ShapeType = ERsTargetingShapeType::Box;
		}
		else if	(WeaponRoot->IsA(UCapsuleComponent::StaticClass()))
		{
			Shape.ShapeType = ERsTargetingShapeType::Capsule;
		}
		else if (WeaponRoot->IsA(USphereComponent::StaticClass()))
		{
			Shape.ShapeType = ERsTargetingShapeType::Sphere;
		}
		else
		{
			// default
			Shape.ShapeType = ERsTargetingShapeType::Box;
		}
		Shape.HalfExtent = WeaponRoot->GetLocalBounds().BoxExtent;
		return Shape;
	}
	UE_LOG(LogRs, Warning, TEXT("Cannot make %s's shape"), *GetName());
	return FRsTargetingShape();
}

void ARsWeapon::BeginPlay()
{
	Super::BeginPlay();
	
}
