#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>

using namespace std;

class Maze 
{
public:
    Maze(int size);
    void printMaze();
    void movePlayer(char direction);
    bool isGameFinished();

private:
    vector<vector<char>> maze;
    int playerX, playerY;
    int exitX, exitY;
    int mapX, mapY;
    int mechanismX, mechanismY;
    int trap1X, trap1Y;
    int trap2X, trap2Y;
    int health;
    int size;
    bool exitVisible;
    bool mapOn;

    void placeExitAndTraps();
    void placeMapAndMechanism();
    bool isValidMove(int x, int y);
};

Maze::Maze(int size) : size(size), exitVisible(false), mapOn(false), health(1) 
{
    maze = vector<vector<char>>(size, vector<char>(size, ' '));
    srand(static_cast<unsigned int>(time(nullptr)));

    // set player
    playerX = playerY = 0;
    maze[playerX][playerY] = 'P';

    // set e and t
    placeExitAndTraps();

    // set map and g
    placeMapAndMechanism();
}

bool Maze::isValidMove(int x, int y) 
{
    return x >= 0 && x < size&& y >= 0 && y < size;
}

void Maze::placeExitAndTraps() 
{
    // set e
    exitX = exitY = size - 1;
    maze[exitX][exitY] = 'E';

    // set trap1
    trap1X = exitX - 1;
    trap1Y = exitY;
    maze[trap1X][trap1Y] = 'T';

    // set trap2
    trap2X = exitX;
    trap2Y = exitY - 1;
    maze[trap2X][trap2Y] = 'T';

    // set random #, and route to e
    for (int i = 0; i < size; ++i) 
    {
        for (int j = 0; j < size; ++j) 
        {
            if (maze[i][j] != 'E' && maze[i][j] != 'T') 
            {
                if (rand() % 5 == 0) 
                {
                    maze[i][j] = '#';
                }
            }
        }
    }
}

void Maze::placeMapAndMechanism() 
{
    // set map
    mapX = rand() % size;
    mapY = rand() % size;
    while (maze[mapX][mapY] == '#' || maze[mapX][mapY] == 'E' || maze[mapX][mapY] == 'T') 
    {
        mapX = rand() % size;
        mapY = rand() % size;
    }
    maze[mapX][mapY] = 'M';

}

void Maze::printMaze() 
{
    system("cls");
    for (int i = 0; i < size; ++i) 
    {
        for (int j = 0; j < size; ++j) 
        {
            cout << maze[i][j] << " ";
        }
        cout << endl;
    }

    cout << "Health: " << health << endl;
}

void Maze::movePlayer(char direction) 
{
    if (health <= 0) 
    {
        cout << "Game Over! Your health is depleted." << endl;
        return;
    }

    int newX = playerX, newY = playerY;

    if (direction == 'W' && playerX > 0) 
    {
        --newX;
    } 
    else if (direction == 'S' && playerX < size - 1) 
    {
        ++newX;
    } 
    else if (direction == 'A' && playerY > 0) 
    {
        --newY;
    } 
    else if (direction == 'D' && playerY < size - 1) 
    {
        ++newY;
    }

    if (isValidMove(newX, newY) && maze[newX][newY] != '#') 
    {
        if (maze[newX][newY] == 'E') 
        {
            exitVisible = true;
            cout << "Congratulations! You found the exit." << endl;
        } 
        else if (maze[newX][newY] == 'T') 
        {
            --health;
            if (health <= 0) 
            {
                cout << "Game Over! You stepped on a trap." << endl;
            }
        } 
        else if (maze[newX][newY] == 'M') 
        {
            mapOn = true;

            // get map, g appear
            if (!exitVisible) 
            {
                mechanismX = rand() % size;
                mechanismY = rand() % size;
                while (maze[mechanismX][mechanismY] == '#' || maze[mechanismX][mechanismY] == 'E' ||
                       maze[mechanismX][mechanismY] == 'T' || maze[mechanismX][mechanismY] == 'M') 
                {
                    mechanismX = rand() % size;
                    mechanismY = rand() % size;
                }
                maze[mechanismX][mechanismY] = 'G';
            }
        } else if (mapOn && maze[newX][newY] == 'G') 
        {
            // get g, destory t
            maze[trap1X][trap1Y] = ' ';
            maze[trap2X][trap2Y] = ' ';
        }

        maze[playerX][playerY] = ' ';
        playerX = newX;
        playerY = newY;
        maze[playerX][playerY] = 'P';
    }
}


bool Maze::isGameFinished() 
{
    return exitVisible || health <= 0;
}

int main() 
{
    int mazeSize;
    cout << "Enter maze size: ";
    cin >> mazeSize;

    Maze mazeGame(mazeSize);

    while (!mazeGame.isGameFinished()) 
    {
        mazeGame.printMaze();

        char move;
        cout << "Enter movement (W/A/S/D): ";
        cin >> move;

        mazeGame.movePlayer(move);
    }

    return 0;
}