// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "MyPlayerP.h"
#include "RoundManager.h"
#include "KICalculator.h"
#include <vector>
#include <windows.h>

// DEBUG
#include <iostream>

class RoundManager;

class POKEIT_API KI : public MyPlayerP
{
public:
	KI(int givenChips, FString nameGiven);
	~KI();

	void makeDecision();

private:
	RoundManager *ptr_roundManager;
	KICalculator *ptr_kiCalculator;
	int remainingPlayers;
	int tablePositionAfterDealer;
	std::vector<Card> communityCards;

	/*
	0 = PREFLOP,
	1 = FLOP,
	2 = TURN,
	3 = RIVER,
	*/
	int currentRound;

	/*
	value between 0% - 100%
	0 = high card
	1 = pair
	2 = two pairs
	3 = trips
	4 = stright
	5 = flush
	6 = full house
	7 = quads
	8 = straight flush
	9 = royal flush
	*/
	std::vector<double> odds;
	int cardOuts;

	std::vector<Card> estimatedCardsOppenents;

	/*
	[0] = High Card
	[1] = First Pair
	[2] = Second Pair
	[3] = Three of a kind
	[4] = Straight
	[5] = Flush
	[6] = Full House
	[7] = Four of a kind
	[8] = Straight Flush
	[9] = Royal Flush
	*/
	std::vector<OwnedCardCombination> ownedCardCombinations;
	
	void updateKIInformations();

	void setRemainingPlayers();
	void setRoundIndex();
	void setCommunityCards();

	void setKIAgressive(); // are we rushin' in ...
	void setKIDefensive(); // ... or goin' sneaky peaky like?

	void updateKICalculator();

	// calculations
	float getPercentagePotOdds();
	//float percentageOpponentHigherPocketPair();

	void checkOwnedCombinations();

	void bluff();

	void performPreFlop();
	void performFlop();
	void performTurn();
	void performRiver();

	void folding();
	void checking();
	void betting(int betAmount);
	void calling();
	void raising(int raiseAmount);

	void sleepDuration();

public:
	// functions
	void setRoundManager(RoundManager *ptr_manager);

};