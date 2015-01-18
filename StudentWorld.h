#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "GameConstants.h"

// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp
#include <vector>
#include "Actor.h"
#include "Level.h"

class StudentWorld : public GameWorld
{
	public:
		// Constructor
		StudentWorld();
		// Mutators
		virtual int init();
		virtual int move();
		virtual void cleanUp();
		bool sameSquare(Actor* actor, Actor* which);
		void completeLevel();
		// Accessors
		bool brickAt(int x, int y, bool& destroyable) const;
		unsigned int getBonus() const;
		int getActiveActors(int type) const;
		Level* getLevelData() const;
	private:
		// Helper functions
		

		// Data members
		std::vector<Actor*> m_actors;
		Player* m_player;
		Level* m_levelData;
		int m_currentBonus;
		bool m_completed;
};

#endif // STUDENTWORLD_H_
