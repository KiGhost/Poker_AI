// Fill out your copyright notice in the Description page of Project Settings.

#include "PoKeIt.h"
#include "KICalculator.h"

KICalculator::KICalculator(const int round, Card *handCardOne, Card *handCardTwo, std::vector<Card> communityCards)
	: currentRound(round), highestPairValue(-1), tripleValue(-2), owningPair(false), owningTriple(false), owningStraight(false), owningFlush(false), owningHighestValue(false), cardOuts(0), usableCards(std::vector<Card>()), ptr_overcardOne(NULL), ptr_overcardTwo(NULL)
{
	/*
	// DEBUG!
	// 1 color - 2 value
	debugCard_0 = Card(0, 12);
	debugCard_1 = Card(0, 10);
	usableCards.push_back(debugCard_0);
	usableCards.push_back(debugCard_1);
	// DEBUG END!
	*/

	usableCards.push_back(*handCardOne);
	usableCards.push_back(*handCardTwo);

	std::sort(usableCards.begin(), usableCards.end(), std::greater<Card>());
}

KICalculator::~KICalculator()
{
}

std::vector<OwnedCardCombination> KICalculator::getVecOwnedCombinations()
{
	allCombinations = std::vector<OwnedCardCombination>();

	// check for highest card
	allCombinations.push_back(checkForHighestCard());

	// check for highest pair
	allCombinations.push_back(checkForFirstPairs());

	// access branch only AFTER PreFlop
	if (currentRound > 0)
	{
		// check for second pair
		allCombinations.push_back(checkForSecondPairs());

		// check for triple
		allCombinations.push_back(checkForTriple());
		
		// check for straight
		allCombinations.push_back(checkForStraight());

		// check for flush
		allCombinations.push_back(checkForFlush());

		// check for full house
		allCombinations.push_back(checkForFullHouse());

		// check for quads
		allCombinations.push_back(checkForQuads());

		// check for straight flush
		allCombinations.push_back(checkForStraightFlush());

		// check for royal flush
		allCombinations.push_back(checkForRoyalFlush());
	}

	return allCombinations;
}

void KICalculator::updateInformation(Card *handOne, Card *handTwo, std::vector<Card> newCommunityCards)
{
	/*
	// DEBUG!
	std::vector<Card> debugCommCard = std::vector<Card>();
	Card debugCard_2 = Card(0, 11);
	Card debugCard_3 = Card(0, 8);
	Card debugCard_4 = Card(0, 9);
	debugCommCard.push_back(debugCard_2);
	debugCommCard.push_back(debugCard_3);
	debugCommCard.push_back(debugCard_4);

	if (currentRound != 0)
	{
		// check for first Overcard
		if (debugCard_0 > debugCommCard[0] &&
			debugCard_0 > debugCommCard[1] &&
			debugCard_0 > debugCommCard[2])
		{
			ptr_overcardOne = handOne;
		}
		else
		{
			ptr_overcardOne = NULL;
		}

		// check for second Overcard
		if (debugCard_1 > debugCommCard[0] &&
			debugCard_1 > debugCommCard[1] &&
			debugCard_1 > debugCommCard[2])
		{
			ptr_overcardTwo = handTwo;
		}
		else
		{
			ptr_overcardTwo = NULL;
		}
	}

	usableCards.insert(std::end(usableCards), std::begin(debugCommCard), std::end(debugCommCard));

	// DEBUG END!
	*/
	
	if (currentRound != 0)
	{
		// check for first Overcard
		if (*handOne > newCommunityCards[0] &&
			*handOne > newCommunityCards[1] &&
			*handOne > newCommunityCards[2])
		{
			ptr_overcardOne = handOne;
		}
		else
		{
			ptr_overcardOne = NULL;
		}

		// check for second Overcard
		if (*handTwo > newCommunityCards[0] &&
			*handTwo > newCommunityCards[1] &&
			*handTwo > newCommunityCards[2])
		{
			ptr_overcardTwo = handTwo;
		}
		else
		{
			ptr_overcardTwo = NULL;
		}
	}

	usableCards.insert(std::end(usableCards), std::begin(newCommunityCards), std::end(newCommunityCards));
	

	// sort cards by descending value for faster cardCombinationCheck
	std::sort(usableCards.begin(), usableCards.end(), std::greater<Card>());

	++currentRound;
}

