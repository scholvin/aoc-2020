#include "week4.h"

#include <fstream>
#include <iostream>
#include <deque>
#include <list>

namespace week4
{
    // use a smaller type than our usual long since copies will be made for 22b
    typedef size_t card_t;
    typedef std::deque<card_t> deck_t;

    void day22_read(deck_t& player1, deck_t& player2)
    {
        std::ifstream infile("../data/day22.dat");
        std::string line;

        std::getline(infile, line); // throw away Player 1:
        while (true)
        {
            std::getline(infile, line);
            if (line.size() == 0)
                break;
            player1.push_back(std::stol(line));
        }

        std::getline(infile, line); // throw away Player 2:
        while (std::getline(infile, line))
            player2.push_back(std::stol(line));
    }

    long day22a()
    {
        deck_t player1, player2;
        day22_read(player1, player2);

        // play game
        while (player1.size() && player2.size())
        {
            card_t card1 = player1.front();
            player1.pop_front();
            card_t card2 = player2.front();
            player2.pop_front();
            if (card1 > card2)
            {
                player1.push_back(card1);
                player1.push_back(card2);
            }
            else
            {
                player2.push_back(card2);
                player2.push_back(card1);
            }
        }

        long mult = 1;
        long sum = 0;
        const deck_t& winner = player1.size() ? player1 : player2;
        for (auto it = winner.rbegin(); it != winner.rend(); it++)
            sum += *it * mult++;
        return sum;
    }

    long gc = 0;

    // returns 1 or 2 depending on who wins this game
    int game(deck_t& player1, deck_t& player2)
    {
        long rc = 1;
        gc++;

        std::list<deck_t> prev1, prev2;
        bool done = false;
        while (!done)
        {
            // player 1 is declared the winner if any of these decks have been seen before
            for (auto p: prev1)
                if (p == player1)
                {
                    std::cout << "cycle! p1 winner" << std::endl;
                    return 1;
                }
            for (auto p: prev2)
                if (p == player2)
                {
                    std::cout << "cycle! p1 winner" << std::endl;
                    return 1;
                }

            prev1.push_back(player1);
            prev2.push_back(player2);

#if 0
            std::cout << "round " << rc << " game " << gc << std::endl;
            std::cout << "p1 deck: "; std::for_each(player1.begin(), player1.end(), [](card_t c)->void {std::cout << c << " ";}); std::cout << std::endl;
            std::cout << "p2 deck: "; std::for_each(player2.begin(), player2.end(), [](card_t c)->void {std::cout << c << " ";}); std::cout << std::endl;
#endif
            card_t card1 = player1.front();
            player1.pop_front();
            card_t card2 = player2.front();
            player2.pop_front();
            //std::cout << "p1 play: " << card1 << std::endl << "p2 play: " << card2 << std::endl;

            if (card1 <= player1.size() && card2 <= player2.size())
            {
                //std::cout << "subgame" << std::endl;
                // recurse on subset of each deck
                deck_t new_player1 = player1;
                while (new_player1.size() > card1)
                    new_player1.pop_back();
                deck_t new_player2 = player2;
                while (new_player2.size() > card2)
                    new_player2.pop_back();
                int winner = game(new_player1, new_player2);
                //std::cout << "subgame winner: " << winner << std::endl;
                if (winner == 1)
                {
                    player1.push_back(card1);
                    player1.push_back(card2);
                }
                else
                {
                    player2.push_back(card2);
                    player2.push_back(card1);
                }
                gc--;
                //return winner;
            }
            else
            {
                // normal play
                if (card1 > card2)
                {
                    //std::cout << "p1 wins round" << std::endl;
                    player1.push_back(card1);
                    player1.push_back(card2);
                }
                else
                {
                    //std::cout << "p2 wins round" << std::endl;
                    player2.push_back(card2);
                    player2.push_back(card1);
                }
                if (player1.size() == 0 || player2.size() == 0)
                    done = true;
                rc++;
            }
        }
        int wp = player1.size() ? 1 : 2;
        return wp;
    }

    long day22b()
    {
        deck_t player1, player2;
        day22_read(player1, player2);

        // play game
        int wp = game(player1, player2);

        // as before
        long mult = 1;
        long sum = 0;
        deck_t& winner = player1.size() ? player1 : player2;
        for (auto it = winner.rbegin(); it != winner.rend(); it++)
            sum += *it * mult++;
        return sum;
    }
}
