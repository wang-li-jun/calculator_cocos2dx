#include "CocosDialog.h"
#include "ConstUtil.h"

CocosDialog::CocosDialog()
	:
	m__pMenu(NULL)
	, m_callbackListener(NULL)
	, m_callback(NULL)
	, m__s9BackGround(NULL)
	, m__s9BackGroundShadow(NULL)
	, m__ltContentText(NULL)
	, m__ltTitle(NULL)
{
}

CocosDialog::~CocosDialog() {
	CC_SAFE_RELEASE(m__pMenu);
	CC_SAFE_RELEASE(m__s9BackGround);
	CC_SAFE_RELEASE(m__s9BackGroundShadow);
	CC_SAFE_RELEASE(m__ltContentText);
	CC_SAFE_RELEASE(m__ltTitle);
}

bool CocosDialog::init() {
	if (!LayerColor::init()) {
		return false;
	}
	m_visibleSize = Director::getInstance()->getVisibleSize();
	m_visibleOrigin = Director::getInstance()->getVisibleOrigin();

	Menu* menu = Menu::create();
	menu->setPosition(Point::ZERO);
	setMenuButton(menu);
	auto listener = EventListenerTouchOneByOne::create();
	listener->setSwallowTouches(true);
	listener->onTouchBegan = CC_CALLBACK_2(CocosDialog::onTouchBegan, this);
	listener->onTouchMoved = CC_CALLBACK_2(CocosDialog::onTouchMoved, this);
	listener->onTouchEnded = CC_CALLBACK_2(CocosDialog::onTouchEnded, this);
	auto dispatcher = Director::getInstance()->getEventDispatcher();
	dispatcher->addEventListenerWithSceneGraphPriority(listener, this);
	//setColor(Color3B::GRAY);
	//setOpacity(0);
	return true;
}

bool CocosDialog::onTouchShieldLayerBegan(Touch* touch, Event* event) {
	return true;
}

bool CocosDialog::onTouchBegan(Touch* touch, Event* event) {
	return true;
}

void CocosDialog::onTouchMoved(Touch* touch, Event* event) {

}

void CocosDialog::onTouchEnded(Touch* touch, Event* event) {

}

CocosDialog* CocosDialog::create(const char* backgoundImage) {
	//创建弹出对话框，指定背景图和大小。
	CocosDialog* layer = CocosDialog::create();
	layer->setSprite9BackGround(ui::Scale9Sprite::create(backgoundImage));
	layer->setSprite9BackGroundShadow(ui::Scale9Sprite::create(DIALOG_BG_SHADOW));
	return layer;
}

void CocosDialog::setTitle(const char* title) {
	Label* label = Label::createWithTTF(title, DEFAULT_CHINESE_FONT, m_visibleSize.width / 15);
	label->setColor(Color3B::BLACK);
	setLabelTitle(label);
}



void CocosDialog::setContentText(const char* text) {
	Label* ltf = Label::createWithTTF(text, DEFAULT_CHINESE_FONT, m_visibleSize.width / 19, Size(m_visibleSize.width * 0.8, 0));
	ltf->setColor(Color3B::BLACK);
	//set dialog size according to the content text
	m_textHeight = ltf->getLineHeight() * ltf->getStringNumLines();
	float m_textExtraHeight = ltf->getLineHeight() * (ltf->getStringNumLines() - 1);
	m_dialogContentSize = Size(m_visibleSize.width * 0.8, m_visibleSize.width * 0.4 + m_textExtraHeight);

	setLabelContentText(ltf);
}

void CocosDialog::setCallBackFunc(Ref*target, SEL_CallFuncN callfun) {
	m_callbackListener = target;
	m_callback = callfun;
}



bool CocosDialog::addButton(const char *normalImage, const char *selectedImage, const char* title, int tag) {
	Point pCenter = Point(m_visibleOrigin.x + m_visibleSize.width / 2, m_visibleOrigin.y + m_visibleSize.height / 2);
	auto menuImage = MenuItemImage::create(
		normalImage,
		selectedImage,
		CC_CALLBACK_1(CocosDialog::buttonCallBack, this));
	menuImage->setTag(tag);
	menuImage->setPosition(pCenter);
	//menuImage->setContentSize(Size(100, 30));
	menuImage->setScale(m_visibleSize.width / 2400,m_visibleSize.width / 2000);
	Size menuSize = menuImage->getContentSize();
	Label* labelttf = Label::createWithTTF(title, DEFAULT_CHINESE_FONT, m_visibleSize.width / 19);
	labelttf->setColor(Color3B(Color3B::BLACK));
	labelttf->setPosition(Point(menuSize.width / 2, menuSize.height / 2));
	labelttf->setScale(2400 / m_visibleSize.width, 2000 / m_visibleSize.width);
	menuImage->addChild(labelttf);
	
	getMenuButton()->addChild(menuImage);
	return true;
}



