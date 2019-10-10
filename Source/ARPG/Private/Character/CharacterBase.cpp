// Fill out your copyright notice in the Description page of Project Settings.

#include "CharacterBase.h"
#include <Engine/World.h>
#include <Animation/AnimInstance.h>
#include <Components/SkeletalMeshComponent.h>
#include <Kismet/GameplayStatics.h>
#include <TimerManager.h>
#include <Kismet/KismetMathLibrary.h>
#include <AIController.h>
#include <Perception/AIPerceptionComponent.h>
#include <Perception/AISenseConfig_Sight.h>
#include <Engine/Engine.h>
#include <DrawDebugHelpers.h>
#include <GameFramework/PlayerController.h>

#include "ARPG_ItemBase.h"
#include "ARPG_ItemCoreBase.h"
#include "ARPG_InventoryComponent.h"
#include "ARPG_MovementComponent.h"
#include "ARPG_Battle_Log.h"
#include "ReceiveDamageActionBase.h"
#include "ARPG_DebugFunctionLibrary.h"
#include "ARPG_LevelFunctionLibrary.h"
#include "XD_TemplateLibrary.h"
#include "UnrealNetwork.h"
#include "ARPG_ActorFunctionLibrary.h"
#include "ARPG_CampInfo.h"
#include "ARPG_CampRelationship.h"
#include "ARPG_HatredControlSystemNormal.h"
#include "ARPG_BattleStyleSystemNormal.h"
#include "ARPG_AlertSystemNormal.h"
#include "ARPG_SneakSystemNormal.h"
#include "ARPG_NavigationQueryFilter.h"
#include "ARPG_CharacterTurnBase.h"
#include "ARPG_EnterReleaseStateBase.h"
#include "ARPG_PlayerControllerBase.h"
#include "XD_DispatchableActionBase.h"
#include "ARPG_AD_CharacterInteract.h"
#include "AIPerceptionExLibrary.h"
#include "ARPG_RelationshipFunctionLibrary.h"
#include "XD_MovementComponentFunctionLibrary.h"
#include "ARPG_ReceiveDamageActionBase.h"
#include "ARPG_HUDBase.h"
#include "Components/AudioComponent.h"
#include "ARPG_Item_Log.h"
#include "ARPG_DA_RoleSelection.h"

// Sets default values
ACharacterBase::ACharacterBase(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer.SetDefaultSubobjectClass<UARPG_MovementComponent>(ACharacter::CharacterMovementComponentName)), 
	bEnableAimOffset(true),
	bEnableFootIk(true)
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ARPG_MovementComponent = CastChecked<UARPG_MovementComponent>(GetCharacterMovement());
	{
		bUseControllerRotationYaw = false;
	}

	Inventory = CreateDefaultSubobject<UARPG_InventoryComponent>(GET_MEMBER_NAME_CHECKED(ACharacterBase, Inventory));
	{

	}

	HatredControlSystem = CreateDefaultSubobject<UARPG_HatredControlSystemNormal>(GET_MEMBER_NAME_CHECKED(ACharacterBase, HatredControlSystem));

	BattleStyleSystem = CreateDefaultSubobject<UARPG_BattleStyleSystemNormal>(GET_MEMBER_NAME_CHECKED(ACharacterBase, BattleStyleSystem));

	AlertSystem = CreateDefaultSubobject<UARPG_AlertSystemNormal>(GET_MEMBER_NAME_CHECKED(ACharacterBase, AlertSystem));

	SneakSystem = CreateDefaultSubobject<UARPG_SneakSystemNormal>(GET_MEMBER_NAME_CHECKED(ACharacterBase, SneakSystem));

	GetMesh()->VisibilityBasedAnimTickOption = EVisibilityBasedAnimTickOption::AlwaysTickPoseAndRefreshBones;

	NavigationQueryFilter = UARPG_NavigationQueryFilter::StaticClass();
}

// Called when the game starts or when spawned
void ACharacterBase::BeginPlay()
{
	Super::BeginPlay();

	if (InteractBehavior)
	{
		InteractBehavior->Character = this;
		InteractBehavior->InitLeader(this);
	}

	if (TurnConfig)
	{
		CharacterTurnAction = TurnConfig->CreateInstance(this);
	}
}

void ACharacterBase::Destroyed()
{
	//删除角色身上的东西
	TArray<AActor*> AttachedActors;
	GetAttachedActors(AttachedActors);
	for (AActor* Actor : AttachedActors)
	{
		Actor->Destroy();
	}

	Super::Destroyed();
}

void ACharacterBase::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}

bool ACharacterBase::NeedSave_Implementation() const
{
	return IsPlayerControlled() == false;
}

void ACharacterBase::PreInitializeComponents()
{
	Super::PreInitializeComponents();

	//先于组件的初始化注册，使得组件中可以覆盖注册
	BattleControl = this;
}

// Called every frame
void ACharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//TODO 挪入bIsLockingOther与bInvokeSprint的Setter驱动而不是轮询
	ARPG_MovementComponent->SetRotationMode(bIsLockingOther && ARPG_MovementComponent->IsSprinting() == false ? ECharacterRotationMode::LookingDirection : ECharacterRotationMode::VelocityDirection);
}

// Called to bind functionality to input
void ACharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ACharacterBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ACharacterBase, bIsLockingOther);
	DOREPLIFETIME(ACharacterBase, InteractingTarget);
	DOREPLIFETIME_CONDITION(ACharacterBase, RootMotionScale, COND_SimulatedOnly);
	DOREPLIFETIME(ACharacterBase, bMirrorFullBodyMontage);
}

void ACharacterBase::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	if (HatredControlSystem)
	{
		HatredControlSystem->InitHatredControlSystem(this);
	}

	if (BattleStyleSystem)
	{
		BattleStyleSystem->InitBattleStyleSystem(this);
	}

	if (AlertSystem)
	{
		AlertSystem->InitAlertSystem(this);
	}

	if (SneakSystem)
	{
		SneakSystem->InitSneakSystem(this);
	}
}

float ACharacterBase::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator,	AActor* DamageCauser)
{
	if (HatredControlSystem)
	{
		HatredControlSystem->TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	}

	return Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
}

void ACharacterBase::WhenGameInit_Implementation()
{
	Inventory->AddItemArray(GetInitItemList());
	Inventory->OnRep_ItemList();

	//初始化角色坐标
	{
		BornLocation = GetActorLocation();
		BornWorldOrigin = UGameplayStatics::GetWorldOriginLocation(this);
	}
}

void ACharacterBase::Reset()
{
	Super::Reset();

	//初始化道具和使用中的道具
	{
		TArray<UARPG_ItemCoreBase*> RemoveItems;
		TArray<UARPG_ItemCoreBase*> FinalInitItemList = GetReInitItemList();
		//对已存在的进行数量修改
		for (UARPG_ItemCoreBase* ItemCore : Inventory->GetItemCoreList())
		{
			if (UARPG_ItemCoreBase** P_Item = FinalInitItemList.FindByPredicate([ItemCore](const UARPG_ItemCoreBase* E_Item) {return ItemCore->IsEqualWithItemCore(E_Item); }))
			{
				if (UARPG_ItemCoreBase* Item = *P_Item)
				{
					ItemCore->Number = Item->Number;
					FinalInitItemList.RemoveAll([&](const UARPG_ItemCoreBase* E) {return E->IsEqualWithItemCore(ItemCore); });
				}
			}
			else
			{
				RemoveItems.Add(ItemCore);
			}
		}
		//移除初始化列表中不存在的
		for (UARPG_ItemCoreBase* ItemCore : RemoveItems)
		{
			Inventory->ItemCoreList.Remove(ItemCore);
		}
		//添加需要添加的
		Inventory->AddItemArray(FinalInitItemList);
		Inventory->OnRep_ItemList();
	}
}

