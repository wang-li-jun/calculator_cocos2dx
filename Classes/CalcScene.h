#ifndef __CALC_SCENE_H__
#define __CALC_SCENE_H__
#include <regex>
#include "cocos2d.h"
#include "cocos-ext.h"

#include "CocosToast.h"
#include "ConstUtil.h"
#include "FuncUtil.h"
#include "DbHelper.h"
#include "CalcHistoryDB.h"

USING_NS_CC;
using namespace cocos2d::extension;

class CalcScene : public LayerColor
{
public:
	static Scene* createScene();
	virtual bool init();
	CREATE_FUNC(CalcScene);

	void initLabel();
	void initCalcButton();
	void onCalcButtonTouchDownCallback(Ref* pSender, Control::EventType event);
	void onCalcButtonTouchUpInsideCallback(Ref* pSender, Control::EventType event);
	void onCalcButtonTouchUpOutsideCallback(Ref* pSender, Control::EventType event);
	void pressButton(float dt);//long press and schedule it to run

	void cacheLastCharacter();//cache the last character of m_calcString to m_lastCharacter
	void processCalcString(const string& newStr);//when clicking the calcButton, the new string will be added to calcString
	int getOperatorPriority(char op);
	void generateSuffixString();
	string calc();//calculate the calcString and give the result
private:

	Size m_visibleSize;
	Vec2 m_visibleOrigin;
	float m_calcButtonWidth;
	float m_calcButtonHeight;

	string m_currentLongPressedString = "";
	bool m_isLongPressed = false;
	string m_calcString; //hold the calculating string
	string m_calcResult = ""; //hold the result of calculation. when it is not blank, it implies that the equal mark is pressed.
	vector <char> m_calcSuffixWector;//hold the transforming result of 1+2 =>1 2 +
	enum ParenthesesWay { LEFT, RIGHT } m_ParenthesesWay = LEFT;//when pressing the button "()", then which parentheses we will use.
	enum LastCharacter { NONE, NUMBER, OPERATOR, DECIMAL_POINT, LEFT_Parenthese, RIGHT_Parenthese, INVALID } m_lastCharacter = NONE;

	Label* m_calcStringLabel;
	ScrollView* m_calcStringScrollView;
	Label* m_calcResultLabel;

};

#endif