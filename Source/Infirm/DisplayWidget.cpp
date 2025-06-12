// Fill out your copyright notice in the Description page of Project Settings.


#include "DisplayWidget.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"


void UDisplayWidget::SetText(int textVal)
{
	if (textVal == 0)
	{
		RetText->SetText(FText::FromString("Pick Up"));
	}
	else if (textVal == 1)
	{
		RetText->SetText(FText::FromString("Search"));
	}
	else if (textVal == 2)
	{
		RetText->SetText(FText::FromString("Locked"));
	}
	else if (textVal == 3)
	{
		RetText->SetText(FText::FromString(""));
	}
	else if (textVal == 4)
	{
		RetText->SetText(FText::FromString("Empty"));
	}
	else if (textVal == 5)
	{
		RetText->SetText(FText::FromString("Read"));
	}
	else if (textVal == 6)
	{
		RetText->SetText(FText::FromString("Searched"));
	}
	else if (textVal == 7)
	{
		RetText->SetText(FText::FromString("Set Down"));
	}
	
}

