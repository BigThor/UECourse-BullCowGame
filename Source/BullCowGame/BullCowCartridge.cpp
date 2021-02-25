// Fill out your copyright notice in the Description page of Project Settings.
#include "BullCowCartridge.h"
#include "Misc/FileHelper.h"
#include "Misc/Paths.h"

void UBullCowCartridge::BeginPlay() // When the game starts
{
    Super::BeginPlay();

    const FString WordListPath = FPaths::ProjectContentDir() / TEXT("WordLists/HiddenWordList.txt");
    FFileHelper::LoadFileToStringArray(Words, *WordListPath);

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

    HiddenWord = GetNewHiddenWord();
    Lives = HiddenWord.Len(); 
    bGameOver = false;
    //PrintLine(TEXT("The Hidden word is %s"), *HiddenWord); // Debug line

    PrintLine(TEXT("You have %i lives"), Lives);
    // Promt player for guess
    PrintLine(TEXT("Guess the %i letter word..."), HiddenWord.Len());
}

void UBullCowCartridge::EndGame()
{
    bGameOver = true;
    PrintLine(TEXT("Game is over. Press ENTER to replay..."));
}

void UBullCowCartridge::ProcessGuess(const FString& Guess) // Checking user's guess
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
    if(!IsIsogram(Guess))
    {
        PrintLine(TEXT("Your guess has repeated letters! Try again"));
        return;
    }
    
    // Remove a life and check if there's any left
    if(--Lives > 0)
    {
        PrintLine(TEXT("You got it wrong. You lose one life"));
        PrintLine(TEXT("You have %i remaining"), Lives);
        FBullCowCount Counter = GetBullsCows(Guess);
        PrintLine(TEXT("You have %i Bulls and %i Cows"), Counter.Bulls, Counter.Cows);

        return;
    }

    // No lives left
    PrintLine(TEXT("You have no lives left. You lost :("));
    PrintLine(TEXT("The hidden word was \"%s\""), *HiddenWord);
    
    EndGame();
}

bool UBullCowCartridge::IsIsogram(const FString& Word) const
{
    int32 Length = Word.Len();
    for(int32 Index = 0; Index < Length; Index++)
    {
        for(int32 Comparison = Index+1; Comparison < Length; Comparison++)
        {
            // Has a repeated letter
            if(Word[Index] == Word[Comparison])
                return false;
        }
    }
    return true;
}

FString UBullCowCartridge::GetNewHiddenWord() const
{
    FString Word;
    do
    {
        int32 RandomIndex = FMath::RandRange(0, Words.Num()-1);
        Word = Words[RandomIndex];
    } while (Word.Len() > 7  || Word.Len() < 4 || !IsIsogram(Word));
    return Word;
}

FBullCowCount UBullCowCartridge::GetBullsCows(const FString& Guess) const
{
    FBullCowCount Counter;

    // for every index guess is same as index hidden, bullCount++
    for(int32 Index = 0; Index < Guess.Len(); Index++)
    {
        if(Guess[Index] == HiddenWord[Index])
        {
            Counter.Bulls++;
            continue;
        }

        for(int32 CompareIndex = 0; CompareIndex < HiddenWord.Len(); CompareIndex++)
        {
            if(Guess[Index] == HiddenWord[CompareIndex])
            {
                Counter.Cows++;
                break;
            }
        }
    }
    return Counter;
}