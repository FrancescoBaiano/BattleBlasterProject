// Fill out your copyright notice in the Description page of Project Settings.


#include "ScreenMessage.h"

void UScreenMessage::SetMessageText(FString message)
{
	FText messageText = FText::FromString(message);
	messageTextBlock->SetText(messageText);
}