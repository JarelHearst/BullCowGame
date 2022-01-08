// Fill out your copyright notice in the Description page of Project Settings.
#include "BullCowCartridge.h"
#include "HiddenWordList.h"

void UBullCowCartridge::BeginPlay() // When the game starts
{
    Super::BeginPlay();
    Isograms = GetValidWords(Words);
    InitGame(); //Set up game
}

void UBullCowCartridge::OnInput(const FString& PlayerInput) // When the player hits enter
{  
    if(bGameOver)
    {
        ClearScreen();
        InitGame();
    }
    else //Checking the player guess.
    {
        ProcessGuess(PlayerInput);
    }
    
}

void UBullCowCartridge::InitGame()
{
    //Welcoming the Player
    PrintLine(TEXT("Welcome to Bulls and Cows."));
    HiddenWord = Isograms[FMath::RandRange(0, Isograms.Num() -1)]; //Set HiddenWord
    Lives = HiddenWord.Len() * 2; //Set Lives
    bGameOver = false;
    PrintLine(TEXT("In this game you need to guess the right  word! Good Luck!"));
    PrintLine(TEXT("Guess the %i letter word"), HiddenWord.Len());
    PrintLine(TEXT("You have %i Lives"), Lives);
    PrintLine(TEXT("Right letter, wrong spot - Cows"));
    PrintLine(TEXT("Right letter, right spot - Bulls"));
    PrintLine(TEXT("Please press tab and type in your guess \nand press enter to continue...")); //Asking for a guess.
    //PrintLine(TEXT("The Hidden Word is: %s."), *HiddenWord); //Debug line

}

void UBullCowCartridge::EndGame()
{
    bGameOver = true;
    PrintLine(TEXT("\nPress enter to play again..."));
}

void UBullCowCartridge::ProcessGuess(const FString& Guess)
{
    if(HiddenWord == Guess)
    {
        PrintLine(TEXT("You Win!"));
        EndGame();
        return;
    }

    if(Guess.Len() != HiddenWord.Len()) //If the length of the input isn't the same as the hidden word length.
    {
        PrintLine(TEXT("The hidden word is %i letters long"), HiddenWord.Len());
        PrintLine(TEXT("Sorry, try guessing again! \nYou have %i lives remaining"), Lives);
        return;
    }

    // Check if its an isogram
    if (!IsIsogram(Guess))
    {
        PrintLine(TEXT("No repeating letters, guess again"));
        return;
    }

    //Remove lives
    PrintLine(TEXT("You have lost a life!"));
    --Lives;
    //Check if lives are > 0
    if(Lives <= 0)
    {
    ClearScreen();
    PrintLine(TEXT("You hav no lives left!"));
    PrintLine(TEXT("The hidden word was: %s"), *HiddenWord);
    EndGame();
    return;
    }

    //Show the player Bulls and Cows.
    FBullCowCount Score = GetBullCows(Guess);

    PrintLine(TEXT("You have %i Bulls and %i Cows"), Score.Bulls, Score.Cows);

    PrintLine(TEXT("Guess again, you have %i lives left"), Lives);
}

//Function to check to see if a word is an Isogram.
bool UBullCowCartridge::IsIsogram(const FString& Word) const
{
    for(int32 Index = 0; Index < Word.Len(); Index++)
    {
        for(int32 Comparison = Index + 1; Comparison < Word.Len(); Comparison++)
        {
            if(Word[Index] == Word[Comparison])
            {
                return false;
            }
        }
    }
    return true;   
}

TArray<FString> UBullCowCartridge::GetValidWords(const TArray<FString>& WordList) const
{
    TArray<FString> ValidWords;
    for(FString Word : WordList)
    {
        if(Word.Len() >= 4 && Word.Len() <= 8 && IsIsogram(Word))
        {
            ValidWords.Emplace(Word);
        }
    }
    return ValidWords;
}

FBullCowCount UBullCowCartridge::GetBullCows(const FString& Guess) const
{
    FBullCowCount Count;
    for (int32 GuessIndex = 0; GuessIndex < Guess.Len(); GuessIndex++)
    {
        if(Guess[GuessIndex] == HiddenWord[GuessIndex])
        {
            Count.Bulls++;
            continue;
        }
        for (int32 HiddenIndex = 0; HiddenIndex < HiddenWord.Len(); HiddenIndex++)
        {
            if(Guess[GuessIndex] == HiddenWord[HiddenIndex])
            {
                Count.Cows++;
                break;
            }
        }  
    }
    return Count;
}