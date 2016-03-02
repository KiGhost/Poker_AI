	// Fill out your copyright notice in the Description page of Project Settings.

#include "PoKeIt.h"
#include "RoundManager.h"
#include "UnrealString.h"


RoundManager::RoundManager(std::vector<MyPlayerP*> playersOfThisRound, APlayerControllerP* pc, int amountOfPlayersRemaining, int dealerIndex, int smallBlind, int bigBlind)
{
	playerController = pc;
	this->amountOfPlayersRemaining = amountOfPlayersRemaining;
	
	resetDeck();

	players = std::vector<MyPlayerP*>(8);

	for (int i = 0; i < amountOfPlayersRemaining; ++i)
	{
		players[i] = playersOfThisRound[i];

		int card0[2] = { FMath::RandRange(0, 3), FMath::RandRange(0, 12) };
		int card1[2] = { FMath::RandRange(0, 3), FMath::RandRange(0, 12) };		

		while (!controlDeck(card0[0], card0[1]))
		{
			card0[0] = FMath::RandRange(0, 3);
			card0[1] = FMath::RandRange(0, 12);
		}

		while (!controlDeck(card1[0], card1[1]))
		{
			card1[0] = FMath::RandRange(0, 3);
			card1[1] = FMath::RandRange(0, 12);
		}

		players[i]->initializeNewRound(card0[0], card0[1], card1[0], card1[1]);
	}


	this->dealerIndex = dealerIndex;
	currentPlayerIndex = (dealerIndex + 3 ) % amountOfPlayersRemaining;
	addPot();
	pot = 0;
	lastBet = bigBlind;
	roundState = PREFLOP;
	this->smallBlind = smallBlind;
	this->bigBlind = bigBlind;
	currentMaxBet = bigBlind;
	playersDidActions = 0;

	settingBlinds();
}

// player actions:
// 1st step in "casual round loop"

void RoundManager::checkRound()
{
	//debug:
	//playerController->debugMessage("pots.Num(): " + FString::FromInt(pots.Num()));
	if (players[currentPlayerIndex]->getBetThisRound() >= currentMaxBet)
		finishTurn();
	else
		playerController->debugMessage("not enough bet to check this round");
}

void RoundManager::callRound()
{
	if ((players[currentPlayerIndex]->getChips()) >= (currentMaxBet - players[currentPlayerIndex]->getBetThisRound()))
		betRaise(currentMaxBet - players[currentPlayerIndex]->getBetThisRound());
	else
		playerController->debugMessage("couldnt call, because too less chips available");
}

void RoundManager::betRaise(int amount)
{
	bool callingAllowed = false;
	bool bettingAllowed = false;
	bool raisingAllowed = false;
	bool allIn = false;

	if ((players[currentPlayerIndex]->getBetThisRound() + amount) == currentMaxBet)						// this is actually calling
	{
		callingAllowed = true;
	}
	else if (players[currentPlayerIndex]->getBetThisRound() == currentMaxBet && amount >= bigBlind)		// this is actually betting
	{
		bettingAllowed = true;
		lastBet = amount;
	}
	else if (players[currentPlayerIndex]->getBetThisRound() <= currentMaxBet
		&& players[currentPlayerIndex]->getBetThisRound() + amount >= currentMaxBet + lastBet)		// this is actually raising
	{
		raisingAllowed = true;
		lastBet = (players[currentPlayerIndex]->getBetThisRound() + amount) - currentMaxBet;
	}

#pragma region not enough chips / allin
	/* TODO
	few probs about this occur:
	1st: p1 bets, p2 allin with half bet, p3 wants to call p1's bet.
	2nd: what is int last bett be set to ?
	3rd: p1 allin, p2 raises above p1's allin

	else if (amount == players[currentPlayerIndex]->getChips()											// not enough chips to call / ALLIN
	&& (players[currentPlayerIndex]->getBetThisRound() + amount) < currentMaxBet)
	{

	allIn = true;
	int allInValue = players[currentPlayerIndex]->getBetThisRound() + amount;

	players[currentPlayerIndex]->decreaseChips(amount);
	players[currentPlayerIndex]->increaseBetThisRound(amount);
	players[currentPlayerIndex]->setPotAssignment(pots.Num() - 1);
	pots[pots.Num() - 1] += amount;
	addPot();

	// creating new side pot with old chip amounts from last pot
	for (int i = 0; i < amountOfPlayersRemaining; ++i)
	{
	if (players[i]->getBetThisRound() > allInValue)
	{
	int moreThanAllIn = players[i]->getBetThisRound() > allInValue;
	pots[pots.Num() - 2] -= moreThanAllIn;
	pots[pots.Num() - 1] += moreThanAllIn;
	players[currentPlayerIndex]->setPotAssignment(pots.Num() - 1);
	}
	}

	finishTurn();
	}
	*/
#pragma endregion

	if (bettingAllowed || raisingAllowed)
		for (int i = 0; i < amountOfPlayersRemaining; ++i)
			if (players[i]->getChips() == 0 && players[i]->getPotAssignment() == pots.Num() - 1) // triggers once one has gone allin and another one bets / raises above
			{
		addPot();
		break;
			}


	if (callingAllowed || bettingAllowed || raisingAllowed)
	{
		players[currentPlayerIndex]->decreaseChips(amount);
		players[currentPlayerIndex]->increaseBetThisRound(amount);
		players[currentPlayerIndex]->setPotAssignment(pots.Num() - 1);

		currentMaxBet = players[currentPlayerIndex]->getBetThisRound();
		increasePot(amount);
		finishTurn();
	}
	else if (!allIn)
	{
		FString a = "bet / raise has failed!";
		FString b = "in case of betting, you have to bet at least the big blind amount, which is " + FString::FromInt(bigBlind)
			+ " for the moment!";
		FString c = "in case of raising, you have to bet at least raise the amount of the last bet, which is " + FString::FromInt(lastBet)
			+ " for the moment!";
		playerController->debugMessage(a);
		playerController->debugMessage(b);
		playerController->debugMessage(c);
	}
}

