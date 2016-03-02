// Fill out your copyright notice in the Description page of Project Settings.

#pragma once


#include "GameFramework/PlayerController.h"
#include "MyPlayerP.h"
#include "Card.h"
#include "RoundManager.h"
#include "KI.h"
#include <vector>
#include "PlayerControllerP.generated.h"



class RoundManager;

/**
 * 
 */
UCLASS()
class POKEIT_API APlayerControllerP : public APlayerController
{
	GENERATED_BODY()

private:
	
	// VARIABLES

	bool roundHasFinished = false;

	AHUD *myAhud;
	UBlueprint* myhud;
	int smallBlind;
	int bigBlind;
	int roundsPlayed = 0;
	int amountOfPlayers = 4; //4
	int amountKI = 2; //2

	// FUNCTIONS

public:

	/**
	* UPROPERTY and UFUNCTION make the variables and functions to appear in blueprints
	*/

	// variables:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "dealerIndex")
		int32 dealerIndex=0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "chips")
		int32 chips;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "chips")
		int32 currentPlayersChips=0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "player")
		FString currentPlayerName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "round management")
		int32 potSize=0;

#pragma region card variables for HUD

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "cards")
		int32 cardColor0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "cards")
		int32 cardValue0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "cards")
		int32 cardColor1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "cards")
		int32 cardValue1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "cards")
		int32 flopCard0Color;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "cards")
		int32 flopCard0Value;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "cards")
		int32 flopCard1Color;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "cards")
		int32 flopCard1Value;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "cards")
		int32 flopCard2Color;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "cards")
		int32 flopCard2Value;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "cards")
		int32 turnColor;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "cards")
		int32 turnValue;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "cards")
		int32 riverColor;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "cards")
		int32 riverValue;

#pragma endregion


	std::vector<MyPlayerP*> players;

	// former implementation
	//MyPlayerP* players[8];
	RoundManager* roundManager;
	Card* currentPlayersHand[2];


	// FUNCTIONS


	// UFunctions to be sent to blueprint

	// actually debug so far
	UFUNCTION(BlueprintImplementableEvent, Category = "updating HUD")
		void debugBlueprintFunc();

	// updating hand cards	
	UFUNCTION(BlueprintImplementableEvent, Category = "updating HUD")
		void updateHUDcardsBP();

	// spawning community cards in blueprint
	UFUNCTION(BlueprintImplementableEvent, Category = "updating HUD")
		void spawnCommunityCardsBP(int32 roundStage);


	// UFunctions to be called from blueprint

	

	UFUNCTION(BlueprintCallable, Category = "check if round is finished")
		bool isRoundFinished();
	
	UFUNCTION(BlueprintCallable, Category = "check if current player is AI")
		bool currentPlayerisAI();

	UFUNCTION(BlueprintCallable, Category = "set player amount")
		void setPlayerAmount(int32 amount);

	UFUNCTION(BlueprintCallable, Category = "players")
		void spawnPlayers();// (int32 amountOfPlayersSelected);

	UFUNCTION(BlueprintCallable, Category = "playerAction")
		void foldRound();

	UFUNCTION(BlueprintCallable, Category = "playerAction")
		void betRaise(int32 amountToBet);

	UFUNCTION(BlueprintCallable, Category = "playerAction")
		void checkRound();

	UFUNCTION(BlueprintCallable, Category = "playerAction")
		void callRound();

	UFUNCTION(BlueprintCallable, Category = "roundstage")
		int32 getRoundstages();

	UFUNCTION(BlueprintCallable, Category = "bets")
		int32 getCurrentMaxBet();

	UFUNCTION(BlueprintCallable, Category = "bets")
		int32 getCurrentPlayersBetThisRound();

	UFUNCTION(BlueprintCallable, Category = "start new round")
		void startNewRound();

	//todo: why is this UFUNCTION? 
	UFUNCTION(BlueprintCallable, Category = "playerAction")
		void updateHUDcards();

	// normal functions:

	void checkForLeavingPlayers();

	void adjustBlinds();

	void updateHUD();

	void finishTurn();

	void roundFinished();

	void debugMessage(FString s);
	
	


	// constructor:

	APlayerControllerP();


};
