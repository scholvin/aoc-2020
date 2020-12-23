#include "day20.h"

#include <fstream>
#include <iostream>
#include <unordered_set>
#include <cassert>
#include <cstring>

namespace day20
{
    /*
        Day 20. This turned out to be a lot of code, something like 400 lines for part a.
        But it works and is fast enough. Proably a lot of room to improve in the backtracker.

        There are two implementaion classes:

            image - the final composite image, made up of an NxN grid of tiles.
            tile - represents an individual tile on the larger assembled image.

        The image class has methods to perform the recusrive backtrack, the entry point of which is solve(). See
        the comments in the class, but this is fairly typical backtracking. The main data structure here is the NxN
        array of tiles, with each cell represented by inedexes into a vector containing a master set of all the tiles,
        and that tile's permutation (flip/rotation) in the cell.

        The tile class uses a couple of tricks to help with performance (hopefully):

            - at construction, it builds the rotated and flipped copies of itself to aid in matching and,
              it turns out, these will be really useful later for assembling the final image for part b
            - a series of match functions to determine whether a given neighbor, in the given permutation,
              matches the tile on the specified side.

        Both classes support printing debug output to a std::ostream.
    */

    class tile
    {
    public:
        friend class image; // TODO all we really need to friend is dump_full

        // cheat alert - looked at the data
        // size of each individual tile is 10x10
        static constexpr size_t GRID = 10;

        // late breaking news! the tiles can be flipped, not just rotated.
        // later breaking news: there are only 8 possible permutations
        static constexpr size_t PERMUTATIONS = 8;

        // pass it a vector of MAX+1 strings - first is Tile: nnnnn, rest are lines as read in input
        tile(const std::vector<std::string>& lines) : m_id(std::stol(lines[0].substr(5, lines[0].find(':') - 5)))
        {
            // set the first grid at permutation 0
            for (size_t y = 1; y < lines.size(); y++)
            {
                for (size_t x = 0; x < lines[y].size(); x++)
                {
                    m_grids[0][x][y-1] = lines[y][x];
                }
            }

            // rotate thrice for permutations 1 thru 3 - help from here https://www.codespeedy.com/rotate-a-matrix-in-cpp/
            // oddly, they appear to be rotating CCW, not CW, but it's fine
            // do this up front so we're not doing it throughout the solution
            for (size_t r = 1; r <= 3; r++)
                for (size_t i = 0; i < GRID/2; i++)
                    for (size_t j = i; j < GRID-i-1; j++)
                    {
                        m_grids[r][i][j]               = m_grids[r-1][GRID-1-j][i];
                        m_grids[r][GRID-1-j][i]        = m_grids[r-1][GRID-1-i][GRID-1-j];
                        m_grids[r][GRID-1-i][GRID-1-j] = m_grids[r-1][j][GRID-1-i];
                        m_grids[r][j][GRID-1-i]        = m_grids[r-1][i][j];
                    }

            // next 4 permutations are horizontal flips of the first 4
            for (size_t r = 4; r < 8; r++)
                for (size_t i = 0; i < GRID; i++)
                    for (size_t j = 0; j < GRID; j++)
                        m_grids[r][i][j] =             m_grids[r-4][GRID-1-i][j];
        }

        long id() const { return m_id; }

        // for debugging
        void dump(std::ostream& os) const
        {
            os << m_id << std::endl;
            for (size_t y = 0; y < GRID; y++)
            {
                for (size_t r = 0; r < PERMUTATIONS; r++)
                {
                    for (size_t x = 0; x < GRID; x++)
                    {
                        os << m_grids[r][x][y];
                    }
                    os << " ";
                }
                os << std::endl;
            }
        }

        // does other at rotation his_rot match to the right of me at rotation my_rot?
        bool match_right(size_t my_rot, const tile& other, size_t his_rot) const
        {
            for (size_t y = 0; y < GRID; y++)
                if (m_grids[my_rot][GRID-1][y] != other.m_grids[his_rot][0][y])
                    return false;
            return true;
        }

        // does other at rotation his_rot match to the left of me at rotation my_rot?
        bool match_left(size_t my_rot, const tile& other, size_t his_rot) const
        {
            for (size_t y = 0; y < GRID; y++)
                if (m_grids[my_rot][0][y] != other.m_grids[his_rot][GRID-1][y])
                    return false;
            return true;
        }

