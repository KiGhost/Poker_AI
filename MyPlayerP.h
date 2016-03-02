// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Card.h"
#include "UnrealString.h"

/**
 * 
 */
class POKEIT_API MyPlayerP
{

private:

	int potAssignment = -1;
	bool boolIsPlayer;

	

public:

	// variables:

	FString playerName;
	int betThisRound;
	int chips;
	Card* cards[2];

	// functions: 

	virtual void makeDecision();
	bool isPlayer();
	void increaseBetThisRound(int amount);
	void initializeNewRound(int a, int b, int c, int d);
	int getBetThisRound();

	FString getName();

	void debugFunc();

	void fold();

	void increaseChips(int amount);
	void decreaseChips(int amount);

	Card* getCard0();
	Card* getCard1();

	int getChips();

	int getPotAssignment();
	void setPotAssignment(int i);

	MyPlayerP(int givenChips, FString nameGiven, bool boolIsPlayer = false);

	void destroyCards();
	~MyPlayerP();
};
