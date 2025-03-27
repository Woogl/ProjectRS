// Copyright 2024 Team BH.


#include "BTTask_ActivateAbility.h"

#include "AbilitySystemComponent.h"
#include "Rs/AbilitySystem/RsAbilitySystemLibrary.h"
#include "Rs/AI/AIController/RsAIControllerBase.h"
#include "Rs/Character/RsCharacterBase.h"

EBTNodeResult::Type UBTTask_ActivateAbility::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* OwnerController = OwnerComp.GetAIOwner();
	ARsCharacterBase* OwnerCharacter = Cast<ARsCharacterBase>(OwnerController->GetCharacter());

	EBTNodeResult::Type Result = Super::ExecuteTask(*MyOwnerComp, NodeMemory);
	MyOwnerComp = &OwnerComp;

	if (UAbilitySystemComponent* ASC = OwnerCharacter->GetAbilitySystemComponent())
	{
		if (ASC->TryActivateAbilitiesByTag(AbilityTags))
		{
			UGameplayAbility* ActivatedAbility = URsAbilitySystemLibrary::FindAbilityWithTag(ASC, AbilityTags,false);
			if (ActivatedAbility->IsActive())
			{
				ActivatedAbility->OnGameplayAbilityEnded.AddLambda([this](UGameplayAbility* ActivatedAbility)->void { this->FinishLatentTask(*(this->MyOwnerComp),EBTNodeResult::Succeeded);});
				Result = EBTNodeResult::InProgress;
			}
		}
	}
	return Result;
}
