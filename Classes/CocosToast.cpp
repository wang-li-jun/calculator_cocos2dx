#include "CocosToast.h"


SEL_CallFun CocosToast::m_pfnSelectior; 

void CocosToast::createToast(cocos2d::Node *node, const std::string &msg, const float &time,Vec2 point, Color3B color)
{
	auto visibleSize = Director::getInstance()->getVisibleSize();

	auto label = Label::createWithTTF(msg.c_str(), DEFAULT_CHINESE_FONT, visibleSize.width / 19);
    label->setColor(Color3B::WHITE);
    label->ignoreAnchorPointForPosition(false);
    label->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    
    auto layer = LayerColor::create();
	layer->setColor(color);
    layer->ignoreAnchorPointForPosition(false);
    layer->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    layer->setContentSize(label->getContentSize() + Size(60,50));

    node->addChild(layer,999);
    node->addChild(label,999);
    layer->setPosition(point);
    label->setPosition(layer->getPosition());
    auto seq1 = Sequence::create(FadeIn::create(time/5), DelayTime::create(time/5*1.5),FadeOut::create(time/5*2.5),CallFuncN::create(layer,callfuncN_selector(CocosToast::removeToast)),NULL);
    auto seq2 = Sequence::create(EaseSineIn::create(MoveBy::create(time/5, Vec2(0,50))),DelayTime::create(time/5*2),EaseSineOut::create(MoveBy::create(time/3, Vec2(0,-50))), NULL);
    auto spawn = Spawn::create(seq1, seq2, NULL);
    auto action = Repeat::create(spawn,1);

    layer->setOpacity(0);
    label->setOpacity(0);
    layer->runAction(action);
	label->runAction(action->clone());
}

void CocosToast::createToast(cocos2d::Node *node, const std::string &msg, const float &time,Vec2 point, Color3B color,SEL_CallFun method)
{
	auto visibleSize = Director::getInstance()->getVisibleSize();

	m_pfnSelectior = method;
	auto label = Label::createWithTTF(msg.c_str(), DEFAULT_CHINESE_FONT, visibleSize.width / 19);
    label->setColor(Color3B::WHITE);
    label->ignoreAnchorPointForPosition(false);
    label->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    
    auto layer = LayerColor::create();
	layer->setColor(color);
    layer->ignoreAnchorPointForPosition(false);
    layer->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    layer->setContentSize(label->getContentSize() + Size(60,50));

    node->addChild(layer);
    node->addChild(label);
    layer->setPosition(point);
    label->setPosition(layer->getPosition());
    auto seq1 = Sequence::create(FadeIn::create(time/5), DelayTime::create(time/5*1.5),FadeOut::create(time/5*2.5),CallFuncN::create(layer,callfuncN_selector(CocosToast::removeToast2)),NULL);
    auto seq2 = Sequence::create(EaseSineIn::create(MoveBy::create(time/5, Vec2(0,50))),DelayTime::create(time/5*2),EaseSineOut::create(MoveBy::create(time/3, Vec2(0,-50))), NULL);
    auto spawn = Spawn::create(seq1, seq2, NULL);
    auto action = Repeat::create(spawn,1);

	auto seq3= Sequence::create(FadeIn::create(time/5), DelayTime::create(time/5*1.5),FadeOut::create(time/5*2.5),NULL,NULL);
    auto seq4 = Sequence::create(EaseSineIn::create(MoveBy::create(time/5, Vec2(0,50))),DelayTime::create(time/5*2),EaseSineOut::create(MoveBy::create(time/3, Vec2(0,-50))), NULL);
    auto spawn2 = Spawn::create(seq3, seq4, NULL);
    auto action2 = Repeat::create(spawn2,1);

    layer->setOpacity(0);
    label->setOpacity(0);
    layer->runAction(action);
    label->runAction(action2);
}

void CocosToast::removeToast(Node* node)
{
    this->removeFromParentAndCleanup(true);
}
void CocosToast::removeToast2(Node* node)
{
	if(m_pfnSelectior != NULL)
	(this->*m_pfnSelectior)();
    this->removeFromParentAndCleanup(true);
	
	
}


CocosToast::CocosToast(void)
{
}


CocosToast::~CocosToast(void)
{
	m_pfnSelectior = NULL;
}