        // does other at rotation his_rot match above me at rotation my_rot?
        bool match_top(size_t my_rot, const tile& other, size_t his_rot) const
        {
            for (size_t x = 0; x < GRID; x++)
                if (m_grids[my_rot][x][0] != other.m_grids[his_rot][x][GRID-1])
                    return false;
            return true;
        }

        // does other at rotation his_rot match below me at rotation my_rot?
        bool match_bottom(size_t my_rot, const tile& other, size_t his_rot) const
        {
            for (size_t x = 0; x < GRID; x++)
                if (m_grids[my_rot][x][GRID-1] != other.m_grids[his_rot][x][0])
                    return false;
            return true;
        }

    private:
        std::array<std::array<std::array<char, GRID>, GRID>, PERMUTATIONS> m_grids;
        long m_id;
    };

    class image
    {
        // cheat alert! looked at the data, and there are 144 tiles, so assuming 12x12 solution for now
        static const size_t GRID = 3;

    public:
        typedef std::vector<tile> tileset_t;

        static const size_t SEA_GRID = GRID * (tile::GRID - 2);
        typedef std::array<std::array<char, SEA_GRID>, SEA_GRID> sea_t;

        image(const tileset_t& tileset) : m_tileset(tileset)
        {
            for (size_t x = 0; x < GRID; x++)
                for (size_t y = 0; y < GRID; y++)
                    m_inner[x][y] = EMPTY_CELL;

            solve();

            // solve only works on candidate objects, so tihs one's own state never gets updated
            // after the work is done. this is a kludge
            if (s_finals.size())
                m_inner = s_finals.front().m_inner;
            else
                throw std::runtime_error("no solution found");
        }

        // dare to dream
        long result() const
        {
            return m_tileset[m_inner[0][0].first].id() *
                   m_tileset[m_inner[0][GRID-1].first].id() *
                   m_tileset[m_inner[GRID-1][0].first].id() *
                   m_tileset[m_inner[GRID-1][GRID-1].first].id();
        }

        void dump(std::ostream& os) const
        {
            for (size_t y = 0; y < GRID; y++)
            {
                for (size_t x = 0; x < GRID; x++)
                {
                    os << m_inner[x][y].first << "." << m_inner[x][y].second << " ";
                }
                os << std::endl;
            }
        }

        void dump_full(std::ostream& os) const
        {
            // x,y = the tile
            for (size_t y = 0; y < GRID; y++) // big rows
            {
                for (size_t z = 0; z < tile::GRID; z++) // little rows
                {
                    for (size_t x = 0; x < GRID; x++) // big columns
                    {
                        for (size_t c = 0; c < tile::GRID; c++) // little columns
                        {
                            os << m_tileset[m_inner[x][y].first].m_grids[m_inner[x][y].second][c][z];
                        }
                        os << " ";
                    }
                    os << std::endl;
                }
                os << std::endl;
            }

            // also print the reference id's
            for (size_t y = 0; y < GRID; y++)
            {
                for (size_t x = 0; x < GRID; x++)
                {
                    os << m_tileset[m_inner[x][y].first].m_id << " ";
                }
                os << std::endl;
            }
        }

        void make_sea(sea_t& sea) const
        {
            // strip off borders of internal tiles and make into one big image
            for (size_t tx = 0; tx < GRID; tx++) // tile x
            {
                for (size_t ty = 0; ty < GRID; ty++) // tile y
                {
                    for (size_t gx = 1; gx < tile::GRID - 1; gx++) // grid x, with borders stripped off
                    {
                        for (size_t gy = 1; gy < tile::GRID - 1; gy++) // grid y, with borders stripped off
                        {
                            // char to place - overly pedantic code to make it visbly workable
                            auto index = m_inner[tx][ty].first;
                            auto perm = m_inner[tx][ty].second;
                            char c = m_tileset[index].m_grids[perm][gx][gy];

                            // convert to sea coordinates
                            size_t sea_x = tx * (tile::GRID - 2) + gx - 1;
                            size_t sea_y = ty * (tile::GRID - 2) + gy - 1;
                            assert (sea_x < SEA_GRID && sea_y < SEA_GRID);
                            sea[sea_x][sea_y] = c;
                        }
                    }
                }
            }
        }

