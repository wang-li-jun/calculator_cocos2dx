#include "CalcHistoryScene.h"

Scene* CalcHistoryScene::createScene() {
	auto scene = Scene::create();
	auto layer = CalcHistoryScene::create();
	scene->addChild(layer);
	return scene;
}

bool CalcHistoryScene::init() {
	if (!LayerColor::initWithColor(Color4B(255, 255, 255, 255))) {
		return false;
	}
	m_visibleSize = Director::getInstance()->getVisibleSize();
	m_visibleOrigin = Director::getInstance()->getVisibleOrigin();
	
	addBackButton();
	
	return true;
}

void CalcHistoryScene::addBackButton()
{
	auto backButtonwSprite = Scale9Sprite::create(BACK_BUTTON_ICO);
	m_backButton = ControlButton::create(backButtonwSprite);
	m_backButton->addTargetWithActionForControlEvents(this, cccontrol_selector(CalcHistoryScene::onBackButtonCallback), Control::EventType::TOUCH_DOWN);
	m_backButton->setPosition(Vec2(m_visibleOrigin.x + 25,
		m_visibleOrigin.y + m_visibleSize.height - m_backButton->getContentSize().height));
	addChild(m_backButton, 2);
}

void CalcHistoryScene::onBackButtonCallback(Ref* pSender, Control::EventType event)
{
	Director::getInstance()->popSceneWithTransition<TransitionFadeTR>(0.3);
	//Director::getInstance()->popScene();
}