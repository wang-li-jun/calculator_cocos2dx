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

	m_historyData = CalcHistoryDB::getFullData();
	calcLineHeight();
	/*
	for (auto row : m_historyData)
	{
		log(row["calc_str"]._textData.c_str());
	}
	*/

	TableView* tableView = TableView::create(this, Size(m_visibleSize.width, m_visibleSize.height - m_backButton->getContentSize().height));
	tableView->setPosition(m_visibleOrigin.x, m_visibleOrigin.y);
	this->addChild(tableView);
	//tableView->setVerticalFillOrder(TableView::VerticalFillOrder::TOP_DOWN);
	tableView->setBounceable(false);
	tableView->setDelegate(this);
	//tableView->reloadData();

	return true;
}

void CalcHistoryScene::addBackButton()
{
	auto backButtonwSprite = Scale9Sprite::create(BACK_BUTTON_ICO);
	m_backButton = ControlButton::create(backButtonwSprite);
	m_backButton->addTargetWithActionForControlEvents(this, cccontrol_selector(CalcHistoryScene::onBackButtonCallback), Control::EventType::TOUCH_DOWN);
	m_backButton->setPosition(Vec2(m_visibleOrigin.x + 25,
		m_visibleOrigin.y + m_visibleSize.height - m_backButton->getContentSize().height/2));
	addChild(m_backButton, 2);
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
			ui::RichElementText* re1 = ui::RichElementText::create(1, Color3B(18, 65, 145), 255, m_historyData.at(i)["calc_str"]._textData.substr(0, m_historyData.at(i)["calc_str"]._textData.find("=")), DEFAULT_CHINESE_FONT, m_visibleSize.width / 15);
			ui::RichElementText* re2 = ui::RichElementText::create(2, Color3B::GREEN, 255, " = " + m_historyData.at(i)["calc_str"]._textData.substr(m_historyData.at(i)["calc_str"]._textData.find("=") + 1	), DEFAULT_CHINESE_FONT, m_visibleSize.width / 15);
			richText->pushBackElement(re1);
			richText->pushBackElement(re2);
			
			richText->formatText();
			//log(m_historyData.at(i)["calc_str"]._textData.c_str());
			//log(to_string(richText->getSize().height).c_str());

			m_labelHeight[i] = richText->getSize().height;

			richText->removeElement(re1);
			richText->removeElement(re2);
		
	}
}

void CalcHistoryScene::onBackButtonCallback(Ref* pSender, Control::EventType event)
{
	Director::getInstance()->popSceneWithTransition<TransitionFadeTR>(0.6);
	//Director::getInstance()->popScene();
	//FuncUtil::printMem();
}

TableViewCell* CalcHistoryScene::tableCellAtIndex(TableView* table, ssize_t idx)
{

	//log(to_string(idx).c_str());
	//log(m_historyData.at(idx)["calc_str"]._textData.c_str());

	TableViewCell* cell = table->dequeueCell();
	Label* label;
	ui::RichText* richText;

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
		
		//Label* label = Label::createWithTTF(to_string(idx), DEFAULT_CHINESE_FONT, 20);
		label = Label::createWithTTF(m_historyData.at(idx)["calc_str"]._textData, DEFAULT_CHINESE_FONT, m_visibleSize.width / 15, Size(m_visibleSize.width * 0.95, 0));
		//label->setDimensions(m_visibleSize.width * 0.95, 48);
		//log(to_string(label->getLineHeight() * label->getStringNumLines()).c_str());
		label->setAnchorPoint(Point::ANCHOR_MIDDLE_LEFT);
		label->setColor(Color3B(18, 65, 145));

		//cell->addChild(label, 0, 2);

		
	}
	else {
		Texture2D* texture = Director::getInstance()->getTextureCache()->addImage(GREY_DOT_ICO);
		Sprite* seperateLine = (Sprite*)cell->getChildByTag(1);
		seperateLine->setTexture(texture);

		//label = (Label*)cell->getChildByTag(2);
		//label->setString(m_historyData.at(idx)["calc_str"]._textData);

		//richText = (ui::RichText*)cell->getChildByTag(3);
		//cell->removeChild(richText);
	}
	//label->setPosition(m_visibleSize.width / 35, (m_tableCellDefaultHeight + m_labelHeight[idx]) / 2);
	
	richText = ui::RichText::create();
	richText->setAnchorPoint(Point::ANCHOR_BOTTOM_LEFT);
	richText->setContentSize(Size(m_visibleSize.width * 0.95, 0));
	richText->ignoreContentAdaptWithSize(false);
	richText->setWrapMode(ui::RichText::WrapMode::WRAP_PER_CHAR);
	cell->addChild(richText, 0, 3);
	ui::RichElementText* re1 = ui::RichElementText::create(1, Color3B(69, 69, 69), 255, m_historyData.at(idx)["calc_str"]._textData.substr(0, m_historyData.at(idx)["calc_str"]._textData.find("=")), DEFAULT_CHINESE_FONT, m_visibleSize.width / 15);
	ui::RichElementText* re2 = ui::RichElementText::create(2, Color3B(18, 65, 145), 255, " = " + m_historyData.at(idx)["calc_str"]._textData.substr(m_historyData.at(idx)["calc_str"]._textData.find("=") + 1), DEFAULT_CHINESE_FONT, m_visibleSize.width / 15);

	richText->pushBackElement(re1);
	richText->pushBackElement(re2);
	richText->setPosition(Vec2(m_visibleSize.width / 35, m_tableCellDefaultHeight / 2));
	//richText->setPosition(Vec2(m_visibleSize.width / 35, (m_tableCellDefaultHeight + m_labelHeight[idx] )/ 2 +60 ));

	return cell;
}

Size CalcHistoryScene::tableCellSizeForIndex(TableView* table, ssize_t idx)
{
	return Size(m_visibleSize.width, m_tableCellDefaultHeight + m_labelHeight[idx]);
	//return Size(m_visibleSize.width, m_tableCellDefaultHeight);
}

ssize_t CalcHistoryScene::numberOfCellsInTableView(TableView* table)
{
	return m_historyData.size();
}

void CalcHistoryScene::tableCellTouched(TableView* table, TableViewCell* cell)
{

	string index;
	ssize_t idx = cell->getIdx();
	stringstream stream;
	stream << idx;
	stream >> index;
	CocosToast::createToast(this, index, 1, Vec2(m_visibleOrigin.x + m_visibleSize.width / 2, m_visibleOrigin.y + m_visibleSize.height / 2));

}

void CalcHistoryScene::scrollViewDidScroll(cocos2d::extension::ScrollView* view) { } 
void CalcHistoryScene::scrollViewDidZoom(cocos2d::extension::ScrollView* view) { } 
