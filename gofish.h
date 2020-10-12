#ifndef GOFISH_H
#define GOFISH_H

/*
   Define any prototype functions
   for gofish.h here.
*/
/*
 *Function: end
 *-------------------
 *  Called when an end of game is flagged, Player 1 decided if they want
 *  play again or not.
 *
 *
 *  Inputs: user and computer player structures
 * 
 *  Returns: integers, determines outcome
*/

int end(struct player *user, struct player *computer){
    printf("Would you like to play again? (Enter y or n): ");
    char input;
    scanf(" %c", &input);
    if((input == 'y') || (input=='Y')){
      reset_player(user);
      reset_player(computer);
      return 1;
    }
    return 0;
}

/*
 *Function: printCards
 *----------------------
 *
 * prints the cards of the user (and/or computer's) hand
 *
 *
 * Inputs: user and computer player structure types
 * Outputs: Void type
 */

void printCards(struct player *user, struct player* computer){
  struct hand *current; // more printing stuff
  current = user->card_list;
      printf("Player 1's hand: ");
      while (current != NULL) {
        printf("%c%c ", current->top.rank[0], current->top.suit);
        current = current->next;
      }
      /*current = computer->card_list; // more printing
      printf("\n");
      printf("%s", "Computer's hand: ");
      while (current != NULL) {
        printf("%c%c ", current->top.rank[0], current->top.suit);
        current = current->next;
      } */
}

/* Function: printBooks
 *-----------------------
 *
 * prints the books of the user and computer
 * 
 * Inputs: user, computer player structure types
 * 
 * Outputs: Void type
 */

void printBooks(struct player *user, struct player *computer){
  printf("\nPlayer 1's book - ");
  int i=0;
  int j=0;

  while(user->book[i] != NULL){
    printf("%c, ", user->book[i]);
    i++;
  }
  printf("\nComputer's book - "); 
  while(computer->book[j] != NULL){
    printf("%c, ", computer->book[j]);
    j++;
  }
  printf("\n");
}

/* Function: swapCards
 * ---------------------
 * 
 * Upon a match with selected rank, swaps cards from one player to another
 * 
 * Inputs: user and computer player structure types
 *         character rank: which card to search and swap
 *         int i: 0 or 1 to determine which way swap will occur
 */

void swapCards(struct player *user, struct player *computer, char rank, int i){
  if(i==0){
    printf("\t-Player 1 has: ");
    output_matches(user, rank);
    printf("\n\t-Computer's Matches: ");
    output_matches(computer, rank);
    transfer_cards(computer, user, rank);
    printf("\n");
    printf("\n\t------ Correct Choice! Please Go Again ------\n\n");
  }
  if(i==1){
    printf("\t-Computer has: ");
    output_matches(computer, rank);
    printf("\n\t-Player 1's Matches: ");
    output_matches(user, rank);
    transfer_cards(user, computer, rank);
    printf("\n");
    printf("\n\t------ Computer Chose Correctly! Computer Goes Again ------\n\n");
  }
  
  printCards(user, computer);
}

/* Function: goFish
 * -------------------
 * 
 * Called if desired rank is not in hand of opponent
 * 
 * Inputs: user and computer player structure type
 *         integer of turn to determine which player is up
 *         character of rank not found
 * Outputs: Void type
 * 
 */

void goFish(struct player *user, struct player *computer, int turn, char rank){
   if(turn==0){
      printf("\t-Computer has no %c's\n", rank);
      struct card go_fish_card = *next_card();
      printf("\t-Go Fish, Player 1 draws %c%c\n", go_fish_card.rank[0], go_fish_card.suit);
      add_card(user, &go_fish_card);
   }
   if(turn == 1){
      printf("\t-Player 1 has no %c's\n", rank);
        struct card go_fish_card = *next_card();
        printf("\t-Go Fish, Computer draws %c%c\n", go_fish_card.rank[0], go_fish_card.suit);
        add_card(computer, &go_fish_card);
   }
}

/* Function: checkSize
 *--------------------------
 * 
 * Checks to see if a player has no more cards. If so, game is over.
 * 
 * 
 * inputs: player structure type to be checked
 *          int of turn number, to determine which player is checked
 * 
 *  outputs: integer to determine if game will continue
 * 
 */

int checkSize(struct player *user, struct player *computer, int turn){
   int newGame = 0;
   size_t hand;
   if(turn==0){
      hand = user->hand_size;
      if(hand!=0){
         return 0;
      }
      printf("\n You Ran out of Cards! Game Over!");
      int x = end(user, computer);
      newGame=1;
      if(x==0){
         newGame = 1;
      }
   }
   if(turn==1){
      hand = computer->hand_size;
      if(hand!=0){
         return 0;
      }
      printf("The Computer Ran out of Cards! Game Over!\n");
      int x = end(user, computer);
      newGame=1;
      if(x==0){
         newGame=2;
      }
   }
   return newGame;
}
int countBook(struct player *user, struct player *computer, int size, int turn){
   if(turn == 0){
      char check_book = check_add_book(user);
      if (check_book != '0'){
         user->book[size] = check_book;
         size += 1;
         printf("\n\n\t------ %s%c ------\n", "Player 1 has 4 of rank: ", check_book);
         printf("\t------Rank added to book------\n\n");
      }
      return size;
   }
   if(turn == 1){
      char check_book = check_add_book(computer);
      if (check_book != '0') {
          computer->book[size] = check_book;
          size += 1;
          printf("\n\n\t------%s%c------\n", "Computer has 4 of rank: ", check_book);
          printf("\t------Rank added to book------\n\n");
      }
      return size;
   }
}


/* Function: isFinished
 *-------------------------
 * 
 * Checks to see if either player has won
 * 
 * Inputs: user and computer player structure types
 *         integer of turn value to determine which player to check
 * 
 * Outputs: integer set to newGame variable
 * 
 */


int isFinished(struct player *user, struct player *computer, int turn){
   int temp = 0;
   if(turn == 0){
      int game = game_over(user);
      if(game == 1){
         printf("Congratiulations! You Won!\n");
         int x = end(user, computer);
         temp = 1;
         if(x==0){
            temp=2;
         }
      }
      return temp;
   }

   if(turn==1){
      int game = game_over(computer);
      if(game==1){
         printf("\nThe Computer Won\n");
         int x = end(user, computer);
         temp = 1;
         if(x==0){
            temp=2;
         }
      }
      return temp;
   }
}
#endif
