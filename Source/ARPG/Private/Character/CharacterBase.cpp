// Fill out your copyright notice in the Description page of Project Settings.

#include "CharacterBase.h"
#include "XD_CharacterMovementComponent.h"
#include "ARPG_MovementComponent.h"


// Sets default values
ACharacterBase::ACharacterBase(const FObjectInitializer& PCIP)
	:Super(PCIP.SetDefaultSubobjectClass<UARPG_MovementComponent>(ACharacter::CharacterMovementComponentName))
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ARPG_MovementComponent = CastChecked<UARPG_MovementComponent>(GetCharacterMovement());
}

// Called when the game starts or when spawned
void ACharacterBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ACharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

float ACharacterBase::PlayMontage(UAnimMontage * MontageToPlay, float InPlayRate /*= 1.f*/, FName StartSectionName /*= NAME_None*/, bool ClientMaster /*= false*/)
{
	if (MontageToPlay)
	{
		if (ClientMaster)
		{
			if (HasAuthority())
			{
				if (GetController() && GetController()->IsLocalController())
				{
					MulticastPlayMontage(MontageToPlay, InPlayRate, StartSectionName);
				}
			}
			else
			{
				PlayAnimMontage(MontageToPlay, InPlayRate, StartSectionName);
				PlayMontageToServer(MontageToPlay, InPlayRate, StartSectionName);
			}
		}
		else if (HasAuthority())
		{
			MulticastPlayMontage(MontageToPlay, InPlayRate, StartSectionName);
		}
		return MontageToPlay->GetPlayLength();
	}
	return 0.f;
}

void ACharacterBase::MulticastPlayMontage_Implementation(UAnimMontage * MontageToPlay, float InPlayRate /*= 1.f*/, FName StartSectionName /*= NAME_None*/)
{
	PlayAnimMontage(MontageToPlay, InPlayRate, StartSectionName);
}

void ACharacterBase::MulticastPlayMontageSkipOwner_Implementation(UAnimMontage * MontageToPlay, float InPlayRate /*= 1.f*/, FName StartSectionName /*= NAME_None*/)
{
	if ((GetController() && GetController()->IsLocalController()) == false)
	{
		PlayAnimMontage(MontageToPlay, InPlayRate, StartSectionName);
	}
}

void ACharacterBase::StopMontage_Implementation()
{
	StopAnimMontage();
}

void ACharacterBase::PlayMontageToServer_Implementation(UAnimMontage * MontageToPlay, float InPlayRate /*= 1.f*/, FName StartSectionName /*= NAME_None*/)
{
	MulticastPlayMontageSkipOwner(MontageToPlay, InPlayRate, StartSectionName);
}

