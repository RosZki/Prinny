#include "HelloWorldScene.h"
#include "Member.h"
#include "SimpleAudioEngine.h"

USING_NS_CC;

Vector<Member *> line;
Sprite* background;
Sprite* food;
Sprite* powerUp;
float timer = 10;
int foodCount = 0;
int totalFoodCount = 0;
bool hasPowerUp = false;
int score = 0;
int level = 1;
int currPlayingSound = 0;
Label *game_over_label, *score_label, *score_value_label, *level_label, *level_value_label;;
Vector<SpriteFrame *> prinnyx_frames_right, prinnyx_frames_left, prinnyx_frames_down, prinnyx_frames_up;
Vector<SpriteFrame *> prinny_frames_right, prinny_frames_left, prinny_frames_down, prinny_frames_up;
Vector<SpriteFrame *> prinny_frames_idle, prinny_frames_spin;
RepeatForever *prinnyx_move_right, *prinnyx_move_left, *prinnyx_move_down, *prinnyx_move_up;
RepeatForever *prinny_idle, *prinny_spin;

Scene* HelloWorld::createScene()
{
    auto scene = Scene::create();
    auto layer = HelloWorld::create();
    scene->addChild(layer);
    return scene;
}

void ppp(float x) {
	char s[256];
	sprintf(s, "%f", x);
	OutputDebugStringA((LPCSTR)s);
	OutputDebugStringA("\n");
}

