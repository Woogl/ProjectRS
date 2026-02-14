// Copyright 2024 Team BH.


#include "RsGameplayAbility.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"
#include "EnhancedInputComponent.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "Rs/RsGameplayTags.h"
#include "Rs/RsLogChannels.h"
#include "Rs/AbilitySystem/RsAbilitySystemLibrary.h"
#include "Rs/AbilitySystem/RsAbilitySystemSettings.h"
#include "Rs/Character/RsCharacterBase.h"
#include "Rs/Player/RsPlayerState.h"

URsGameplayAbility::URsGameplayAbility()
{
	// Sets the ability to default to Instanced Per Actor.
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	
	CostGameplayEffectClass = URsAbilitySystemSettings::Get().DefaultCostEffect;
	CooldownGameplayEffectClass = URsAbilitySystemSettings::Get().DefaultCooldownEffect;
}

ARsCharacterBase* URsGameplayAbility::GetAvatarCharacter() const
{
	if (AActor* AvatarActor = GetAvatarActorFromActorInfo())
	{
		return Cast<ARsCharacterBase>(AvatarActor);
	}
	return nullptr;
}

AController* URsGameplayAbility::GetController() const
{
	if (CurrentActorInfo)
	{
		if (AController* PC = CurrentActorInfo->PlayerController.Get())
		{
			return PC;
		}
		if (AActor* Avatar = GetAvatarActorFromActorInfo())
		{
			return Avatar->GetInstigatorController();
		}
	}
	return nullptr;
}

