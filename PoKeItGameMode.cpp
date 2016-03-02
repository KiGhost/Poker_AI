// Fill out your copyright notice in the Description page of Project Settings.

#include "PoKeIt.h"
#include "PoKeItGameMode.h"




APoKeItGameMode::APoKeItGameMode()
{
}

void APoKeItGameMode::debugFunc()
{
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, "GameMode.cpp debugFunc was called");
}