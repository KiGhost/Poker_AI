// Fill out your copyright notice in the Description page of Project Settings.

#include "PoKeIt.h"
#include "Calculator.h"

Calculator::Calculator()
{
	keyCards.Empty();
}

int Calculator::qualityOfCards(Card* hand1, Card* hand2, Card* flop0, Card* flop1, Card* flop2, Card* turn, Card* river)
{
	cards[0] = hand1;
	cards[1] = hand2;
	cards[2] = flop0;
	cards[3] = flop1;
	cards[4] = flop2;
	cards[5] = turn;
	cards[6] = river;

	/*	HIGHCARD,
		PAIR,
		TWOPAIR,
		TRIPS,
		STRAIGHT,
		FLUSH,
		FULLHOUSE,
		QUADS,
		STRAIGHTFLUSH,
		ROYALFLUSH		*/

	bubbleSortByValue();

	
	int flushCheckValue = flushCheck();
	if (flushCheckValue >= 0 && straightCheck(flushCheckValue) >= 0)
	{
		printKeyCards();
		return STRAIGHTFLUSH;
	}
	keyCards.Empty();

	if (quadsCheck() >= 0)
	{
		fillKeyCards();
		return QUADS;
	}
	keyCards.Empty();
	
	int tripsCheckValue = tripsCheck(-1);
	if ((tripsCheckValue >= 0) && pairCheck(tripsCheckValue) != -1)
	{
		fillKeyCards();
		return FULLHOUSE;
	}

	keyCards.Empty();
		
	if (flushCheck() >= 0)
	{
		printKeyCards();
		return FLUSH;
	}

	keyCards.Empty();
		
	if (straightCheck(-1) >= 0)
	{
		printKeyCards();
		return STRAIGHT;
	}
		
	
	keyCards.Empty();
		
	if (tripsCheck(-1) >= 0)
	{
		fillKeyCards();
		return TRIPS;
	}

	keyCards.Empty();
		
	if (pairCheck(pairCheck(-1)) >= 0)
	{
		fillKeyCards();
		return TWOPAIR;
	}
	keyCards.Empty();
		
	if (pairCheck(-1) >= 0)
	{
		fillKeyCards();
		return PAIR;
	}
	keyCards.Empty();
	

	//if (returnValue == HIGHCARD)
	fillKeyCards();

	return HIGHCARD;

}

void Calculator::fillKeyCards()
{
	// debugging:
	int x = 0;

	for (int i = amountOfCards-1; i >= 0; --i)
	{
		bool elementAlreadyWithin = false;
		for (int n = 0; n < keyCards.Num(); ++n)
			if (keyCards[n]->getValue() == cards[i]->getValue())
				elementAlreadyWithin = true;
		if (!elementAlreadyWithin)
			if (keyCards.Num() <= 4)
			{
				keyCards.Add(cards[i]);
				playerController->debugMessage("called adding times: " + FString::FromInt(++x) + " --- (should be 2)");
			}
	}

	printKeyCards();
}

// actually debugging stuff
void Calculator::setPlayerController(APlayerControllerP* pc)
{
	this->playerController = pc;
}

// actually debugging stuff
void Calculator::printKeyCards()
{
	//playerController->debugMessage("keyCardsNum is: " + FString::FromInt(keyCards.Num()));
	for (int i = 0; i < keyCards.Num(); ++i)
		playerController->debugMessage("keyCards[" + FString::FromInt(i) + "]: " + FString::FromInt(keyCards[i]->getValue()));

}