TArray<UARPG_ItemCoreBase*> ACharacterBase::GetReInitItemList() const
{
	TArray<UARPG_ItemCoreBase*> Res = GetInitItemList();
	Res.Append(ArrayCast<UARPG_ItemCoreBase*>(Inventory->InitItems));
	return Res;
}

TArray<UARPG_ItemCoreBase*> ACharacterBase::GetInitItemList() const
{
	return ReceivedGetInitItemList();
}

FXD_DispatchableActionList ACharacterBase::GetCurrentDispatchableActions_Implementation()
{
	return CurrentActions;
}

UARPG_ActionDispatcherBase* ACharacterBase::GetCurrentMainDispatcher_Implementation() const
{
	return CurrentMainDispatcher;
}

void ACharacterBase::SetCurrentMainDispatcher_Implementation(UXD_ActionDispatcherBase* Dispatcher)
{
	CurrentMainDispatcher = Dispatcher ? CastChecked<UARPG_ActionDispatcherBase>(Dispatcher) : nullptr;
}

bool ACharacterBase::CanExecuteDispatcher_Implementation() const
{
	return AlertValue == 0.f;
}

bool ACharacterBase::IsInRoleSelection() const
{
	return CurrentActions.ContainsByPredicate([](UXD_DispatchableActionBase* Action) {return Action->IsA<UARPG_DA_RoleSelectionBase>(); });
}

void ACharacterBase::AbortRoleSelection()
{
	check(IsInRoleSelection());
	for (UXD_DispatchableActionBase* Action : CurrentActions)
	{
		if (UARPG_DA_RoleSelectionBase* Selection = Cast<UARPG_DA_RoleSelectionBase>(Action))
		{
			Selection->AbortDispatcher();
			break;
		}
	}
}

void ACharacterBase::SetRebornLocation(const FVector& RebornLocation)
{
	BornLocation = RebornLocation;
	BornWorldOrigin = GetWorld()->OriginLocation;
}

FVector ACharacterBase::GetRebornLocation()
{
	return UARPG_LevelFunctionLibrary::GetFixedWorldLocation(this, BornWorldOrigin, BornLocation);
}

void ACharacterBase::ARPG_InputPressed(EARPG_InputType InputType)
{
	InputBuffer.InputPressed(InputType);
}

void ACharacterBase::ARPG_InputReleased(EARPG_InputType InputType)
{
	InputBuffer.InputReleased(InputType);
}

bool ACharacterBase::ARPG_InputIsPressed(int32 InputType) const
{
	return InputBuffer.IsPressed(InputType);
}

bool ACharacterBase::ARPG_InputIsReleased(int32 InputType) const
{
	return InputBuffer.IsReleased(InputType);
}

bool ACharacterBase::ARPG_AnyInputIsPressed(int32 InputType) const
{
	return InputBuffer.IsAnyPressed(InputType);
}

bool ACharacterBase::ARPG_AllInputIsReleased(int32 InputType) const
{
	return InputBuffer.IsAllReleased(InputType);
}

void ACharacterBase::InvokeChangeMoveGait(ECharacterGait Gait)
{
	if (ARPG_MovementComponent->CurrentGait != Gait)
	{
		InvokeChangeMoveGaitToServer_Implementation(Gait);
		InvokeChangeMoveGaitToServer(Gait);
	}
}

void ACharacterBase::InvokeChangeMoveGaitToServer_Implementation(const ECharacterGait& Gait)
{
	ARPG_MovementComponent->bInvokeSprint = (Gait == ECharacterGait::Sprinting);
	ARPG_MovementComponent->SetGait(Gait);
}

void ACharacterBase::StopMovement()
{
	if (Controller)
	{
		Controller->StopMovement();
	}
	if (CharacterTurnAction && CharacterTurnAction->bIsExecuting)
	{
		CharacterTurnAction->AbortTurnTo(this, {});
	}
	UARPG_ActorMoveUtils::StopActorMove(this);
}

FVector ACharacterBase::GetMovementInputVector() const
{
	if (GetController()->IsPlayerController())
	{
		return UXD_MovementComponentFunctionLibrary::GetMovementInput(this);
	}
	else
	{
		return UXD_MovementComponentFunctionLibrary::GetPathFollowingInput(this);
	}
}

FVector ACharacterBase::GetTargetLocation_Implementation(const FName& CurLockSocketName) const
{
	return GetMesh()->GetSocketLocation(CurLockSocketName);
}

bool ACharacterBase::CanLockedOnTarget_Implementation(AController* Invoker, const FName& InvokeLockedSocketName) const
{
	bool TryLockAgain;
	if (CanLockingOnTarget_Implementation(Invoker, InvokeLockedSocketName, TryLockAgain))
	{
		if (CanLockedSocketNames.Contains(InvokeLockedSocketName))
		{
			return true;
		}
	}
	return false;
}

bool ACharacterBase::CanLockingOnTarget_Implementation(AController* Invoker, const FName& CurLockSocketName, bool& TryLockAgain) const
{
	if (APawn* Pawn = Invoker->GetPawn())
	{
		if ((Pawn->GetActorLocation() - GetActorLocation()).Size() > MaxLockingDistance)
		{
			TryLockAgain = false;
			return false;
		}

		return true;
	}
	return false;
}

void ACharacterBase::SetLockedTarget(AActor* NewLockedTarget)
{
	bIsLockingOther = NewLockedTarget ? true : false;
	if (AAIController* AIController = Cast<AAIController>(GetController()))
	{
		AIController->SetFocus(NewLockedTarget);
	}
}

float ACharacterBase::PlayMontage(UAnimMontage* MontageToPlay, const FARPG_MontagePlayConfig& Config, float InPlayRate /*= 1.f*/, FName StartSectionName /*= NAME_None*/, bool ClientMaster /*= true*/)
{
	if (MontageToPlay)
	{
		if (HasAuthority())
		{
			MulticastPlayMontage(MontageToPlay, Config, InPlayRate, StartSectionName);
		}
		else if (ClientMaster)
		{
			if (HasAuthority())
			{
				if (GetController() && GetController()->IsLocalController())
				{
					MulticastPlayMontage(MontageToPlay, Config, InPlayRate, StartSectionName);
				}
			}
			else
			{
				PlayMontageImpl(MontageToPlay, Config, InPlayRate, StartSectionName);
				PlayMontageToServer(MontageToPlay, Config, InPlayRate, StartSectionName);
			}
		}
		else
		{
			//客户端先行播放
			PlayMontageImpl(MontageToPlay, Config, InPlayRate, StartSectionName);
		}
		return MontageToPlay->GetPlayLength();
	}
	return 0.f;
}

float ACharacterBase::PlayMontageImpl(UAnimMontage* MontageToPlay, const FARPG_MontagePlayConfig& Config, float InPlayRate /*= 1.f*/, FName StartSectionName /*= NAME_None*/)
{
	UAnimInstance * AnimInstance = GetMesh()->GetAnimInstance();
	if (MontageToPlay && AnimInstance)
	{
		float const Duration = AnimInstance->Montage_Play(MontageToPlay, InPlayRate, EMontagePlayReturnType::MontageLength, 0.f, true);
		if (Config.bMirrorMontage)
		{
			SetMirrorFullBodyMontage(true);
			AddMontageWithBlendingOutDelegate(MontageToPlay, FOnMontageBlendingOutStarted::CreateWeakLambda(this, [=](UAnimMontage* Montage, bool bInterrupted)
			{
				SetMirrorFullBodyMontage(false);
			}));
		}
		if (Duration > 0.f)
		{
			// Start at a given Section.
			if (StartSectionName != NAME_None)
			{
				AnimInstance->Montage_JumpToSection(StartSectionName, MontageToPlay);
			}

			return Duration;
		}
	}

	return 0.f;
}

void ACharacterBase::MulticastPlayMontage_Implementation(UAnimMontage* MontageToPlay, const FARPG_MontagePlayConfig& Config, float InPlayRate /*= 1.f*/, FName StartSectionName /*= NAME_None*/)
{
	PlayMontageImpl(MontageToPlay, Config, InPlayRate, StartSectionName);
}

