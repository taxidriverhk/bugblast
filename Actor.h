#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"

class StudentWorld;

// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp
class Actor : public GraphObject
{
	public:
		// Constructor
		Actor(int imageID, int startX, int startY, StudentWorld* sw);
		// Mutators
		virtual void doSomething() = 0;
		void killActor();
		// Accessors
		StudentWorld* getWorld() const;
		bool isAlive() const;
	private:
		StudentWorld* m_world;
		bool m_alive;
};

class Player : public Actor
{
	public:
		// Constructor
		Player(int startX, int startY, StudentWorld* sw);
		// Mutator
		virtual void doSomething();
	private:
		const int MAX_DEFAULT_SPRAYERS;
		int m_nExtraBugSprayers;
		bool m_walkThrough;
};

class Brick : public Actor
{
	public:
		Brick(int imageID, int startX, int startY, StudentWorld* sw);
		virtual void doSomething() = 0;
	private:
};

class PermanentBrick : public Brick
{
	public:
		// Constructor
		PermanentBrick(int startX, int startY, StudentWorld* sw);
		virtual void doSomething();
	private:
};

class DestroyableBrick : public Brick
{
	public:
		// Constructor
		DestroyableBrick(int startX, int startY, StudentWorld* sw);
		virtual void doSomething();
	private:
};

class Exit : public Actor
{
	public:
		Exit(int startX, int startY, StudentWorld* sw);
		virtual void doSomething();
		void reveal();
	private:
};

class Sprayer : public Actor
{
};

class Spray : public Actor
{
};

class Goodie : public Actor
{
};

class LifeGoodie : public Goodie
{
};

class WalkGoodie : public Goodie
{
};

class SprayGoodie : public Goodie
{
};

class Zumi : public Actor
{
	public:
		Zumi(int imageID, int startX, int startY, StudentWorld* sw);
		virtual void doSomething() = 0;
	private:
};

class SimpleZumi : public Zumi
{
	public:
		SimpleZumi(int startX, int startY, StudentWorld* sw);
		virtual void doSomething();
	private:
};

class ComplexZumi : public Zumi
{
	public:
		ComplexZumi(int startX, int startY, StudentWorld* sw);
		virtual void doSomething();
	private:
};

#endif // ACTOR_H_
