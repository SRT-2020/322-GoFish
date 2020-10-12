#include <stdio.h>
#include <stdio.h>
#include <stdlib.h>
#include "card.h"
#include <time.h>
#include "deck.h"
#include "player.h"
#include "gofish.h"
#include "deck.h"
#include <stdbool.h>

int main(int args, char* argv[]) 
{

  //printf("Please Enter the First Letter of your Name: ");
  //scanf(" %c", &name);

  printf("\nShuffling deck...\n\n");
  shuffle(); // shuffles deck and assigns it to deck_instance

  struct player player_object1; // temporary player node
  struct player *user = &player_object1; // node for the user
  struct player player_object2;
  struct player *computer = &player_object2; // node for the computer

  int user_book_size = 0;
  int computer_book_size = 0;

  int turn = 0;
  int newGame = 0;
  int temp;

  deal_player_cards(user); // deals cards to the user
  deal_player_cards(computer); // deals cards to the computer

  printCards(user, computer);


  while (true) {
    if(newGame == 1){   //If a new game is created
      shuffle();
      deal_player_cards(user); // deals cards to the user
      deal_player_cards(computer); // deals cards to the computer
      printf("\n\n\n---------------------------\n");
      printf("------NEW GAME------");
      printf("\n---------------------------------\n\n\n");
      printCards(user, computer);
      turn = 0;
      newGame = 0;
      user_book_size = 0;
      computer_book_size = 0;
    }
    if(newGame == 2){   //If the user wants to quit
      printf("\n");
      printf("Goodbye!\n");
      return 0;
    }

// ----------------------------- Begin Play ---------------------------------
    while (turn == 0 && newGame == 0) {

      printBooks(user, computer);

      char search_rank = user_play(user);

      if (search(computer, search_rank) == 1) {
        swapCards(user, computer, search_rank, 0);
        temp = 0;
      }
      else {
        goFish(user, computer, turn, search_rank);
        temp = 1;
      }
      
      user_book_size = countBook(user, computer, user_book_size, turn);
      
      newGame = isFinished(user, computer, turn);
      if(newGame != 0){
        break;
      }

      newGame = checkSize(user, computer, turn);
      if(newGame != 0){
        break;
      }

      turn = temp;

    

      if(turn == 1){
        printf("\n\n---------------------------------------");
        printf("\n------ Computer's Turn ------\n");
        printf("---------------------------------------\n\n");
      }
    }

    // --------------------------------------- Computer Play -----------------------------------------
    while(turn == 1 && newGame == 0){
      printCards(user, computer);
      printBooks(user, computer);
      char search_rank = computer_play(computer);

      printf("\t-Computer's Chosen Rank ");
      printf("%c\n", search_rank);

      if (search(user, search_rank) == 1) {
        swapCards(user, computer, search_rank, 1);
        temp = 1;
      }
      else {
        goFish(user, computer, turn, search_rank);
        temp = 0;
      }

      computer_book_size = countBook(user, computer, computer_book_size, turn);

      newGame = isFinished(user, computer, turn);
      if(newGame != 0){
        break;
      }

      newGame = checkSize(user, computer, turn);
      if(newGame != 0){
        break;
      }

      turn = temp;

      

      

    

    
      if(turn == 0){
        printf("\n\n---------------------------------------");
        printf("\n------ Player 1's Turn ------\n");
        printf("---------------------------------------\n\n");
        printCards(user, computer);
      }
    }
  }
  return 0;
}
