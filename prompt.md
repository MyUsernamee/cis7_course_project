### Reference
This was taken directly from the course assignment and is put here as reference.

# Problem Statement

Blackjack is one of the most popular casino card games in the world. The name blackjack comes from the fact that when blackjack was first introduced in the U.S. it wasn't very popular, so casinos and gambling houses tried offering different bonus payoffs. One of those was a 10-to-1 payoff for a hand consisting of the ace of spades and a black jack (that is, the jack of spades or the jack of clubs). With the current rules, a blackjack hand doesn't even need to contain a jack.

## Rules 

A blackjack game has a dealer and one or more players. Each player plays against the dealer. All players are initially dealt two cards and the dealer is dealt one card face down and one face up (these are called the hole card and up card respectively). Each player can then hit (ask for an additional card) until her total exceeds 21 (this is called busting) or she decides to stand (stop taking cards for the rest of the hand). Face cards count as 10 and an ace may be counted as 1 or 11. After all of the players have finished, the dealer reveals the hole card and plays the hand with a fixed strategy: hit on 16 or less and stand on 17 or more.

The player loses if she busts and wins if she does not bust and the dealer does (observe that if both the player and the dealer bust, the player loses). Otherwise, the player wins if her total is closer to 21 than the dealer's. If the player wins, she gets twice her bet; if she loses, she loses her money. If the dealer and player tie it is called a "push;" the player keeps her bet but does not earn any additional money. If the player's first two cards total 21, this is a blackjack and she wins 1.5 times her bet (unless the dealer also has a blackjack, in which case a tie results), so she gets back 2.5 times her bet.

## Soft Hand
A hand that contains an ace that can be counted as 11 is called a soft hand, since one cannot bust by taking a card. With soft hands, the basic strategy is to always hit 17 or less and even hit 18 if the dealer's up card is 9 or 10 (where the 10 refers to a 10, J, Q, or K).

## Doubling down

After the player is dealt her initial two cards she has the option of doubling her bet and asking for one additional card (which is dealt face down). The player may not hit beyond this single required card. With the basic strategy, you should always double with a total of 11, double with 10 unless the dealer's up card is 10 or A, and double with 9 only against a dealer's 2 to 6. (Some casinos only allow doubling down on 11).

## Splitting pairs
At the beginning of a hand, if the player has two cards with the same number (that is, a pair) she has the option of splitting the pair and playing two hands. In principle, a pair of aces should of course be split, but in this case blackjack rules allow you to get only one card on each hand, and getting a 10 does not make a blackjack. With the basic strategy, you should never split 10's, 5's or 4's, always split 8's, and, in the other cases, split against an up card of 2 to 7, but not otherwise.

## Strategies for the Player

Blackjack is almost always disadvantageous for the player, meaning that no strategy yields a positive expected payoff for the player. In the long run, whatever you do, you will on average lose money. Exceptions exist: some casinos offer special rules that allow a player using the right strategy to have a positive expected payoff; such casinos are counting on the players making mistakes. This basic strategy is based on the player's point total and the dealer's visible card. It consists of a table that describes what you should do in any situation in the game.

Many people assume that the best strategy for the player is to mimic the dealer. A second conservative
strategy is called never bust: hit 11 or less, stand on 12 or more. Each of these strategies leads to a
player disadvantage of about 6%.

--- 

1. Based on the above description of Black Jack, create a C++ program that represents the Black Jack casino game using single card deck (52 cards). In the program, provide player advantage probability in for each hand.

 - The program must show dealer hand and player hand.
 - The program must determine the winning probability for each player hand before and after card hit. *Probability cannot be hard-coded. Calculation of probability must be integrated into the program.

2. In the documentation, include your programming approaches and solutions that apply course.