bool HelloWorld::init()
{
    if ( !Layer::init() )
    {
        return false;
    }
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("prinny.plist");
	
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    Vec2 visibleSize = Director::getInstance()->getVisibleSize();
	prinnyx_frames_right = getAnimation("PRINNYX/WALK_RIGHT/frame%d.png", 4);
	prinnyx_move_right = RepeatForever::create(Animate::create(Animation::createWithSpriteFrames(prinnyx_frames_right, 1.0f / 8)));
	prinnyx_move_right->retain();
	prinnyx_frames_left = getAnimation("PRINNYX/WALK_LEFT/frame%d.png", 4);
	prinnyx_move_left = RepeatForever::create(Animate::create(Animation::createWithSpriteFrames(prinnyx_frames_left, 1.0f / 8)));
	prinnyx_move_left->retain();
	prinnyx_frames_down = getAnimation("PRINNYX/WALK_DOWN/frame%d.png", 4);
	prinnyx_move_down = RepeatForever::create(Animate::create(Animation::createWithSpriteFrames(prinnyx_frames_down, 1.0f / 8)));
	prinnyx_move_down->retain();
	prinnyx_frames_up = getAnimation("PRINNYX/WALK_UP/frame%d.png", 4);
	prinnyx_move_up = RepeatForever::create(Animate::create(Animation::createWithSpriteFrames(prinnyx_frames_up, 1.0f / 8)));
	prinnyx_move_up->retain();

	prinny_frames_right = getAnimation("PRINNY/WALK_RIGHT/frame%d.png", 4);
	prinny_frames_left = getAnimation("PRINNY/WALK_LEFT/frame%d.png", 4);
	prinny_frames_down = getAnimation("PRINNY/WALK_DOWN/frame%d.png", 4);
	prinny_frames_up = getAnimation("PRINNY/WALK_UP/frame%d.png", 4);

	prinny_frames_idle = getAnimation("PRINNY/IDLE/frame%d.png", 2);
	prinny_idle = RepeatForever::create(Animate::create(Animation::createWithSpriteFrames(prinny_frames_idle, 1.0f / 4)));
	prinny_idle->retain();

	prinny_frames_spin = getAnimation("PRINNY/SPINSPIN/frame%d.png", 8);
	prinny_spin = RepeatForever::create(Animate::create(Animation::createWithSpriteFrames(prinny_frames_spin, 1.0f / 16)));
	prinny_spin->retain();

	//preload effects
	for (int i = 1; i <= 135; i++) {
		char s[256];
		sprintf(s, "sounds/prinny (%d).wav", i);
		CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect(s);
	}
	CocosDenshion::SimpleAudioEngine::getInstance()->preloadBackgroundMusic("sounds/Start Menu.wav");

    // background
    background = Sprite::createWithSpriteFrameName("background.png");
    background->setPosition(origin.x + visibleSize.x / 2,origin.y + visibleSize.y/2);
    this->addChild(background);
    
	Member* sprite = Member::create(prinnyx_frames_right.front(), 1, prinnyx_move_right);
	line.pushBack(sprite);
    background->addChild(sprite);
    sprite->setPosition(origin.x + visibleSize.x / 2, origin.y + visibleSize.y / 2);

	food = Sprite::createWithSpriteFrame(prinny_frames_spin.front());
	background->addChild(food);
	float sizeX = prinny_frames_spin.front()->getOriginalSizeInPixels().width;
	float sizeY = prinny_frames_spin.front()->getOriginalSizeInPixels().height;
	float bgSizeX = background->getContentSize().width;
	float bgSizeY = background->getContentSize().height;
	food->setPosition(generateRandomPosition(sizeX,bgSizeX - sizeX, sizeY, bgSizeY - sizeY));
	food->runAction(prinny_spin);
	
	this->scheduleUpdate();

	auto eventListener = EventListenerKeyboard::create();
	eventListener->onKeyPressed = [&](EventKeyboard::KeyCode keyCode, Event* event) {
		switch (keyCode) {
		case EventKeyboard::KeyCode::KEY_LEFT_ARROW:
		case EventKeyboard::KeyCode::KEY_A:
			((Member *)event->getCurrentTarget())->setSpriteFrame(prinnyx_frames_left.front());
			((Member *)event->getCurrentTarget())->setDirection(0, prinnyx_move_left);
			break;
		case EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
		case EventKeyboard::KeyCode::KEY_D:
			((Member *)event->getCurrentTarget())->setSpriteFrame(prinnyx_frames_right.front());
			((Member *)event->getCurrentTarget())->setDirection(1, prinnyx_move_right);
			break;
		case EventKeyboard::KeyCode::KEY_UP_ARROW:
		case EventKeyboard::KeyCode::KEY_W:
			((Member *)event->getCurrentTarget())->setSpriteFrame(prinnyx_frames_up.front());
			((Member *)event->getCurrentTarget())->setDirection(2, prinnyx_move_up);
			break;
		case EventKeyboard::KeyCode::KEY_DOWN_ARROW:
		case EventKeyboard::KeyCode::KEY_S:
			((Member *)event->getCurrentTarget())->setSpriteFrame(prinnyx_frames_down.front());
			((Member *)event->getCurrentTarget())->setDirection(3, prinnyx_move_down);
			break;
		case EventKeyboard::KeyCode::KEY_SPACE: game_over_label->setVisible(false);
			this->start();
			break;
		}
	};

	this->_eventDispatcher->addEventListenerWithSceneGraphPriority(eventListener, sprite);

	game_over_label = cocos2d::Label::createWithSystemFont("Game Over", "Calibri", 72);
	game_over_label->setPosition(origin.x + visibleSize.x / 2, origin.y + visibleSize.y / 2);
	background->addChild(game_over_label, 5);
	game_over_label->setVisible(false);

	score_label = cocos2d::Label::createWithSystemFont("Score:", "Calibri", 20);
	score_label->setAnchorPoint(Vec2(0, 0));
	score_label->setPosition(10,10);
	background->addChild(score_label, 5);

	score_value_label = cocos2d::Label::createWithSystemFont("0", "Calibri", 20);
	score_value_label->setAnchorPoint(Vec2(0, 0));
	score_value_label->setPosition(10 + score_label->getBoundingBox().getMaxX(), 10);
	background->addChild(score_value_label, 5);

	level_label = cocos2d::Label::createWithSystemFont("Level ", "Calibri", 20);
	level_label->setAnchorPoint(Vec2(0, 0));
	level_label->setPosition(10, 10 + score_label->getBoundingBox().getMaxY());
	background->addChild(level_label, 5);

	level_value_label = cocos2d::Label::createWithSystemFont("1", "Calibri", 20);
	level_value_label->setAnchorPoint(Vec2(0, 0));
	level_value_label->setPosition(10 + level_label->getBoundingBox().getMaxX(), 10 + score_label->getBoundingBox().getMaxY());
	background->addChild(level_value_label, 5);


	CocosDenshion::SimpleAudioEngine::getInstance()->setBackgroundMusicVolume(0.5);
	CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic("sounds/Start Menu.wav", true);

    return true;
}

void HelloWorld::updateTimer(float dt) {
	timer--;
}