void CocosDialog::buttonCallBack(Ref* pSender) {
	Node* node = dynamic_cast<Node*>(pSender);
	//log("[========Dialog:buttonCallBack=======]touch tag:%d",node->getTag());
	if (m_callback&&m_callbackListener) {
		(m_callbackListener->*m_callback)(node);
	}
	this->removeFromParentAndCleanup(true);
}



void CocosDialog::onEnter() {

	//log("Dialog::onEnter is loading");
	LayerColor::onEnter();
	Point pCenter = Point(m_visibleOrigin.x + m_visibleSize.width / 2, m_visibleOrigin.y + m_visibleSize.height / 2);
	
	//add a shield layer
	LayerColor *shieldLayer = LayerColor::create(cocos2d::Color4B(0, 0, 0, 50), m_visibleSize.width, m_visibleSize.height);
	shieldLayer->setPosition(Vec2::ZERO);
	auto touchListener = cocos2d::EventListenerTouchOneByOne::create();
	touchListener->setSwallowTouches(true);
	touchListener->onTouchBegan = CC_CALLBACK_2(CocosDialog::onTouchShieldLayerBegan, this);
	shieldLayer->getEventDispatcher()->addEventListenerWithSceneGraphPriority(touchListener, shieldLayer);
	this->addChild(shieldLayer, 0);

	//add background
	ui::Scale9Sprite* background = getSprite9BackGround();
	background->setContentSize(m_dialogContentSize);
	background->setPosition(pCenter);
	this->addChild(background, 2);
	Action* popupActions = Sequence::create(//FadeOut::create(0),
		//caleTo::create(0, 0),
		//FadeIn::create(0.1),
		//ScaleTo::create(0.15, 1.0),
		CallFunc::create(CC_CALLBACK_0(CocosDialog::backgroundFinish, this))
		, NULL);
	background->runAction(popupActions);
	
	//add background shadow
	ui::Scale9Sprite* backgroundShadow = getSprite9BackGroundShadow();
	backgroundShadow->setContentSize(m_dialogContentSize + Size(20, 20));
	backgroundShadow->setPosition(pCenter + Point(10,-10));
	this->addChild(backgroundShadow, 1);
}

void CocosDialog::backgroundFinish() {
	Point pCenter = Point(m_visibleOrigin.x + m_visibleSize.width / 2, m_visibleOrigin.y + m_visibleSize.height / 2);
	
	//add content
	if (getLabelContentText()) {
		Label* ltf = getLabelContentText();
		ltf->setPosition(pCenter);
		//m_labelHeight[i] = ltf->getLineHeight() * (ltf->getStringNumLines() - 1);
		ltf->setDimensions(m_dialogContentSize.width * 0.9, m_textHeight);
		ltf->setHorizontalAlignment(TextHAlignment::CENTER);
		this->addChild(ltf, 3);
	}
	//add title
	if (getLabelTitle()) {
		getLabelTitle()->setPosition(Point(pCenter.x, (pCenter.y + m_textHeight / 2 + pCenter.y + m_dialogContentSize.height / 2) / 2));
		this->addChild(getLabelTitle(), 3);
	}
	//add button
	this->addChild(getMenuButton(), 3);
	float btnWidth = m_dialogContentSize.width / (getMenuButton()->getChildrenCount() + 1);
	Vector<Node*> vector = getMenuButton()->getChildren();
	Ref* pObj = NULL;
	int i = 0;
	for (Node*pObj : vector) {
		Node* node = dynamic_cast<Node*>(pObj);
		node->setPosition(Point(pCenter.x - m_dialogContentSize.width / 2 + btnWidth*(i + 1), (pCenter.y - m_textHeight / 2 + pCenter.y - m_dialogContentSize.height / 2) / 2));
		i++;
	}
}



void CocosDialog::onExit() {
	//log("Dialog onExit");
	LayerColor::onExit();
}