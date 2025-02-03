// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ItemViewUI.generated.h"


class UImage;

UCLASS()
class UE5_TIMETRAVLEHUNTER_API UItemViewUI : public UUserWidget
{
	GENERATED_BODY()
public:
	//======================================================
	//=					- Variables -					   =
	//======================================================
	UPROPERTY(meta = (BindWidget))
	UImage *ObjectViewer;

	//======================================================
	//=					- Functionary -					   =
	//======================================================
	void InitializeItemViewUI();

	//======================================================
	//=					- Variables -					   =
	//======================================================


	//======================================================
	//=					- Functionary -					   =
	//======================================================
};
