// Fill out your copyright notice in the Description page of Project Settings.

#include "AttackAreaManagerVisualizer.h"
#include "ARPG_AttackAreaManager.h"
#include "SceneView.h"
#include "SceneManagement.h"
#include "DrawDebugHelpers.h"
#include "EditorViewportClient.h"
#include "VisualizerUtils.h"
#include "IDetailCustomization.h"
#include "PropertyHandle.h"
#include "DetailCategoryBuilder.h"
#include "DetailLayoutBuilder.h"

IMPLEMENT_HIT_PROXY(FAttackAreaVisProxy, HComponentVisProxy)

int32 FAttackAreaManagerVisualizer::EditIndex = INDEX_NONE;

void FAttackAreaManagerVisualizer::DrawVisualization(const UActorComponent* Component, const FSceneView* View, FPrimitiveDrawInterface* PDI)
{
	AttackAreaManagerW = const_cast<UARPG_AttackAreaManager*>(Cast<UARPG_AttackAreaManager>(Component));
	if (UARPG_AttackAreaManager* AttackAreaManager = AttackAreaManagerW.Get())
	{
		FTransform Transform = AttackAreaManager->GetOwner()->GetActorTransform();

		for (int32 i = 0; i < AttackAreaManager->AttackConfigs.Num(); ++i)
		{
			if (UARPG_AttackAreaBase* AttackArea = AttackAreaManager->AttackConfigs[i].AttackArea)
			{
				const float WidgetSize = 0.035f;
				const float ZoomFactor = FMath::Min<float>(View->ViewMatrices.GetProjectionMatrix().M[0][0], View->ViewMatrices.GetProjectionMatrix().M[1][1]);

				if (UARPG_AttackArea_Sphere* Sphere = Cast<UARPG_AttackArea_Sphere>(AttackArea))
				{
					FTransform WorldWidgetTransform(Transform.Rotator(), Transform.TransformPosition(Sphere->Origin));

					PDI->SetHitProxy(new FAttackAreaVisProxy(Component, i));
					const float WidgetRadius = View->Project(WorldWidgetTransform.GetTranslation()).W * (WidgetSize / ZoomFactor);
					DrawWireDiamond(PDI, WorldWidgetTransform.ToMatrixNoScale(), WidgetRadius, EditIndex == i ? FColor::White : FColor(128, 128, 255), SDPG_Foreground);
					PDI->SetHitProxy(NULL);

					DrawWireSphere(PDI, WorldWidgetTransform, FLinearColor::Red, Sphere->GetWorldRadius(Component->GetOwner()->GetActorScale()), 36, SDPG_Foreground);
				}
				else if (UARPG_AttackArea_Box* Box = Cast<UARPG_AttackArea_Box>(AttackArea))
				{
					FTransform WorldWidgetTransform(Transform.TransformRotation(Box->Rotation.Quaternion()), Transform.TransformPosition(Box->Origin));

					PDI->SetHitProxy(new FAttackAreaVisProxy(Component, i));
					const float WidgetRadius = View->Project(WorldWidgetTransform.GetTranslation()).W * (WidgetSize / ZoomFactor);
					DrawWireDiamond(PDI, WorldWidgetTransform.ToMatrixNoScale(), WidgetRadius, EditIndex == i ? FColor::White : FColor(128, 128, 255), SDPG_Foreground);
					PDI->SetHitProxy(NULL);

					
					DrawWireBox(PDI, WorldWidgetTransform.ToMatrixWithScale(), FBox(-Box->Extent / 2.f, Box->Extent / 2.f), FColor::Red, SDPG_Foreground);
				}
			}
		}
	}
}

bool FAttackAreaManagerVisualizer::VisProxyHandleClick(FEditorViewportClient* InViewportClient, HComponentVisProxy* VisProxy, const FViewportClick& Click)
{
	if (VisProxy && VisProxy->Component.IsValid())
	{
		if (VisProxy->IsA(FAttackAreaVisProxy::StaticGetType()))
		{
			FAttackAreaVisProxy* AttackAreaVisProxy = (FAttackAreaVisProxy*)VisProxy;
			EditIndex = AttackAreaVisProxy->Index;
		}
		return true;
	}
	return true;
}

bool FAttackAreaManagerVisualizer::GetWidgetLocation(const FEditorViewportClient* ViewportClient, FVector& OutLocation) const
{
	if (UARPG_AttackAreaManager* AttackAreaManager = AttackAreaManagerW.Get())
	{
		if (EditIndex != INDEX_NONE && EditIndex < AttackAreaManager->AttackConfigs.Num())
		{
			if (UARPG_AttackAreaBase* AttackArea = AttackAreaManager->AttackConfigs[EditIndex].AttackArea)
			{
				FTransform WorldTransform = AttackAreaManager->GetOwner()->GetActorTransform();
				if (UARPG_AttackArea_Sphere* Sphere = Cast<UARPG_AttackArea_Sphere>(AttackArea))
				{
					OutLocation = WorldTransform.TransformPosition(Sphere->Origin);
				}
				else if (UARPG_AttackArea_Box* Box = Cast<UARPG_AttackArea_Box>(AttackArea))
				{
					OutLocation = WorldTransform.TransformPosition(Box->Origin);
				}
			}
		}

		return true;
	}
	return false;
}

