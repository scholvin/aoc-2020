#include "day20.h"

#include <fstream>
#include <iostream>
#include <unordered_set>
#include <cassert>
#include <cstring>

namespace day20
{
    /*

            All men live enveloped in whale-lines. All are born with halters round their necks; but it is
            only when caught in the swift, sudden turn of death, that mortals realize the silent, subtle,
            ever-present perils of life. And if you be a philosopher, though seated in the whale-boat, you
            would not at heart feel one whit more of terror, than though seated before your evening fire
            with a poker, and not a harpoon, by your side.

        -- Herman Melville, _Moby-Dick; or, The Whale_

        Day 20a turned out to be a lot of code, something like 400 lines, but it works and is fast enough
        in an optimized build. Proably a lot of room to improve in the backtracker.

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

        Both classes support printing debug output at various granularities to a std::ostream.

        -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

            Towards thee I roll, thou all-destroying but unconquering whale; to the last I grapple with thee;
            from hell’s heart I stab at thee; for hate’s sake I spit my last breath at thee. Sink all coffins
            and all hearses to one common pool! and since neither can be mine, let me then tow to pieces, while
            still chasing thee, though tied to thee, thou damned whale! Thus, I give up the spear!

        -- Ibid.

        Day 20b was more of an exercise in accounting than complicated backtracking or other suchlike. Simply,
        given the solution to 20a, create a map of the sea, and figure out where the sprite of the "sea monster"
        matches the map. The hint here is that the sea has to be rotated and flipped in all the same permutations
        as the individual tiles were. (See comments in early part of tile class below.)

        This is just a lot of ugly, detail-oriented code, hewn to follow the arbitrary and somewhat inscrutable
        instructions. But that's how life is. This is more like a real-world programming assignment than the first
        part.

        Surely it could be written to run faster or more elegantly, and with fewer baked-in assumptions about
        the test data, but ultimately, the idea is to get the star.

        Implementation note: tired of waiting for the backtracker to run each time, I chose to write the solution
        out to a file in part a, and read it in in part b (if it's there).
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
        static const size_t GRID = 12;

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

        // generate a composite of the tiles, but delete the border layer in each
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

        void write_sea(const sea_t& sea, const std::string& filename) const
        {
            std::ofstream outfile(filename);

            for (std::size_t y = 0; y < SEA_GRID; y++)
            {
                for (std::size_t x = 0; x < SEA_GRID; x++)
                {
                    outfile << sea[x][y];
                }
                outfile << std::endl;
            }
            outfile.close();
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

    const std::string SEA_FILE = "../data/day20sea.dat";

    long day20a()
    {
        image::tileset_t tiles;
        load_tiles(tiles);

        image solution(tiles);

        // write the sea map out to a file, to save step 20b from calculating this idiocy every time
        // this takes about 1ms to write
        image::sea_t sea;
        solution.make_sea(sea);
        solution.write_sea(sea, SEA_FILE); // save this for later, takes about 1ms

        return solution.result();
    }

    long day20b()
    {
        // start by getting the map of the sea
        image::sea_t sea;

        // is the file there? use it, else solve
        std::ifstream infile(SEA_FILE);
        if (infile.good())
        {
            std::string line;
            size_t y = 0;
            while (std::getline(infile, line))
            {
                assert(line.size() == image::SEA_GRID); // in case we have conflicting versions
                for (size_t x = 0; x < line.size(); x++)
                {
                    sea[x][y] = line[x];
                }
                y++;
            }
        }
        else
        {
            // solve it to make the sea
            image::tileset_t tiles;
            load_tiles(tiles);
            image solution(tiles);
            solution.make_sea(sea);
            solution.write_sea(sea, SEA_FILE); // save this for later, too
        }

        // we need to permute the sea map like we did the individual tiles in the original solution.
        // there are 8 variants: the original, plus 3 rotations, then take each of those 4 versions
        // and flip each one horizontally.

        std::vector<image::sea_t> seas;
        seas.push_back(sea);

        // rotate them
        for (size_t r = 1; r <= 3; r++)
        {
            image::sea_t next;
            for (size_t i = 0; i < image::SEA_GRID/2; i++)
            {
                for (size_t j = i; j < image::SEA_GRID-i-1; j++)
                {
                    next[i][j]                                     = seas[r-1][image::SEA_GRID-1-j][i];
                    next[image::SEA_GRID-1-j][i]                   = seas[r-1][image::SEA_GRID-1-i][image::SEA_GRID-1-j];
                    next[image::SEA_GRID-1-i][image::SEA_GRID-1-j] = seas[r-1][j][image::SEA_GRID-1-i];
                    next[j][image::SEA_GRID-1-i]                   = seas[r-1][i][j];
                }
            }
            seas.push_back(next);
        }

        // flip them
        for (size_t r = 4; r < 8; r++)
        {
            image::sea_t next;
            for (size_t i = 0; i < image::SEA_GRID; i++)
            {
                for (size_t j = 0; j < image::SEA_GRID; j++)
                {
                    next[i][j] = seas[r-4][image::SEA_GRID-i-1][j];
                }
            }
            seas.push_back(next);
        }

        const std::vector<std::string> SEA_MONSTER =
        {
            "                  # ",
            "#    ##    ##    ###",
            " #  #  #  #  #  #   "
        };

        // cheat alert
        const size_t MON_X = 20;
        const size_t MON_Y = 3;
        typedef std::array<std::array<char, MON_Y>, MON_X> monster_t;

        // convert from string above to an array for more efficient processing
        monster_t monster;
        for (size_t x = 0; x < MON_X; x++)
        {
            for (size_t y = 0; y < MON_Y; y++)
            {
                monster[x][y] = SEA_MONSTER[y][x]; // looks weird, is right
            }
        }

        // we will use this sea map to track the matches
        image::sea_t match_sea;
        memset(&match_sea, ' ', sizeof(match_sea));
        size_t match_id = 99999;

        // find matches - in theory, they only happen on 1 of the 8 permutations we just calculated
        for (size_t s = 0; s < seas.size(); s++)
        {
           for (size_t x = 0; x < image::SEA_GRID - MON_X; x++)
            {
                for (size_t y = 0; y < image::SEA_GRID - MON_Y; y++)
                {
                    bool match = true;
                    for (size_t i = x; i < x + MON_X; i++)
                    {
                        for (size_t j = y; j < y + MON_Y; j++)
                        {
                            if (monster[i-x][j-y] == '#' && seas[s][i][j] != '#')
                            {
                                match = false;
                                goto no_bueno;
                            }
                        }
                    }
no_bueno:
                    if (match)
                    {
                        match_id = s;
                        for (size_t i = x; i < x + MON_X; i++)
                        {
                            for (size_t j = y; j < y + MON_Y; j++)
                            {
                                if (monster[i-x][j-y] == '#')
                                    match_sea[i][j] = '!';
                            }
                        }
                    }
                }
            }
        }

        // at this point, match_sea should contain a ! wherever there was a match
        // count the # in seas[match_id], count the ! in match, and subtract
        long sea_hash = 0;
        long monster_bang = 0;
        for (size_t x = 0; x < image::SEA_GRID; x++)
        {
            for (size_t y = 0; y < image::SEA_GRID; y++)
            {
                if (seas[match_id][x][y] == '#')
                {
                    sea_hash++;
                }
                if (match_sea[x][y] == '!')
                {
                    monster_bang++;
                }
            }
        }

        return sea_hash - monster_bang;
    }


    std::vector<image> image::s_finals;
    int image::s_depth = 0;
    int image::s_iterations = 0;
}


