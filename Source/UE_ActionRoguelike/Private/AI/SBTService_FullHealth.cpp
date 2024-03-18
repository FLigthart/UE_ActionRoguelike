#include "AI/SBTService_FullHealth.h"

#include "AIController.h"
#include "SAttributeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"

void USBTService_FullHealth::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	APawn* AIPawn = OwnerComp.GetAIOwner()->GetPawn();

	USAttributeComponent* AttributeComp = Cast<USAttributeComponent>(AIPawn->GetComponentByClass(USAttributeComponent::StaticClass()));
	if (ensure(AttributeComp))
	{
		UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
		BlackboardComp->SetValueAsBool(FullHealthKey.SelectedKeyName, AttributeComp->IsMaxHealth());
	}
}