float KICalculator::getProbabilityDrawingUsefulCard(int probForRound)
{
	return calcProbabilityDrawingUsefulCard(probForRound);
}

OwnedCardCombination KICalculator::checkForHighestCard()
{
	return OwnedCardCombination(0, true, &usableCards[0]);
}

OwnedCardCombination KICalculator::checkForFirstPairs()
{
	// check for first pair
	for (int i = 0; i < usableCards.size(); ++i)
	{
		// two identical values found
		if (usableCards[i].getValue() == usableCards[i + 1].getValue())
		{
			highestPairValue = usableCards[i].getValue();
			owningPair = true;
			return OwnedCardCombination(1, true, &usableCards[i], &usableCards[i + 1]);
		}
	}

	// no first pair found
	return OwnedCardCombination(1);
}

OwnedCardCombination KICalculator::checkForSecondPairs()
{
	// check for first pair
	for (int i = 0; i < usableCards.size(); ++i)
	{
		// two identical values found
		if (usableCards[i].getValue() == usableCards[i + 1].getValue())
		{
			// Not the First Pair
			if (usableCards[i].getValue() != highestPairValue)
			{
				if (highestPairValue < usableCards[i].getValue())
				{
					highestPairValue = usableCards[i].getValue();
				}

				return OwnedCardCombination(2, true, &usableCards[i], &usableCards[i + 1]);
			}
		}
	}

	// no second pair found
	return OwnedCardCombination(2);
}

OwnedCardCombination KICalculator::checkForTriple()
{
	for (int i = 0; i < usableCards.size() - 1; ++i)
	{
		// three identical values found
		if (usableCards[i].getValue() == usableCards[i + 1].getValue() &&
			usableCards[i + 1].getValue() == usableCards[i + 2].getValue())
		{
			owningTriple = true;
			tripleValue = usableCards[i].getValue();
			return OwnedCardCombination(3, true, &usableCards[i], &usableCards[i + 1], &usableCards[i + 2]);
		}
	}

	// no triple found
	return OwnedCardCombination(3);
}

OwnedCardCombination KICalculator::checkForStraight()
{
	for (int i = 0; i < currentRound; ++i)
	{
		// five ascending values found
		if (usableCards[i].getValue() == usableCards[i + 1].getValue() + 1 &&
			usableCards[i + 1].getValue() == usableCards[i + 2].getValue() + 1 &&
			usableCards[i + 2].getValue() == usableCards[i + 3].getValue() + 1 && 
			usableCards[i + 3].getValue() == usableCards[i + 4].getValue() + 1)
		{	
			if (usableCards[i].getValue() == 12)
			{
				owningHighestValue = true;
			}

			owningStraight = true;
			return OwnedCardCombination(4, true, &usableCards[i], &usableCards[i + 1], &usableCards[i + 2], &usableCards[i + 3], &usableCards[i + 4]);
		}
	}

	// no straight found
	return OwnedCardCombination(4);
}

