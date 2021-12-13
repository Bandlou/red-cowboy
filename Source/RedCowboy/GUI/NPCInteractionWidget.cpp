// Fill out your copyright notice in the Description page of Project Settings.


#include "NPCInteractionWidget.h"

#include "Components/CanvasPanel.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Components/VerticalBox.h"

void UNPCInteractionWidget::NativeConstruct()
{
	Super::NativeConstruct();

	bIsInputGamepad = true;

	BadLabel->SetText(FText::FromString("Antagonize"));
	FriendlyLabel->SetText(FText::FromString("Greet"));
	RobLabel->SetText(FText::FromString("Rob"));
	AimLabel->SetText(FText::FromString("Aim Weapon"));

	// Dialog menu
	StopDialog();
}

void UNPCInteractionWidget::SetInputType(bool bIsGamepad)
{
	if (bIsInputGamepad == bIsGamepad)
		return;

	bIsInputGamepad = bIsGamepad;
	LockSpritePC->SetVisibility(bIsInputGamepad ? ESlateVisibility::Collapsed : ESlateVisibility::Visible);
	LockSpritePS4->SetVisibility(bIsInputGamepad ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
	BadSpritePC->SetVisibility(bIsInputGamepad ? ESlateVisibility::Collapsed : ESlateVisibility::Visible);
	BadSpritePS4->SetVisibility(bIsInputGamepad ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
	FriendlySpritePC->SetVisibility(bIsInputGamepad ? ESlateVisibility::Collapsed : ESlateVisibility::Visible);
	FriendlySpritePS4->SetVisibility(bIsInputGamepad ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
	RobSpritePC->SetVisibility(bIsInputGamepad ? ESlateVisibility::Collapsed : ESlateVisibility::Visible);
	RobSpritePS4->SetVisibility(bIsInputGamepad ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
	AimSpritePC->SetVisibility(bIsInputGamepad ? ESlateVisibility::Collapsed : ESlateVisibility::Visible);
	AimSpritePS4->SetVisibility(bIsInputGamepad ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
}

void UNPCInteractionWidget::SetCanLock(bool bCanLock)
{
	InteractionMenu->SetVisibility(bCanLock ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
}

void UNPCInteractionWidget::SetIsLocked(bool bIsLocked)
{
	ActionMenu->SetVisibility(bIsLocked ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
}

void UNPCInteractionWidget::SetName(const FString& Name)
{
	LockLabel->SetText(FText::FromString(Name));
}

void UNPCInteractionWidget::SetDialog(const FString& Name, const FString& Phrase)
{
	SpeakerName->SetText(FText::FromString(Name));
	SpeakerPhrase->SetText(FText::FromString(Phrase));
	DialogMenu->SetVisibility((ESlateVisibility::Visible));
}

void UNPCInteractionWidget::StopDialog()
{
	DialogMenu->SetVisibility((ESlateVisibility::Hidden));
}
