// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "NPCInteractionWidget.generated.h"

/**
 * 
 */
UCLASS()
class REDCOWBOY_API UNPCInteractionWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Interaction)
	bool bIsInputGamepad;
	
protected:

	/** Contains the whole interaction widget */
	UPROPERTY(meta=(BindWidget))
	class UVerticalBox* InteractionMenu;

	/** Contains the actions when locked to something/someone */
	UPROPERTY(meta=(BindWidget))
	class UVerticalBox* ActionMenu;

	/** The lock action label */
	UPROPERTY(meta=(BindWidget))
	class UTextBlock* LockLabel;

	/** The lock action sprite */
	UPROPERTY(meta=(BindWidget))
	class UImage* LockSpritePC;

	/** The lock action sprite */
	UPROPERTY(meta=(BindWidget))
	class UImage* LockSpritePS4;

	/** The bad action label */
	UPROPERTY(meta=(BindWidget))
	class UTextBlock* BadLabel;

	/** The bad action sprite */
	UPROPERTY(meta=(BindWidget))
	class UImage* BadSpritePC;

	/** The bad action sprite */
	UPROPERTY(meta=(BindWidget))
	class UImage* BadSpritePS4;

	/** The friendly action label */
	UPROPERTY(meta=(BindWidget))
	class UTextBlock* FriendlyLabel;

	/** The friendly action sprite */
	UPROPERTY(meta=(BindWidget))
	class UImage* FriendlySpritePC;

	/** The friendly action sprite */
	UPROPERTY(meta=(BindWidget))
	class UImage* FriendlySpritePS4;

	/** The rob action label */
	UPROPERTY(meta=(BindWidget))
	class UTextBlock* RobLabel;

	/** The rob action sprite */
	UPROPERTY(meta=(BindWidget))
	class UImage* RobSpritePC;

	/** The rob action sprite */
	UPROPERTY(meta=(BindWidget))
	class UImage* RobSpritePS4;

	/** The aim action label */
	UPROPERTY(meta=(BindWidget))
	class UTextBlock* AimLabel;

	/** The aim action sprite */
	UPROPERTY(meta=(BindWidget))
	class UImage* AimSpritePC;

	/** The aim action sprite */
	UPROPERTY(meta=(BindWidget))
	class UImage* AimSpritePS4;

	virtual void NativeConstruct() override;

public:

	/** Set the input controller type */
	void SetInputType(bool bIsGamepad);

	/** Set the input controller type */
	void SetCanLock(bool bCanLock);

	/** Set the input controller type */
	void SetIsLocked(bool bIsLocked);

	/** Set the interaction target name */
	void SetName(FString Name);
};
