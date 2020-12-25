#include "week4.h"

#include <fstream>
#include <iostream>
#include <deque>
#include <list>
#include <map>

#include <boost/container_hash/hash.hpp>

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

    typedef std::list<long> list_t;
    const list_t seed = { 8, 5, 3, 1, 9, 2, 6, 4, 7 };

    // for this exercise, we maintain a vector from value to an iterator ("pointer") to the element in the list
    // because trying to find the insertion point element by a brute force in part b is untenable
    typedef std::vector<list_t::iterator> pointers_t;

    void day23(list_t& data, pointers_t& pointers, long moves, long cups)
    {
        const long PICKUPS = 3;

        for (auto s: seed)
        {
            data.push_back(s);
            pointers[s] = std::prev(data.end());
        }

        for (long s = 10; s <= cups; s++)
        {
            data.push_back(s);
            pointers[s] = std::prev(data.end());
        }

        list_t::iterator current = data.begin();
        for (auto move = 1; move <= moves; move++)
        {
            list_t::iterator cit = current;
            list_t pickup;
            cit++;
            while (pickup.size() < PICKUPS)
            {
                if (cit == data.end())
                    cit = data.begin();
                pickup.push_back(*cit);
                cit = data.erase(cit);
                pointers[pickup.back()] = data.end(); // we will need to fix this up
            }

            // currrent should point at current cup; data should be the main list with 3 removed, pickup should hold the 3
            long dest = *current;
            do
            {
                dest--;
                if (dest == 0)
                    dest = cups;
            }
            while (dest < 1 || dest > cups || std::find(pickup.begin(), pickup.end(), dest) != pickup.end());

            // now insert pickup
            list_t::iterator place = pointers[dest];
            for (auto p = 0; p < PICKUPS; p++)
            {
                // insert inserts before the iterator, so move it one to the right
                place++;
                place = data.insert(place, pickup.front());
                pointers[pickup.front()] = place; // we are fixing up what we broke above
                pickup.pop_front();
            }

            current++;
            if (current == data.end())
                current = data.begin();
        }
    }

    long day23a()
    {
        const long CUPS = 9;
        const long MOVES = 100;

        list_t data;
        pointers_t pointers(CUPS+1);

        day23(data, pointers, MOVES, CUPS);

        // using the fact that each number is a single digit as a cheat, build a string and convert
        std::string answer;
        list_t::iterator final = pointers[1];

        auto it = final;
        it++;
        while (it != final)
        {
            answer += (*it + '0');
            it++;
            if (it == data.end())
                it = data.begin();
        }
        return std::stol(answer);
    }

    long day23b()
    {
        const long CUPS = 1000000;
        const long MOVES = 10000000;

        list_t data;
        pointers_t pointers(CUPS+1);

        day23(data, pointers, MOVES, CUPS);

        list_t::iterator final = pointers[1];
        final++;
        long product = *final;
        final++;
        product *= *final;
        return product;
    }

    long day24a()
    {
        std::ifstream infile("../data/day24.dat");
        std::string line;

        /*
            use empire coordinates:

                 -1,-1   -1,1
             -2,0     0,0    2,0
                 -1,1     1,1

            keep a hash of the coordinates to the tile color
        */

        struct coordinates
        {
            long x, y;
            bool operator==(const coordinates& that) const { return x == that.x && y == that.y; }
        };

        struct coordinates_hasher {
            size_t operator()(const coordinates& c) const
            {
                size_t seed = 110571;
                boost::hash_combine(seed, c.x);
                boost::hash_combine(seed, c.y);
                return seed;
            }
        };

        enum tile { white, black };

        typedef std::unordered_map<coordinates, tile, coordinates_hasher> floor_t;
        floor_t floor;

        floor[{0, 0}] = white;

        while (std::getline(infile, line))
        {
            long x = 0, y = 0;
            for (size_t i = 0; i < line.size(); )
            {
                char c = line[i++];
                if (c == 'e') x += 2;
                else if (c == 'w') x -= 2;
                else if (c == 'n' || c == 's')
                {
                    char d = line[i++];
                    if (c == 'n' && d == 'e')      { x++; y--; }
                    else if (c == 'n' && d == 'w') { x--; y--; }
                    else if (c == 's' && d == 'e') { x++; y++; }
                    else if (c == 's' && d == 'w') { x--; y++; }
                }
            }
            auto t = floor.find({x, y});
            if (t == floor.end())
                floor[{x, y}] = black;
            else if (t->second == white)
                t->second = black;
            else t->second = white;
        }

        return std::count_if(floor.begin(), floor.end(), [](const floor_t::value_type& t) { return t.second == black; });
    }
}