OwnedCardCombination KICalculator::checkForFlush()
{
	int clubsCount = 0, spadesCount = 0, diamondsCount = 0, heartsCount = 0;
	int clubsIndexPositions[5], spadesIndexPositions[5], diamondsIndexPositions[5], heartsIndexPositions[5];

	for (int i = 0; i < currentRound + 5; ++i)
	{
		switch (usableCards[i].getColor())
		{
		case 0:
		{
			clubsIndexPositions[clubsCount] = i;
			++clubsCount;
			break;
		}
		case 1:
		{
			spadesIndexPositions[spadesCount] = i;
			++spadesCount;
			break;
		}
		case 2:
		{
			diamondsIndexPositions[diamondsCount] = i;
			++diamondsCount;
			break;
		}
		case 3:
		{
			heartsIndexPositions[heartsCount] = i;
			++heartsCount;
			break;
		}
		default:
		{
			break;
		}
		}

		// specific color flush found
		if (clubsCount == 5)
		{
			owningFlush = true;
			return OwnedCardCombination(5, true, &usableCards[clubsIndexPositions[0]], &usableCards[clubsIndexPositions[1]], &usableCards[clubsIndexPositions[2]], &usableCards[clubsIndexPositions[3]], &usableCards[clubsIndexPositions[4]]);
		}
		else if (spadesCount == 5)
		{
			owningFlush = true;
			return OwnedCardCombination(5, true, &usableCards[spadesIndexPositions[0]], &usableCards[spadesIndexPositions[1]], &usableCards[spadesIndexPositions[2]], &usableCards[spadesIndexPositions[3]], &usableCards[spadesIndexPositions[4]]);
		}
		else if (diamondsCount == 5)
		{
			owningFlush = true;
			return OwnedCardCombination(5, true, &usableCards[diamondsIndexPositions[0]], &usableCards[diamondsIndexPositions[1]], &usableCards[diamondsIndexPositions[2]], &usableCards[diamondsIndexPositions[3]], &usableCards[diamondsIndexPositions[4]]);
		}
		else if (heartsCount == 5)
		{
			owningFlush = true;
			return OwnedCardCombination(5, true, &usableCards[heartsIndexPositions[0]], &usableCards[heartsIndexPositions[1]], &usableCards[heartsIndexPositions[2]], &usableCards[heartsIndexPositions[3]], &usableCards[heartsIndexPositions[4]]);
		}
	}

	// no straight found
	return OwnedCardCombination(5);
}

OwnedCardCombination KICalculator::checkForFullHouse()
{
	if (owningPair && owningTriple && (highestPairValue != tripleValue))
	{
		if (highestPairValue == allCombinations[1].getComboCardPtrs()[0]->getValue())
		{
			// full house with first pair = higher pair full house
			return OwnedCardCombination(6, true,	allCombinations[1].getComboCardPtrs()[0],
													allCombinations[1].getComboCardPtrs()[1],
													allCombinations[3].getComboCardPtrs()[0],
													allCombinations[3].getComboCardPtrs()[1],
													allCombinations[3].getComboCardPtrs()[2]);
		}
		else
		{
			// full house with second pair = higher pair full house
			return OwnedCardCombination(6, true,	allCombinations[2].getComboCardPtrs()[0],
													allCombinations[2].getComboCardPtrs()[1],
													allCombinations[3].getComboCardPtrs()[0],
													allCombinations[3].getComboCardPtrs()[1],
													allCombinations[3].getComboCardPtrs()[2]);
		}
	}

	// no full hpuse found found
	return OwnedCardCombination(6);
}

OwnedCardCombination KICalculator::checkForQuads()
{
	for (int i = 0; i < usableCards.size() - 1; ++i)
	{
		// four identical values found
		if (usableCards[i].getValue() == usableCards[i + 1].getValue() &&
			usableCards[i + 1].getValue() == usableCards[i + 2].getValue() &&
			usableCards[i + 2].getValue() == usableCards[i + 3].getValue())
		{
			return OwnedCardCombination(7, true, &usableCards[i], &usableCards[i + 1], &usableCards[i + 2], &usableCards[i + 3]);
		}
	}

	// no quads found
	return OwnedCardCombination(7);
}

OwnedCardCombination KICalculator::checkForStraightFlush()
{
	if (owningStraight && owningFlush)
	{
		return OwnedCardCombination(8, true, allCombinations[4].getComboCardPtrs()[0], allCombinations[4].getComboCardPtrs()[1], allCombinations[4].getComboCardPtrs()[2], allCombinations[4].getComboCardPtrs()[3], allCombinations[4].getComboCardPtrs()[4]);
	}

	// no straight flush found
	return OwnedCardCombination(8);
}