void RoundManager::fold()
{
	/*
	1. reduce amountOfPlayeresRemaining--
	2. adjust array to fill the gaps
	3. check if theres more than 1 available
	4. if so, keep going
	5. if not, trigger roundOver();
	*/

	amountOfPlayersRemaining--;

	if (amountOfPlayersRemaining > 1)
	{
		for (int i = currentPlayerIndex; i < amountOfPlayersRemaining; ++i)
			players[i] = players[i + 1];

		playersDidActions--;
		currentPlayerIndex--;

		finishTurn();
	}
	else
		roundOver();




}

// 2nd step in "casual round loop"
void RoundManager::finishTurn()
{
	checkForCommunityCards();
	currentPlayerIndex = ++currentPlayerIndex % amountOfPlayersRemaining;


	// there's the possibility, that there are a few players available, but all went all-in,
	// so the round must come to an end. this if-branch checks it and triggers remaining communitycards if necesary
	if (players[currentPlayerIndex]->getChips() == 0)
	{
		int playersRemainingWithChips = 0;
		for (int i = 0; i < amountOfPlayersRemaining; ++i)
		{
			if (players[i]->getChips() != 0)
			{
				playersRemainingWithChips++;
			}
		}
		if (playersRemainingWithChips <= 1)
		{
			roundStateSwitch();
			roundStateSwitch();
			roundStateSwitch();
			roundStateSwitch();
		}
		else
		{
			finishTurn();
		}
	}

	if (currentPlayerIndex >= amountOfPlayersRemaining)
		currentPlayerIndex = 0;

	if (!players[currentPlayerIndex]->isPlayer())
	{
		players[currentPlayerIndex]->makeDecision();
		playerController->debugMessage("AI's makeDecision() is called");		
	}

	playerController->finishTurn();
}

// 3rd step in "casual round loop"
void RoundManager::checkForCommunityCards()
{
	playersDidActions++;

	if (playersDidActions >= amountOfPlayersRemaining)
	{
		bool everyPlayerOnSameBet = false;
		for (int i = 0; i < amountOfPlayersRemaining; ++i)
		{
			if (players[i]->getBetThisRound() == currentMaxBet)
				everyPlayerOnSameBet = true;
			else if (players[i]->getChips() == 0)
				everyPlayerOnSameBet = true;
			else
			{
				everyPlayerOnSameBet = false;
				break;
			}
		}
		if (everyPlayerOnSameBet)
			roundStateSwitch();
	}
}