void ACharacterBase::MulticastPlayMontageSkipOwner_Implementation(UAnimMontage* MontageToPlay, const FARPG_MontagePlayConfig& Config, float InPlayRate /*= 1.f*/, FName StartSectionName /*= NAME_None*/)
{
	if (IsLocallyControlled() == false || GetMesh()->GetAnimInstance()->Montage_IsPlaying(MontageToPlay) == false)
	{
		PlayMontageImpl(MontageToPlay, Config, InPlayRate, StartSectionName);
	}
}

void ACharacterBase::StopMontage_Implementation(UAnimMontage* MontageToStop)
{
	StopAnimMontage(MontageToStop);
}

void ACharacterBase::PlayMontageToServer_Implementation(UAnimMontage* MontageToPlay, const FARPG_MontagePlayConfig& Config, float InPlayRate /*= 1.f*/, FName StartSectionName /*= NAME_None*/)
{
	MulticastPlayMontageSkipOwner(MontageToPlay, Config, InPlayRate, StartSectionName);
}

void ACharacterBase::TryPlayMontage(const FARPG_MontageParameter& Montage, const FARPG_MontagePlayConfig& Config)
{
	if (Montage.Condition == nullptr || Montage.Condition.GetDefaultObject()->CanPlayMontage(this))
	{
		PlayMontage(Montage.Montage, Config, 1.f, Montage.StartSectionName, Montage.bClientMaster);
	}
}

bool ACharacterBase::CanPlayFullBodyMontage() const
{
	if (UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance())
	{
		return AnimInstance->GetSlotMontageGlobalWeight(FullBodySlotName) == 0.f;
	}
	return false;
}

void ACharacterBase::EnableAutoUpdateLookAtRotation(bool Enable)
{
	ARPG_MovementComponent->bAutoUpdateControlRotation = Enable;
}

void ACharacterBase::SetLookAtRotation(const FRotator& LookAtRotation)
{
	ARPG_MovementComponent->ControlRotation = LookAtRotation;
}

void ACharacterBase::SetLookAtLocation(const FVector& WorldLocation)
{
	ARPG_MovementComponent->ControlRotation = (WorldLocation - GetPawnViewLocation()).Rotation();
}

FRotator ACharacterBase::GetLookAtRotation() const
{
	return ARPG_MovementComponent->ControlRotation;
}

void ACharacterBase::InvokeDodge()
{
	if (ARPG_InputIsPressed(ARPG_InputType::ToBitMask(EARPG_InputType::Dodge)))
	{
		if (ARPG_InputIsPressed(ARPG_InputType::ToBitMask(EARPG_InputType::Forward)))
		{
			DodgeByControlRotation(0.f);
		}
		else if (ARPG_InputIsPressed(ARPG_InputType::ToBitMask(EARPG_InputType::Backward)))
		{
			DodgeByControlRotation(-180.f);
		}
		else if (ARPG_InputIsPressed(ARPG_InputType::ToBitMask(EARPG_InputType::Left)))
		{
			DodgeByControlRotation(-90.f);
		}
		else if (ARPG_InputIsPressed(ARPG_InputType::ToBitMask(EARPG_InputType::Right)))
		{
			DodgeByControlRotation(90.f);
		}
	}
}

void ACharacterBase::DodgeByControlRotation(float Direction)
{
	FRotator ControllerRotation = GetControlRotation();
	ControllerRotation.Yaw += Direction;
	float DodgeDirection = (GetActorRotation() - ControllerRotation).GetNormalized().Yaw;
	if (DodgeDirection >= -45.f && DodgeDirection <= 45.f)
	{
		InvokeDodgeByDirection(EDodgeDirection::Forward);
	}
	else if (DodgeDirection >= -135.f && DodgeDirection < -45.f)
	{
		InvokeDodgeByDirection(EDodgeDirection::Right);
	}
	else if (DodgeDirection > 45.f && DodgeDirection <= 135.f)
	{
		InvokeDodgeByDirection(EDodgeDirection::Left);
	}
	else
	{
		InvokeDodgeByDirection(EDodgeDirection::Backword);
	}
}

void ACharacterBase::InvokeDodgeByDirection(EDodgeDirection Direction)
{
	if (DodgeAnimSet)
	{
		DodgeAnimSet->InvokeDodge(this, Direction);
	}
}

bool ACharacterBase::CanDodge() const
{
	return DodgeAnimSet && DodgeAnimSet->CanDodge(this);
}

void ACharacterBase::SetRootMotionTranslationScale(const FVector& Scale)
{
	SetAnimRootMotionTranslationScale(Scale.GetAbsMax());
	RootMotionScale = Scale;
}

FVector ACharacterBase::GetRootMotionTranslationScale() const
{
	return RootMotionScale;
}

void ACharacterBase::InvokeThrowItem_Implementation(UARPG_ItemCoreBase* ItemCore, int32 Number /*= 1*/)
{
	Inventory->ThrowItemCore(ItemCore, Number);
}

bool ACharacterBase::InvokeThrowItem_Validate(UARPG_ItemCoreBase* ItemCore, int32 Number /*= 1*/)
{
	return true;
}

UARPG_CharacterBehaviorBase* ACharacterBase::EnterReleaseState(const FOnCharacterBehaviorFinished& OnBehaviorFinished)
{
	if (EnterReleaseStateAction && IsInReleaseState() == false)
	{
		EnterReleaseStateAction->EnterReleaseState(this, OnBehaviorFinished);
		return EnterReleaseStateAction;
	}
	else
	{
		OnBehaviorFinished.ExecuteIfBound(true);
		return nullptr;
	}
}

bool ACharacterBase::CanTurnTo() const
{
	return GetMesh()->GetAnimInstance()->GetSlotMontageGlobalWeight(TurnSlotName) < 0.5f && IsPlayingRootMotion() == false && CharacterTurnAction && CharacterTurnAction->CanTurnTo(this);
}

void ACharacterBase::TurnTo(const FRotator& TargetWorldRotation, const FOnCharacterBehaviorFinished& OnCharacterBehaviorFinished, bool ForceSnapRotation)
{
	auto SnapAction = [=]()
	{
		if (FMath::Abs(GetActorRotation().Yaw - TargetWorldRotation.Yaw) > 2.f)
		{
			UARPG_ActorMoveUtils::MoveCharacterToRotationFitGround(this, TargetWorldRotation, FOnActorMoveFinished::CreateLambda([=](bool bIsAborted) mutable
				{
					OnCharacterBehaviorFinished.ExecuteIfBound(!bIsAborted);
				}), 0.1f);
		}
		else
		{
			OnCharacterBehaviorFinished.ExecuteIfBound(true);
		}
	};
	if (CanTurnTo())
	{
		FOnCharacterBehaviorFinished OnFinish = ForceSnapRotation ? FOnCharacterBehaviorFinished::CreateWeakLambda(this, [=](bool Succeed)
			{
				if (Succeed)
				{
					SnapAction();
				}
				else
				{
					OnCharacterBehaviorFinished.ExecuteIfBound(false);
				}
			}) : OnCharacterBehaviorFinished;
		CharacterTurnAction->TurnTo(this, TargetWorldRotation, OnFinish);
	}
	else
	{
		SnapAction();
	}
}

void ACharacterBase::TurnTo(const FRotator& TargetWorldRotation, bool ForceSnapRotation)
{
	TurnTo(TargetWorldRotation, {}, ForceSnapRotation);
}

UAudioComponent* ACharacterBase::GetMouthComponent()
{
	if (!MouthComponentRuntime)
	{
		MouthComponentRuntime = NewObject<UAudioComponent>(this, TEXT("Mouth"), EObjectFlags::RF_Transient);
		AddOwnedComponent(MouthComponentRuntime);
		MouthComponentRuntime->RegisterComponent();
	}
	return MouthComponentRuntime;
}

