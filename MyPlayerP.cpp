// Fill out your copyright notice in the Description page of Project Settings.

#include "PoKeIt.h"
#include "MyPlayerP.h"

MyPlayerP::MyPlayerP(int chips, FString name, bool boolIsPlayer)
{
	this->boolIsPlayer = boolIsPlayer;
	playerName = name;
	this->chips = chips;
	betThisRound = 0;
}

void MyPlayerP::makeDecision()
{
	
}

Card* MyPlayerP::getCard0()
{
	return cards[0];
}

Card* MyPlayerP::getCard1()
{
	return cards[1];
}

void MyPlayerP::increaseChips(int amount)
{
	chips += amount;
}

void MyPlayerP::decreaseChips(int amount)
{
	chips -= amount;
}

void MyPlayerP::increaseBetThisRound(int amount)
{
	betThisRound += amount;
}

int MyPlayerP::getBetThisRound()
{
	return betThisRound;
}

int MyPlayerP::getChips()
{
	return chips;
}

void MyPlayerP::initializeNewRound(int card0Color, int card0Value, int card1Color, int card1Value)
{
	betThisRound = 0;
	
	cards[0]->~Card();
	cards[1]->~Card();

	cards[0] = new Card(card0Color, card0Value);
	cards[1] = new Card(card1Color, card1Value);
}

FString MyPlayerP::getName()
{
	return playerName;
}

int MyPlayerP::getPotAssignment()
{
	return potAssignment;
}

bool MyPlayerP::isPlayer()
{
	return boolIsPlayer;
}

void MyPlayerP::setPotAssignment(int i)
{
	potAssignment = i;
}

void MyPlayerP::destroyCards()
{
	cards[0]->~Card();
	cards[1]->~Card();
	cards[0] = 0;
	cards[1] = 0;
}

MyPlayerP::~MyPlayerP()
{
	cards[0]->~Card();
	cards[1]->~Card();
}
