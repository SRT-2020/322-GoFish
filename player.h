#ifndef PLAYER_H
#define PLAYER_H

#include <stdlib.h>
#include "card.h"
#include <stdbool.h>
#include <stdio.h>
#include <time.h>

/*
 * Structure: player
 * -----------------
 *  Each player holds some number of cards in their hand
 *  Each player can have at most 7 "books" before winning 
 */
struct player {
  struct hand* card_list;
  char book[7];
  size_t hand_size;
};

/* 
 * Instance Variables: user, computer
 * ----------------------------------
 *
 *  We only support 2 users: a human and a computer
 */
struct player user;
struct player computer;

/*
 * Function: add_card
 * -------------------
 *  Add a new card to the player's hand. 
 *
 *  target: the target player
 *  new_card: pointer to the new card to add
 *
 *  returns: return 0 if no error, non-zero otherwise
 */
int add_card(struct player* target, struct card* new_card) {
  struct hand* adding_card = NULL;
  adding_card = (struct hand*)malloc(sizeof(struct hand));
  struct hand *current;
  current = target->card_list;
  while (current != NULL) {
    if (current->next == NULL) {
      struct card temp = *new_card;
      current->next = adding_card;
      current->next->top = *new_card;
      current->next->next = NULL;
      target->hand_size += 1;
      break;
    }
    current = current->next;
  }
  current = adding_card;
  struct card temp = *new_card;
  current->top = temp;
  current->next = NULL;


  return 0;
}

/*
 * Function: remove_card
 * ---------------------
 *  Remove a card from the player's hand. 
 *
 *  target: the target player
 *  old_card: pointer to the old card to remove
 *
 *  returns: return 0 if no error, non-zero otherwise
 */
int remove_card(struct player* target, struct card* old_card) {
  struct hand *current;
  struct hand *next;
  current = target->card_list;
  next = target->card_list->next;
  while (next != NULL) {
    struct card temp;
    temp = next->top;
    struct card* temp_pointer = &temp;
    if (temp_pointer->rank[0] == old_card->rank[0] && temp_pointer->suit == old_card->suit) {
      current->next = current->next->next;
      target->hand_size -= 1;
      return 0;
    }
    current = current->next;
    next = next->next;
  }

  current = target->card_list; // to take care of when trying to remove head of the list
  struct card temp2;
  temp2 = current->top;
  struct card* temp_pointer2 = &temp2;
  if (temp_pointer2->rank[0] == old_card->rank[0] && temp_pointer2->suit == old_card->suit) {
    target->card_list = target->card_list->next;
    target->hand_size -= 1;
    return 0;
  }

  return 1;
}

/*
 * Function: check_add_book
 * ------------------------
 *  Check if a player has all 4 cards of the same rank.
 *  If so, remove those cards from the hand, and add the rank to the book.
 *  Returns after finding one matching set of 4, so should be called after adding each a new card.
 * 
 *  target: pointer to the player to check
 *  
 *  Return: a char that indicates the book that was added; return 0 if no book added.
 */
char check_add_book(struct player* target) {
  char rankArray[13];
  int rankCount[13];
  int num = 0;
  struct card* removeArray[13][4];
  int finalRemoveArray[4];
  char remove_rank = '0';
  int remove_index;

  for (int i = 0; i < 13; i++) {
    rankCount[i] = 0;
  }

  struct hand *current; // more printing stuff
  current = target->card_list;
  while (current != NULL) {
    for (int i = 0; i < 13; i++) {
      if (rankCount[i] == 0) {
        rankCount[i] = 1;
        rankArray[i] = current->top.rank[0];
        removeArray[i][rankCount[i] - 1] = &current->top; //special
        break;
      }
      if (current->top.rank[0] == rankArray[i]) {
        rankCount[i] += 1;
        removeArray[i][rankCount[i] - 1] = &current->top; //special
        if (rankCount[i] == 4) {
          remove_rank =  rankArray[i]; // modified return
          remove_index = i; // special
          break;
        }
      }
    }
    num += 1;
    current = current->next;
  }
  if (remove_rank != '0') {
    for (int i = 0; i < 4; i++) {
      remove_card(target, removeArray[remove_index][i]);
    }
    return remove_rank;
  }
  else {
    return '0';
  }
}

/*
 * Function: search
 * ----------------
 *  Search a player's hand for a requested rank.
 *  
 *  rank: the rank to search for
 *  target: the player (and their hand) to search
 *
 *  Return: If the player has a card of that rank, return 1, else return 0
 */