void URsGameplayAbility::CommitExecute(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
{
	if (!CommitAbilityCost(Handle, ActorInfo, ActivationInfo))
	{
		return;
	}
	
	// Don't apply cooldown while recharging.
	if (MaxRechargeStacks == 0 || GetCooldownTimeRemaining() <= 0.f)
	{
		if (CooldownApplyTiming == ERsCooldownApplyTiming::OnActivate)
		{
			CommitAbilityCooldown(Handle, ActorInfo, ActivationInfo, false);
		}
	}
}

const FGameplayTagContainer* URsGameplayAbility::GetCooldownTags() const
{
	CurrentCooldownTags.Reset();
	if (const FGameplayTagContainer* ParentTags = Super::GetCooldownTags())
	{
		CurrentCooldownTags.AppendTags(*ParentTags);
	}
	CurrentCooldownTags.AddTag(CooldownTag);
	return &CurrentCooldownTags;
}

bool URsGameplayAbility::CheckCooldown(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, FGameplayTagContainer* OptionalRelevantTags) const
{
	if (CurrentRechargeStacks > 0)
	{
		return true;
	}
	
	return Super::CheckCooldown(Handle, ActorInfo, OptionalRelevantTags);
}

void URsGameplayAbility::ApplyCooldown(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) const
{
	UGameplayEffect* CooldownGE = GetCooldownGameplayEffect();
	if (!CooldownGE)
	{
		return;
	}
	if (CooldownGameplayEffectClass == URsAbilitySystemSettings::Get().DefaultCooldownEffect)
	{
		if (CooldownDuration == 0.f || !CooldownTag.IsValid())
		{
			return;
		}
	}
	FGameplayEffectSpecHandle SpecHandle = MakeOutgoingGameplayEffectSpec(CooldownGE->GetClass(), GetAbilityLevel());
	SpecHandle.Data.Get()->DynamicGrantedTags.AddTag(CooldownTag);
	CurrentCooldownHandle = ApplyGameplayEffectSpecToOwner(Handle, ActorInfo, ActivationInfo, SpecHandle);
}

void URsGameplayAbility::ModifyCooldownRemaining(float TimeDiff)
{
	if (CurrentCooldownHandle.IsValid())
	{
		if (UAbilitySystemComponent* ASC = GetAbilitySystemComponentFromActorInfo())
		{
			ASC->ModifyActiveEffectStartTime(CurrentCooldownHandle, TimeDiff);
		}
	}
}

void URsGameplayAbility::SetCooldownRemaining(float NewRemaining)
{
	if (!CurrentCooldownHandle.IsValid() && NewRemaining > 0)
	{
		CommitAbilityCooldown(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true);
	}

	if (CurrentCooldownHandle.IsValid())
	{
		if (UAbilitySystemComponent* ASC = GetAbilitySystemComponentFromActorInfo())
		{
			if (const FActiveGameplayEffect* CooldownEffect = ASC->GetActiveGameplayEffect(CurrentCooldownHandle))
			{
				float TimeRemaining = CooldownEffect->GetTimeRemaining(GetWorld()->GetTimeSeconds());
				ASC->ModifyActiveEffectStartTime(CurrentCooldownHandle, -TimeRemaining + NewRemaining);
			}
		}
	}
}

int32 URsGameplayAbility::GetCurrentRechargeStacks() const
{
	return CurrentRechargeStacks;
}

void URsGameplayAbility::ModifyCurrentRechargeStacks(int32 Diff)
{
	CurrentRechargeStacks = FMath::Clamp(CurrentRechargeStacks + Diff, 0, MaxRechargeStacks);
	OnRechargeStacksChanged.Broadcast(CurrentRechargeStacks);
}

void URsGameplayAbility::SetCurrentRechargeStacks(int32 NewStacks)
{
	CurrentRechargeStacks = FMath::Clamp(NewStacks, 0, MaxRechargeStacks);
	OnRechargeStacksChanged.Broadcast(CurrentRechargeStacks);
}

void URsGameplayAbility::SetupEnhancedInputBindings(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec)
{
	// Check to see if the "Activation Input Action" is valid.
	URsGameplayAbility* const AbilityInstance = Cast<URsGameplayAbility>(Spec.Ability.Get());
	if (!AbilityInstance || !AbilityInstance->ActivationInputAction)
	{
		return;
	}
	
	const APawn* const AvatarPawn = Cast<APawn>(ActorInfo->AvatarActor.Get());
	if (!AvatarPawn || !Spec.Ability)
	{
		return;
	}
	
	const AController* const PawnController = AvatarPawn->GetController();
	UEnhancedInputComponent* const EnhancedInputComponent = PawnController ? Cast<UEnhancedInputComponent>(PawnController->InputComponent.Get()) : nullptr;
	if (!EnhancedInputComponent)
	{
		return;
	}
	
	// Input pressed event.
	EnhancedInputComponent->BindAction(AbilityInstance->ActivationInputAction, ETriggerEvent::Triggered, AbilityInstance, &ThisClass::HandleInputPressedEvent, ActorInfo, Spec.Handle);
	
	// Input released event.
	EnhancedInputComponent->BindAction(AbilityInstance->ActivationInputAction, ETriggerEvent::Completed, AbilityInstance, &ThisClass::HandleInputReleasedEvent, ActorInfo, Spec.Handle);
}

void URsGameplayAbility::TeardownEnhancedInputBindings(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec)
{
	const APawn* const AvatarPawn = Cast<APawn>(ActorInfo->AvatarActor.Get());
	if (!AvatarPawn || !Spec.Ability)
	{
		return;
	}
	
	if (const AController* const PawnController = AvatarPawn->GetController())
	{
		if (UEnhancedInputComponent* const EnhancedInputComponent = Cast<UEnhancedInputComponent>(PawnController->InputComponent.Get()))
		{
			EnhancedInputComponent->ClearBindingsForObject(Spec.Ability.Get());
		}
	}
}

void URsGameplayAbility::RevertGameplayEvent(FGameplayTag EventTag)
{
	if (FActiveGameplayEffectHandle* EffectHandle = EventEffectHandles.Find(EventTag))
	{
		GetAbilitySystemComponentFromActorInfo()->RemoveActiveGameplayEffect(*EffectHandle);
	}
}

void URsGameplayAbility::OnAvatarSet(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec)
{
	Super::OnAvatarSet(ActorInfo, Spec);

	// Set up Bindings for Enhanced Input.
	SetupEnhancedInputBindings(ActorInfo, Spec);

	// Set up Cooldowns stacks for recharge.
	CurrentRechargeStacks = MaxRechargeStacks;
	RechargeDelegateHandle = ActorInfo->AbilitySystemComponent->RegisterGameplayTagEvent(CooldownTag, EGameplayTagEventType::AnyCountChange).AddUObject(this, &ThisClass::HandleRechargeStacksChanged);
}

void URsGameplayAbility::OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec)
{
	Super::OnGiveAbility(ActorInfo, Spec);
	
	// Try to Activate immediately if "Activate Ability On Granted" is true.
	if (bActivateOnGranted)
	{
		ActorInfo->AbilitySystemComponent->TryActivateAbility(Spec.Handle);
	}
	
	K2_OnGiveAbility();
}

void URsGameplayAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	// Native child classes should override ActivateAbility and call CommitAbility.
	// CommitAbility is used to do one last check for spending resources.
	if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}
	
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	MontageToPlay = SelectMontageToPlay();
	if (MontageToPlay)
	{
		if (UAbilityTask_PlayMontageAndWait* MontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, NAME_None, MontageToPlay))
		{
			MontageTask->OnCompleted.AddDynamic(this, &ThisClass::HandleMontageCompleted);
			MontageTask->OnBlendOut.AddDynamic(this, &ThisClass::HandleMontageCompleted);
			MontageTask->OnInterrupted.AddDynamic(this, &ThisClass::HandleMontageCancelled);
			MontageTask->OnCancelled.AddDynamic(this, &ThisClass::HandleMontageCancelled);
			MontageTask->ReadyForActivation();
		}
	}
	
	TArray<FGameplayTag> EventTags { RsGameplayTags::ANIMNOTIFY_EVENT1, RsGameplayTags::ANIMNOTIFY_EVENT2, RsGameplayTags::ANIMNOTIFY_EVENT3, RsGameplayTags::ANIMNOTIFY_EVENT4, RsGameplayTags::ANIMNOTIFY_EVENT5 };
	for (const FGameplayTag& EventTag : EventTags)
	{
		UAbilityTask_WaitGameplayEvent* WaitEventTask = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(this, EventTag);
		WaitEventTask->EventReceived.AddDynamic(this, &ThisClass::HandleGameplayEvent);
		WaitEventTask->ReadyForActivation();
	}

	if (MaxRechargeStacks > 0)
	{
		ModifyCurrentRechargeStacks(-1);
	}
}

void URsGameplayAbility::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);

	if (CooldownApplyTiming == ERsCooldownApplyTiming::OnEnd)
	{
		// Prevents the cooldown effect from being applied multiple times.
		if (MaxRechargeStacks <= 1 || (CurrentRechargeStacks == MaxRechargeStacks - 1))
		{
			CommitAbilityCooldown(Handle, ActorInfo, ActivationInfo, false);
		}
	}
}

void URsGameplayAbility::HandleInputPressedEvent(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpecHandle SpecHandle)
{
	// Find the Ability Spec based on the passed in information and set a reference.
	FGameplayAbilitySpec* const Spec = ActorInfo->AbilitySystemComponent->FindAbilitySpecFromHandle(SpecHandle);
	if (!Spec || !Spec->Ability || !ActorInfo->AbilitySystemComponent.IsValid())
	{
		return;
	}

	UAbilitySystemComponent* const AbilitySystemComponent = ActorInfo->AbilitySystemComponent.Get();
	if (!AbilitySystemComponent)
	{
		return;
	}
	
	Spec->InputPressed = true;
	
	if (!Spec->IsActive() || bRetriggerInstancedAbility)
	{
		// Ability is not active, so try to activate it
		AbilitySystemComponent->TryActivateAbility(SpecHandle);
		return;
	}

	if (Spec->Ability->bReplicateInputDirectly && !AbilitySystemComponent->IsOwnerActorAuthoritative())
	{
		AbilitySystemComponent->ServerSetInputPressed(Spec->Ability.Get()->GetCurrentAbilitySpecHandle());
	}

	AbilitySystemComponent->AbilitySpecInputPressed(*Spec);

	// Invoke the InputPressed event. This is not replicated here. If someone is listening, they may replicate the InputPressed event to the server.
	UGameplayAbility* PrimaryInstance = Spec->GetPrimaryInstance();
	const FGameplayAbilityActivationInfo& ActivationInfo = PrimaryInstance->GetCurrentActivationInfoRef();
	AbilitySystemComponent->InvokeReplicatedEvent(EAbilityGenericReplicatedEvent::InputPressed, Spec->Handle, ActivationInfo.GetActivationPredictionKey());
}

void URsGameplayAbility::HandleInputReleasedEvent(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpecHandle SpecHandle)
{
	// Find the Ability Spec based on the passed in information and set a reference.
	FGameplayAbilitySpec* const Spec = ActorInfo->AbilitySystemComponent->FindAbilitySpecFromHandle(SpecHandle);
	if (!Spec || !Spec->IsActive() || !ActorInfo->AbilitySystemComponent.IsValid())
	{
		return;
	}

	UAbilitySystemComponent* const AbilitySystemComponent = ActorInfo->AbilitySystemComponent.Get();
	if (!AbilitySystemComponent)
	{
		return;
	}
	
	Spec->InputPressed = false;
	if (Spec->Ability->bReplicateInputDirectly && !AbilitySystemComponent->IsOwnerActorAuthoritative())
	{
		AbilitySystemComponent->ServerSetInputReleased(SpecHandle);
	}

	AbilitySystemComponent->AbilitySpecInputReleased(*Spec);

	// Invoke the InputReleased event. This is not replicated here. If someone is listening, they may replicate the InputPressed event to the server.
	UGameplayAbility* PrimaryInstance = Spec->GetPrimaryInstance();
	const FGameplayAbilityActivationInfo& ActivationInfo = PrimaryInstance->GetCurrentActivationInfoRef();
	AbilitySystemComponent->InvokeReplicatedEvent(EAbilityGenericReplicatedEvent::InputReleased, Spec->Handle, ActivationInfo.GetActivationPredictionKey());
}

