#include "AI/SAIController.h"

void ASAIController::BeginPlay()
{
	Super::BeginPlay();

	if (ensureMsgf(BehaviourTree, TEXT("Behaviour Tree is nullptr! Please assign BehaviourTree in your AI Controller.")))
	{
		RunBehaviorTree(BehaviourTree);
	}
}