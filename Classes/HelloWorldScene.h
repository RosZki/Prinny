#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"

class HelloWorld : public cocos2d::Layer
{
public:
    static cocos2d::Scene* createScene();
	
    virtual bool init();
	virtual void start();

    CREATE_FUNC(HelloWorld);
    
private:
    cocos2d::Vector<cocos2d::SpriteFrame*> static getAnimation(const char *format, int count);
	void update(float) override;
	void addNewMember();
	void updateTimer(float);
	cocos2d::Vec2 generateRandomPosition(float minX, float maxX, float minY, float maxY);
	bool CollisionWithFood();
	bool CollisionWithPowerUp();
	bool CollisionWithDeath();
	void playRandomPrinnySound();
};

#endif