bool FAttackAreaManagerVisualizer::GetCustomInputCoordinateSystem(const FEditorViewportClient* ViewportClient, FMatrix& OutMatrix) const
{
	if (UARPG_AttackAreaManager* AttackAreaManager = AttackAreaManagerW.Get())
	{
		if (EditIndex != INDEX_NONE && EditIndex < AttackAreaManager->AttackConfigs.Num())
		{
			FTransform Transform = AttackAreaManager->GetOwner()->GetActorTransform();
			if (UARPG_AttackAreaBase* AttackArea = AttackAreaManager->AttackConfigs[EditIndex].AttackArea)
			{
				if (UARPG_AttackArea_Box* Box = Cast<UARPG_AttackArea_Box>(AttackArea))
				{
					OutMatrix = FRotationMatrix::Make(Transform.TransformRotation(Box->Rotation.Quaternion()));
					return true;
				}
			}
		}
	}

	return false;
}

bool FAttackAreaManagerVisualizer::HandleInputDelta(FEditorViewportClient* ViewportClient, FViewport* Viewport, FVector& DeltaTranslate, FRotator& DeltalRotate, FVector& DeltaScale)
{
	if (UARPG_AttackAreaManager* AttackAreaManager = AttackAreaManagerW.Get())
	{
		if (EditIndex != INDEX_NONE && EditIndex < AttackAreaManager->AttackConfigs.Num())
		{
			FTransform Transform = AttackAreaManager->GetOwner()->GetActorTransform();
			if (UARPG_AttackAreaBase* AttackArea = AttackAreaManager->AttackConfigs[EditIndex].AttackArea)
			{
				if (UARPG_AttackArea_Sphere* Sphere = Cast<UARPG_AttackArea_Sphere>(AttackArea))
				{
					Sphere->Origin += Transform.InverseTransformVector(DeltaTranslate);

					float Max = DeltaScale.GetMax();
					Sphere->Radius += Max > 0.f ? Max : DeltaScale.GetMin();
				}
				else if (UARPG_AttackArea_Box* Box = Cast<UARPG_AttackArea_Box>(AttackArea))
				{
					Box->Origin += Transform.InverseTransformVector(DeltaTranslate);
					Box->Rotation += DeltalRotate;
					Box->Extent += DeltaScale * 100.f;
				}
			}
		}

		return true;
	}
	return false;
}

void FAttackAreaManagerVisualizer::EndEditing()
{
	EditIndex = INDEX_NONE;
}

bool FAttackAreaManagerVisualizer::IsVisualizingArchetype() const
{
	return true;
}

TSharedPtr<SWidget> FAttackAreaManagerVisualizer::GenerateContextMenu() const
{
	if (UARPG_AttackAreaManager* AttackAreaManager = AttackAreaManagerW.Get())
	{
		TSharedPtr<FVisualizerUtils::SRightMenuEditorObjectWidget> InteractableActorManagerWidget = SNew(FVisualizerUtils::SRightMenuEditorObjectWidget);

		class FARPG_AttackAreaManagerCustomization : public IDetailCustomization
		{
		public:
			// IDetailCustomization interface
			virtual void CustomizeDetails(IDetailLayoutBuilder& DetailBuilder) override
			{
				FVisualizerUtils::HideActorComponentCategory(DetailBuilder);

				TSharedRef<IPropertyHandle> AttackConfigs = DetailBuilder.GetProperty(GET_MEMBER_NAME_CHECKED(UARPG_AttackAreaManager, AttackConfigs));
				TSharedPtr<IPropertyHandle> ActivedElement = AttackConfigs->GetChildHandle(FAttackAreaManagerVisualizer::EditIndex);
				IDetailCategoryBuilder& ActiveBehaviorRow = DetailBuilder.EditCategory(TEXT("选中的攻击区域配置"));

				uint32 ChildNum;
				if (ActivedElement->GetNumChildren(ChildNum) == FPropertyAccess::Success)
				{
					for (uint32 i = 0; i < ChildNum; ++i)
					{
						ActiveBehaviorRow.AddProperty(ActivedElement->GetChildHandle(i));
					}
				}
			}
		};
		InteractableActorManagerWidget->PropertyWidget->RegisterInstancedCustomPropertyLayout(UARPG_AttackAreaManager::StaticClass(), FOnGetDetailCustomizationInstance::CreateLambda([] {return MakeShareable(new FARPG_AttackAreaManagerCustomization); }));
		InteractableActorManagerWidget->PropertyWidget->SetObject(AttackAreaManager);
		return InteractableActorManagerWidget;
	}
	return nullptr;
}
