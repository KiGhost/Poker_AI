// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/GameMode.h"
#include "PoKeItGameMode.generated.h"

/**
 * 
 */
UCLASS()
class POKEIT_API APoKeItGameMode : public AGameMode
{
	GENERATED_BODY()
	
public:
	APoKeItGameMode();

	UFUNCTION(BlueprintCallable, Category = "GameMode")
		void debugFunc();
};
