#ifndef __CALC_HISTORY_EDIT_SCENE_H__
#define __CALC_HISTORY_EDIT_SCENE_H__

#include "cocos2d.h"
#include "cocos-ext.h"

#include "CocosToast.h"
#include "ConstUtil.h"
#include "FuncUtil.h"
#include "CalcHistoryDB.h"

USING_NS_CC;
using namespace cocos2d::extension;

class CalcHistoryEditScene : public LayerColor, public TableViewDataSource, public TableViewDelegate, public ui::EditBoxDelegate
{
public:
	static Scene* createScene();
	virtual bool init();
	virtual void onEnter();
	CREATE_FUNC(CalcHistoryEditScene);
	void addTopMenu();
	void addBackButton();
	void onBackButtonCallback(Ref* pSender, Control::EventType event);
	void calcLineHeight();//to auto adjust the cell height

	virtual Size tableCellSizeForIndex(TableView* table, ssize_t idx);      //根据idx来设置每项cell的尺寸大小
	virtual TableViewCell* tableCellAtIndex(TableView* table, ssize_t idx); //获取编号为idx的cell
	virtual ssize_t numberOfCellsInTableView(TableView* table);               //一共多少项cell

	virtual void tableCellTouched(TableView* table, TableViewCell* cell);        //某项cell被点击时回调函数
	virtual void scrollViewDidScroll(cocos2d::extension::ScrollView* view);        //滚动时回调函数
	virtual void scrollViewDidZoom(cocos2d::extension::ScrollView* view);         //放缩时回调函数

	virtual void editBoxEditingDidBegin(ui::EditBox* editBox);
	virtual void editBoxEditingDidEnd(ui::EditBox* editBox);
	virtual void editBoxTextChanged(ui::EditBox* editBox, const std::string &text);
	virtual void editBoxReturn(ui::EditBox *editBox);
private:
	Size m_visibleSize;
	Vec2 m_visibleOrigin;

	float m_topMenuHeight;

	string m_oldDescription;
	TableView* m_tableView;
	ControlButton* m_backButton;
	float m_labelHeight[10] = { 0.0f };
	unordered_map<string, sqlData> m_queryData;
	CC_SYNTHESIZE(long, m__dbId, DbId);
};

#endif
