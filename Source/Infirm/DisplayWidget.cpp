// Fill out your copyright notice in the Description page of Project Settings.


#include "DisplayWidget.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"

/*
function: display interaction text on the screen based on the integer value passed in
*/
void UDisplayWidget::SetText(int textVal)
{
	TMap<int32, FString> TextMap = {
		{0, "Pick Up"},
		{1, "Search"},
		{2, "Locked"},
		{3, ""},
		{4, "Empty"},
		{5, "Read"},
		{6, "Searched"},
		{7, "Set Down"}
	};

	if (FString* TextVal = TextMap.Find(textVal))
	{
		RetText->SetText(FText::FromString(*TextVal));
	}
}

