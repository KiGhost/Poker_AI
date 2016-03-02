// Fill out your copyright notice in the Description page of Project Settings.

#include "PoKeIt.h"
#include "OwnedCardCombination.h"

OwnedCardCombination::OwnedCardCombination(int cardRank, bool owned, Card *ptr_cardOne, Card *ptr_cardTwo, Card *ptr_cardThree, Card *ptr_cardFour, Card *ptr_cardFive)
	:comboCardsPtrs(std::vector<Card*>()), comboCardRank(cardRank), comboOwned(owned)
{
	if (ptr_cardOne)
	{
		comboCardsPtrs.push_back(ptr_cardOne);

		if (ptr_cardTwo)
		{
			comboCardsPtrs.push_back(ptr_cardTwo);

			if (ptr_cardThree)
			{
				comboCardsPtrs.push_back(ptr_cardThree);

				if (ptr_cardFour)
				{
					comboCardsPtrs.push_back(ptr_cardFour);

					if (ptr_cardFive)
					{
						comboCardsPtrs.push_back(ptr_cardFive);
					}
				}
			}
		}
	}
}

OwnedCardCombination::~OwnedCardCombination()
{
}

int OwnedCardCombination::getComboCardRanking()
{
	return comboCardRank;
}

std::vector<Card*> OwnedCardCombination::getComboCardPtrs()
{
	return comboCardsPtrs;
}

bool OwnedCardCombination::getComboOwned()
{
	return comboOwned;
}