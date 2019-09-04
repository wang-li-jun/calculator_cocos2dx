#include "CalcScene.h"


Scene* CalcScene::createScene() {
	auto scene = Scene::create();
	auto layer = CalcScene::create();
	scene->addChild(layer);
	return scene;
}

bool CalcScene::init() {
	if (!LayerColor::initWithColor(Color4B(255, 255, 255, 255))) {
		return false;
	}
	m_visibleSize = Director::getInstance()->getVisibleSize();
	m_visibleOrigin = Director::getInstance()->getVisibleOrigin();

	m_calcButtonWidth = m_visibleSize.width / 4;
	m_calcButtonHeight = m_visibleSize.height / 7.5;

	m_calcString = "";

	//DbHelper::connect();
	//log(to_string(DbHelper::getVersion()).c_str());
	//DbHelper::setVersion(0);
	//log(to_string(DbHelper::getVersion()).c_str());
	//DbHelper::close();

	//CalcHistoryDB::add("aa");
	CalcHistoryDB::getFullData();
	initLabel();
	initCalcButton();
	return true;
}

void CalcScene::initLabel() {
	m_calcStringLabel = Label::createWithTTF("", DEFAULT_FONT, m_calcButtonWidth / 3, Size(m_visibleSize.width * 0.9, m_calcButtonHeight / 3), TextHAlignment::RIGHT, TextVAlignment::BOTTOM);
	m_calcStringLabel->setColor(Color3B(18, 65, 145));
	m_calcStringLabel->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
	//m_calcStringLabel->setPosition(m_visibleOrigin.x + m_visibleSize.width * 0.05, m_visibleOrigin.y + m_calcButtonHeight * 6);
	//addChild(m_calcStringLabel, 1);

	m_calcStringScrollView = ScrollView::create(Size(m_visibleSize.width, m_calcButtonHeight), m_calcStringLabel);
	m_calcStringScrollView->setDirection(ScrollView::Direction::VERTICAL);
	m_calcStringScrollView->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
	m_calcStringScrollView->setPosition(m_visibleOrigin.x, m_visibleOrigin.y + m_calcButtonHeight * 6);
	m_calcStringScrollView->setContentOffset(Point(m_visibleSize.width * 0.05,0));
	//m_calcStringScrollView->setBounceable(false);
	addChild(m_calcStringScrollView, 1);

	m_calcResultLabel = Label::createWithTTF("", DEFAULT_FONT, m_calcButtonWidth / 2, Size(m_visibleSize.width * 0.9, m_calcButtonHeight), TextHAlignment::RIGHT,TextVAlignment::CENTER);
	m_calcResultLabel->setColor(Color3B(18, 65, 145));
	m_calcResultLabel->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
	m_calcResultLabel->setPosition(m_visibleOrigin.x + m_visibleSize.width * 0.05, m_visibleOrigin.y + m_calcButtonHeight * 5);

	addChild(m_calcResultLabel, 1);
}

