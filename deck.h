#ifndef DECK_H
#define DECK_H

#include <stdlib.h>
#include "player.h"
#include "card.h"

/*
 * Structure: deck
 * ---------------
 *  An array of 52 cards.
 */
struct deck {
  struct card list[52];
  int top_card;
};

/* 
 * Variable: deck_instance
 * -----------------------
 *
 * Go Fish uses a single deck
 */
struct deck deck_instance; 

/*
 * Function: shuffle
 * --------------------
 *  Initializes deck_instance and shuffles it.
 *  Resets the deck if a prior game has been played.
 * 
 *  returns: 0 if no error, and non-zero on error
 */ 
int shuffle() {
    srand(time(NULL));
    int rank_length = 13; // number of ranks
    int suit_length = 4; // number of suits
    char ranks[13][2] = {"2", "3", "4", "5", "6", "7", "8", "9", "10", "J", "Q", "K", "A"}; // array of ranks
    char suits[4] = "SCHD"; // "array" of suits
    int card_num = 0;

    // for loop iterates through ranks and suits, creating a temporary card instance for each card in the deck, then assigning the deck list indices to each temporary card (rank of 10 uses char value of "1" to identify)
    for (int i = 0; i < rank_length; i++) {
        for (int j = 0; j < suit_length; j++) {
            struct card temp_card;
            temp_card.suit = suits[j];
            temp_card.rank[0] = ranks[i][0];
            deck_instance.list[card_num] = temp_card;
            card_num += 1;
        }
    }
    deck_instance.top_card = 52; // number of cards in the deck

    for (int i = 0; i < deck_instance.top_card; i++) {
        int rand_index = rand() % deck_instance.top_card;
        struct card temp_card_rand = deck_instance.list[rand_index];
        struct card temp_card_i = deck_instance.list[i];
        deck_instance.list[i] = temp_card_rand;
        deck_instance.list[rand_index] = temp_card_i;
    }

    return 0;
}

/*
 * Function: deal_player_cards
 * ---------------------------
 *  Deal 7 random cards to the player specified in the function.
 *  Remove the dealt cards from the deck. 
 *
 *  target: pointer to the player to be dealt cards
 *
 *  returns: 0 if no error, and non-zero on error
 */
int deal_player_cards(struct player* target) {
    srand(time(NULL));
    struct hand* head = NULL;
    struct hand* current = NULL;
    head = (struct hand*)malloc(sizeof(struct hand));
    current = (struct hand*)malloc(sizeof(struct hand));
    for (int i = 0; i < 7; i++) {
        int rand_index = rand() % deck_instance.top_card; // random card is chosen from list and set to current
        if (i == 0) {
            head->top = deck_instance.list[rand_index];
            head->next = (struct hand*)malloc(sizeof(struct hand));
            current = head->next;
        }
        else {
            current->top = deck_instance.list[rand_index];
            if (i < 6) {
                current->next = (struct hand*)malloc(sizeof(struct hand));
                current = current->next;
            }
            else {
                current->next = NULL;
            }
        }
        for (int j = rand_index + 1; j < deck_instance.top_card; j++) { // for loop shifts indices above rand_index to the left by 1 in the deck, to fill the gap that would be created by "removing" the card from the deck
            deck_instance.list[j - 1] = deck_instance.list[j];
        }
        deck_instance.top_card -=1; // subtract by one since we've removed a card
    }

    target->card_list = head;
    target->hand_size = 7;
}

/*
 * Function: next_card
 * -------------------
 *  Return a pointer to the top card on the deck.
 *  Removes that card from the deck. 
 *
 *  returns: pointer to the top card on the deck.
 */
struct card* next_card() {
  struct card temp_card;
  struct card* card_pointer;
  temp_card = deck_instance.list[0];
  card_pointer = &temp_card;
  for (int j = 1; j < deck_instance.top_card; j++) { // for loop shifts indices above rand_index to the left by 1 in the deck, to fill the gap that would be created by "removing" the card from the deck
      deck_instance.list[j - 1] = deck_instance.list[j];
  }
  deck_instance.top_card -= 1;
  return card_pointer;
}

/*
 * Function: size
 * --------------
 *  Return the number of cards left in the current deck.
 *
 *  returns: number of cards left in the deck.
 */
size_t deck_size() {
  return deck_instance.top_card;
}

#endif
