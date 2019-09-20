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
	addDeleteAllButton();

	m_historyData = CalcHistoryDB::getFullData();
	calcLineHeight();
	/*
	for (auto row : m_historyData)
	{
		log(row["calc_str"]._textData.c_str());
	}
	*/

	m_tableView = TableView::create(this, Size(m_visibleSize.width, m_visibleSize.height - m_backButton->getContentSize().height - m_visibleSize.width / 30));
	m_tableView->setPosition(m_visibleOrigin.x, m_visibleOrigin.y);
	this->addChild(m_tableView);
	//tableView->setVerticalFillOrder(TableView::VerticalFillOrder::TOP_DOWN);
	m_tableView->setBounceable(false);
	m_tableView->setDelegate(this);
	//tableView->reloadData();

	return true;
}

void CalcHistoryScene::addBackButton()
{
	auto backButtonwSprite = Scale9Sprite::create(MENU_BUTTON_BG);
	auto label = Label::createWithTTF(FuncUtil::getLang("goBack"), DEFAULT_CHINESE_FONT, m_visibleSize.width / 19);
	label->setColor(Color3B::BLACK);
	m_backButton = ControlButton::create(label,backButtonwSprite);
	m_backButton->setPreferredSize(Size(m_visibleSize.width / 5, m_visibleSize.width / 10));
	m_backButton->addTargetWithActionForControlEvents(this, cccontrol_selector(CalcHistoryScene::onBackButtonCallback), Control::EventType::TOUCH_DOWN);
	m_backButton->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
	m_backButton->setPosition(Vec2(m_visibleOrigin.x + m_visibleSize.width / 30,
		m_visibleOrigin.y + m_visibleSize.height - m_visibleSize.width / 30));
	label->setPosition(m_backButton->getContentSize().width / 2.15, m_backButton->getContentSize().height / 2.15);

	addChild(m_backButton, 2);
}

void CalcHistoryScene::addDeleteAllButton()
{
	auto buttonwSprite = Scale9Sprite::create(MENU_BUTTON_BG);
	auto label = Label::createWithTTF(FuncUtil::getLang("deleteAll"), DEFAULT_CHINESE_FONT, m_visibleSize.width / 19);
	label->setColor(Color3B::BLACK);
	m_deleteAllButton = ControlButton::create(label, buttonwSprite);
	m_deleteAllButton->setPreferredSize(Size(m_visibleSize.width / 3.7, m_visibleSize.width / 10));
	m_deleteAllButton->addTargetWithActionForControlEvents(this, cccontrol_selector(CalcHistoryScene::onDeleteAllButtonCallback), Control::EventType::TOUCH_DOWN);
	m_deleteAllButton->setAnchorPoint(Vec2::ANCHOR_TOP_RIGHT);
	m_deleteAllButton->setPosition(Vec2(m_visibleOrigin.x + m_visibleSize.width - m_visibleSize.width / 30,
		m_visibleOrigin.y + m_visibleSize.height - m_visibleSize.width / 30));
	label->setPosition(m_backButton->getContentSize().width / 1.58, m_backButton->getContentSize().height / 2.2);

	addChild(m_deleteAllButton, 2);
}

/*
void CalcHistoryScene::calcLineHeight() {
	Label* label;
	int dataSize = m_historyData.size();
	for (int i = 0; i < dataSize; i++) {
		label = Label::createWithTTF(m_historyData.at(i)["calc_str"]._textData, DEFAULT_CHINESE_FONT, m_visibleSize.width / 15, Size(m_visibleSize.width * 0.95, 0));
		label->setPosition(-9999,-9999);
		m_labelHeight[i] = label->getLineHeight() * (label->getStringNumLines() - 1);
	}
}
*/

