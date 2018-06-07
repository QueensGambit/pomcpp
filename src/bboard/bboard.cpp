#include <iostream>
#include <random>

#include "bboard.hpp"

namespace bboard
{


State* InitState(int a0, int a1, int a2, int a3)
{
    State* result = new State();
    int b = Item::AGENT0; // agent no. offset

    // Randomly put obstacles
    std::mt19937_64 rng(0x1337);
    std::uniform_int_distribution<int> intDist(0,6);
    for(int i = 0; i < BOARD_SIZE; i++)
    {
        for(int  j = 0; j < BOARD_SIZE; j++)
        {
            result->board[i][j] = intDist(rng) < 5 ? 0 : 2;
        }
    }

    // Put agents
    result->board[0][0] = b + a0;
    result->board[0][BOARD_SIZE - 1] = b + a1;
    result->board[BOARD_SIZE - 1][BOARD_SIZE - 1] = b + a2;
    result->board[BOARD_SIZE - 1][0] = b + a3;

    // note: the rest of the vals can remain 0
    result->agentX[a1] = result->agentX[a2] = BOARD_SIZE - 1;
    result->agentY[a2] = result->agentY[a3] = BOARD_SIZE - 1;

    return result;
}

void Step(State* state, Move* moves)
{
    //TODO: calculate step transition
    for(int i = 0; i < AGENT_COUNT; i++)
    {
        Move m = moves[i];
        int x = state->agentX[i];
        int y = state->agentY[i];

        if(m == Move::RIGHT)
        {
            if(x == BOARD_SIZE - 1 || state->board[y][x + 1] != 0)
            {
                continue;
            }

            state->board[y][x] = 0;
            state->board[y][x + 1] = Item::AGENT0 + i;
            state->agentX[i]++;
        }
        else if(m == Move::LEFT)
        {
            if(x == 0  || state->board[y][x - 1] != 0)
            {
                continue;
            }
            state->board[y][x] = 0;
            state->board[y][x - 1] = Item::AGENT0 + i;
            state->agentX[i]--;
        }
        else if(m == Move::UP)
        {
            if(y == 0 || state->board[y - 1][x] != 0)
            {
                continue;
            }
            state->board[y][x] = 0;
            state->board[y - 1][x] = Item::AGENT0 + i;
            state->agentY[i]--;
        }
        else if(m == Move::DOWN)
        {
            if(y == BOARD_SIZE - 1 || state->board[y + 1][x] != 0)
            {
                continue;
            }
            state->board[y][x] = 0;
            state->board[y + 1][x] = Item::AGENT0 + i;
            state->agentY[i]++;
        }
    }
}

void PrintState(State* state)
{
    std::string result = "";

    for(int i = 0; i < BOARD_SIZE; i++)
    {
        for(int j = 0; j < BOARD_SIZE; j++)
        {
            int item = state->board[i][j];
            result += PrintItem(item);
            if(j == BOARD_SIZE - 1)
            {
                result += "\n";
            }
        }
    }
    std::cout << result;
}

std::string PrintItem(int item)
{
    switch(item)
    {
        case Item::PASSAGE:
            return "   ";
        case Item::RIGID:
            return "[X]";
        case Item::WOOD:
            return "[\u25A0]";
        case Item::BOMB:
            return " \u2B24 ";
        case Item::FLAMES:
            return " \u2B24 ";
    }
    //agent number
    if(item >= Item::AGENT0)
    {
        return " "  +  std::to_string(item - 10) + " ";
    }
    else
    {
        return std::to_string(item);
    }
}

}