void CalcScene::initCalcButton() {
	string calcButtonConfig[20][5] = {
		{ "0", CALC_BUTTON_BG, "0", "0", "1" } ,
		{ ".", CALC_BUTTON_BG, "1", "0", "1" } ,
		{ "H", CALC_BUTTON_BG, "2", "0", "1" } ,
		{ "=", CALC_BUTTON_BG, "3", "0", "1" } ,
		{ "1", CALC_BUTTON_BG, "0", "1", "1" } ,
		{ "2", CALC_BUTTON_BG, "1", "1", "1" } ,
		{ "3", CALC_BUTTON_BG, "2", "1", "1" } ,
		{ "+", CALC_BUTTON_BG, "3", "1", "1" } ,
		{ "4", CALC_BUTTON_BG, "0", "2", "1" } ,
		{ "5", CALC_BUTTON_BG, "1", "2", "1" } ,
		{ "6", CALC_BUTTON_BG, "2", "2", "1" } ,
		{ "-", CALC_BUTTON_BG, "3", "2", "1" } ,
		{ "7", CALC_BUTTON_BG, "0", "3", "1" } ,
		{ "8", CALC_BUTTON_BG, "1", "3", "1" } ,
		{ "9", CALC_BUTTON_BG, "2", "3", "1" } ,
		{ FuncUtil::getLang("multiplyMark"), CALC_BUTTON_BG, "3", "3", "0.75" } ,
		{ "C", CALC_BUTTON_BG, "0", "4", "1" } ,
		{ "()", CALC_BUTTON_BG, "1", "4", "1" } ,
		{ FuncUtil::getLang("leftArrowMark"), CALC_BUTTON_BG, "2", "4", "1" } ,
		{ FuncUtil::getLang("divideMark"), CALC_BUTTON_BG, "3", "4", "0.75" } ,
	};

	for (auto &row : calcButtonConfig) {
		//CocosToast::createToast(this, row[0], 1, Vec2(m_visibleOrigin.x + m_visibleSize.width / 2, m_visibleOrigin.y + m_visibleSize.height / 2));
		auto calcButtonSprite = Scale9Sprite::create(row[1]);
		auto calcButtonLabel = Label::createWithTTF(row[0], DEFAULT_CHINESE_FONT, m_calcButtonWidth / 2 * FuncUtil::stringToDouble(row[4]));
		calcButtonLabel->setColor(Color3B::BLACK);
		auto calcButton = ControlButton::create(calcButtonLabel, calcButtonSprite);
		calcButton->setPreferredSize(Size(m_calcButtonWidth, m_calcButtonHeight));
		calcButton->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
		calcButton->setPosition(Vec2(m_visibleOrigin.x + m_calcButtonWidth * std::atoi(row[2].c_str()),
			m_visibleOrigin.y + m_calcButtonHeight * std::atoi(row[3].c_str())));
		calcButton->addTargetWithActionForControlEvents(this, cccontrol_selector(CalcScene::onCalcButtonTouchDownCallback), Control::EventType::TOUCH_DOWN);
		calcButton->addTargetWithActionForControlEvents(this, cccontrol_selector(CalcScene::onCalcButtonTouchUpInsideCallback), Control::EventType::TOUCH_UP_INSIDE);
		calcButton->addTargetWithActionForControlEvents(this, cccontrol_selector(CalcScene::onCalcButtonTouchUpOutsideCallback), Control::EventType::TOUCH_UP_OUTSIDE);
		addChild(calcButton, 1);
	}
}

void CalcScene::onCalcButtonTouchDownCallback(Ref* pSender, Control::EventType event) {
	auto calcButton = (ControlButton *)pSender;
	auto calcButtonLabel = (Label *)calcButton->getTitleLabel();
	m_currentLongPressedString = calcButtonLabel->getString();
	this->schedule(schedule_selector(CalcScene::pressButton), 0.15f, 999999, 0.8f);
}

void CalcScene::onCalcButtonTouchUpInsideCallback(Ref* pSender, Control::EventType event) {
	if (m_isLongPressed == false) {
		auto calcButton = (ControlButton *)pSender;
		auto calcButtonLabel = (Label *)calcButton->getTitleLabel();
		processCalcString(calcButtonLabel->getString());
	}
	this->unschedule(schedule_selector(CalcScene::pressButton));
	m_isLongPressed = false;
	m_currentLongPressedString = "";
}

void CalcScene::onCalcButtonTouchUpOutsideCallback(Ref* pSender, Control::EventType event) {
	this->unschedule(schedule_selector(CalcScene::pressButton));
	m_isLongPressed = false;
	m_currentLongPressedString = "";
}

void CalcScene::pressButton(float dt) {
	m_isLongPressed = true;
	processCalcString(m_currentLongPressedString);
}


void CalcScene::cacheLastCharacter() {
	if (m_calcString.length() != 0) {
		char lastCharacter = m_calcString[m_calcString.length() - 1];
		switch (lastCharacter) {
		case '(':
			m_lastCharacter = LastCharacter::LEFT_Parenthese;
			break;
		case ')':
			m_lastCharacter = LastCharacter::RIGHT_Parenthese;
			break;
		case '1':
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case '7':
		case '8':
		case '9':
		case '0':
			m_lastCharacter = LastCharacter::NUMBER;
			break;
		case '+':
		case '-':
		case '*':
		case '/':
			m_lastCharacter = LastCharacter::OPERATOR;
			break;
		case '.':
			m_lastCharacter = LastCharacter::DECIMAL_POINT;
			break;
		default:
			m_lastCharacter = LastCharacter::INVALID;
		}
	}
	else {
		m_lastCharacter = LastCharacter::NONE;
	}
}