void HelloWorld::start() {
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
	Vec2 visibleSize = Director::getInstance()->getVisibleSize();
	for (int i = line.size() - 1; i >= 1; i--) {
		background->removeChild(line.at(i),true);
		line.eraseObject(line.at(i));
	}
	background->removeChild(powerUp, true);
	hasPowerUp = false;
	foodCount = 0;
	totalFoodCount = 0;
	timer = 10;
	line.front()->setPosition(origin.x + visibleSize.x / 2, origin.y + visibleSize.y / 2);
	line.front()->setDirection(1, prinnyx_move_right);
	float sizeX = prinny_frames_idle.front()->getOriginalSizeInPixels().width;
	float sizeY = prinny_frames_idle.front()->getOriginalSizeInPixels().height;
	float bgSizeX = background->getContentSize().width;
	float bgSizeY = background->getContentSize().height;
	food->setPosition(generateRandomPosition(sizeX, bgSizeX - sizeX, sizeY, bgSizeY - sizeY));
	score = 0;
	score_value_label->setString(std::to_string(score));
	level = 1;
	level_value_label->setString(std::to_string(level));
	cocos2d::Director::getInstance()->setAnimationInterval(1.0 / 10);
	cocos2d::Director::getInstance()->resume();
}

Vector<SpriteFrame*> HelloWorld::getAnimation(const char *format, int count)
{
    auto spritecache = SpriteFrameCache::getInstance();
    Vector<SpriteFrame*> animFrames;
    char str[100];
    for(int i = 0; i < count; i++)
    {
        sprintf(str, format, i);
        animFrames.pushBack(spritecache->getSpriteFrameByName(str));
    }
    return animFrames;
}

void HelloWorld::update(float delta) {
	if (foodCount >=5) {
		hasPowerUp = true;
		foodCount = 0;
		timer = 10;
		this->schedule(schedule_selector(HelloWorld::updateTimer), 1.0f);
		float sizeX = prinny_frames_idle.front()->getOriginalSizeInPixels().width;
		float sizeY = prinny_frames_idle.front()->getOriginalSizeInPixels().height;
		float bgSizeX = background->getContentSize().width;
		float bgSizeY = background->getContentSize().height;
		powerUp = Sprite::createWithSpriteFrame(prinny_frames_idle.front());
		powerUp->runAction(prinny_idle);
		background->addChild(powerUp);
		powerUp->setPosition(generateRandomPosition(sizeX, bgSizeX - sizeX, sizeY, bgSizeY - sizeY));
	}

	if (timer <= 0) {
		hasPowerUp = false;
		background->removeChild(powerUp, true);
		this->unschedule(schedule_selector(HelloWorld::updateTimer));
	}

	auto front = line.front();
	auto position = front->getPosition();

	switch (front->getDirection()) {
	case 0: position.x -= front->getSpriteFrame()->getOriginalSizeInPixels().width + 1;
		break;
	case 1: position.x += front->getSpriteFrame()->getOriginalSizeInPixels().width + 1;
		break;
	case 2: position.y += front->getSpriteFrame()->getOriginalSizeInPixels().height + 1;
		break;
	case 3: position.y -= front->getSpriteFrame()->getOriginalSizeInPixels().height + 1;
		break;
	default:
		break;
	}
	if (CollisionWithDeath()) {
		game_over_label->setVisible(true);
		cocos2d::Director::getInstance()->pause();
		return;
	}
	if (CollisionWithPowerUp()) {
		hasPowerUp = false;
		background->removeChild(powerUp, true);
		this->unschedule(schedule_selector(HelloWorld::updateTimer));
		score += 500;
		score_value_label->setString(std::to_string(score));

		cocos2d::Director::getInstance()->getScheduler()->performFunctionInCocosThread(CC_CALLBACK_0(HelloWorld::playRandomPrinnySound, this));
	}
	if (CollisionWithFood()) {
		float sizeX = prinny_frames_idle.front()->getOriginalSizeInPixels().width;
		float sizeY = prinny_frames_idle.front()->getOriginalSizeInPixels().height;
		float bgSizeX = background->getContentSize().width;
		float bgSizeY = background->getContentSize().height;
		food->setPosition(generateRandomPosition(sizeX, bgSizeX - sizeX, sizeY, bgSizeY - sizeY));
		if (!hasPowerUp) {
			foodCount++;
		}
		cocos2d::Director::getInstance()->getScheduler()->performFunctionInCocosThread(CC_CALLBACK_0(HelloWorld::playRandomPrinnySound, this));
		totalFoodCount++;
		score += 50;
		score_value_label->setString(std::to_string(score));
		addNewMember();
		if (totalFoodCount % 3 == 0) {
			auto interval = 10 + totalFoodCount/3;
			cocos2d::Director::getInstance()->setAnimationInterval(1.0 / interval);
			level++;
			level_value_label->setString(std::to_string(level));
		}
	}
	else{
		for (int i = line.size() - 1; i >= 1; i--) {
			auto prev = line.at(i - 1);
			auto curr = line.at(i);

			if (prev->getDirection() != curr->getDirection()) {
				auto new_dir = prev->getDirection();
				switch (new_dir) {
				case 0: curr->setDirection(new_dir, RepeatForever::create(Animate::create(Animation::createWithSpriteFrames(prinny_frames_left, 1.0f / 8))));
					break;
				case 1: curr->setDirection(new_dir, RepeatForever::create(Animate::create(Animation::createWithSpriteFrames(prinny_frames_right, 1.0f / 8))));
					break;
				case 2: curr->setDirection(new_dir, RepeatForever::create(Animate::create(Animation::createWithSpriteFrames(prinny_frames_up, 1.0f / 8))));
					break;
				case 3: curr->setDirection(new_dir, RepeatForever::create(Animate::create(Animation::createWithSpriteFrames(prinny_frames_down, 1.0f / 8))));
					break;
				default:
					break;
				}
			}
			curr->setPosition(prev->getPosition());
		}
		
		front->setPosition(position);
	}
}