void ACharacterBase::Speak(USoundBase* Sentence, const FOnCharacterBehaviorFinished& OnSpeakFinished)
{
	//绑定事件
	UAudioComponent* MouthComponent = GetMouthComponent();
	if (MouthComponent->IsPlaying())
	{
		OnSpeakFinishedDelegate.ExecuteIfBound(false);
	}
	else
	{
		MouthComponent->OnAudioFinishedNative.AddWeakLambda(this, [=](UAudioComponent* AudioComponent)
			{
				MouthComponent->OnAudioFinishedNative.RemoveAll(this);
				//最后再执行，否则可能执行流中又向OnAudioFinishedNative绑定了委托
				if (OnSpeakFinishedDelegate.IsBound())
				{
					auto CachedDelegate = OnSpeakFinishedDelegate;
					OnSpeakFinishedDelegate.Unbind();
					CachedDelegate.Execute(true);
				}
			});
	}
	OnSpeakFinishedDelegate = OnSpeakFinished;

	//开始说话
	SpeakToAllClient(Sentence);
}

void ACharacterBase::StopSpeak()
{
	StopSpeakToAllClient();
}

void ACharacterBase::StopSpeakToAllClient_Implementation()
{
	if (MouthComponentRuntime)
	{
		MouthComponentRuntime->Stop();
	}
}

bool ACharacterBase::StopSpeakToAllClient_Validate()
{
	return true;
}

void ACharacterBase::SpeakToAllClient_Implementation(USoundBase* Sentence)
{
	UAudioComponent* MouthComponent = GetMouthComponent();
	if (MouthComponent->IsPlaying())
	{
		MouthComponent->Stop();
	}
	MouthComponent->SetSound(Sentence);
	MouthComponent->Play();
}

bool ACharacterBase::SpeakToAllClient_Validate(USoundBase* Sentence)
{
	return true;
}

UPathFollowingComponent* ACharacterBase::GetPathFollowingComponent()
{
	AController* OwingController = GetController();
	if (PathFollowingComponent == nullptr)
	{
		if (const AAIController * AIController = Cast<const AAIController>(OwingController))
		{
			PathFollowingComponent = AIController->GetPathFollowingComponent();
		}
		else if (const AARPG_PlayerControllerBase * PlayerController = Cast<const AARPG_PlayerControllerBase>(OwingController))
		{
			PathFollowingComponent = (UPathFollowingComponent*)PlayerController->PathFollowingComponent;
		}
		else
		{
			PathFollowingComponent = OwingController->FindComponentByClass<UPathFollowingComponent>();
		}
	}
	return PathFollowingComponent;
}

void ACharacterBase::PauseMove()
{
	GetPathFollowingComponent()->PauseMove(FAIRequestID::CurrentRequest, EPathFollowingVelocityMode::Reset);
}

void ACharacterBase::ResumeMove()
{
	GetPathFollowingComponent()->ResumeMove(FAIRequestID::CurrentRequest);
}

void ACharacterBase::ForceSetClientWorldLocation(const FVector& Location)
{
	ForceSetClientWorldLocationImpl(FRepMovement::RebaseOntoZeroOrigin(Location, this));
}

void ACharacterBase::ForceSetClientWorldLocationAndRotation(const FVector& Location, const FRotator& Rotation)
{
	ForceSetClientWorldLocationAndRotationImpl(FRepMovement::RebaseOntoZeroOrigin(Location, this), Rotation);
}

void ACharacterBase::AddStateTag(FGameplayTag Tag)
{
	StateTags.AddTag(Tag);
}

void ACharacterBase::RemoveStateTag(FGameplayTag Tag)
{
	StateTags.RemoveTag(Tag);
}

bool ACharacterBase::HasStateTag(FGameplayTag Tag) const
{
	return StateTags.HasTag(Tag);
}

bool ACharacterBase::AD_HasStateTag_Implementation(const FGameplayTag& Tag) const
{
	return HasStateTag(Tag);
}

void ACharacterBase::AD_AddStateTag_Implementation(const FGameplayTag& Tag)
{
	AddStateTag(Tag);
}

void ACharacterBase::ForceSetClientWorldLocationImpl_Implementation(const FVector& Location)
{
	SetActorLocation(FRepMovement::RebaseOntoLocalOrigin(Location, this));
}

bool ACharacterBase::ForceSetClientWorldLocationImpl_Validate(const FVector& Location)
{
	return true;
}

void ACharacterBase::ForceSetClientWorldRotation_Implementation(const FRotator& Rotation)
{
	SetActorRotation(Rotation);
}

bool ACharacterBase::ForceSetClientWorldRotation_Validate(const FRotator& Rotation)
{
	return true;
}

void ACharacterBase::ForceSetClientWorldLocationAndRotationImpl_Implementation(const FVector& Location, const FRotator& Rotation)
{
	SetActorLocationAndRotation(FRepMovement::RebaseOntoLocalOrigin(Location, this), Rotation);
}

bool ACharacterBase::ForceSetClientWorldLocationAndRotationImpl_Validate(const FVector& Location, const FRotator& Rotation)
{
	return true;
}

void ACharacterBase::PlayMontageWithBlendingOutDelegate(UAnimMontage* Montage, const FOnMontageBlendingOutStarted& OnMontageBlendingOutStarted, const FARPG_MontagePlayConfig& Config, float InPlayRate /*= 1.f*/, FName StartSectionName /*= NAME_None*/, bool ClientMaster /*= false*/)
{
	PlayMontage(Montage, Config, InPlayRate, StartSectionName, ClientMaster);
	AddMontageWithBlendingOutDelegate(Montage, OnMontageBlendingOutStarted);
}

void ACharacterBase::AddMontageWithBlendingOutDelegate(UAnimMontage* Montage, const FOnMontageBlendingOutStarted& OnMontageBlendingOutStarted)
{
	FOnMontageBlendingOutStarted* P_PreOnMontageBlendingOutStarted = GetMesh()->GetAnimInstance()->Montage_GetBlendingOutDelegate(Montage);
	if (P_PreOnMontageBlendingOutStarted && P_PreOnMontageBlendingOutStarted->IsBound())
	{
		FOnMontageBlendingOutStarted OnMontageBlendingOutStartedCombined = FOnMontageBlendingOutStarted::CreateWeakLambda(this, [=, PreOnMontageBlendingOutStarted = *P_PreOnMontageBlendingOutStarted](UAnimMontage* Montage, bool bInterrupted)
		{
			PreOnMontageBlendingOutStarted.Execute(Montage, bInterrupted);
			OnMontageBlendingOutStarted.Execute(Montage, bInterrupted);
		});
		GetMesh()->GetAnimInstance()->Montage_SetBlendingOutDelegate(OnMontageBlendingOutStartedCombined, Montage);
	}
	else
	{
		GetMesh()->GetAnimInstance()->Montage_SetBlendingOutDelegate(const_cast<FOnMontageBlendingOutStarted&>(OnMontageBlendingOutStarted), Montage);
	}
}

void ACharacterBase::ClearMontageBlendingOutDelegate(UAnimMontage* Montage)
{
	if (FOnMontageBlendingOutStarted* OnMontageBlendingOutStarted = GetMesh()->GetAnimInstance()->Montage_GetBlendingOutDelegate(Montage))
	{
		OnMontageBlendingOutStarted->Unbind();
	}
}

void ACharacterBase::SetMirrorFullBodyMontage(bool IsMirror)
{
	MirrorFullBodyMontageCounter += IsMirror ? 1 : -1;
	bMirrorFullBodyMontage = MirrorFullBodyMontageCounter > 0;
	check(MirrorFullBodyMontageCounter >= 0 && MirrorFullBodyMontageCounter <= 2);
}

void ACharacterBase::InvokeMoveItem_Implementation(UARPG_InventoryComponent* SourceInventory, UARPG_InventoryComponent* TargetInventory, class UARPG_ItemCoreBase* ItemCore, int32 Number /*= 1*/)
{
	if (SourceInventory && TargetInventory)
	{
		TargetInventory->GetItemFromOther(SourceInventory, ItemCore, Number);
	}
}

