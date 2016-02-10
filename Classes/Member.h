#include "cocos2d.h"

class Member: public cocos2d::Sprite
{
public:
	Member(int direction);
	~Member();
	static Member* create(cocos2d::SpriteFrame* frame, int direction, cocos2d::RepeatForever* new_animation);
	int getDirection();
	void setDirection(int direction, cocos2d::RepeatForever* new_animation);
private:
	int direction;
};