OwnedCardCombination KICalculator::checkForRoyalFlush()
{
	if (owningStraight && owningFlush && owningHighestValue && allCombinations[4].getComboCardPtrs()[0]->getValue() == 12)
	{
		return OwnedCardCombination(9, true, allCombinations[4].getComboCardPtrs()[0], allCombinations[4].getComboCardPtrs()[1], allCombinations[4].getComboCardPtrs()[2], allCombinations[4].getComboCardPtrs()[3], allCombinations[4].getComboCardPtrs()[4]);
	}

	// no royal flush found
	return OwnedCardCombination(9);
}

void KICalculator::calcOvercardOuts()
{
	std::vector<Card> outs;

	if (ptr_overcardOne)
	{
		for (int i = 0; i < 4; ++i)
		{
			if (ptr_overcardOne->getColor() != i)
			{
				outs.push_back(Card(ptr_overcardOne->getValue(), i));
			}

			// check if second overcard available
			if (ptr_overcardTwo && ptr_overcardTwo->getColor() != i)
			{
				outs.push_back(Card(ptr_overcardTwo->getValue(), i));
			}
		}
	}

	cardOuts.insert(cardOuts.end(), outs.begin(), outs.end());
}

void KICalculator::calcPairsOuts(int index)
{
	std::vector<Card> outs = std::vector<Card>();
	
	int pairValue = allCombinations[index].getComboCardPtrs()[0]->getValue();
	int colorCardOne = allCombinations[index].getComboCardPtrs()[0]->getColor();
	int colorCardTwo = allCombinations[index].getComboCardPtrs()[1]->getColor();

	if (colorCardOne != 0 || colorCardTwo != 0)
	{
		outs.push_back(Card(pairValue, 0));
	}
	if (colorCardOne != 1 || colorCardTwo != 1)
	{
		outs.push_back(Card(pairValue, 1));
	}
	if (colorCardOne != 2|| colorCardTwo != 2)
	{
		outs.push_back(Card(pairValue, 2));
	}
	if (colorCardOne != 3 || colorCardTwo != 3)
	{
		outs.push_back(Card(pairValue, 3));
	}

	cardOuts.insert(cardOuts.end(), outs.begin(), outs.end());
}

void KICalculator::calcTripleOuts()
{
	std::vector<Card> outs = std::vector<Card>();

	int tripleValue = allCombinations[3].getComboCardPtrs()[0]->getValue();
	int colorCardOne = allCombinations[3].getComboCardPtrs()[0]->getColor();
	int colorCardTwo = allCombinations[3].getComboCardPtrs()[1]->getColor();
	int colorCardThree = allCombinations[3].getComboCardPtrs()[2]->getColor();

	if (colorCardOne != 0 || colorCardTwo != 0 || colorCardThree != 0)
	{
		outs.push_back(Card(tripleValue, 0));
	}
	if (colorCardOne != 1 || colorCardTwo != 1 || colorCardThree != 1)
	{
		outs.push_back(Card(tripleValue, 1));
	}
	if (colorCardOne != 2 || colorCardTwo != 2 || colorCardThree != 2)
	{
		outs.push_back(Card(tripleValue, 2));
	}
	if (colorCardOne != 3 || colorCardTwo != 3 || colorCardThree != 3)
	{
		outs.push_back(Card(tripleValue, 3));
	}

	cardOuts.insert(cardOuts.end(), outs.begin(), outs.end());
}