void HelloWorld::addNewMember() {
	auto new_pos = line.back()->getPosition();
	new_pos.x -= line.back()->getSpriteFrame()->getOriginalSizeInPixels().width;
	auto new_dir = line.back()->getDirection();
	auto new_anim = RepeatForever::create(Animate::create(Animation::createWithSpriteFrames(prinny_frames_left, 1.0f / 4)));
	auto new_frame = prinny_frames_left.front();
	switch (new_dir) {
	case 0:
		break;
	case 1: new_anim = RepeatForever::create(Animate::create(Animation::createWithSpriteFrames(prinny_frames_right, 1.0f / 4)));
		new_frame = prinny_frames_right.front();
		break;
	case 2: new_anim = RepeatForever::create(Animate::create(Animation::createWithSpriteFrames(prinny_frames_up, 1.0f / 4)));
		new_frame = prinny_frames_up.front();
		break;
	case 3: new_anim = RepeatForever::create(Animate::create(Animation::createWithSpriteFrames(prinny_frames_down, 1.0f / 4)));
		new_frame = prinny_frames_up.front();
		break;
	}
	Member* new_sprite = Member::create(new_frame, new_dir, new_anim);
	line.pushBack(new_sprite);
	background->addChild(new_sprite);
	new_sprite->setPosition(new_pos);

}

cocos2d::Vec2 HelloWorld::generateRandomPosition(float minX, float maxX, float minY, float maxY){
	float newX = random(minX, maxX);
	float newY = random(minY, maxY);
	return Vec2(newX, newY);
}

bool HelloWorld::CollisionWithFood() {
	auto foodRect = food->getBoundingBox();
	auto headRect = line.front()->getBoundingBox();

	return foodRect.intersectsRect(headRect);

}

bool HelloWorld::CollisionWithPowerUp() {
	if (hasPowerUp) {
		auto powerUpRect = powerUp->getBoundingBox();
		auto headRect = line.front()->getBoundingBox();

		return powerUpRect.intersectsRect(headRect);
	}
	return false;
}

bool HelloWorld::CollisionWithDeath() {
	auto headRect = line.front()->getBoundingBox();
	auto headPos = line.front()->getPosition();
	if (headPos.x < 0 || headPos.y < 0 || headPos.x > background->getBoundingBox().getMaxX() || headPos.y > background->getBoundingBox().getMaxY()) {
		return true;
	}
	for (int i = 1; i < line.size() - 1; i++) {
		auto currRect = line.at(i)->getBoundingBox();
		if (headRect.intersectsRect(currRect)) {
			return true;
		}
	}
	return false;
}

void HelloWorld::playRandomPrinnySound() {
	
	int x = random(1, 135);
	char s[256];
	sprintf(s, "sounds/prinny (%d).wav", x);
	CocosDenshion::SimpleAudioEngine::getInstance()->stopEffect(currPlayingSound);
	currPlayingSound = CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(s);

}