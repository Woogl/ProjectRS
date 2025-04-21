// Copyright 2024 Team BH.


#include "RsPlayerCharacterViewModel.h"

#include "AbilitySystemGlobals.h"
#include "RsAbilityViewModel.h"
#include "RsEnergySetViewModel.h"
#include "Rs/AbilitySystem/RsAbilitySystemLibrary.h"

#include "Rs/Character/RsPlayerCharacter.h"
#include "Rs/System/RsGameSetting.h"

URsPlayerCharacterViewModel* URsPlayerCharacterViewModel::CreateRsPlayerCharacterViewModel(ARsPlayerCharacter* Model)
{
	URsPlayerCharacterViewModel* ViewModel = NewObject<URsPlayerCharacterViewModel>(Model);
	ViewModel->Initialize();
	return ViewModel;
}

void URsPlayerCharacterViewModel::Initialize()
{
	Super::Initialize();
	
	if (ARsPlayerCharacter* Model = Cast<ARsPlayerCharacter>(GetOuter()))
	{
		EnergySetViewModel = URsEnergySetViewModel::CreateEnergySetViewModel(Model);
		if (UAbilitySystemComponent* ASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(Model))
		{
			if (URsGameplayAbility* Skill_E = URsAbilitySystemLibrary::FindRsAbilityWithTag(ASC, URsGameSetting::Get()->ESkillTag.GetSingleTagContainer(), true))
			{
				AbilityViewModel_E = URsAbilityViewModel::CreateRsAbilityViewModel(Skill_E);
			}
			if (URsGameplayAbility* Skill_Q = URsAbilitySystemLibrary::FindRsAbilityWithTag(ASC, URsGameSetting::Get()->QSkillTag.GetSingleTagContainer(), true))
			{
				AbilityViewModel_Q = URsAbilityViewModel::CreateRsAbilityViewModel(Skill_Q);
			}
			if (URsGameplayAbility* Skill_Ult = URsAbilitySystemLibrary::FindRsAbilityWithTag(ASC, URsGameSetting::Get()->UltSkillTag.GetSingleTagContainer(), true))
			{
				AbilityViewModel_Ult = URsAbilityViewModel::CreateRsAbilityViewModel(Skill_Ult);
			}
		}
	}
}