int search(struct player* target, char rank) {
  struct hand *current; // more printing stuff
  current = target->card_list;
  while (current != NULL) {
    if (rank == current->top.rank[0]) {
      return 1;
    }
    current = current->next;
  }
    return 0;
}

/*
 * Function: transfer_cards
 * ------------------------
 *   Transfer cards of a given rank from the source player's 
 *   hand to the destination player's hand. Remove transferred
 *   cards from the source player's hand. Add transferred cards
 *   to the destination player's hand.
 *   
 *   src: a pointer to the source player
 *   dest: a pointer to the destination player
 *   rank: the rank to transfer
 *
 *   Return: 0 if no cards found/transferred, <0 if error, otherwise 
 *   return value indicates number of cards transferred
 */   
int transfer_cards(struct player* src, struct player* dest, char rank){
  struct hand *tempHand = src->card_list;
  int count = 0;
  while(tempHand != NULL){
    if(rank == tempHand->top.rank[0]){
      add_card(dest, &tempHand->top);
      remove_card(src, &tempHand->top);
      count++;
    }
    tempHand = tempHand->next;
  }
  if(count==0){
    return 0;
  }
  if(count > 0){
    return count;
  }
 return -1;
} 

/*
 * Function: game_over
 * -------------------
 *   Boolean function to check if a player has 7 books yet
 *   and the game is over
 *
 *   target: the player to check
 *   
 *   Return: 1 if game is over, 0 if game is not over
 */
int game_over(struct player* target){
  int count = 0;
  for(int i=0; i<7; i++){
    if(target->book[i] != NULL){
      count++;
    }
  }
  if(count == 7){
    return 1;
  }
  else{
    return 0;
  }
}

/* 
 * Function: reset_player
 * ----------------------
 *
 *   Reset player by free'ing any memory of cards remaining in hand,
 *   and re-initializes the book.  Used when playing a new game.
 * 
 *   target: player to reset
 * 
 *   Return: 0 if no error, and non-zero on error.
 */
int reset_player(struct player* target){
  struct hand *temp = target->card_list;
  struct hand *prev;
  while(temp != NULL){
   prev = temp;
   free(&temp->top);
   temp = temp->next;
  }
  for(int i=0; i<7; i++){
    target->book[i] = NULL;
  }
  target->card_list = temp;
  return 0;
}

/* 
 * Function: computer_play
 * -----------------------
 *
 *   Select a rank randomly to play this turn. The player must have at least
 *   one card of the selected rank in their hand.
 *
 *   target: the player's hand to select from
 *
 *   Rank: return a valid selected rank
 */
char computer_play(struct player* target) {
  srand(time(NULL));
  int hand_size = 0;
  struct hand *current;
  current = target->card_list;
  while (current != NULL) {
    current = current->next;
    hand_size += 1;
  }
  int playing_card_number = rand() % hand_size;
  int num = 0;
  current = target->card_list;
  while (current != NULL) {
    if (num == playing_card_number) {
      return current->top.rank[0];
    }
    num += 1;
    current = current->next;
  }
}

/* 
 * Function: user_play
 * -------------------
 *
 *   Read standard input to get rank user wishes to play.  Must perform error
 *   checking to make sure at least one card in the player's hand is of the 
 *   requested rank.  If not, print out "Error - must have at least one card from rank to play"
 *   and then re-prompt the user.
 *
 *   target: the player's hand to check
 * 
 *   returns: return a valid selected rank
 */
char user_play(struct player* target) {
  printf("Player 1's turn, enter a rank: ");
  while (true) {
      char rank;
      while((rank=getchar())!='\n'&&rank!=EOF) {
        struct hand *current;
        current = target->card_list;
        while (current != NULL) {
          if (current->top.rank[0] == rank) {
            return rank;
          }
          current = current->next;
        }
        printf("\nError - must have at least one card from rank to play\n");
        printf("Please enter a valid rank: ");
      }
    }
}

void output_matches(struct player* target, char rank) {
  struct hand *current;
  current = target->card_list;
  int num = 0;
  while (current != NULL) {
    struct card temp;
    temp = current->top;
    struct card* temp_pointer = &temp;
    if (temp_pointer->rank[0] ==rank) {
      if (num == 0) {
        printf("%c%c", temp_pointer->rank[0], temp_pointer->suit);
      }
      else {
        printf(", %c%c", temp_pointer->rank[0], temp_pointer->suit);
      }
      num += 1;
    }
    current = current->next;
  }
}

#endif