int Calculator::pairCheck(int pairCheckValue)
{
	int returnValue = -1;

	if (pairCheckValue == -1)
	{
		for (int i = (amountOfCards-2); i >= 0; --i)
			if (cards[i]->getValue() == cards[i + 1]->getValue())
			{
				returnValue = cards[i]->getValue();
				if (keyCards.Num() < 2)
				{
					keyCards.Add(cards[i]);
					keyCards.Add(cards[i + 1]);
				}
				break;
			}
	}

	if (pairCheckValue >= 0)
	{
		for (int i = (amountOfCards - 2); i >= 0; --i)
			if (cards[i]->getValue() == cards[i + 1]->getValue() && cards[i]->getValue() != pairCheckValue)
			{
				if (keyCards.Num() < 4)
				{
					returnValue = cards[i]->getValue();
					playerController->debugMessage("cards[" + FString::FromInt(i) + "]: " + FString::FromInt(cards[i]->getValue()));
					keyCards.Add(cards[i]);
					keyCards.Add(cards[i + 1]);
				}
			}
	}
	return returnValue;
}

int Calculator::tripsCheck(int pairCheckValue)
{
	int returnValue = -1;

	if (pairCheckValue == -1)
	{
		int counter = 0;

		for (int i = (amountOfCards - 2); i >= 0; --i)
		{
			if (cards[i]->getValue() == cards[i + 1]->getValue())
			{
				keyCards.Add(cards[i]);
				counter++;
			}
			else
			{
				counter = 0;
				keyCards.Empty();
			}
				
			if (counter == 2)
			{
				keyCards.Add(cards[i+1]);
				returnValue = cards[i + 1]->getValue();
				break;
			}
				
		}
	}

	if (pairCheckValue >= 0)
	{
		int counter = 0;

		for (int i = (amountOfCards - 2); i >= 0; --i)
		{
			if ((cards[i]->getValue() == cards[i + 1]->getValue()) && cards[i]->getValue() != pairCheckValue)
			{
				keyCards.Add(cards[i]);
				counter++;
			}
			else
			{
				counter = 0;
				keyCards.Empty();
			}
			if (counter == 2)
			{
				keyCards.Add(cards[i + 1]);
				returnValue = cards[i + 1]->getValue();
				break;
			}
		}
	}	
	return returnValue;
}

int Calculator::straightCheck(int straightFlushCheck)
{
	int returnValue = -1;

	if (straightFlushCheck == -1)
	{
		int counter = 0;

		for (int i = amountOfCards-2; i >= 0; --i)
		{
			if (cards[i]->getValue() + 1 == cards[i + 1]->getValue())
			{
				keyCards.Add(cards[i + 1]);
				counter++;
			}
			else if (cards[i]->getValue() != cards[i + 1]->getValue())
			{
				keyCards.Empty();
				counter = 0;
			}
			if (counter == 4)
			{
				keyCards.Add(cards[i]);
				returnValue = keyCards[0]->getValue();
				break;
			}		
		}
		if (counter != 4)
			keyCards.Empty();
	}
	if (straightFlushCheck == -1 && keyCards.Num() == 0) // if branch for straight starting with ace only -> 12 - 0 - 1 - 2 - 3
	{

		int counter = 0;

		if (cards[amountOfCards - 1]->getValue() == 12 && cards[0]->getValue() == 0)
			counter++;

		for (int i = amountOfCards - 2; i >= 0; --i)
		{
			if (cards[i]->getValue() + 1 == cards[i + 1]->getValue())
			{
				keyCards.Add(cards[i + 1]);
				counter++;
			}
			else if(cards[i]->getValue() != cards[i + 1]->getValue())
			{
				keyCards.Empty();
				if (cards[amountOfCards - 1]->getValue() == 12 && cards[0]->getValue() == 0)
					counter=1;
				else
					counter = 0;
			}
			if (counter == 4)
			{
				keyCards.Add(cards[i]);
				keyCards.Add(cards[amountOfCards - 1]);
				returnValue = keyCards[0]->getValue();
				break;
			}
		}
	}
	if (straightFlushCheck >= 0)
	{
		keyCards.Empty();
		int counter = 0;

		for (int i = amountOfCards - 2; i >= 0; --i)
		{
			if (cards[i]->getValue() + 1 == cards[i + 1]->getValue())
			{
				if (cards[i]->getColor() == straightFlushCheck && cards[i + 1]->getColor() == straightFlushCheck)
				{
					keyCards.Add(cards[i + 1]);
					counter++;
				}
			}
			else if(cards[i]->getValue() != cards[i + 1]->getValue())
			{
				keyCards.Empty();
				counter = 0;
			}
			if (counter == 4)
			{
				keyCards.Add(cards[i]);
				returnValue = cards[i + 1]->getValue();
				break;
			}
		}
		if (counter != 4)
			keyCards.Empty();
	}
	if (straightFlushCheck >= 0 && keyCards.Num() == 0) // if branch for straight starting with ace only -> 12 - 0 - 1 - 2 - 3
	{
		int counter = 0;
		if (cards[amountOfCards - 1]->getValue() == 12 && cards[0]->getValue() == 0)
			counter++;

		for (int i = amountOfCards - 2; i >= 0; --i)
		{
			if (cards[i]->getValue() + 1 == cards[i + 1]->getValue())
			{
				if (cards[i]->getColor() == straightFlushCheck && cards[i + 1]->getColor() == straightFlushCheck)
				{
					keyCards.Add(cards[i + 1]);
					counter++;
				}
			}
			else if (cards[i]->getValue() != cards[i + 1]->getValue())
			{
				keyCards.Empty();
				if (cards[amountOfCards - 1]->getValue() == 12 && cards[0]->getValue() == 0)
					counter = 1;
				else
					counter = 0;
			}
			if (counter == 4)
			{
				keyCards.Add(cards[i]);
				keyCards.Add(cards[amountOfCards - 1]);
				returnValue = keyCards[0]->getValue();
				break;
			}
		}
	}

	return returnValue;
}