void KICalculator::calcStraightOuts()
{
	std::vector<Card> outs = std::vector<Card>();
	std::vector<bool> ownedValues = std::vector<bool>(13, false);
	int gapCounter = 0, connectedCounter = 0;

	for (size_t i = 0; i < usableCards.size(); ++i)
	{
		// owning a value in straight
		ownedValues[usableCards[i].getValue()] = true;

		// Ace is also a One in Straights
		if (usableCards[i].getValue() == 12)
		{
			ownedValues[0] = true;
		}
	}

	// Missing 4 Cards in 1st and 5th place of Straight
	for (int i = 0; i < 13; ++i)
	{
		if (i > 0 && i < 9 &&
			ownedValues[i] &&
			ownedValues[i + 1] &&
			ownedValues[i + 2] &&
			ownedValues[i + 3])
		{
			// Ace as One
			if (i == 0)
			{
				for (int j = 0; j < 4; ++j)
				{
					outs.push_back(Card(12, j));
				}
			}
			else
			{
				for (int j = 0; j < 4; ++j)
				{
					outs.push_back(Card(i - 1, j));
				}
			}

			for (int j = 0; j < 4; ++j)
			{
				outs.push_back(Card(i + 4, j));
			}
		}
		// Missing 4 Cards in 2nd place of Straight
		else if (i < 10 &&
				 ownedValues[i] &&
				 ownedValues[i + 2] &&
			 	 ownedValues[i + 3] &&
			  	 ownedValues[i + 4])
		{
			for (int j = 0; j < 4; ++j)
			{
				outs.push_back(Card(i + 1, j));
			}
		}
		// Missing 4 Cards in 3rd place of Straight
		else if (i < 10 &&
				 ownedValues[i] &&
				 ownedValues[i + 1] &&
				 ownedValues[i + 3] &&
				 ownedValues[i + 4])
		{
			for (int j = 0; j < 4; ++j)
			{
				outs.push_back(Card(i + 2, j));
			}
		}
		// Missing 4 Cards in 4th place of Straight
		else if (i < 10 &&
				 ownedValues[i] &&
				 ownedValues[i + 1] &&
				 ownedValues[i + 2] &&
				 ownedValues[i + 4])
		{
			for (int j = 0; j < 4; ++j)
			{
				outs.push_back(Card(i + 3, j));
			}
		}
	}

	cardOuts.insert(cardOuts.end(), outs.begin(), outs.end());
}

void KICalculator::calcFlushOuts()
{
	std::vector<Card> outs = std::vector<Card>();
	std::vector<std::vector<int>> seperatedValuesOfEachColor(4, std::vector<int>());
	int outsColor = 0;

	// looping through each color
	for (int i = 0; i < 4; ++i)
	{ 
		int cardCounter = 0;

		// find four Cards of one Color
		for (int j = 0; j < 4; ++j)
		{
			for (size_t k = 0; k < usableCards.size(); ++k)
			{
				if (usableCards[k].getColor() == i)
				{
					seperatedValuesOfEachColor[i].push_back(usableCards[k].getValue());
					++cardCounter;

					// four Cards found, exit Loop
					if (cardCounter == 4)
					{
						goto loopExit;
					}
				}
			}
		}
		++outsColor;
	}

	loopExit: std::sort(seperatedValuesOfEachColor[outsColor].begin(), seperatedValuesOfEachColor[outsColor].end());
	int cardIndex = 0;

	for (int i = 0; i < 13; ++i)
	{
		if (seperatedValuesOfEachColor[outsColor][cardIndex] != i)
		{
			outs.push_back(Card(outsColor, i));
			++cardIndex;
		}
	}

	cardOuts.insert(cardOuts.end(), outs.begin(), outs.end());
}