void CalcScene::processCalcString(string newStr) {
	// ×=>* ÷=>/
	newStr = FuncUtil::markToOperator(newStr);

	//when last time equal mark is pressed, if newStr is not operator, begin from blank.
	if (m_calcResult != "") {
		if (!strcmp(newStr.c_str(), "+") || !strcmp(newStr.c_str(), "-") || !strcmp(newStr.c_str(), "*") || !strcmp(newStr.c_str(), "/")) {
			//preceed normally
		}
		else {
			m_calcString = "";
		}
		m_calcResultLabel->setString("");
		m_calcResult = "";

	}

	//cache the last charactor of calcString so that we can judge with newStr together
	cacheLastCharacter();

	//when the last character is unknown one, output INVALID INPUT value
	if (m_lastCharacter == LastCharacter::INVALID) {
		m_calcResultLabel->setString("INVALID INPUT");
		return;
	}

	if (!strcmp(newStr.c_str(), "()")) {
		if (m_ParenthesesWay == ParenthesesWay::LEFT) {
			if (m_lastCharacter == LastCharacter::NONE || m_lastCharacter == LastCharacter::OPERATOR) {
				m_calcString += '(';
				m_ParenthesesWay = ParenthesesWay::RIGHT;
			}
			else {
				return;
			}
		}
		else {
			if (m_lastCharacter == LastCharacter::NUMBER) {
				m_calcString += ')';
				m_ParenthesesWay = ParenthesesWay::LEFT;
			}
			else {
				return;
			}
		}
	}
	else if (!strcmp(newStr.c_str(), "+") || !strcmp(newStr.c_str(), "-") || !strcmp(newStr.c_str(), "*") || !strcmp(newStr.c_str(), "/")) {
		if (m_lastCharacter == LastCharacter::LEFT_Parenthese || m_lastCharacter == LastCharacter::NONE) {
			if (!strcmp(newStr.c_str(), "-")) {
				m_calcString += newStr;
			}
		}
		else if (m_lastCharacter == LastCharacter::NUMBER || m_lastCharacter == LastCharacter::RIGHT_Parenthese) {
			m_calcString += newStr;
		}
		else if (m_lastCharacter == LastCharacter::OPERATOR) {
			m_calcString.erase(m_calcString.end() - 1);
			m_calcString += newStr;
		}
		else {
			return;
		}
	}
	else if (!strcmp(newStr.c_str(), "0") || !strcmp(newStr.c_str(), "1") || !strcmp(newStr.c_str(), "2") || !strcmp(newStr.c_str(), "3") || !strcmp(newStr.c_str(), "4") || !strcmp(newStr.c_str(), "5") || !strcmp(newStr.c_str(), "6") || !strcmp(newStr.c_str(), "7") || !strcmp(newStr.c_str(), "8") || !strcmp(newStr.c_str(), "9")) {
		if (m_lastCharacter == LastCharacter::RIGHT_Parenthese) {
			return;
		}
		else {
			m_calcString += newStr;
		}
	}
	else if (!strcmp(newStr.c_str(), ".")) {
		if (m_lastCharacter == LastCharacter::NUMBER) {
			m_calcString += ".";
		}
		else {
			return;
		}
	}
	else if (!strcmp(newStr.c_str(), "<=")) {
		if (m_calcString.length() != 0) {
			if (m_lastCharacter == LastCharacter::LEFT_Parenthese) {
				m_ParenthesesWay = ParenthesesWay::LEFT;
			}
			else if (m_lastCharacter == LastCharacter::RIGHT_Parenthese) {
				m_ParenthesesWay = ParenthesesWay::RIGHT;
			}
			m_calcString.erase(m_calcString.end() - 1);
		}
	}
	else if (!strcmp(newStr.c_str(), "H")) {
		auto calcHistoryScene = CalcHistoryScene::createScene();
		Director::getInstance()->pushScene(CCTransitionMoveInR::create(0.3, calcHistoryScene));
	}
	else if (!strcmp(newStr.c_str(), "C")) {
		m_calcString = "";
		m_calcResultLabel->setString("");
		m_ParenthesesWay = ParenthesesWay::LEFT;
	}
	else if (!strcmp(newStr.c_str(), "=")) {
		if (m_lastCharacter == LastCharacter::NUMBER || m_lastCharacter == LastCharacter::RIGHT_Parenthese) {
			m_calcResult = calc();
			m_calcResultLabel->setString(m_calcResult);
			CalcHistoryDB::add(FuncUtil::operatorToMark(m_calcString) + "=" + m_calcResult);
		}
		else {
			return;
		}
	}
	else {
		m_calcString += newStr;
	}
	if (m_calcResult == "") {
		m_calcStringLabel->setString(FuncUtil::operatorToMark(m_calcString));
	}else{
		m_calcStringLabel->setString(FuncUtil::operatorToMark(m_calcString) + "=" + m_calcResult);
		m_calcString = m_calcResult;
	}
	if (m_calcString == "inf") {//when 0 is divided
		m_calcString = "";
	}else{
		generateSuffixString();
	}
	//string m_calcSuffixString(m_calcSuffixWector.begin(), m_calcSuffixWector.end());
	//log(("calc suffix string: " + m_calcSuffixString).c_str());
	//log(("calc string: " + m_calcString).c_str());
	
	//log(m_calcStringLabel->getString().c_str());
	//log(to_string(m_calcStringLabel->getStringLength()).c_str());
	//log(to_string(m_calcStringLabel->getStringNumLines()).c_str());
	m_calcStringLabel->setDimensions(m_visibleSize.width * 0.9, m_calcStringLabel->getLineHeight() * m_calcStringLabel->getStringNumLines());
	m_calcStringScrollView->setContentOffset(Point(m_visibleSize.width * 0.05, 0));

	if (m_calcResult.length() > 13) {
		float scaleFactor = (float)13 / m_calcResult.length();
		m_calcResultLabel->setDimensions(m_visibleSize.width * 0.9 / scaleFactor, m_calcButtonHeight / scaleFactor);
		m_calcResultLabel->setScale(scaleFactor);
	}else{
		m_calcResultLabel->setDimensions(m_visibleSize.width * 0.9, m_calcButtonHeight);
		m_calcResultLabel->setScale(1);
	}
}


