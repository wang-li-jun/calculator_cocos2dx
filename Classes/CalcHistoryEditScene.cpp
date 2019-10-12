#include "CalcHistoryEditScene.h"

Scene* CalcHistoryEditScene::createScene() {
	auto scene = Scene::create();
	auto layer = CalcHistoryEditScene::create();
	scene->addChild(layer);
	return scene;
}

bool CalcHistoryEditScene::init() {
	if (!LayerColor::initWithColor(Color4B(255, 255, 255, 255))) {
		return false;
	}
	m_visibleSize = Director::getInstance()->getVisibleSize();
	m_visibleOrigin = Director::getInstance()->getVisibleOrigin();

	addBackButton();

	Sprite* seperateLine = Sprite::create(GREY_DOT_ICO);
	seperateLine->setScaleX(m_visibleSize.width);
	seperateLine->setAnchorPoint(Point::ZERO);
	seperateLine->setPosition(m_visibleOrigin.x, m_visibleOrigin.y + m_visibleSize.height - m_backButton->getContentSize().height - m_visibleSize.width / 15);
	this->addChild(seperateLine);

	return true;
}

void CalcHistoryEditScene::onEnter()
{
	LayerColor::onEnter();

	m_queryData = CalcHistoryDB::getItemById(getDbId());
	if (m_queryData.size() != 0) {
		calcLineHeight();
	}

	//log(FuncUtil::longToString(getDbId()).c_str());
	//log(FuncUtil::longToString(m_queryData.size()).c_str());

	if (!m_tableView) {
		m_tableView = TableView::create(this, Size(m_visibleSize.width, m_visibleSize.height - m_backButton->getContentSize().height - m_visibleSize.width / 15));
		m_tableView->setPosition(m_visibleOrigin.x, m_visibleOrigin.y);
		this->addChild(m_tableView);
		m_tableView->setVerticalFillOrder(TableView::VerticalFillOrder::TOP_DOWN);
		m_tableView->setDelegate(this);
	}
}

void CalcHistoryEditScene::addBackButton()
{
	auto backButtonwSprite = Scale9Sprite::create(BUTTON_BG);
	auto label = Label::createWithTTF(FuncUtil::getLang("goBack"), DEFAULT_CHINESE_FONT, m_visibleSize.width / 19);
	label->setColor(Color3B::BLACK);
	m_backButton = ControlButton::create(label, backButtonwSprite);
	m_backButton->setPreferredSize(Size(m_visibleSize.width / 5, m_visibleSize.width / 10));
	m_backButton->addTargetWithActionForControlEvents(this, cccontrol_selector(CalcHistoryEditScene::onBackButtonCallback), Control::EventType::TOUCH_DOWN);
	m_backButton->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
	m_backButton->setPosition(Vec2(m_visibleOrigin.x + m_visibleSize.width / 30,
		m_visibleOrigin.y + m_visibleSize.height - m_visibleSize.width / 30));
	label->setPosition(m_backButton->getContentSize().width / 2.15, m_backButton->getContentSize().height / 2.15);

	addChild(m_backButton, 2);
}

void CalcHistoryEditScene::onBackButtonCallback(Ref* pSender, Control::EventType event)
{
	Director::getInstance()->popSceneWithTransition<TransitionFadeTR>(0.6f);
}

void CalcHistoryEditScene::calcLineHeight() {
	Label* label;
	for (int i = 0; i < 4; i++) {
		m_labelHeight[i] = m_visibleSize.width * 0.2;
		if (i == 1) {
			label = Label::createWithTTF(m_queryData["calc_str"]._textData, DEFAULT_FONT, m_visibleSize.width / 19, Size(m_visibleSize.width * 0.65, 0));
			label->setPosition(-9999, -9999);
			m_labelHeight[i] += label->getLineHeight() * (label->getStringNumLines() - 1);
		}
	}
}

