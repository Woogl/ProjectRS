// Copyright 2024 Team BH.


#include "BTTask_ActivateAbility.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"
#include "AIController.h"
#include "Rs/AbilitySystem/RsAbilitySystemLibrary.h"

UBTTask_ActivateAbility::UBTTask_ActivateAbility()
{
	bCreateNodeInstance = true;
}

EBTNodeResult::Type UBTTask_ActivateAbility::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);
	
	if (UAbilitySystemComponent* ASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(OwnerComp.GetAIOwner()))
	{
		// BT 노드에서 설정할 수 있는 태그인풋으로 어빌리티 실행
		if (ASC->TryActivateAbilitiesByTag(AbilityTags))
		{
			// 실행한 어빌리티 인스턴스 찾아오기
			if (UGameplayAbility* ActivatedAbility = URsAbilitySystemLibrary::FindAbilityWithTag(ASC, AbilityTags, true))
			{
				// 바로 종료되지 않는 GA일 경우
				if (ActivatedAbility->IsActive())
				{
					// FinishLatentTask 함수 어빌리티의 Ended 델리게이트에 바인드, Task를 Progress 상태로 전환
					MyOwnerComp = &OwnerComp;
					ActivatedAbility->OnGameplayAbilityEnded.AddWeakLambda(this, [this](UGameplayAbility* Ability)
					{
						FinishLatentTask(*MyOwnerComp, EBTNodeResult::Succeeded);
					});
					Result = EBTNodeResult::InProgress;
				}
			}
		}
	}
	return Result;
}
