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

    // returns 1 or 2 depending on who wins this game
    int game(deck_t& player1, deck_t& player2)
    {
        // first: check if these versionf of the deck hav happened before
        std::list<deck_t> prev1, prev2;
        bool done = false;
        while (!done)
        {
            // player 1 is declared the winner if any of these decks have been seen before
            for (auto p: prev1)
                if (p == player1)
                    return 1;
            for (auto p: prev2)
                if (p == player2)
                    return 1;
            prev1.push_back(player1);
            prev2.push_back(player2);

            // OK, now start regular play
            card_t card1 = player1.front();
            player1.pop_front();
            card_t card2 = player2.front();
            player2.pop_front();

            if (card1 <= player1.size() && card2 <= player2.size())
            {
                // recurse on subset of each deck per the instructions
                deck_t new_player1 = player1;
                while (new_player1.size() > card1)
                    new_player1.pop_back();
                deck_t new_player2 = player2;
                while (new_player2.size() > card2)
                    new_player2.pop_back();
                int winner = game(new_player1, new_player2);
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
            }
            else
            {
                // normal play
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
                if (player1.size() == 0 || player2.size() == 0)
                    done = true;
            }
        }
        return player1.size() ? 1 : 2;
    }

    long day22b()
    {
        deck_t player1, player2;
        day22_read(player1, player2);

        // play game
        game(player1, player2);

        // as before
        long mult = 1;
        long sum = 0;
        deck_t& winner = player1.size() ? player1 : player2;
        for (auto it = winner.rbegin(); it != winner.rend(); it++)
            sum += *it * mult++;
        return sum;
    }

    long day23a()
    {
        typedef std::list<long> list_t;
        list_t data = { 8, 5, 3, 1, 9, 2, 6, 4, 7 };

        list_t::iterator current = data.begin();
        for (auto move = 1; move <= 100; move++)
        {
            list_t::iterator cit = current;
            list_t pickup;
            cit++;
            while (pickup.size() < 3)
            {
                if (cit == data.end())
                    cit = data.begin();
                pickup.push_back(*cit);
                cit = data.erase(cit);
            }

            // currrent should point at current cup; data should be the main list with 3 removed, pickup should hold the 3
            long dest = *current;
            do
            {
                dest--;
                if (dest == 0)
                    dest = 9;
            }
            while (dest < 1 || dest > 9 || std::find(pickup.begin(), pickup.end(), dest) != pickup.end());

            // now insert pickup after dest.
            list_t::iterator place = std::find(data.begin(), data.end(), dest);
            // splice inserts before the iterator
            place++;
            data.splice(place, pickup);

            current++;
            if (current == data.end())
                current = data.begin();
        }

        // using the fact that each number is a single digit as a cheat
        std::string answer;
        list_t::iterator final = std::find(data.begin(), data.end(), 1);

        auto it = final;
        it++;
        while (it != final)
        {
            answer += (*it + '0');
            it++;
            if (it == data.end())
                it = data.begin();
        }
        std::cout << answer << std::endl;
        return std::stol(answer);
    }

    long day23b()
    {
        typedef std::list<long> list_t;
        //list_t data = { 8, 5, 3, 1, 9, 2, 6, 4, 7 };
        list_t data = { 3,8,9,1,2,5,4,6,7}; // test

        const long MILLION = 1000000;

        long next = 10;
        while (next <= MILLION)
            data.push_back(next++);

        std::cout << "Start" << std::endl;

        list_t::iterator current = data.begin();
        for (auto move = 1; move <= 10; move++)
        {

            std::cout << "\nmove " << move << std::endl;
            auto p = current;
            for (auto x = 0; x < 25; x++)
            {
                std::cout << *p++ << " ";
            }
            std::cout << "\ncurr " << *current << std::endl;

            list_t::iterator cit = current;
            list_t pickup;
            cit++;
            while (pickup.size() < 3)
            {
                if (cit == data.end())
                    cit = data.begin();
                pickup.push_back(*cit);
                cit = data.erase(cit);
            }
            std::cout << "pick "; std::for_each(pickup.begin(), pickup.end(), [](long l)->void{ std::cout << l << " ";}); std::cout << std::endl;

            // currrent should point at current cup; data should be the main list with 3 removed, pickup should hold the 3
            long dest = *current;
            do
            {
                dest--;
                if (dest == 0)
                    dest = MILLION;
            }
            while (dest < 1 || dest > MILLION || std::find(pickup.begin(), pickup.end(), dest) != pickup.end());
            std::cout << "dest " << dest << std::endl;

            // now insert pickup after dest.
            list_t::iterator place = std::find(current, data.end(), dest);
            if (place == data.end())
                place = std::find(data.begin(), current, dest);
            // splice inserts before the iterator
            place++;
            data.splice(place, pickup);

            current++;
            if (current == data.end())
                current = data.begin();
        }

        std::string answer;
        list_t::iterator final = std::find(data.begin(), data.end(), 1);

        final++;
        long product = *final;
        final++;
        product *= *final;

        return product;

    }

}
