// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Card.h"
#include "OwnedCardCombination.h"
#include <vector>
#include <algorithm>
#include <functional>


class POKEIT_API KICalculator
{
public:
	KICalculator(const int round, Card *handCardOne, Card *handCardTwo, std::vector<Card> communityCards);
	~KICalculator();

	std::vector<OwnedCardCombination> getVecOwnedCombinations();
	void updateInformation(Card *handOne, Card *handTwo, std::vector<Card> newCommunityCards);

	float getProbabilityDrawingUsefulCard(int probForRound);
	bool preFlopRaiseDecision();

private:
	int currentRound, highestPairValue, tripleValue;
	bool owningPair, owningTriple, owningStraight, owningFlush, owningHighestValue;
	Card *ptr_overcardOne, *ptr_overcardTwo;
	std::vector<Card> usableCards, cardOuts;

	// DEBUG
	Card debugCard_0;
	Card debugCard_1;
	// DEBUG END!

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
	std::vector<OwnedCardCombination> allCombinations;
	
	// functions for checking cardCombinations
	OwnedCardCombination checkForHighestCard();
	OwnedCardCombination checkForFirstPairs();
	OwnedCardCombination checkForSecondPairs();
	OwnedCardCombination checkForTriple();
	OwnedCardCombination checkForStraight();
	OwnedCardCombination checkForFlush();
	OwnedCardCombination checkForFullHouse();
	OwnedCardCombination checkForQuads();
	OwnedCardCombination checkForStraightFlush();
	OwnedCardCombination checkForRoyalFlush();

	void calcOvercardOuts();
	/*
	index:
	1 = first pair
	2 = second pair
	*/
	void calcPairsOuts(int index);
	void calcTripleOuts();
	void calcStraightOuts();
	void calcFlushOuts();
	void calcFullHouseOuts();

	// poker-specific functions
	void calcfinalCardOuts();
	float calcProbabilityDrawingUsefulCard(int probForRound);

	// mathematical functions
	int factorial(int n);
	float binomialKoefficient(int n, int k);
};