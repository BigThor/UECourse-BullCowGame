// Fill out your copyright notice in the Description page of Project Settings.
#include "BullCowCartridge.h"

void UBullCowCartridge::BeginPlay() // When the game starts
{
    Super::BeginPlay();

    SetupGame(); // Setting up game
}

void UBullCowCartridge::OnInput(const FString& Input) // When the player hits enter
{
    // Game is not over yet
    if(!bGameOver)
    {
        ProcessGuess(Input);
    }
    else
    {
        // Play again?
        // Yes, Play again
        // No, then game over
        ClearScreen();
        SetupGame();
    }
}

void UBullCowCartridge::SetupGame()
{
    // Welcome the player
    PrintLine(TEXT("Hi, and welcome to the game \"BULLS & COWS\""));

    HiddenWord = TEXT("cake");
    Lives = HiddenWord.Len(); 
    bGameOver = false;
    PrintLine(TEXT("The Hidden word is %s"), *HiddenWord); // Debug line

    PrintLine(TEXT("You have %i lives"), Lives);
    // Promt player for guess
    PrintLine(TEXT("Guess the %i letter word..."), HiddenWord.Len());
}

void UBullCowCartridge::EndGame()
{
    bGameOver = true;
    PrintLine(TEXT("Game is over. Press ENTER to continue..."));
}

void UBullCowCartridge::ProcessGuess(const FString Guess) // Checking user's guess
{
    // Strings are equal (guess is correct)
    if(HiddenWord.Compare(Guess, ESearchCase::IgnoreCase) == 0)
    {
        PrintLine(TEXT("You guessed the word. You WIN!!!"));
        EndGame();
        return;
    }
    // Guess was wrong

    // Check if is not same length
    if(Guess.Len() != HiddenWord.Len())
    {
        PrintLine(TEXT("Hidden Word has a length of %i. Try again!"), HiddenWord.Len());
        return;
    }

    // Check if is not Isogram
    /* if(!bIsIsogram)
    {

    } */
    
    // Remove a life and check if there's any left
    if(--Lives > 0)
    {
        PrintLine(TEXT("You got it wrong. You lose one life"));
        PrintLine(TEXT("You have %i remaining"), Lives);
        return;
    }

    // No lives left
    PrintLine(TEXT("You have no lives left. You lost :("));
    PrintLine(TEXT("The hidden word was %s"), HiddenWord);
    
    EndGame();
}