    private:

        void solve()
        {
            // there are many solutions - for now, we only care about the first one
            // lots of stack unwinding to do here maybe
            if (s_finals.size())
            {
                s_depth--;
                return;
            }

            s_iterations++;
            s_depth++;

            if (accept())
            {
                // std::cout << "ACCEPTING at iteration=" << s_iterations << " stack depth=" << s_depth<< std::endl;
                // dump(std::cout);
                s_finals.push_back(*this);
                s_depth--;
                return;
            }

            for (size_t y = 0; y < GRID; y++)
            {
                for (size_t x = 0; x < GRID; x++)
                {
                    if (m_inner[x][y] != EMPTY_CELL)
                    {
                        // keep searching until we find an empty cell
                        continue;
                    }

                    image candidate(*this);

                    // x,y is the first empty cell, so find a tile/rotation to try there
                    for (size_t t = 0; t < m_tileset.size(); t++)
                    {
                        for (size_t r = 0; r < tile::PERMUTATIONS; r++)
                        {
                            // encapsulation fail
                            candidate.m_inner[x][y].first = t;
                            candidate.m_inner[x][y].second = r;
                            if (candidate.reject())
                            {
                                // there's no point in trying this tile here
                                continue;
                            }
                            else
                            {
                                // this is worth trying
                                candidate.solve();
                            }
                        }
                    }

                    // backtrack - set the cell we were working on to empty and head back up
                    candidate.m_inner[x][y] = EMPTY_CELL;
                    s_depth--;
                    return;
                }
            }
            throw std::logic_error("shouldn't get here");
        }

        // return true if this partial candidate isn't worth considering further
        bool reject() const
        {
            // first make sure we're not using a tile twice
            std::unordered_set<size_t> used;
            for (size_t x = 0; x < GRID; x++)
            {
                for (size_t y = 0; y < GRID; y++)
                {
                    if (m_inner[x][y] == EMPTY_CELL)
                        continue;
                    if (used.count(m_inner[x][y].first))
                        return true;
                    else
                        used.insert(m_inner[x][y].first);
                }
            }

            // for every non-empty cell, check all his non-empty neighbors for a match
            for (size_t x = 0; x < GRID; x++)
            {
                for (size_t y = 0; y < GRID; y++)
                {
                    if (m_inner[x][y] == EMPTY_CELL)
                        continue;

                    const tile& curr_tile = m_tileset[m_inner[x][y].first];
                    const size_t curr_rot = m_inner[x][y].second;

                    if (x > 0 && m_inner[x-1][y] != EMPTY_CELL && !curr_tile.match_left(curr_rot, m_tileset[m_inner[x-1][y].first], m_inner[x-1][y].second))
                        return true;
                    if (y > 0 && m_inner[x][y-1] != EMPTY_CELL && !curr_tile.match_top(curr_rot, m_tileset[m_inner[x][y-1].first], m_inner[x][y-1].second))
                        return true;
                    if (x < GRID-1 && m_inner[x+1][y] != EMPTY_CELL && !curr_tile.match_right(curr_rot, m_tileset[m_inner[x+1][y].first], m_inner[x+1][y].second))
                        return true;
                    if (y < GRID-1 && m_inner[x][y+1] != EMPTY_CELL && !curr_tile.match_bottom(curr_rot, m_tileset[m_inner[x][y+1].first], m_inner[x][y+1].second))
                        return true;
                }
            }
            return false;
        }

        // is this solution totally valid? return true if so
        bool accept() const
        {
            // if any cell is still empty, I'm no good
            for (size_t x = 0; x < GRID; x++)
                for (size_t y = 0; y < GRID; y++)
                    if (m_inner[x][y] == EMPTY_CELL)
                        return false;

            for (size_t x = 0; x < GRID; x++)
            {
                for (size_t y = 0; y < GRID; y++)
                {
                    const tile& curr_tile = m_tileset[m_inner[x][y].first];
                    const size_t curr_rot = m_inner[x][y].second;

                    if (x > 0 && !curr_tile.match_left(curr_rot, m_tileset[m_inner[x-1][y].first], m_inner[x-1][y].second))
                        return false;
                    if (y > 0 && !curr_tile.match_top(curr_rot, m_tileset[m_inner[x][y-1].first], m_inner[x][y-1].second))
                        return false;
                    if (x < GRID-1 && !curr_tile.match_right(curr_rot, m_tileset[m_inner[x+1][y].first], m_inner[x+1][y].second))
                        return false;
                    if (y < GRID-1 && !curr_tile.match_bottom(curr_rot, m_tileset[m_inner[x][y+1].first], m_inner[x][y+1].second))
                        return false;
                }
            }
            return true;
        }