int CalcScene::getOperatorPriority(char op) {
	if (op == '+' || op == '-')
		return 0;
	if (op == '*' || op == '/')
		return 1;
	return -1;
}

//https://blog.csdn.net/akyj1021/article/details/83186519
void CalcScene::generateSuffixString() {
	const char* calcString = m_calcString.c_str();
	int i = 0, len = strlen(calcString);
	m_calcSuffixWector.clear();
	stack<char> m_stack;

	m_stack.push('#');// 特殊符号用来区分元素
	while (i < len)
	{
		if (calcString[i] == '(')
		{
			m_stack.push(calcString[i]);
			i++;
		}
		else if (calcString[i] == ')')
		{
			while (m_stack.top() != '(')
			{
				m_calcSuffixWector.push_back(m_stack.top());
				m_calcSuffixWector.push_back(' ');
				m_stack.pop();
			}
			m_stack.pop();
			i++;
		}
		else if (calcString[i] == '+' || calcString[i] == '-' || calcString[i] == '/' || calcString[i] == '*')
		{
			while (getOperatorPriority(m_stack.top()) >= getOperatorPriority(calcString[i]))
			{
				m_calcSuffixWector.push_back(m_stack.top());
				m_calcSuffixWector.push_back(' ');
				m_stack.pop();
			}
			m_stack.push(calcString[i]);
			i++;
		}
		else if (calcString[i] >= '0' && calcString[i] <= '9' || calcString[i] == '.')
		{
			while (calcString[i] >= '0' && calcString[i] <= '9' || calcString[i] == '.' || calcString[i] == 'e')
			{
				m_calcSuffixWector.push_back(calcString[i]);
				if (calcString[i] == 'e') {
					i++;
					if (calcString[i] == '+') {
						m_calcSuffixWector.push_back(calcString[i]);
					}
				}
				i++;
			}
			m_calcSuffixWector.push_back(' ');
		}
		else
		{
			i++;
		}
	}
	while (m_stack.top() != '#') {
		m_calcSuffixWector.push_back(m_stack.top());
		m_calcSuffixWector.push_back(' ');
		m_stack.pop();
	}
}

string CalcScene::calc() {
	double x;
	stack<double> m_stack;
	vector<char>::iterator i = m_calcSuffixWector.begin();
	string figure;

	while (i != m_calcSuffixWector.end()) {
		if (*i == ' ') {
			i++;
			continue;
		}
		switch (*i) {
		case '+':
			x = m_stack.top();
			m_stack.pop();
			x = m_stack.top() + x;
			m_stack.pop();
			i++;
			break;
		case '-':
			x = m_stack.top();
			m_stack.pop();
			if (m_stack.size() == 0) {
				x = 0 - x;
			}
			else {
				x = m_stack.top() - x;
				m_stack.pop();
			}
			i++;
			break;
		case '*':
			x = m_stack.top();
			m_stack.pop();
			x = m_stack.top()*x;
			m_stack.pop();
			i++;
			break;
		case '/':
			x = m_stack.top();
			m_stack.pop();
			x = m_stack.top() / x;
			m_stack.pop();
			i++;
			break;
		default:
		{
			figure = "";
			while (*i >= '0'&& *i <= '9' || *i == '.' || *i == 'e') {
				figure += *i;
				if (*i == 'e') {
					i++;
					if (*i == '+') {
						figure += *i;
					}
				}
				i++;
			}
			x = atof(figure.c_str());
		}
		}
		m_stack.push(x);
	}
	return FuncUtil::doubleToString(m_stack.top(), 9);
}

