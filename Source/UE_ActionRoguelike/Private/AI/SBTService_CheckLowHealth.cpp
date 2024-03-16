#include "AI/SBTService_CheckLowHealth.h"

#include "AIController.h"
#include "SAttributeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"

USBTService_CheckLowHealth::USBTService_CheckLowHealth()
{
	LowHealthFraction = 0.3f;
}


void USBTService_CheckLowHealth::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	APawn* AIPawn = OwnerComp.GetAIOwner()->GetPawn();

	USAttributeComponent* AttributeComp = Cast<USAttributeComponent>(AIPawn->GetComponentByClass(USAttributeComponent::StaticClass()));
	if (ensure(AttributeComp))
	{
		bool bLowHealth = (AttributeComp->GetHealth() / AttributeComp->GetHealthMax() < LowHealthFraction);

		UBlackboardComponent* BlackBoardComp = OwnerComp.GetBlackboardComponent();
		BlackBoardComp->SetValueAsBool(LowHealthKey.SelectedKeyName, bLowHealth);
	}
}