        // each cell in the final grid
        // -- first is the index of the tile in the vector of them that we read in
        // -- second is the rotation [0, 3]
        typedef std::pair<size_t, size_t> cell_t;
        static constexpr cell_t EMPTY_CELL = { 666, 666 };

        typedef std::array<std::array<cell_t, GRID>, GRID> inner_t;
        inner_t m_inner;

        const tileset_t& m_tileset;

        static std::vector<image> s_finals;
        static int s_depth;
        static int s_iterations;
    };

    void load_tiles(image::tileset_t& tiles)
    {
        std::ifstream infile("../data/day20.dat");
        std::string line;

        while (true)
        {
            std::vector<std::string> batch;
            for (size_t i = 0; i < tile::GRID + 1; i++)
            {
                std::getline(infile, line);
                batch.push_back(line);
            }
            tiles.push_back(tile(batch));
            // either read a blank line, or hit EOF
            if (!std::getline(infile, line))
                break;
        }

#if 0
        // a big chunk of "test" code just to make sure the functions in the tile class do the right thing
        std::cout << "sizeof(tile)=" << sizeof(tile) << " sizeof(image)=" << sizeof(image) << std::endl;

        tiles[0].dump(std::cout);

        // make sure the match_ functions find something and print the results for visual inspection
        bool top = false, bottom = false, left = false, right = false;

        for (size_t i = 0; i < tiles.size(); i++)
            for (size_t j = i + 1; j < tiles.size(); j++)
                for (size_t r = 0; r < tile::PERMUTATIONS; r++)
                    for (size_t s = 0; s < tile::PERMUTATIONS; s++)
                    {
                        if (!right && tiles[i].match_right(r, tiles[j], s))
                        {
                            std::cout << "tile " << i << "." << r << " match_right " << j << "." << s << std::endl;
                            tiles[i].dump(std::cout);
                            tiles[j].dump(std::cout);
                            right = true;
                        }
                        if (!left && tiles[i].match_left(r, tiles[j], s))
                        {
                            std::cout << "tile " << i << "." << r << " match_left " << j << "." << s << std::endl;
                            tiles[i].dump(std::cout);
                            tiles[j].dump(std::cout);
                            left = true;
                        }
                        if (!top && tiles[i].match_top(r, tiles[j], s))
                        {
                            std::cout << "tile " << i << "." << r << " match_top " << j << "." << s << std::endl;
                            tiles[i].dump(std::cout);
                            tiles[j].dump(std::cout);
                            top = true;
                        }
                        if (!bottom && tiles[i].match_bottom(r, tiles[j], s))
                        {
                            std::cout << "tile " << i << "." << r << " match_bottom " << j << "." << s << std::endl;
                            tiles[i].dump(std::cout);
                            tiles[j].dump(std::cout);
                            bottom = true;
                        }
                        if (top && bottom && left && right)
                            goto done;
            }
done:
#endif
    }

    long day20a()
    {
        image::tileset_t tiles;
        load_tiles(tiles);

        image solution(tiles);
        return solution.result();
    }

    long day20b()
    {
        image::tileset_t tiles;
        load_tiles(tiles);

        image intermediate(tiles);

        intermediate.dump_full(std::cout);

        image::sea_t sea;
        memset(&sea, '*', sizeof(sea));
        intermediate.make_sea(sea);

        for (size_t y = 0; y < image::SEA_GRID; y++)
        {
            for (size_t x = 0; x < image::SEA_GRID; x++)
            {
                std::cout << sea[x][y];
            }
            std::cout << std::endl;
        }

        return -1;
    }


    std::vector<image> image::s_finals;
    int image::s_depth = 0;
    int image::s_iterations = 0;
}


