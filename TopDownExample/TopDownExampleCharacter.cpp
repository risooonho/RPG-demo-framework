// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.

#include "TopDownExample.h"
#include "TopDownExampleCharacter.h"

ATopDownExampleCharacter::ATopDownExampleCharacter(const class FPostConstructInitializeProperties& PCIP)
	: Super(PCIP)
{
	// Set size for player capsule
	CapsuleComponent->InitCapsuleSize(42.f, 96.0f);

	// Don't rotate character to camera direction
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	CharacterMovement->bOrientRotationToMovement = true; // Rotate character to moving direction
	CharacterMovement->RotationRate = FRotator(0.f, 640.f, 0.f);
	CharacterMovement->bConstrainToPlane = true;
	CharacterMovement->bSnapToPlaneAtStart = true;

	// Create a camera boom...
	CameraBoom = PCIP.CreateDefaultSubobject<USpringArmComponent>(this, TEXT("CameraBoom"));
	CameraBoom->AttachTo(RootComponent);
	CameraBoom->bAbsoluteRotation = true; // Don't want arm to rotate when character does
	CameraBoom->TargetArmLength = 800.f;
	CameraBoom->RelativeRotation = FRotator(-60.f, 0.f, 0.f);
	CameraBoom->bDoCollisionTest = false; // Don't want to pull camera in when it collides with level

	// Create a camera...
	TopDownCameraComponent = PCIP.CreateDefaultSubobject<UCameraComponent>(this, TEXT("TopDownCamera"));
	TopDownCameraComponent->AttachTo(CameraBoom, USpringArmComponent::SocketName);
	TopDownCameraComponent->bUseControllerViewRotation = false; // Camera does not rotate relative to arm

	PrimaryActorTick.bCanEverTick = true;

	Inventory = NewObject<UPlayerInventory>(this);

}



void ATopDownExampleCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	FVector  MyLocation = GetActorLocation();
	
	TArray<FHitResult> HitResults = TArray<FHitResult>();
	FVector StartLocation = TopDownCameraComponent->ComponentToWorld.GetLocation();

	TArray<TWeakObjectPtr<class UPrimitiveComponent>> blockingComponents;


	UWorld* WorldReference = GetWorld();

	FCollisionQueryParams TraceParams = FCollisionQueryParams(FName(TEXT("RV_Trace")), true, this);
	TraceParams.bTraceComplex = true;
	TraceParams.AddIgnoredActor(this);

	if (WorldReference->LineTraceMulti(HitResults, StartLocation, MyLocation, TraceParams, NULL)) {

		for (auto It = HitResults.CreateIterator(); It; ++It)
		{
			TWeakObjectPtr<class UPrimitiveComponent>  component = It->Component;

			blockingComponents.Add(component);
		}
	}

	PurgeHiddenObjects(blockingComponents);
}

void ATopDownExampleCharacter::PurgeHiddenObjects(TArray<TWeakObjectPtr<class UPrimitiveComponent>> newComponents) {
	for (auto It = StillBlocking.CreateIterator(); It; It++) {
		TWeakObjectPtr<class UPrimitiveComponent> component = *It;
		if (!newComponents.Contains(component)) {

			// Something like this is what I would love to do... However, Not quite there yet.
			/*
			UMaterialInstanceDynamic* MILight = UMaterialInstanceDynamic::Create(component->GetMaterial(0), this);

			MILight->SetScalarParameterValue(TEXT("Opacity"), 1);
			component->SetMaterial(0, MILight);
			*/

			component->SetVisibility(true);

			StillBlocking.Remove(component);
		}
	}

	for (auto It = newComponents.CreateIterator(); It; It++) {
		TWeakObjectPtr<class UPrimitiveComponent> component = *It;
		if (!StillBlocking.Contains(component)) {

			// Something like this is what I would love to do.. However, Not quite there yet
			/*
			UMaterialInstanceDynamic* MILight = UMaterialInstanceDynamic::Create(component->GetMaterial(0), this);

			MILight->SetScalarParameterValue(TEXT("Opacity"), 0.5);
			component->SetMaterial(0, MILight);
			*/

			component->SetVisibility(false);
			StillBlocking.Add(component);
		}
	}
}

void ATopDownExampleCharacter::OnMousePressed(AController* user, FVector pos, FKey key)
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, TEXT("Pressed on player"));
}

void ATopDownExampleCharacter::OnMouseReleased(AController* user, FVector pos, FKey key)
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, TEXT("Released outisde of player"));
}

void ATopDownExampleCharacter::OnMouseReleased(AController* user, FVector pos, AActor * target, FKey key)
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, target->GetName());
}

void ATopDownExampleCharacter::OnMouseFocusedReleased(AController* user, FVector pos, FKey key)
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, TEXT("Released on player"));
}

void ATopDownExampleCharacter::OnMouseHoverIn(AController* user, FVector pos)
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, TEXT("Hover In on player"));
}

void ATopDownExampleCharacter::OnMouseHoverOut(AController* user, FVector pos)
{
	
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, TEXT("Hover out from player"));
}

void ATopDownExampleCharacter::OnTakeDmg()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, TEXT("Take dmg"));
}

void ATopDownExampleCharacter::OnDealDmg()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, TEXT("Deal dmg"));
}

void ATopDownExampleCharacter::OnDeath()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, TEXT("Died"));
}

void ATopDownExampleCharacter::OnResurrect()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, TEXT("Resurrected"));
}

void ATopDownExampleCharacter::AddEnergy(int value)
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, TEXT("Add Energy"));
}

void ATopDownExampleCharacter::AddHealth(int value)
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, TEXT("Add Health"));
}

void ATopDownExampleCharacter::AddMana(int value)
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, TEXT("Add Mana"));
}

// Subtraction of status
void ATopDownExampleCharacter::SubtractHealth(int value)
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, TEXT("Subtract Health"));
}

void ATopDownExampleCharacter::SubtractEnergy(int value)
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, TEXT("Subtract Energy"));
}

void ATopDownExampleCharacter::SubtractMana(int value)
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, TEXT("Subtract Mana"));
}

int ATopDownExampleCharacter::GetCurrentEnergy()
{
	return 0;
}

int ATopDownExampleCharacter::GetCurrentHealth()
{
	return 0;
}

int ATopDownExampleCharacter::GetCurrentMana()
{
	return 0;
}

int ATopDownExampleCharacter::GetMaxHealth()
{
	return 0;
}

int ATopDownExampleCharacter::GetMaxEnergy()
{
	return 0;
}

int ATopDownExampleCharacter::GetMaxMana()
{
	return 0;
}

void ATopDownExampleCharacter::CastSpell(USpell * spell)
{
	if (!spell->IsAOE()) {
		FTransform SpawnTM(this->GetActorRotation(), GetActorLocation()+FVector(10.0f, 0.5f, 0.5f));

		ASpellProjectile* projectile = Cast<ASpellProjectile>(UGameplayStatics::BeginSpawningActorFromClass(this, spell->GetSpellProjectileClass(), SpawnTM));
		projectile->SetSpell(spell);

		UGameplayStatics::FinishSpawningActor(projectile, SpawnTM);
	}
}