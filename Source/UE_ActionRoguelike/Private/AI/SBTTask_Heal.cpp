#include "AI/SBTTask_Heal.h"

#include "AIController.h"
#include "SAttributeComponent.h"

USBTTask_Heal::USBTTask_Heal()
{
	HealAmount = 20.f;
}

EBTNodeResult::Type USBTTask_Heal::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	APawn* AIPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (ensure(AIPawn))
	{
		USAttributeComponent* AttributeComp = Cast<USAttributeComponent>(AIPawn->GetComponentByClass(USAttributeComponent::StaticClass()));
		if (ensure(AttributeComp))
		{
			bool bHasHealed = AttributeComp->ApplyHealthChange(HealAmount);
			return bHasHealed ? EBTNodeResult::Succeeded : EBTNodeResult::Failed;
		}
		return EBTNodeResult::Failed;
	}
	return EBTNodeResult::Failed;
}