bool ACharacterBase::InvokeMoveItem_Validate(UARPG_InventoryComponent* SourceInventory, UARPG_InventoryComponent* TargetInventory, class UARPG_ItemCoreBase* ItemCore, int32 Number /*= 1*/)
{
	return true;
}

void ACharacterBase::InvokeBuyItem_Implementation(UARPG_InventoryComponent* BuyFromInventory, class UARPG_ItemCoreBase* ItemCore, int32 Number /*= 1*/)
{
	if (ItemCore)
	{
		Inventory->BuyItemFromOther(BuyFromInventory, ItemCore, Number);
	}
}

bool ACharacterBase::InvokeBuyItem_Validate(UARPG_InventoryComponent* BuyFromInventory, class UARPG_ItemCoreBase* ItemCore, int32 Number /*= 1*/)
{
	return true;
}

void ACharacterBase::InvokeSellItem_Implementation(UARPG_InventoryComponent* SellToInventory, class UARPG_ItemCoreBase* ItemCore, int32 Number /*= 1*/)
{
	if (ItemCore)
	{
		Inventory->SellItemToOther(SellToInventory, ItemCore, Number);
	}
}

bool ACharacterBase::InvokeSellItem_Validate(UARPG_InventoryComponent* SellToInventory, class UARPG_ItemCoreBase* ItemCore, int32 Number /*= 1*/)
{
	return true;
}

void ACharacterBase::OpenMoveItemPanel_Implementation(UARPG_InventoryComponent* OtherInventory)
{
	if (OtherInventory == nullptr)
	{
		return;
	}

	if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		if (AARPG_HUDBase* HUD = Cast<AARPG_HUDBase>(PlayerController->GetHUD()))
		{
			HUD->OpenMoveItemPanel(OtherInventory);
		}
	}
}

bool ACharacterBase::OpenMoveItemPanel_Validate(UARPG_InventoryComponent* OtherInventory)
{
	return true;
}

void ACharacterBase::OpenTradeItemPanel_Implementation(UARPG_InventoryComponent* OtherInventory)
{
	if (OtherInventory == nullptr)
	{
		return;
	}

	if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		if (AARPG_HUDBase* HUD = Cast<AARPG_HUDBase>(PlayerController->GetHUD()))
		{
			HUD->OpenTradeItemPanel(OtherInventory);
		}
	}
}

bool ACharacterBase::OpenTradeItemPanel_Validate(UARPG_InventoryComponent* OtherInventory)
{
	return true;
}

void ACharacterBase::CloseMoveItemPanel_Implementation()
{
	if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		if (AARPG_HUDBase* HUD = Cast<AARPG_HUDBase>(PlayerController->GetHUD()))
		{
			HUD->CloseMoveItemPanel();
		}
	}
}

bool ACharacterBase::CloseMoveItemPanel_Validate()
{
	return true;
}

void ACharacterBase::WhenCloseMoveItemPanel_Implementation()
{
	OnCloseMoveItemPanelNative.Broadcast();
}

bool ACharacterBase::WhenCloseMoveItemPanel_Validate()
{
	return true;
}

void ACharacterBase::CloseTradeItemPanel_Implementation()
{
	if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		if (AARPG_HUDBase* HUD = Cast<AARPG_HUDBase>(PlayerController->GetHUD()))
		{
			HUD->CloseTradeItemPanel();
		}
	}
}

bool ACharacterBase::CloseTradeItemPanel_Validate()
{
	return true;
}

void ACharacterBase::WhenCloseTardeItemPanel_Implementation()
{
	OnCloseTradeItemPanelNative.Broadcast();
}

bool ACharacterBase::WhenCloseTardeItemPanel_Validate()
{
	return true;
}

void ACharacterBase::InvokeUseItem(const class UARPG_ItemCoreBase* ItemCore, EUseItemInput UseItemInput /*= EUseItemInput::LeftMouse*/)
{
	if (ARPG_MovementComponent->IsMovingOnGround() && CanPlayFullBodyMontage())
	{
		InvokeUseItem_Server(ItemCore, UseItemInput);
	}
}

void ACharacterBase::InvokeUseItem_Server_Implementation(const class UARPG_ItemCoreBase* ItemCore, EUseItemInput UseItemInput)
{
	if (ARPG_MovementComponent->IsMovingOnGround() && CanPlayFullBodyMontage())
	{
		if (ItemCore->UseItemMontage)
		{
			PendingUseItem = CastChecked<AARPG_ItemBase>(ItemCore->SpawnItemActorForOwner(this, this));
			PendingUseItem->SetItemSimulatePhysics(false);
			PendingUseItem->SetActorHiddenInGame(true);
			PendingUseItem->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, ItemCore->UseItemAttachSocketName);
			PlayMontageWithBlendingOutDelegate(ItemCore->UseItemMontage, FOnMontageBlendingOutStarted::CreateWeakLambda(this, [=](UAnimMontage* Montage, bool bInterrupted)
				{
					if (PendingUseItem)
					{
						if (bInterrupted)
						{
							PendingUseItem->Destroy();
						}
						else
						{
							Item_Warning_LOG("%s的动画%s已经播放完，但是角色还引用该道具，执行销毁", *UXD_DebugFunctionLibrary::GetDebugName(this), *UXD_DebugFunctionLibrary::GetDebugName(Montage));
							PendingUseItem->Destroy();
						}
					}
				}), {}, 1.f, NAME_None, false);
		}
		else
		{
			UseItemImmediately(ItemCore, UseItemInput);
		}
	}
}

void ACharacterBase::UseItemImmediately(const class UARPG_ItemCoreBase* ItemCore, EUseItemInput UseItemInput)
{
	if (!ItemCore)
		return;
	for (UARPG_ItemCoreBase* E_ItemCore : Inventory->GetItemCoreList())
	{
		if (E_ItemCore->IsEqualWithItemCore(ItemCore))
		{
			OnUseItem.Broadcast(this, ItemCore, UseItemInput);
			E_ItemCore->UseItem(this, UseItemInput);
			break;
		}
	}
}

void ACharacterBase::NearAttackSuccessTimeDilation(float Duration)
{
	if (!GetWorld()->GetTimerManager().IsTimerActive(AttackSuccessTimeDilation_TimeHandle))
	{
		CustomTimeDilation *= AttackSuccessTimeDilationRate;
	}
	GetWorld()->GetTimerManager().SetTimer(AttackSuccessTimeDilation_TimeHandle, this, &ACharacterBase::RecoverAttackSuccessTimeDilation, Duration, false);
}

void ACharacterBase::RecoverAttackSuccessTimeDilation()
{
	CustomTimeDilation /= AttackSuccessTimeDilationRate;
}

void ACharacterBase::WhenDodgeSucceed(float BaseDamage, class ACharacterBase* InstigatedBy, const FHitResult& HitResult)
{
	Battle_Display_LOG("%s成功闪避%s的攻击", *UARPG_DebugFunctionLibrary::GetDebugName(this), *UARPG_DebugFunctionLibrary::GetDebugName(InstigatedBy));
	ReceiveWhenDodgeSucceed(BaseDamage, InstigatedBy, HitResult);
	OnDodgeSucceed.Broadcast(this, BaseDamage, InstigatedBy, HitResult);
}

bool ACharacterBase::IsDefenseSucceed_Implementation(const FVector& DamageFromLocation, const FHitResult& HitInfo) const
{
	return bIsDefense;
}

void ACharacterBase::WhenDefenseSucceed(float BaseDamage, class ACharacterBase* InstigatedBy, const FVector& HitFromDirection, float DefenseBeakBackDistance, const FHitResult& HitResult)
{
	Battle_Display_LOG("%s成功防御%s的攻击", *UARPG_DebugFunctionLibrary::GetDebugName(this), *UARPG_DebugFunctionLibrary::GetDebugName(InstigatedBy));
	ReceiveWhenDefenseSucceed(BaseDamage, InstigatedBy, HitFromDirection, DefenseBeakBackDistance, HitResult);
	OnDefenseSucceed.Broadcast(this, BaseDamage, InstigatedBy, HitResult);

	//击退效果
	FVector BeakBackOffset = HitFromDirection.GetSafeNormal2D() * DefenseBeakBackDistance;
	UARPG_ActorMoveUtils::PushActorTo(this, BeakBackOffset);
}

