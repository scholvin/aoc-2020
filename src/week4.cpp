#include "week4.h"

#include <fstream>
#include <deque>

namespace week4
{
    long day22a()
    {
        std::ifstream infile("../data/day22.dat");
        std::string line;

        typedef std::deque<long> deck_t;

        deck_t player1, player2;

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

        // play game
        while (player1.size() && player2.size())
        {
            long card1 = player1.front();
            player1.pop_front();
            long card2 = player2.front();
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
        deck_t& winner = player1.size() ? player1 : player2;
        for (auto it = winner.rbegin(); it != winner.rend(); it++)
            sum += *it * mult++;
        return sum;
    }
}