// possible 4th step in "casual round loop". might be called from step 3, but not necessarily
void RoundManager::roundStateSwitch()
{
	if (roundState == PREFLOP)
	{
		currentPlayerIndex = (dealerIndex) % amountOfPlayersRemaining;

		int flop0[2] = { FMath::RandRange(0, 3), FMath::RandRange(0, 12) };
		int flop1[2] = { FMath::RandRange(0, 3), FMath::RandRange(0, 12) };
		int flop2[2] = { FMath::RandRange(0, 3), FMath::RandRange(0, 12) };

		while (!controlDeck(flop0[0], flop0[1]))
		{
			flop0[0] = FMath::RandRange(0, 3);
			flop0[1] = FMath::RandRange(0, 12);
		}

		while (!controlDeck(flop1[0], flop1[1]))
		{
			flop1[0] = FMath::RandRange(0, 3);
			flop1[1] = FMath::RandRange(0, 12);
		}

		while (!controlDeck(flop2[0], flop2[1]))
		{
			flop2[0] = FMath::RandRange(0, 3);
			flop2[1] = FMath::RandRange(0, 12);
		}

		flop[0] = new Card(flop0[0], flop0[1]);
		flop[1] = new Card(flop1[0], flop1[1]);
		flop[2] = new Card(flop2[0], flop2[1]);

	}
	else if (roundState == FLOP)
	{
		currentPlayerIndex = (dealerIndex) % amountOfPlayersRemaining;

		int turnA[2] = { FMath::RandRange(0, 3), FMath::RandRange(0, 12) };

		while (!controlDeck(turnA[0], turnA[1]))
		{
			turnA[0] = FMath::RandRange(0, 3);
			turnA[1] = FMath::RandRange(0, 12);
		}

		turn = new Card(turnA[0], turnA[1]);
	}
	else if (roundState == TURN)
	{
		currentPlayerIndex = (dealerIndex) % amountOfPlayersRemaining;

		int riverA[2] = { FMath::RandRange(0, 3), FMath::RandRange(0, 12) };

		while (!controlDeck(riverA[0], riverA[1]))
		{
			riverA[0] = FMath::RandRange(0, 3);
			riverA[1] = FMath::RandRange(0, 12);
		}

		river = new Card(riverA[0], riverA[1]);
	}
	else if (roundState == RIVER)
	{
		roundOver();
	}


	lastBet = bigBlind;
	playersDidActions = 0;
	roundState++;
}

// prevents from creating duplicate cards
bool RoundManager::controlDeck(int color, int value)
{
	// this is only doable, because there can only be a maximum of 8 x 2 + 3 + 1 + 1 = 21 cards out of 52 within the game

	if ((deck[color][value]) == 0)
	{
		deck[color][value] = 1;

		return true;
	}
	else // if ((deck[color][value]) == 1)
	{
		return false;
	}
}

