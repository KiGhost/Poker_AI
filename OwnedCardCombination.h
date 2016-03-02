// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Card.h"
#include <vector>

class POKEIT_API OwnedCardCombination
{
public:
	OwnedCardCombination(int cardRank, bool owned = false,	Card *ptr_cardOne = NULL,
															Card *ptr_cardTwo = NULL,
															Card *ptr_cardThree = NULL,
															Card *ptr_cardFour = NULL,
															Card *ptr_cardFive = NULL);
	~OwnedCardCombination();

	// Getter
	int getComboCardRanking();
	std::vector<Card*> getComboCardPtrs();
	bool getComboOwned();
	

private:
	int comboCardRank;
	std::vector<Card*> comboCardsPtrs;
	bool comboOwned;
};