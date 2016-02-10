#include "Member.h"

Member::Member(int direction) {
	this->direction = direction;
}

Member::~Member() {}

Member* Member::create(cocos2d::SpriteFrame* frame, int direction, cocos2d::RepeatForever* new_animation) {
	Member* member = new Member(direction);
	if (member->initWithSpriteFrame(frame))
	{
		member->autorelease();
		member->runAction(new_animation);
		return member;
	}

	CC_SAFE_DELETE(member);
	return NULL;
}

int Member::getDirection() {
	return this->direction;
}

void Member::setDirection(int direction, cocos2d::RepeatForever* new_animation) {
	this->direction = direction;
	stopAllActions();
	runAction(new_animation);
}