void CalcHistoryScene::calcLineHeight() {
	auto richText = ui::RichText::create();
	richText->setAnchorPoint(Point::ANCHOR_BOTTOM_LEFT);
	richText->setContentSize(Size(m_visibleSize.width * 0.95, 0));
	richText->setWrapMode(ui::RichText::WrapMode::WRAP_PER_CHAR);
	richText->ignoreContentAdaptWithSize(false);
	
	int dataSize = m_historyData.size();
	for (int i = 0; i < dataSize; i++) {
		ui::RichElementText* re1 = ui::RichElementText::create(1, Color3B::GREEN, 255, m_historyData.at(i)["calc_str"]._textData.substr(0, m_historyData.at(i)["calc_str"]._textData.find("=")), DEFAULT_FONT, m_visibleSize.width / 15);
		ui::RichElementText* re2 = ui::RichElementText::create(2, Color3B::GREEN, 255, " = " + m_historyData.at(i)["calc_str"]._textData.substr(m_historyData.at(i)["calc_str"]._textData.find("=") + 1	), DEFAULT_FONT, m_visibleSize.width / 15);
		richText->pushBackElement(re1);
		richText->pushBackElement(re2);
			
		richText->formatText();
		//log(m_historyData.at(i)["calc_str"]._textData.c_str());
		//log(to_string(richText->getSize().height).c_str());

		m_richTextHeight[i + 1] = richText->getContentSize().height;

		richText->removeElement(re1);
		richText->removeElement(re2);
	}

	Label* label;
	for (int i = 0; i < dataSize; i++) {
		label = Label::createWithTTF(FuncUtil::showTime(m_historyData.at(i)["time"]._longData), DEFAULT_FONT, m_visibleSize.width / 25, Size(m_visibleSize.width * 0.95, 0));
		label->setPosition(-9999, -9999);
		m_labelHeight[i + 1] = label->getLineHeight() * label->getStringNumLines();
	}
}

void CalcHistoryScene::onBackButtonCallback(Ref* pSender, Control::EventType event)
{
	Director::getInstance()->popSceneWithTransition<TransitionFadeTR>(0.6f);
	//Director::getInstance()->popScene();
	FuncUtil::printMem();
}

void CalcHistoryScene::onDeleteAllButtonCallback(Ref* pSender, Control::EventType event)
{
	CocosDialog* popup = CocosDialog::create(DIALOG_BG);
	popup->setTitle(FuncUtil::getLang("alert").c_str());
	popup->setContentText(FuncUtil::getLang("deleteConfirm").c_str());
	popup->setCallBackFunc(this, callfuncN_selector(CalcHistoryScene::popButtonCallback));
	popup->addButton(MENU_BUTTON_BG, MENU_BUTTON_BG, FuncUtil::getLang("ok").c_str(), 0);
	popup->addButton(MENU_BUTTON_BG, MENU_BUTTON_BG, FuncUtil::getLang("cancel").c_str(), 1);
	this->addChild(popup,20);
}

TableViewCell* CalcHistoryScene::tableCellAtIndex(TableView* table, ssize_t idx)
{

	//log(to_string(idx).c_str());
	//log(m_historyData.at(idx)["calc_str"]._textData.c_str());

	TableViewCell* cell = table->dequeueCell();
	ui::RichText* richText;
	ui::RichElementText* re1;
	ui::RichElementText* re2;
	ui::RichElementText* re3;
	Label* bottomLabel;
	if (!cell)
	{
		//log(("new" + to_string(idx)).c_str());
		cell = new TableViewCell();
		cell->autorelease();

		Sprite* seperateLine = Sprite::create(GREY_DOT_ICO);
		seperateLine->setScaleX(m_visibleSize.width);
		seperateLine->setAnchorPoint(Point::ZERO);
		seperateLine->setPosition(Point::ZERO);
		cell->addChild(seperateLine, 0, 1);
		
	}
	else {
		//Texture2D* texture = Director::getInstance()->getTextureCache()->addImage(GREY_DOT_ICO);
		//Sprite* seperateLine = (Sprite*)cell->getChildByTag(1);
		//seperateLine->setTexture(texture);
		richText = (ui::RichText*)cell->getChildByTag(2);
		cell->removeChild(richText);

		bottomLabel = (Label*)cell->getChildByTag(3);
		cell->removeChild(bottomLabel);
	}
	if (idx != 0) {
		richText = ui::RichText::create();
		richText->setAnchorPoint(Point::ANCHOR_BOTTOM_LEFT);
		richText->setContentSize(Size(m_visibleSize.width * 0.95, 0));
		richText->ignoreContentAdaptWithSize(false);
		richText->setWrapMode(ui::RichText::WrapMode::WRAP_PER_CHAR);
		cell->addChild(richText, 0, 2);
		re1 = ui::RichElementText::create(1, Color3B(0, 0, 0), 255, m_historyData.at(idx - 1)["calc_str"]._textData.substr(0, m_historyData.at(idx - 1)["calc_str"]._textData.find("=")), DEFAULT_FONT, m_visibleSize.width / 15);
		re2 = ui::RichElementText::create(2, Color3B(18, 65, 145), 255, " = " + m_historyData.at(idx - 1)["calc_str"]._textData.substr(m_historyData.at(idx - 1)["calc_str"]._textData.find("=") + 1), DEFAULT_FONT, m_visibleSize.width / 15);

		richText->pushBackElement(re1);
		richText->pushBackElement(re2);
		richText->setPosition(Vec2(m_visibleSize.width / 35, m_tableCellDefaultHeight / 2 + m_labelHeight[idx]));

		bottomLabel = Label::createWithTTF(FuncUtil::showTime(m_historyData.at(idx - 1)["time"]._longData), DEFAULT_FONT, m_visibleSize.width / 25, Size(m_visibleSize.width * 0.95, 0));
		bottomLabel->enableBold();
		bottomLabel->setColor(Color3B(150, 150, 150));
		bottomLabel->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
		bottomLabel->setPosition(m_visibleSize.width / 35, m_visibleSize.width / 50);
		cell->addChild(bottomLabel, 0, 3);
	}
	return cell;
}

