#include "Actor.h"
#include "StudentWorld.h"

// Students:  Add code to this file (if you wish), Actor.h, StudentWorld.h, and StudentWorld.cpp

// ========================================================
// Actor
// ========================================================
Actor::Actor(int imageID, int startX, int startY, StudentWorld* sw)
	: GraphObject(imageID, startX, startY), m_world(sw), m_alive(true)
{}

bool Actor::isAlive() const { return m_alive; }

void Actor::killActor()
{ 
	setVisible(false);
	m_alive = false; 
}

StudentWorld* Actor::getWorld() const { return m_world; }

// ========================================================
// Player
// ========================================================
Player::Player(int startX, int startY, StudentWorld* sw)
	: Actor(IID_PLAYER, startX, startY, sw), m_nExtraBugSprayers(0), MAX_DEFAULT_SPRAYERS(2),
	m_walkThrough(false)
{ setVisible(true); }

void Player::doSomething()
{
	if(!isAlive())
	{
		getWorld()->playSound(SOUND_PLAYER_DIE);
		return;
	}

	Zumi* zumi = NULL;
	if(getWorld()->sameSquare(this, zumi) && zumi != NULL)
	{
		killActor();
		return;
	}
	
	if(m_nExtraBugSprayers > 0)
		m_nExtraBugSprayers--;

	int ch;
	if (getWorld()->getKey(ch))
	{
		bool destroyable;
		// User hit a key this tick!
		switch (ch)
		{
			case KEY_PRESS_LEFT:
				if(!getWorld()->brickAt(getX()-1, getY(), destroyable)
					|| (destroyable && m_walkThrough))
					moveTo(getX()-1, getY());
			break;
			case KEY_PRESS_RIGHT:
				if(!getWorld()->brickAt(getX()+1, getY(), destroyable)
					|| (destroyable && m_walkThrough))
					moveTo(getX()+1, getY());
			break;
			case KEY_PRESS_UP:
				if(!getWorld()->brickAt(getX(), getY()+1, destroyable)
					|| (destroyable && m_walkThrough))
					moveTo(getX(), getY()+1);
			break;
			case KEY_PRESS_DOWN:
				if(!getWorld()->brickAt(getX(), getY()-1, destroyable)
					|| (destroyable && m_walkThrough))
					moveTo(getX(), getY()-1);
			break;
			case KEY_PRESS_SPACE:
				/*
				if(getActiveActors(BugSprayers) < m_nExtraBugSprayers + MAX_DEFAULT_SPRAYERS
					&& sprayer location not on the same square as a Brick or an existing Bug Sprayer)
					...Add the bug sprayer object...
				*/
			break;
		}
	}
}

// ========================================================
// Brick
// ========================================================
Brick::Brick(int imageID, int startX, int startY, StudentWorld* sw)
	: Actor(imageID, startX, startY, sw)
{}

PermanentBrick::PermanentBrick(int startX, int startY, StudentWorld* sw)
	: Brick(IID_PERMA_BRICK, startX, startY, sw)
{ setVisible(true); }

void PermanentBrick::doSomething(){}

DestroyableBrick::DestroyableBrick(int startX, int startY, StudentWorld* sw)
	: Brick(IID_DESTROYABLE_BRICK, startX, startY, sw)
{ setVisible(true); }

void DestroyableBrick::doSomething(){}

// ========================================================
// Exit
// ========================================================
Exit::Exit(int startX, int startY, StudentWorld* sw)
	: Actor(IID_EXIT, startX, startY, sw)
{ setVisible(false); }

void Exit::doSomething()
{
	Player* player = NULL;
	if(getWorld()->sameSquare(this, player) && player != NULL && isVisible())
		getWorld()->completeLevel();
}

void Exit::reveal()
{
	getWorld()->playSound(SOUND_REVEAL_EXIT);
	setVisible(true);
}

// ========================================================
// Zumi
// ========================================================
Zumi::Zumi(int imageID, int startX, int startY, StudentWorld* sw)
	: Actor(imageID, startX, startY, sw)
{ setVisible(true); }

// ========================================================
// SimpleZumi
// ========================================================
SimpleZumi::SimpleZumi(int startX, int startY, StudentWorld* sw)
	: Zumi(IID_SIMPLE_ZUMI, startX, startY, sw)
{}

void SimpleZumi::doSomething()
{

}

// ========================================================
// ComplexZumi
// ========================================================
ComplexZumi::ComplexZumi(int startX, int startY, StudentWorld* sw)
	: Zumi(IID_COMPLEX_ZUMI, startX, startY, sw)
{}

void ComplexZumi::doSomething()
{

}