// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Card.h"
#include "MyPlayerP.h"
#include "PlayerControllerP.h" //debugging



class APlayerControllerP;

/**
 * 
 */
class POKEIT_API Calculator
{

private:

	

	static const int amountOfCards = 7;

	Card* cards[amountOfCards];

public:

	// debugging:
	void setPlayerController(APlayerControllerP* pc);

	APlayerControllerP* playerController;
	
	TArray<Card*> keyCards;

	enum CardRanking
	{
		HIGHCARD,
		PAIR,
		TWOPAIR,
		TRIPS,
		STRAIGHT,
		FLUSH,
		FULLHOUSE,
		QUADS,
		STRAIGHTFLUSH,
		ROYALFLUSH
	};


	// functions:

	//debug stuff
	void printKeyCards();


	void fillKeyCards();

	Card* getKeyCard(int index);

	int pairCheck(int pairCheckValue);
	int tripsCheck(int pairCheckValue);
	int quadsCheck();
	

	int flushCheck();

	int straightCheck(int straightFlushCheck);

	void bubbleSortByValue();
	
	int qualityOfCards(Card* hand1, Card* hand2, Card* flop0, Card* flop1, Card* flop2, Card* turn, Card* river);

	Calculator();
	~Calculator();
};