// triggers winning calculation and manages winning pot(s) 
void RoundManager::roundOver()
{
	// can be used for debugging calculator:
	

	//players[0]->initializeNewRound(3, 10, 3, 6);
	//players[1]->initializeNewRound(2, 12, 1, 12);

	//amountOfPlayersRemaining = 2;

	//flop[0] = new Card(0,5);
	//flop[1] = new Card(0,2);
	//flop[2] = new Card(0,3);
	//turn    = new Card(2,5);
	//river   = new Card(1,9);

	// -- debugging


	if (amountOfPlayersRemaining > 1)
	{
		Calculator* calc = new Calculator();

		// debugging:
		calc->setPlayerController(playerController);

		int value = -1;
		int player = 0;

		int highestPlayerSoFar[2] = { -1, -1 };					// [0] = quality of cards, [1] player's index within players[];
		int currentPlayer[2];									// [0] = quality of cards, [1] player's index within players[];
		int comparisonKeyCardsArray[5];
		TArray<int> splitPotCandidates;

		for (int k = 0; k < pots.Num(); ++k)					// going throught this procedure for each pot
		{
			for (int i = 0; i < amountOfPlayersRemaining; ++i)	// checking each player
			{
				if (players[i]->getPotAssignment() >= k)		// checking to which pot player belongs to
				{												// the player with the highest potAssignment, obviously plays for each pot available
					currentPlayer[0] = calc->qualityOfCards(players[i]->getCard0(), players[i]->getCard1(), flop[0], flop[1], flop[2], turn, river);
					currentPlayer[1] = i;

					if (currentPlayer[0] > highestPlayerSoFar[0])
					{
						highestPlayerSoFar[0] = currentPlayer[0];
						highestPlayerSoFar[1] = currentPlayer[1];

						splitPotCandidates.Empty();

						for (int n = 0; n < 5; ++n)
							comparisonKeyCardsArray[n] = calc->keyCards[n]->getValue();

						// so keyCards[] will always be the current array, and comparisonKeyCards[] will always be the highest player's one
					}
					else if (currentPlayer[0] == highestPlayerSoFar[0]) // checking for possible splitPot
					{
						int splitPotCounter = 0;
						for (int n = 0; n < 5; ++n)
						{
							if (calc->keyCards[n]->getValue() > comparisonKeyCardsArray[n])
							{
								highestPlayerSoFar[0] = currentPlayer[0];
								highestPlayerSoFar[1] = currentPlayer[1];
								splitPotCandidates.Empty();
								break;
							}
							else if (calc->keyCards[n]->getValue() == comparisonKeyCardsArray[n])
							{
								splitPotCounter++;
							}
						}

						if (splitPotCounter == 5)
						{
							splitPotCandidates.Add(highestPlayerSoFar[1]);
							splitPotCandidates.Add(currentPlayer[1]);
						}
					}
				}
			}

			#pragma region defining winner String
			FString winner;
			if (highestPlayerSoFar[0] == 0)
				winner = "High Card!";
			if (highestPlayerSoFar[0] == 1)
				winner = "Pair!";
			if (highestPlayerSoFar[0] == 2)
				winner = "Two Pair!";
			if (highestPlayerSoFar[0] == 3)
				winner = "Triple!";
			if (highestPlayerSoFar[0] == 4)
				winner = "Straight!";
			if (highestPlayerSoFar[0] == 5)
				winner = "Flush!";
			if (highestPlayerSoFar[0] == 6)
				winner = "Full House!";
			if (highestPlayerSoFar[0] == 7)
				winner = "Quads!";
			if (highestPlayerSoFar[0] == 8)
				winner = "Straight Flush!";
#pragma endregion

			if (splitPotCandidates.Num() > 0) // that means split pot
			{
				playerController->debugMessage("split pot ! splitting pot " + FString::FromInt(k) + " with: ");
				for (int i = 0; i < splitPotCandidates.Num(); ++i)
				{
					players[splitPotCandidates[i]]->increaseChips(pots[k] / splitPotCandidates.Num());

					playerController->debugMessage("" + players[splitPotCandidates[i]]->getName() + " (" + winner + ")");
				}
			}
			else
			{
				playerController->debugMessage("" + players[player]->getName() +" with: " + winner + "wins pot " + FString::FromInt(k));
				players[player]->increaseChips(pots[k]);
			}
		}
		calc->~Calculator();
	}
	else
		playerController->debugMessage("aaaaand the winner is: " + players[currentPlayerIndex]->getName() + " !");



	pots.Empty(); // should be redundant

	resetDeck();
	playerController->roundFinished();
}


// Setters or stuff similar to it

// i dont like this method
// todo: remove
void RoundManager::increasePot(int amount)
{
	pots[pots.Num() - 1] += amount;
	pot = pots[pots.Num() - 1];	// for debugging reasons / showing nooby interface stuff
}

// add one more side pot
void RoundManager::addPot()
{
	pots.Add(0);
}

// resets deck, equal to real life shuffling
void RoundManager::resetDeck()
{
	for (int i = 0; i < 4; ++i)
		for (int n = 0; n < 13; ++n)
			deck[i][n] = 0;
}

// setting blinds for the players
void RoundManager::settingBlinds()
{
	players[dealerIndex + 1]->decreaseChips(smallBlind);
	players[dealerIndex + 1]->increaseBetThisRound(smallBlind);
	players[dealerIndex + 2]->decreaseChips(bigBlind);
	players[dealerIndex + 2]->increaseBetThisRound(bigBlind);

	increasePot(smallBlind + bigBlind);
}

// Getters:

Card* RoundManager::getFlop(int index)
{
	if (roundState >= FLOP)
		return flop[index];
	else return NULL;
}

Card* RoundManager::getTurn()
{
	if (roundState >= TURN)
		return turn;
	else return NULL;
}

Card* RoundManager::getRiver()
{
	if (roundState >= RIVER)
		return river;
	else return NULL;
}

int RoundManager::getRoundstages()
{
	return roundState;
}

int RoundManager::getPot()
{
	return pot;
}

int RoundManager::getAmountOfPlayersRemaining()
{
	return amountOfPlayersRemaining;
}

int RoundManager::getCurrentPlayerIndex()
{
	return currentPlayerIndex;
}

int RoundManager::getCurrentMaxBet()
{
	return currentMaxBet;
}

int RoundManager::getCurrentPlayersBetThisRound()
{
	return players[currentPlayerIndex]->getBetThisRound();
}

RoundManager::~RoundManager()
{
	flop[0]->~Card();
	flop[0] = 0;
	flop[1]->~Card();
	flop[1] = 0;
	flop[2]->~Card();
	flop[2] = 0;
	turn->~Card();
	turn = 0;
	river->~Card();
	river = 0;
	
}
