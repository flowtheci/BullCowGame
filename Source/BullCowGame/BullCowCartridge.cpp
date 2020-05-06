// Fill out your copyright notice in the Description page of Project Settings.
#include "BullCowCartridge.h"
#include <iostream>
#include <ctime>


// uue sõna võtmine listist

void UBullCowCartridge::BeginPlay()
{
    // Tutvustustekst
    Super::BeginPlay();
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
    //UBullCowCartridge::RandomGen();
    //HiddenWord = RandomWord;
    HiddenWord = TEXT("lehmad");
    Lives = HiddenWord.Len();

    // const TCHAR HW[] = TEXT("lehmad");
    // PrintLine(TEXT("Character 1 of the hidden word is %c"), HiddenWord[0]);
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
    //PrintLine(TEXT("DEBUG WORD %s"), *HiddenWord); // DEBUG
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
}

/*
void UBullCowCartridge::RandomGen()
{
    srand(time(NULL));
    int32 WordSeed;
    FString NewWord;
    WordSeed = rand() % 3;
    if (WordSeed == 1)
    {
        NewWord = TEXT("lehmad");
    }
    else if (WordSeed == 2)
    {
        NewWord = TEXT("siga");
    }
    else
    {
        NewWord = TEXT("kuulilennuteetunneliluuk");
    }

    RandomWord = NewWord;
}

*/

void UBullCowCartridge::ProcessGuess(FString Guess)
{
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

        if (Guess.Len() == HiddenWord.Len() && Lives > 1) // isogrammi kontroll vaja lisada
        {
            ClearScreen();
            PrintLine(TEXT("Incorrect guess! Lost a life.\n")); // pullid ja lehmad vaja lisada
            --Lives;
            UBullCowCartridge::GameIntro();
            return;
        }

        if (Guess.Len() == HiddenWord.Len() && Lives == 1)
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

        if (Guess.Len() == HiddenWord.Len() && !IsIsogram(Guess))
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

bool UBullCowCartridge::IsIsogram(FString) const
{
    return true;
}