void ACharacterBase::WhenAttackedDefenseCharacter(float BaseDamage, ACharacterBase* DefenseSucceedCharacter, const FHitResult& HitResult)
{
	ReceiveWhenAttackedDefenseCharacter(BaseDamage, DefenseSucceedCharacter, HitResult);
}

bool ACharacterBase::IsDefenseSwipeSucceed_Implementation(const FVector& DamageFromLocation, const FHitResult& HitInfo) const
{
	return bIsDefenseSwipe;
}

float ACharacterBase::AddHitStun(float Value)
{
	HitStunValue += Value;
	if (HitStunValue >= ToughnessValue)
	{
		float OverflowValue = HitStunValue - ToughnessValue;
		HitStunValue = 0;
		GetWorld()->GetTimerManager().ClearTimer(ClearHitStun_TimeHandle);
		return OverflowValue;
	}
	GetWorld()->GetTimerManager().SetTimer(ClearHitStun_TimeHandle, this, &ACharacterBase::ClearHitStun, HitStunClearTime);
	return -1;
}

void ACharacterBase::ClearHitStun()
{
	HitStunValue = 0;
}

bool ACharacterBase::CanBeBackstab(ACharacterBase* BackstabInvoker) const
{
	if (GetCharacterMovement()->IsMovingOnGround())
	{
		FRotator Rotation = UKismetMathLibrary::FindLookAtRotation(BackstabInvoker->GetActorLocation(), GetActorLocation());
		return FMath::Abs(FRotator::NormalizeAxis(GetActorRotation().Yaw - Rotation.Yaw)) <= 45.f;
	}
	return false;
}

void ACharacterBase::ExecuteOther(ACharacterBase* ExecuteTarget, const FVector& TargetLocation, const FRotator& TargetRotation, UAnimMontage* ExecuteMontage, UAnimMontage* BeExecutedMontage)
{
	if (CanPlayFullBodyMontage())
	{
		if (!HasAuthority())
		{
			ExecuteOtherToServer_Implementation(ExecuteTarget, TargetLocation, TargetRotation, ExecuteMontage, BeExecutedMontage);
		}
		ExecuteOtherToServer(ExecuteTarget, TargetLocation, TargetRotation, ExecuteMontage, BeExecutedMontage);
	}
}

void ACharacterBase::ExecuteOtherToServer_Implementation(ACharacterBase* ExecuteTarget, const FVector& TargetLocation, const FRotator& TargetRotation, UAnimMontage* ExecuteMontage, UAnimMontage* BeExecutedMontage)
{
	if (ExecuteTarget && CanPlayFullBodyMontage())
	{
		ExecuteTargetCharacter = ExecuteTarget;
		ExecuteTarget->ExecuteFromCharacter = this;

		if (HasAuthority())
		{
			Battle_Display_LOG("%s对%s进行处决", *UXD_DebugFunctionLibrary::GetDebugName(this), *UXD_DebugFunctionLibrary::GetDebugName(ExecuteTarget));
		}
		UARPG_ActorMoveUtils::MoveActorTo(ExecuteTarget, TargetLocation, TargetRotation);
		PlayMontageWithBlendingOutDelegate(ExecuteMontage, FOnMontageBlendingOutStarted::CreateWeakLambda(ExecuteTarget, [=](UAnimMontage* Montage, bool bInterrupted)
			{
				if (bInterrupted)
				{
					ExecuteTarget->StopMontage(BeExecutedMontage);
				}
				WhenExecuteEnd();
			}), {}, 1.f, NAME_None, false);
		ExecuteTarget->PlayMontageWithBlendingOutDelegate(BeExecutedMontage, FOnMontageBlendingOutStarted::CreateWeakLambda(this, [=](UAnimMontage* Montage, bool bInterrupted)
			{
				if (bInterrupted)
				{
					StopMontage(ExecuteMontage);
				}
				WhenExecuteEnd();
			}), {}, 1.f, NAME_None, false);
	}
}

void ACharacterBase::WhenExecuteEnd()
{
	if (ExecuteTargetCharacter)
	{
		if (HasAuthority())
		{
			Battle_Display_LOG("%s对%s处决结束", *UXD_DebugFunctionLibrary::GetDebugName(this), *UXD_DebugFunctionLibrary::GetDebugName(ExecuteTargetCharacter));
		}
		ExecuteTargetCharacter->ExecuteFromCharacter = nullptr;
		ExecuteTargetCharacter = nullptr;
	}
}

void ACharacterBase::WhenKillOther(ACharacterBase* WhoBeKilled, UObject* KillInstigator)
{
	Battle_Display_LOG("%s杀死了%s", *UARPG_DebugFunctionLibrary::GetDebugName(this), *UARPG_DebugFunctionLibrary::GetDebugName(WhoBeKilled));
	ReceiveWhenKillOther(WhoBeKilled, KillInstigator);
	OnKillOther.Broadcast(this, WhoBeKilled, KillInstigator);
}

void ACharacterBase::WhenDamagedOther(ACharacterBase* WhoBeDamaged, float DamageValue, UObject* DamageInstigator)
{
	ensure(DamageValue > 0.f);

	Battle_Display_LOG("%s对%s造成了[%f]点伤害", *UARPG_DebugFunctionLibrary::GetDebugName(this), *UARPG_DebugFunctionLibrary::GetDebugName(WhoBeDamaged), DamageValue);
	ReceiveWhenDamagedOther(WhoBeDamaged, DamageValue, DamageInstigator);
	OnDamagedOther.Broadcast(this, WhoBeDamaged, DamageValue, DamageInstigator);
}

float ACharacterBase::ApplyPointDamage(float BaseDamage, const FVector& HitFromDirection, const FHitResult& HitInfo, class ACharacterBase* InstigatorBy, AActor* DamageCauser, TSubclassOf<class UDamageType> DamageTypeClass, const FApplyPointDamageParameter& Param)
{
	float FinalReduceValue = BaseDamage;

	if (!bCanBeDamaged || FinalReduceValue <= 0.f)
	{
		return 0.f;
	}

	if (InstigatorBy)
	{
		if (ACharacterBase* InstigatorPawn = Cast<ACharacterBase>(InstigatorBy))
		{
			//闪避
			if (Param.bCanDodge && bIsDodging)
			{
				WhenDodgeSucceed(FinalReduceValue, InstigatorBy, HitInfo);
				return 0.f;
			}
			//防御反击
			else if (Param.bCanDefenseSwipe && IsDefenseSwipeSucceed(InstigatorPawn->GetActorLocation(), HitInfo))
			{
				return 0.f;
			}
			//防御
			else if (Param.bCanDefense && IsDefenseSucceed(InstigatorPawn->GetActorLocation(), HitInfo))
			{
				WhenDefenseSucceed(FinalReduceValue, InstigatorBy, HitFromDirection, Param.DefenseBeakBackDistance, HitInfo);
				InstigatorPawn->WhenAttackedDefenseCharacter(FinalReduceValue, this, HitInfo);
				return 0.f;
			}
		}
	}

	if (UARPG_ReceiveDamageActionBase* ReceiveDamageAction = GetReceiveDamageAction())
	{
		if (!(Param.ReceiveDamageAction && ReceiveDamageAction->PlayReceiveDamageSpecialAction(this, Param.ReceiveDamageAction, HitFromDirection, HitInfo, InstigatorBy, DamageCauser)))
		{
			float HitStunOverflowValue = AddHitStun(Param.AddHitStunValue);
			if (HitStunOverflowValue >= 0.f)
			{
				ReceiveDamageAction->PlayHitStunMontage(this, BaseDamage, HitStunOverflowValue, HitFromDirection, HitInfo, InstigatorBy, DamageCauser);
				OnDamageInterrupt.Broadcast();
				HitStunOverflowValue = 0.f;
			}
			else
			{
				ReceiveDamageAction->PlayNormalAdditiveDamageMontage(this, BaseDamage, HitFromDirection, HitInfo, InstigatorBy, DamageCauser);

				//击退效果，处于根骨骼位移时不动
				if (!IsPlayingRootMotion())
				{
					FVector BeakBackOffset = HitFromDirection.GetSafeNormal2D() * Param.NormalBeakBackDistance;
					UARPG_ActorMoveUtils::PushActorTo(this, BeakBackOffset);
				}
			}
		}
		else
		{
			OnDamageInterrupt.Broadcast();
		}
	}

	UGameplayStatics::ApplyPointDamage(this, FinalReduceValue, GetActorLocation(), HitInfo, InstigatorBy ? InstigatorBy->GetController() : nullptr, DamageCauser, DamageTypeClass);
	return FinalReduceValue;
}

