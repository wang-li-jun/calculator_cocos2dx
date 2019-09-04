#ifndef __CALC_HISTORY_SCENE_H__
#define __CALC_HISTORY_SCENE_H__

#include "cocos2d.h"
#include "cocos-ext.h"
#include "ui/UIRichText.h"

#include "CocosToast.h"
#include "ConstUtil.h"
#include "FuncUtil.h"
#include "CalcHistoryDB.h"

USING_NS_CC;
using namespace cocos2d::extension;

class CalcHistoryScene : public LayerColor, public TableViewDataSource, public TableViewDelegate
{
public:
	static Scene* createScene();
	virtual bool init();
	CREATE_FUNC(CalcHistoryScene);
	void addBackButton();
	void onBackButtonCallback(Ref* pSender, Control::EventType event);
	void calcLineHeight();//to auto adjust the cell height

	virtual Size tableCellSizeForIndex(TableView* table, ssize_t idx);      //根据idx来设置每项cell的尺寸大小
	virtual TableViewCell* tableCellAtIndex(TableView* table, ssize_t idx); //获取编号为idx的cell
	virtual ssize_t numberOfCellsInTableView(TableView* table);               //一共多少项cell

	virtual void tableCellTouched(TableView* table, TableViewCell* cell);        //某项cell被点击时回调函数
	virtual void scrollViewDidScroll(cocos2d::extension::ScrollView* view);        //滚动时回调函数
	virtual void scrollViewDidZoom(cocos2d::extension::ScrollView* view);          //放缩时回调函数
private:
	Size m_visibleSize;
	Vec2 m_visibleOrigin;
	float m_calcButtonWidth;
	float m_calcButtonHeight;
	float m_tableCellDefaultHeight = 30.0;
	float m_labelHeight[50];

	ControlButton* m_backButton;
	vector<unordered_map<string, sqlData>> m_historyData;
};





#endif