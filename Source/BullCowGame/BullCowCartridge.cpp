// Fill out your copyright notice in the Description page of Project Settings.
#include "BullCowCartridge.h"
#include <ctime>
#include "FileHelper.h"
#include "Paths.h"


// uue sõna võtmine listist

void UBullCowCartridge::BeginPlay()
{
    // Tutvustustekst
    Super::BeginPlay();
    srand(time(NULL));
    TArray<FString> WordList;
    const FString WordListPath = FPaths::ProjectContentDir() / TEXT("HiddenWordList.txt");
    FFileHelper::LoadFileToStringArray(WordList, *WordListPath);
    GetValidWords(WordList);

    UBullCowCartridge::SetupGame();
    UBullCowCartridge::GameIntro();
}

void UBullCowCartridge::OnInput(const FString& Input) // Enterit vajutades
{
    ProcessGuess(Input);  
}

void UBullCowCartridge::SetupGame()
{
    ClearScreen();
    bGameOver = false;
    bGameWon = false;
    HiddenWord = ValidWords[FMath::RandRange(0, ValidWords.Num() - 1)];
    Lives = HiddenWord.Len() * 2;
} 

void UBullCowCartridge::EndGame()
{
    bGameOver = true;
    if (bGameWon == false)
    {
        PrintLine(TEXT("The correct word was %s."), *HiddenWord);
    }
    PrintLine(TEXT("Please press ENTER to continue."));

}

void UBullCowCartridge::GameIntro()
{
    // PrintLine(TEXT("DEBUG WORD %s"), *HiddenWord); // DEBUG
    PrintLine(TEXT("Bulls & Cows!\nPress TAB to type."));
    PrintLine(TEXT("Guess the %i letter word."), HiddenWord.Len());
    if (Lives != 1)
    {
        PrintLine(TEXT("You have %i lives."), Lives);
    }
    else
    {
        PrintLine(TEXT("You have %i life."), Lives);
    }
    
    PrintLine(TEXT("Type in your guess and press ENTER."));
    // PrintLine(TEXT("DEBUG: %i valid words found"), ValidWords.Num());
}

void UBullCowCartridge::ProcessGuess(const FString& Guess)
{
    if(Guess == "HESOYAM")
    {
        Lives = Lives + 20;
        ClearScreen();
        PrintLine(TEXT("Cheats activated! Health set.\n")); 
        UBullCowCartridge::GameIntro();
        return;
    }
    
    if(bGameOver == false)
    {
        if (Guess == HiddenWord)
        {
            ClearScreen();
            PrintLine(TEXT("Correct! You guessed the word!")); 
            bGameWon = true;
            UBullCowCartridge::EndGame();
            return;
        }    

        if (Guess.Len() == HiddenWord.Len() && Lives > 1 && IsIsogram(Guess))
        {
            ClearScreen();
            PrintLine(TEXT("Incorrect guess! Lost a life.")); // pullid ja lehmad vaja lisada
            --Lives;
            int32 Bulls, Cows;
            GetBullCows(Guess, Bulls, Cows);
            PrintLine(TEXT("You have %i Bulls and %i Cows.\n"), Bulls, Cows);
            UBullCowCartridge::GameIntro();
            return;
        }

        if (Guess.Len() == HiddenWord.Len() && Lives == 1 && IsIsogram(Guess))
        {
            ClearScreen();
            --Lives;
            PrintLine(TEXT("Incorrect guess! Out of lives!\n"));
            bGameWon = false;
            EndGame();
            return;
        }
    
        if (Guess.Len() != HiddenWord.Len())
        {
            ClearScreen();
            PrintLine(TEXT("Your word was not %i characters long.\n"), HiddenWord.Len());
            UBullCowCartridge::GameIntro();
            return;
        }

        if (!IsIsogram(Guess))
        {
            ClearScreen();
            PrintLine(TEXT("There are no repeating letters.\n"));
            UBullCowCartridge::GameIntro();
            return;
        }
    }
    else
    {
        UBullCowCartridge::SetupGame();
        UBullCowCartridge::GameIntro();
    }
}

bool UBullCowCartridge::IsIsogram(const FString& Word) const
{
    int32 Index = 0;
    int32 Comparison = Index + 1;
    while (Index < Word.Len() - 1)
    {
        while (Comparison < Word.Len())
        {
            if (Word[Index] == Word[Comparison])
            {
               return false;
            }
            else
            {
                Comparison += 1;
            }
        }
        Index += 1;
        Comparison = Index + 1;
    }
    return true;
    
}

void UBullCowCartridge::GetValidWords(const TArray<FString> &WordList)
{
   for (FString TempWord : WordList)
   {
       if (TempWord.Len() >= 4 && TempWord.Len() <= 8 && IsIsogram(TempWord))
       {
           ValidWords.Emplace(TempWord);
       }
   }
}

void UBullCowCartridge::GetBullCows(const FString& Guess, int32& BullCount, int32& CowCount) const
{
    BullCount = 0;
    CowCount = 0;

    for (int32 GuessIndex = 0; GuessIndex < Guess.Len(); GuessIndex++)
    {
        if (Guess[GuessIndex] == HiddenWord[GuessIndex])
        {
            BullCount++;
            continue;
        }

        for (int32 HiddenIndex = 0; HiddenIndex < Guess.Len(); HiddenIndex++)
        {
            if (Guess[GuessIndex] == HiddenWord[HiddenIndex])
            {
                CowCount++;
                break;
            }
        }
    }
}