void URsGameplayAbility::HandleRechargeStacksChanged(FGameplayTag GameplayTag, int NewStacks)
{
	if (CurrentRechargeStacks < MaxRechargeStacks && GetCooldownTimeRemaining() <= 0.f)
	{
		ModifyCurrentRechargeStacks(+1);
		
		if (CurrentRechargeStacks < MaxRechargeStacks)
		{
			// Apply cooldown again for recharging.
			ApplyCooldown(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo);
		}
	}
}

void URsGameplayAbility::OnRemoveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec)
{
	TeardownEnhancedInputBindings(ActorInfo, Spec);
	ActorInfo->AbilitySystemComponent->UnregisterGameplayTagEvent(RechargeDelegateHandle, CooldownTag, EGameplayTagEventType::AnyCountChange);
	
	Super::OnRemoveAbility(ActorInfo, Spec);

	K2_OnRemoveAbility();
}

UAnimMontage* URsGameplayAbility::SelectMontageToPlay_Implementation() const
{
	const int32 MontagesNum = Montages.Num();
	if (MontagesNum == 0)
	{
		return nullptr;
	}
	
	if (MontagesNum == 1)
	{
		return Montages[0];
	}
	
	int32 Index = GetDeterministicRandomNumber(0, MontagesNum - 1);
	return Montages[Index];
}

int32 URsGameplayAbility::GetDeterministicRandomNumber(int32 Min, int32 Max) const
{
	int16 PredKey = CurrentActivationInfo.GetActivationPredictionKey().Current;
	int32 UserId = 0;
	int32 StartTime = 0;
	if (const AController* Controller = GetController())
	{
		if (const ARsPlayerState* PS = Controller->GetPlayerState<ARsPlayerState>())
		{
			UserId = PS->GetUserId();
			StartTime = PS->GetStartTime();
		}
	}
	
	uint32 Seed = GetTypeHash(PredKey);
	Seed = HashCombineFast(Seed, GetTypeHash(UserId));
	Seed = HashCombineFast(Seed, GetTypeHash(StartTime));

	FRandomStream Stream(Seed);
	UE_LOG(LogRsAbility, Verbose, TEXT("%s Deterministic Seed : %d"), *GetName(), Stream.GetCurrentSeed());
	return Stream.RandRange(Min, Max);
}

void URsGameplayAbility::HandleMontageCompleted()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void URsGameplayAbility::HandleMontageCancelled()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
}

void URsGameplayAbility::HandleGameplayEvent(FGameplayEventData EventData)
{
	UAbilitySystemComponent* SourceASC = GetAbilitySystemComponentFromActorInfo();
	UAbilitySystemComponent* TargetASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(EventData.Target);
	
	if (const TSubclassOf<UGameplayEffect>* Effect = EffectMap.Find(EventData.EventTag))
	{
		FGameplayEffectContextHandle EffectContext = EventData.ContextHandle.IsValid() ? EventData.ContextHandle : SourceASC->MakeEffectContext();
		FGameplayEffectSpecHandle Spec = SourceASC->MakeOutgoingSpec(*Effect, GetAbilityLevel(), EffectContext);
		if (!Spec.IsValid())
		{
			UE_LOG(LogRsAbility, Error, TEXT("Invalid spec: %s"), *Effect->GetDefaultObject()->GetName());
			return;
		}
		Spec.Data->AppendDynamicAssetTags(GetAssetTags());
		FActiveGameplayEffectHandle Handle = SourceASC->ApplyGameplayEffectSpecToTarget(*Spec.Data, TargetASC);
		if (Handle.IsValid())
		{
			EventEffectHandles.Add(EventData.EventTag, Handle);
		}
	}

	if (const FDataTableRowHandle* EffectTableRow = EffectTableMap.Find(EventData.EventTag))
	{
		FGameplayEffectContextHandle EffectContext = EventData.ContextHandle.IsValid() ? EventData.ContextHandle : SourceASC->MakeEffectContext();
		FGameplayEffectSpecHandle Spec = URsAbilitySystemLibrary::MakeEffectSpecByTable(EffectTableRow->DataTable, EffectTableRow->RowName, SourceASC, GetAbilityLevel(), EffectContext);
		if (!Spec.IsValid())
		{
			UE_LOG(LogRsAbility, Error, TEXT("Invalid spec: %s, %s"), *EffectTableRow->DataTable.GetName(), *EffectTableRow->RowName.ToString());
			return;
		}
		Spec.Data->AppendDynamicAssetTags(GetAssetTags());
		FActiveGameplayEffectHandle Handle = SourceASC->ApplyGameplayEffectSpecToTarget(*Spec.Data, TargetASC);
		if (Handle.IsValid())
		{
			EventEffectHandles.Add(EventData.EventTag, Handle);
		}
	}
	
	K2_OnGameplayEvent(EventData);
}