bool ACharacterBase::IsSneaking() const
{
	return GetCharacterMovement()->IsCrouching();
}

void ACharacterBase::InvokeInteract(AActor* InteractTarget)
{
	if (CanInteractWithTarget(InteractTarget))
	{
		InvokeInteract_ToServer(InteractTarget);
	}
}

void ACharacterBase::InvokeInteract_ToServer_Implementation(AActor* InteractTarget)
{
	if (CanInteractWithTarget(InteractTarget))
	{
		InvokeInteractWithEndEvent(InteractTarget, {});
	}
}

bool ACharacterBase::InvokeInteract_ToServer_Validate(AActor* InteractTarget)
{
	return true;
}

void ACharacterBase::InvokeInteractWithEndEvent(AActor* InteractTarget, const FOnInteractEnd& OnInteractEndEvent)
{
	check(HasAuthority() && InteractTarget);
#if WITH_EDITOR
	checkf(PreInvokeInteractTarget.IsValid() == false, TEXT("PreInvokeInteractTarget 还存在，上一个交互未调用ExecuteInteractEnd或ExecuteInteractAbortEnd"));
	PreInvokeInteractTarget = InteractTarget;
#endif
	InvokeInteractTarget = InteractTarget;
	OnInteractEnd = OnInteractEndEvent;
	IARPG_InteractInterface::WhenInvokeInteract(InteractTarget, this);
}

bool ACharacterBase::CanInteractWithTarget(AActor* InteractTarget) const
{
	return InteractTarget && 
		InvokeInteractTarget == nullptr &&
		InteractingTarget == nullptr &&
		CanPlayFullBodyMontage() && 
		InteractTarget->Implements<UARPG_InteractInterface>() && 
		IARPG_InteractInterface::CanInteract(InteractTarget, this);
}

void ACharacterBase::InvokeAbortInteract()
{
	if (InvokeInteractTarget)
	{
		InvokeAbortInteract_ToServer();
	}
}

void ACharacterBase::InvokeAbortInteractWithAbortEvent(const FOnInteractAbortEnd& OnInteractAbortEndEvent)
{
	check(HasAuthority());

	OnInteractAbortEnd = OnInteractAbortEndEvent;
	IARPG_InteractInterface::WhenAbortInteract(InvokeInteractTarget, this);
}

void ACharacterBase::InvokeAbortInteract_ToServer_Implementation()
{
	if (InvokeInteractTarget)
	{
		InvokeAbortInteractWithAbortEvent({});
	}
}

bool ACharacterBase::InvokeAbortInteract_ToServer_Validate()
{
	return true;
}

void ACharacterBase::InvokeFinishPathFollowing_ToServer_Implementation()
{
	GetController()->StopMovement();
}

bool ACharacterBase::InvokeFinishPathFollowing_ToServer_Validate()
{
	return true;
}

void ACharacterBase::OnRep_InteractingTarget(AActor* PreInteractTarget)
{
	if (PreInteractTarget)
	{
		IARPG_InteractInterface::WhenEndInteract(PreInteractTarget, this);
	}
	if (InteractingTarget)
	{
		IARPG_InteractInterface::WhenBeginInteract(InteractingTarget, this);
	}
}

void ACharacterBase::SetInteractingTarget(AActor* InInteractingTarget)
{
	if (InInteractingTarget)
	{
		check(InteractingTarget == nullptr);
	}
	AActor* PreInteractTarget = InteractingTarget;
	InteractingTarget = InInteractingTarget;
	OnRep_InteractingTarget(PreInteractTarget);
}

void ACharacterBase::WhenExecuteInteract_Implementation(ACharacterBase* InteractInvoker)
{
	//TODO 呼叫InteractEndEvent
	if (OnPreInteractEvents.Num() > 0)
	{
		for (FOnPreInteractEvent& OnPreInteractEvent : OnPreInteractEvents)
		{
			if (OnPreInteractEvent.IsBound())
			{
				OnPreInteractEvent.Execute();
				break;
			}
		}
	}
	else if (InteractBehavior)
	{
		InteractInvoker->SetInteractingTarget(this);
		InteractBehavior->InteractTarget = InteractInvoker;
		if (InteractBehavior->CanStartDispatcher())
		{
			InteractBehavior->WhenDispatchFinishedNative = FWhenDispatchFinishedNative::CreateWeakLambda(InteractInvoker, [InteractInvoker](const FName & Tag)
				{
					InteractInvoker->ExecuteInteractEnd(EInteractEndResult::Succeed);
				});
			InteractBehavior->OnDispatcherAbortedNative = FOnDispatcherAbortedNative::CreateWeakLambda(InteractInvoker, [InteractInvoker]()
				{
					InteractInvoker->ExecuteInteractAbortEnd();
				});
			InteractBehavior->StartDispatch();
		}
	}
	else
	{
		InteractInvoker->ExecuteInteractEnd(EInteractEndResult::Failed);
	}
}

bool ACharacterBase::CanInteract_Implementation(const class ACharacterBase* InteractInvoker) const
{
	if (InteractInvoker == this)
	{
		return false;
	}
	if (CurrentMainDispatcher)
	{
		if (CurrentMainDispatcher->bInteractable == false || CurrentMainDispatcher->State != EActionDispatcherState::Deactive)
		{
			return false;
		}
	}
	if (GetRelationshipTowards(InteractInvoker) == ECharacterRelationship::Hostile)
	{
		return false;
	}

	return true;
}

void ACharacterBase::ExecuteInteractEnd(EInteractEndResult Result)
{
#if WITH_EDITOR
	PreInvokeInteractTarget = nullptr;
#endif
	InvokeInteractTarget = nullptr;
	SetInteractingTarget(nullptr);
	if (OnInteractEnd.IsBound())
	{
		OnInteractEnd.Execute(Result);
		OnInteractEnd.Unbind();
	}
}

void ACharacterBase::ExecuteInteractAbortEnd()
{
#if WITH_EDITOR
	PreInvokeInteractTarget = nullptr;
#endif
	InvokeInteractTarget = nullptr;
	SetInteractingTarget(nullptr);
	if (OnInteractAbortEnd.IsBound())
	{
		OnInteractAbortEnd.Execute();
		OnInteractAbortEnd.Unbind();
	}
}

class UARPG_CampInfo* ACharacterBase::GetCampInfo() const
{
	return Cast<UARPG_CampInfo>(CampConfig.GetCamp(this));
}

class UARPG_CampRelationship* ACharacterBase::GetCampRelationshipToward(ACharacterBase* Other) const
{
	return Other ? Cast<UARPG_CampRelationship>(GetCampInfo()->GetCampRelationshipRef(this, Other->GetCampInfo())) : nullptr;
}