void KICalculator::calcFullHouseOuts()
{
	std::vector<Card> quadOuts = std::vector<Card>();
	std::vector<Card> tripleOuts = std::vector<Card>();

	int tripleValue = allCombinations[3].getComboCardPtrs()[0]->getValue();
	int colorCardOne = allCombinations[3].getComboCardPtrs()[0]->getColor();
	int colorCardTwo = allCombinations[3].getComboCardPtrs()[1]->getColor();
	int colorCardThree = allCombinations[3].getComboCardPtrs()[2]->getColor();

	if (colorCardOne != 0 || colorCardTwo != 0 || colorCardThree != 0)
	{
		quadOuts.push_back(Card(tripleValue, 0));
	}
	if (colorCardOne != 1 || colorCardTwo != 1 || colorCardThree != 1)
	{
		quadOuts.push_back(Card(tripleValue, 1));
	}
	if (colorCardOne != 2 || colorCardTwo != 2 || colorCardThree != 2)
	{
		quadOuts.push_back(Card(tripleValue, 2));
	}
	if (colorCardOne != 3 || colorCardTwo != 3 || colorCardThree != 3)
	{
		quadOuts.push_back(Card(tripleValue, 3));
	}

	// triple is only useful, if it will be higher then owned triple
	if (allCombinations[1].getComboCardPtrs()[0]->getValue() > allCombinations[3].getComboCardPtrs()[0]->getValue())
	{
		int pairValue = allCombinations[1].getComboCardPtrs()[0]->getValue();
		int colorCardOne = allCombinations[1].getComboCardPtrs()[0]->getColor();
		int colorCardTwo = allCombinations[1].getComboCardPtrs()[1]->getColor();

		if (colorCardOne != 0 || colorCardTwo != 0)
		{
			tripleOuts.push_back(Card(pairValue, 0));
		}
		if (colorCardOne != 1 || colorCardTwo != 1)
		{
			tripleOuts.push_back(Card(pairValue, 1));
		}
		if (colorCardOne != 2 || colorCardTwo != 2)
		{
			tripleOuts.push_back(Card(pairValue, 2));
		}
		if (colorCardOne != 3 || colorCardTwo != 3)
		{
			tripleOuts.push_back(Card(pairValue, 3));
		}

		quadOuts.insert(quadOuts.end(), tripleOuts.begin(), tripleOuts.end());
	}

	cardOuts.insert(cardOuts.end(), quadOuts.begin(), quadOuts.end());
}

void KICalculator::calcfinalCardOuts() // TO DO
{
	calcOvercardOuts();
	if (allCombinations[1].getComboOwned())
	{
		calcPairsOuts(1);
	}

	if (allCombinations[2].getComboOwned())
	{
		calcPairsOuts(2);
	}

	if (allCombinations[3].getComboOwned())
	{
		calcTripleOuts();
	}

	calcStraightOuts();
	calcFlushOuts();

	if (allCombinations[1].getComboOwned() && allCombinations[3].getComboOwned())
	{
		calcFullHouseOuts();
	}

	std::sort(cardOuts.begin(), cardOuts.end());
	cardOuts.erase(std::unique(cardOuts.begin(), cardOuts.end()), cardOuts.end());
}

float KICalculator::calcProbabilityDrawingUsefulCard(int probForRound)
{
	calcfinalCardOuts();

	switch (probForRound)
	{
	case 1:
	{
		return 0;
		break;
	}
	case 2:
	{
		// from flop to turn
		return cardOuts.size() / 47;
	}
	case 3:
	{
		// from flop to turn + river
		if (currentRound == 1)
		{
			return 1 - ((47 - cardOuts.size()) / 47) * ((46 - cardOuts.size()) / 46);
		}
		// from turn to river
		else
		{
			return cardOuts.size() / 46;
		}
	}
	default:
	{
		return 0.0f;
	}
	}
}

bool KICalculator::preFlopRaiseDecision()
{
	currentRound = 0;
	allCombinations = std::vector<OwnedCardCombination>();

	// check for highest card
	allCombinations.push_back(checkForHighestCard());

	// check for highest pair
	allCombinations.push_back(checkForFirstPairs());

	if ((owningPair && allCombinations[1].getComboCardPtrs()[0]->getValue() > 8) ||
		(usableCards[0].getValue() > 8 && usableCards[1].getValue() > 8))
	{
		return true;
	}
	else
	{
		return false;
	}
}

int KICalculator::factorial(int n)
{
	return (n == 1 || n == 0) ? 1 : factorial(n - 1) * n;
}

float KICalculator::binomialKoefficient(int n, int k)
{
	return (factorial(n)) / (factorial(k) * (factorial(n - k)));
}