TableViewCell* CalcHistoryEditScene::tableCellAtIndex(TableView* table, ssize_t idx)
{
	Label* titleLabel;
	Label* dataLabel;

	string titleStr;
	string dataStr;

	TableViewCell* cell = table->dequeueCell();

	if (!cell)
	{
		cell = new TableViewCell();
		cell->autorelease();
	}
	else {
		cell->removeAllChildrenWithCleanup(true);
	}

	Sprite* seperateLine = Sprite::create(GREY_DOT_ICO);
	seperateLine->setScaleX(m_visibleSize.width);
	seperateLine->setAnchorPoint(Point::ZERO);
	seperateLine->setPosition(Point::ZERO);
	cell->addChild(seperateLine, 0, 1);

	switch (idx) {
		case 0:
			titleStr = FuncUtil::getLang("time");
			dataStr = FuncUtil::showTime(m_queryData["time"]._longData);
			break;
		case 1:
			titleStr = FuncUtil::getLang("calcStr");
			dataStr = m_queryData["calc_str"]._textData;
			break;
		case 2:
			titleStr = FuncUtil::getLang("description");
			break;
	}
	titleLabel = Label::createWithTTF(titleStr, DEFAULT_CHINESE_FONT, m_visibleSize.width / 19);
	titleLabel->setColor(Color3B::BLACK);
	titleLabel->enableBold();
	titleLabel->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
	titleLabel->setPosition(m_visibleSize.width * 0.03, m_labelHeight[idx] / 2);
	cell->addChild(titleLabel, 0, 2);

	if (idx < 2) {
		dataLabel = Label::createWithTTF(dataStr, DEFAULT_FONT, m_visibleSize.width / 19, Size(m_visibleSize.width * 0.65, 0));
		dataLabel->setAlignment(TextHAlignment::LEFT);
		dataLabel->setColor(Color3B(40,40,40));
		dataLabel->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
		dataLabel->setPosition(m_visibleSize.width * 0.97, m_labelHeight[idx] / 2);
		cell->addChild(dataLabel, 0, 3);
	}else{
		auto descriptionEditBox = ui::EditBox::create(Size(m_visibleSize.width * 0.65, m_labelHeight[idx] / 2), Scale9Sprite::create(EDITBOX_BG));
		descriptionEditBox->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
		descriptionEditBox->setPosition(Vec2(m_visibleSize.width * 0.97, m_labelHeight[idx] / 2));
		descriptionEditBox->setFontColor(Color3B::BLACK); 
		descriptionEditBox->setText(m_queryData["description"]._textData.c_str()); 
		descriptionEditBox->setMaxLength(8);
		descriptionEditBox->setInputMode(ui::EditBox::InputMode::SINGLE_LINE);//可以输入任何，但是不包括换行 
		descriptionEditBox->setInputFlag(ui::EditBox::InputFlag::INITIAL_CAPS_WORD);//设置输入标志位  
		descriptionEditBox->setReturnType(ui::EditBox::KeyboardReturnType::DONE);//设置返回类型  
		descriptionEditBox->setDelegate(this);
		descriptionEditBox->setTag(101);
		cell->addChild(descriptionEditBox,4);
	}

	return cell;
}

Size CalcHistoryEditScene::tableCellSizeForIndex(TableView* table, ssize_t idx)
{
	return Size(m_visibleSize.width, m_labelHeight[idx]);
}

ssize_t CalcHistoryEditScene::numberOfCellsInTableView(TableView* table)
{
	return 3;
}

void CalcHistoryEditScene::tableCellTouched(TableView* table, TableViewCell* cell)
{
	return ;
	string index;
	ssize_t idx = cell->getIdx();
	if (idx == 0) {
		return;
	}
	stringstream stream;
	stream << idx;
	stream >> index;
	CocosToast::createToast(this, index, 2, Vec2(m_visibleOrigin.x + m_visibleSize.width / 2, m_visibleOrigin.y + m_visibleSize.height / 2));
}

void CalcHistoryEditScene::scrollViewDidScroll(cocos2d::extension::ScrollView* view) { }
void CalcHistoryEditScene::scrollViewDidZoom(cocos2d::extension::ScrollView* view) { }

void CalcHistoryEditScene::editBoxEditingDidBegin(ui::EditBox *editBox)
{

}
void CalcHistoryEditScene::editBoxEditingDidEnd(ui::EditBox *editBox)
{

}
void CalcHistoryEditScene::editBoxReturn(ui::EditBox *editBox)
{
	string value = editBox->getText();
	if (CalcHistoryDB::updateDescriptionById(getDbId(), value)) {
		CocosToast::createToast(this, FuncUtil::getLang("updateComplete"), 2, Vec2(m_visibleOrigin.x + m_visibleSize.width / 2, m_visibleOrigin.y + m_visibleSize.height / 2));
	}else{
		CocosToast::createToast(this, FuncUtil::getLang("updateFailed"), 2, Vec2(m_visibleOrigin.x + m_visibleSize.width / 2, m_visibleOrigin.y + m_visibleSize.height / 2));
	}
}
void CalcHistoryEditScene::editBoxTextChanged(ui::EditBox *editBox, const std::string &text)
{

}