ETeamAttitude::Type ACharacterBase::GetRelationshipTowards(const AActor* Actor) const
{
	if (const ACharacterBase* Character = Cast<ACharacterBase>(Actor))
	{
		if (UARPG_CampInfo* SelfCampInfo = GetCampInfo())
		{
			EXD_CampRelationship CampRelationship = SelfCampInfo->GetCampRelationship(this, Character->GetCampInfo());
			switch (CampRelationship)
			{
			case EXD_CampRelationship::SelfCamp:
				return ETeamAttitude::Friendly;
			case EXD_CampRelationship::Friend:
				return ETeamAttitude::Friendly;
			case EXD_CampRelationship::Neutral:
				return ETeamAttitude::Neutral;
			case EXD_CampRelationship::Hostile:
				return ETeamAttitude::Hostile;
			}
		}
	}
	return ETeamAttitude::Neutral;
}

ECharacterRelationship ACharacterBase::GetRelationshipTowards(const ACharacterBase* Towards) const
{
	float CampRelationShip = GetCampInfo()->GetCampRelationshipValue(this, Towards->GetCampInfo());

	//TODO : 添加角色关系
	float FinalRelationShip = CampRelationShip;
	if (FinalRelationShip > 75.f)
	{
		return ECharacterRelationship::Cordial;
	}
	else if (FinalRelationShip > 25.f)
	{
		return ECharacterRelationship::Friendly;
	}
	else if (FinalRelationShip > -25.f)
	{
		return ECharacterRelationship::Neutral;
	}
	else if (FinalRelationShip > -75.f)
	{
		return ECharacterRelationship::Hate;
	}
	else
	{
		return ECharacterRelationship::Hostile;
	}
}

void ACharacterBase::GetActorEyesViewPoint(FVector& out_Location, FRotator& out_Rotation) const
{
	GetMesh()->GetSocketWorldLocationAndRotation(HeadSocketName, out_Location, out_Rotation);
}

bool ACharacterBase::CanBeSeenFrom(const FVector& ObserverLocation, FVector& OutSeenLocation, int32& NumberOfLoSChecksPerformed, float& OutSightStrength, const AActor* IgnoreActor /*= NULL*/) const
{
	if (const ACharacterBase* SightListener = Cast<ACharacterBase>(IgnoreActor))
	{
		// we need to do tests ourselves
		FHitResult HitResult;
		const bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult, ObserverLocation, GetActorLocation()
			, SightCollisionChannel
			, FCollisionQueryParams(SCENE_QUERY_STAT(AILineOfSight), true, SightListener));

		if (HitResult.GetActor() == this)
		{
			float SightVigilanceValue = SightListener->AlertSystem->GetSightAddAlertValue(ObserverLocation, HitResult.ImpactPoint, this);
			if (SightVigilanceValue > 0.f)
			{
				NumberOfLoSChecksPerformed += 1;
				OutSightStrength = SightVigilanceValue;
				OutSeenLocation = HitResult.ImpactPoint;
				return true;
			}
		}
	}

	return false;
}

void ACharacterBase::WhenReceivedMoveRequest()
{

}

TArray<ACharacterBase*> ACharacterBase::GetSeeingCharacters() const
{
	if (AAIController* AIController = Cast<AAIController>(GetController()))
	{
		if (UAIPerceptionComponent* PerceptionComponent = AIController->GetPerceptionComponent())
		{
			TArray<ACharacterBase*> CurrentlyPerceivedCharacters = UAIPerceptionExLibrary::GetCurrentlyPerceivedActorsEx<ACharacterBase>(PerceptionComponent, UAISense_Sight::StaticClass());
			return CurrentlyPerceivedCharacters;
		}
	}
	return {};
}

TArray<ACharacterBase*> ACharacterBase::GetSeeingTraceCharacters() const
{
	if (AAIController * AIController = Cast<AAIController>(GetController()))
	{
		if (UAIPerceptionComponent * PerceptionComponent = AIController->GetPerceptionComponent())
		{
			TArray<ACharacterBase*> KnowPerceivedCharacters = UAIPerceptionExLibrary::GetKnownPerceivedActorsEx<ACharacterBase>(PerceptionComponent, UAISense_Sight::StaticClass());
			return UAIPerceptionExLibrary::FilterPerceivedActorsByMaxAge(PerceptionComponent, KnowPerceivedCharacters, UAISense_Sight::StaticClass(), SightMaxTraceTime);
		}
	}
	return {};
}

TArray<ACharacterBase*> ACharacterBase::GetSeeingTraceEnemies() const
{
	return UARPG_RelationshipFunctionLibrary::FilterByRelationshipLE_Towards(this, ACharacterBase::GetSeeingTraceCharacters(), MaxAlertRelationship);
}

void ACharacterBase::AddAlertValue(float AddValue)
{
	if (AddValue > 0.f)
	{
		if (AlertValue < AlertEntirelyValue)
		{
			if (AlertValue == 0.f)
			{
				BattleStyleSystem->WhenEnterAlertState();
			}

			AlertValue += AddValue;
			if (AlertValue >= AlertEntirelyValue)
			{
				AlertValue = AlertEntirelyValue;
				BattleStyleSystem->WhenEnterBattleState();

				GetWorld()->GetTimerManager().SetTimer(AlertSubsided_TimerHandle, this, &ACharacterBase::AlertSubsided, 1.f, true, AlertEntirelySubsidedDelay);
			}
			else
			{
				GetWorld()->GetTimerManager().SetTimer(AlertSubsided_TimerHandle, this, &ACharacterBase::AlertSubsided, 1.f, true, AlertNotEntirelySubsidedDelay);
			}
		}
		else
		{
			GetWorld()->GetTimerManager().SetTimer(AlertSubsided_TimerHandle, this, &ACharacterBase::AlertSubsided, 1.f, true, AlertNotEntirelySubsidedDelay);
		}
	}
}

void ACharacterBase::ReduceAlertValue(float ReduceValue)
{
	if (ReduceValue > 0.f && AlertValue > 0.f)
	{
		FTimerManager& TimerManager = GetWorld()->GetTimerManager();
		if (AlertValue == AlertEntirelyValue)
		{
			BattleStyleSystem->WhenLeaveBattleState();
		}

		AlertValue -= ReduceValue;
		if (AlertValue <= 0.f)
		{
			AlertValue = 0.f;
			BattleStyleSystem->WhenLeaveAlertState();

			GetWorld()->GetTimerManager().ClearTimer(AlertSubsided_TimerHandle);
		}
	}
}

void ACharacterBase::AlertSubsided()
{
	ReduceAlertValue(AlertSubsidedSpeed);
}

EAlertState ACharacterBase::GetAlertState() const
{
	if (AlertValue == 0.f)
	{
		return EAlertState::None;
	}
	else if (AlertValue == AlertEntirelyValue)
	{
		return EAlertState::AlertEntirely;
	}
	else
	{
		return EAlertState::Alerting;
	}
}

bool ACharacterBase::CanInDailyBehavior() const
{
	return GetAlertState() == EAlertState::None && (CurrentActions.Num() == 0 
		|| bIsInBTreeDispatching
		|| CurrentActions.ContainsByPredicate([](auto Action)
			{
				return Action->State != EDispatchableActionState::Deactive;
			}) == false);
}

void ACharacterBase::WhenStartNavLink_Implementation(EARPG_NavAreaFlag NavAreaFlag, const FVector& StartLocation, const FVector& EndLocation)
{
	if (NavAreaFlag == EARPG_NavAreaFlag::Jump)
	{
		TurnTo((EndLocation - GetActorLocation()).Rotation(), FOnCharacterBehaviorFinished::CreateWeakLambda(this, [=](bool Succeed)
			{
				if (Succeed)
				{
					LaunchCharacter((EndLocation - GetActorLocation()) * 7.f, true, true);
				}
			}));
	}
	else
	{
		DrawDebugSphere(GetWorld(), StartLocation, 100.f, 32, FColor::Red, true, 2.f);
		DrawDebugSphere(GetWorld(), EndLocation, 100.f, 32, FColor::Red, true, 2.f);
	}
}
