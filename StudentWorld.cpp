#include "StudentWorld.h"

GameWorld* createStudentWorld()
{
	return new StudentWorld();
}

// Students:  Add code to this file (if you wish), StudentWorld.h, Actor.h and Actor.cpp
#include <string>
#include <sstream>
using namespace std;

// ========================================================
// Non-member Functions
// ========================================================
int stringToInt(string s)
{
	int result;
	stringstream ss(s);
	ss >> result;
	return result;
}

string intToString(int i, int digits, bool leadingZeros)
{
	string result;
	stringstream ss;
	ss << i;
	result = ss.str();
	for(int i = result.length(); i < digits; i++)
		result = (leadingZeros ? "0" : " ") + result;
	return result;
}

void setDisplayText(StudentWorld* sw)
{
	int score = sw->getScore();
	int level = sw->getLevel();
	unsigned int bonus = sw->getBonus();
	int livesLeft = sw->getLives();

	// Next, create a string from your statistics, of the form:
	// "Score: 00000100 Level: 09 Lives: 003 Bonus: 345"
	string s = "Score: " + intToString(score, 8, true)
			+ "  Level: " + intToString(level, 2, true)
			+ "  Lives: " + intToString(livesLeft, 3, true)
			+ "  Bonus: " + intToString(bonus, 6, false);

	// Finally, update the display text at the top of the screen with your
	// newly created stats
	sw->setGameStatText(s); // calls our provided GameWorld::setGameStatText
}

// ========================================================
// StudentWorld
// ========================================================
StudentWorld::StudentWorld()
	: m_player(NULL), m_levelData(NULL)
{}

int StudentWorld::init()
{
	// Initialize data structures
	

	// Load the maze details from a level data file
	m_levelData = new Level();
	Level::LoadResult result = m_levelData->loadLevel("level" + 
												   intToString(getLevel(), 2, true) +
												   ".dat");
	if(getLevel() == 0 && result == Level::LoadResult::load_fail_file_not_found)
		return GWSTATUS_NO_FIRST_LEVEL;
	else if(getLevel() != 0 && result == Level::LoadResult::load_fail_file_not_found)
		return GWSTATUS_PLAYER_WON;
	else if(getLevel() != 0 && result == Level::LoadResult::load_fail_bad_format)
		return GWSTATUS_LEVEL_ERROR;

	// Retrieve the level data
	m_completed = false;
	m_currentBonus = m_levelData->getOptionValue(optionLevelBonus);

	// Allocate and insert player object and all other actors
	for(unsigned int i = 0; i < VIEW_HEIGHT; i++)
		for(unsigned int j = 0; j < VIEW_WIDTH; j++)
		{
			Level::MazeEntry entry = m_levelData->getContentsOf(i, j);
			switch(entry)
			{
				case Level::MazeEntry::empty:
				break;
				case Level::MazeEntry::exit:
					m_actors.push_back(new Exit(i, j, this));
				break;
				case Level::MazeEntry::player:
					m_player = new Player(i, j, this);
				break;
				case Level::MazeEntry::perma_brick:
					m_actors.push_back(new PermanentBrick(i, j, this));
				break;
				case Level::MazeEntry::destroyable_brick:
					m_actors.push_back(new DestroyableBrick(i, j, this));
				break;
				case Level::MazeEntry::simple_zumi:
					m_actors.push_back(new SimpleZumi(i, j, this));
				break;
				case Level::MazeEntry::complex_zumi:
					m_actors.push_back(new ComplexZumi(i, j, this));
				break;
			}
		}

	return GWSTATUS_CONTINUE_GAME;
}

int StudentWorld::move()
{
	// Update the Game Status Line
	setDisplayText(this); // update the score/lives/level text at screen top

	// Give each actor a chance to do something
	Exit* exit = NULL;
	int activeZumie = 0;
	m_player->doSomething();
	for(size_t i = 0; i < m_actors.size(); i++)
	{
		if(m_actors[i]->isAlive())
		{
			m_actors[i]->doSomething();

			if(!m_player->isAlive())
				return GWSTATUS_PLAYER_DIED;

			if(m_completed)
			{
				increaseScore(m_currentBonus);
				return GWSTATUS_FINISHED_LEVEL;
			}
		}

		// Keep track of number of exits and zumie
		if(dynamic_cast<Exit*>(m_actors[i]) != NULL)
			exit = dynamic_cast<Exit*>(m_actors[i]);
		else if(dynamic_cast<Zumi*>(m_actors[i]) != NULL)
			activeZumie++;
	}

	for(size_t j = 0; j < m_actors.size(); j++)
		if(!m_actors[j]->isAlive())
		{
			delete m_actors[j];
			m_actors.erase(m_actors.begin()+j);
		}

	// Reduce the current bonus for the Level by one
	if(m_currentBonus > 0)
		m_currentBonus--;

	// If the Player has killed all the Zumi on the level, then we
	// must expose the exit so the Player can advance to the next level
	if(activeZumie <= 0 && !exit->isVisible())
		exit->reveal(); // make the exit Active

	if(!m_player->isAlive())
		return GWSTATUS_PLAYER_DIED;

	if(m_completed)
	{
		increaseScore(m_currentBonus);
		return GWSTATUS_FINISHED_LEVEL;
	}

	return GWSTATUS_CONTINUE_GAME;
}

void StudentWorld::cleanUp()
{
	m_currentBonus = 0;
	delete m_levelData;
	delete m_player;
	for(size_t i = 0; i < m_actors.size(); i++)
		delete m_actors[i];
	m_actors.clear();
}

bool StudentWorld::sameSquare(Actor* actor, Actor* which)
{
	for(size_t i = 0; i < m_actors.size(); i++)
		if(m_actors[i]->getX() == actor->getX()
			&& m_actors[i]->getY() == actor->getY())
		{
			which = m_actors[i];
			return true;
		}
	return false;
}

void StudentWorld::completeLevel(){ m_completed = true; }

bool StudentWorld::brickAt(int x, int y, bool& destroyable) const
{
	if(m_levelData->getContentsOf(x, y) == Level::MazeEntry::perma_brick)
	{
		destroyable = false;
		return true;
	}
	else if(m_levelData->getContentsOf(x, y) == Level::MazeEntry::destroyable_brick)
	{
		destroyable = true;
		return true;
	}
	else
		return false;
}

unsigned int StudentWorld::getBonus() const { return m_currentBonus; }

Level* StudentWorld::getLevelData() const { return m_levelData; }