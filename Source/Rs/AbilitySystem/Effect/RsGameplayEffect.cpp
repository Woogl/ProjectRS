// Copyright 2024 Team BH.


#include "RsGameplayEffect.h"

#include "Rs/AbilitySystem/Calculation/ModifierMagnitude/RsDurationCalculation.h"

URsGameplayEffect::URsGameplayEffect()
{
	// /** Duration calculation */
	// DurationPolicy = EGameplayEffectDurationType::HasDuration;
	//
	// FCustomCalculationBasedFloat CustomDuration;
	// CustomDuration.CalculationClassMagnitude = URsDurationCalculation::StaticClass();
	//
	// FGameplayEffectModifierMagnitude CustomModifierMagnitude(CustomDuration);
	// DurationMagnitude = CustomDuration;
	//
	// /** Magnitude calculation */
	// FGameplayModifierInfo Modifier;
	//
	// FGameplayEffectModifierMagnitude ModifierMagnitude(CustomModifierMagnitude);
	//
	// Modifier.ModifierMagnitude
 //    
	// // **Modifier에 커스텀 Magnitude 사용 설정**
	// Modifier.ModifierMagnitude = FScalableFloat();
	// Modifier.ModifierMagnitude.MagnitudeCalculationType = EGameplayEffectMagnitudeCalculation::CustomCalculationClass;
	// Modifier.ModifierMagnitude.MagnitudeCalculationClass = UMyCustomMagnitudeCalculation::StaticClass();
	//
	// // 적용할 Attribute 설정 (예제: Damage 속성 변경)
	// Modifier.Attribute = FGameplayAttribute();  // 여기에 실제 Attribute 설정 필요
	// Modifier.ModifierOp = EGameplayModOp::Additive;  // 예제: 현재 값에 추가
	//
	// // Modifier 리스트에 추가
	// Modifiers.Add(Modifier);
}