int Calculator::flushCheck()
{
	int counter = 0;
	int returnValue = -1;

	int colorReference = cards[0]->getColor();

	for (int i = amountOfCards - 1; i >= 0; --i)
	{
		if (cards[i]->getColor() == colorReference)
		{
			keyCards.Add(cards[i]);
			counter++;
		}
		if (counter == 5)
			returnValue = colorReference;
	}

	counter = 0;
	if (keyCards.Num() != 5)
		keyCards.Empty();

	colorReference = cards[1]->getColor();

	for (int i = amountOfCards - 1; i >= 0; --i)
	{
		if (cards[i]->getColor() == colorReference)
		{
			keyCards.Add(cards[i]);
			counter++;
		}
		if (counter == 5)
			returnValue = colorReference;
	}

	counter = 0;
	if (keyCards.Num() != 5)
		keyCards.Empty();

	colorReference = cards[2]->getColor();

	for (int i = amountOfCards - 1; i >= 0; --i)
	{
		if (cards[i]->getColor() == colorReference)
		{
			keyCards.Add(cards[i]);
			counter++;
		}
		if (counter == 5)
			returnValue = colorReference;
	}
	return returnValue;
}

int Calculator::quadsCheck()
{
	int counter = 0;
	int returnValue = -1;

	for (int i = 0; i < amountOfCards; ++i)
	{
		if (cards[i]->getValue() == cards[i + 1]->getValue())
			counter++;
		else
			counter = 0;
		if (counter == 3)
			returnValue = cards[i + 1]->getValue();
	}
	return returnValue;
}

void Calculator::bubbleSortByValue()
{
	Card* tmp = cards[0];

	bool unsorted = true;

	while (unsorted)
	{
		unsorted = false;
		for (int i = 0; i < amountOfCards-1; ++i)
		{
			if (cards[i]->getValue() > cards[i + 1]->getValue())
			{
				tmp = cards[i];
				cards[i] = cards[i + 1];
				cards[i + 1] = tmp;
				unsorted = true;
			}
		}
	}
}

Card* Calculator::getKeyCard(int index)
{
	return keyCards[index];
}

Calculator::~Calculator()
{
	for (int i = 0; i < amountOfCards; ++i)
		cards[i]->~Card();
}