Size CalcHistoryScene::tableCellSizeForIndex(TableView* table, ssize_t idx)
{
	if (idx == 0) {
		return Size(m_visibleSize.width, 0);
	}else{
		return Size(m_visibleSize.width, m_tableCellDefaultHeight + m_richTextHeight[idx] + m_labelHeight[idx]);
	}
	//return Size(m_visibleSize.width, m_tableCellDefaultHeight);
}

ssize_t CalcHistoryScene::numberOfCellsInTableView(TableView* table)
{
	return m_historyData.size() + 1;
}

void CalcHistoryScene::tableCellTouched(TableView* table, TableViewCell* cell)
{
	string index;
	ssize_t idx = cell->getIdx();
	if (idx == 0) {
		return;
	}
	stringstream stream;
	stream << idx;
	stream >> index;
	CocosToast::createToast(this, index, 1, Vec2(m_visibleOrigin.x + m_visibleSize.width / 2, m_visibleOrigin.y + m_visibleSize.height / 2));
	ui::RichText* richText = (ui::RichText*)cell->getChildByTag(2);
	if (isCellSelected(idx)) {
		Action* slideBackActions = Sequence::create(MoveBy::create(0.2f, Vec2(100, 0))
			, NULL);
		richText->runAction(slideBackActions);
		recordCellSelected(idx, false);
	}
	else {
		//check if there is any selected cell
		ssize_t selectedCellIdx = getSelectedCellIdx();
		if (selectedCellIdx > 0) {
			ui::RichText* selectedRichText = (ui::RichText*)(table->cellAtIndex(selectedCellIdx))->getChildByTag(2);
			Action* slideBackActions = Sequence::create(MoveBy::create(0.2f, Vec2(100, 0))
				, NULL);
			selectedRichText->runAction(slideBackActions);
		}
		Action* slideActions = Sequence::create(MoveBy::create(0.2f, Vec2(-100, 0))
			, NULL);
		richText->runAction(slideActions);
		recordCellSelected(idx, true);
	}
}

void  CalcHistoryScene::popButtonCallback(Node* pNode) {
	if (pNode->getTag() == 0) {
		if (CalcHistoryDB::deleteAll()) {
			CocosToast::createToast(this, FuncUtil::getLang("deleteComplete"), 1, Vec2(m_visibleOrigin.x + m_visibleSize.width / 2, m_visibleOrigin.y + m_visibleSize.height / 2));
			m_historyData = CalcHistoryDB::getFullData();
			m_tableView->reloadData();
		}
		else {
			CocosToast::createToast(this, FuncUtil::getLang("deleteFailed"), 1, Vec2(m_visibleOrigin.x + m_visibleSize.width / 2, m_visibleOrigin.y + m_visibleSize.height / 2));
		}
		pNode->removeFromParent();
	}else if (pNode->getTag() == 1) {
		pNode->removeFromParent();
	}

}

void CalcHistoryScene::recordCellSelected(ssize_t selectedCellIdx, bool setOrUnset) {
	int i;
	for (i = 0; i < 50; i++) {
		m_cellSelected[i] = 0;
	}
	if (setOrUnset) {
		m_cellSelected[selectedCellIdx] = 1;
	}
}

bool CalcHistoryScene::isCellSelected(ssize_t selectedCellIdx){
	if (m_cellSelected[selectedCellIdx] == 1) {
		return true;
	}else {
		return false;
	}
}

ssize_t CalcHistoryScene::getSelectedCellIdx() {
	ssize_t i;
	for (i = 0; i < 50; i++) {
		if (m_cellSelected[i] == 1) {
			return i;
		}
	}
	return 0;//we never use 0 as cell index;
}

void CalcHistoryScene::scrollViewDidScroll(cocos2d::extension::ScrollView* view) { } 
void CalcHistoryScene::scrollViewDidZoom(cocos2d::extension::ScrollView* view) { } 
