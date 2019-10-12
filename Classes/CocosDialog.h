#ifndef __CocosDialog_H__
#define __CocosDialog_H__

#include "cocos2d.h"
#include "cocos-ext.h"

USING_NS_CC;
USING_NS_CC_EXT;

class CocosDialog :public LayerColor {

public:

	CocosDialog();
	~CocosDialog();
	virtual bool init();
	CREATE_FUNC(CocosDialog);
	static CocosDialog* create(const char* backgroudImage);
	bool onTouchShieldLayerBegan(Touch* touch, Event* event);
	bool onTouchBegan(Touch* touch, Event* event);
	void onTouchMoved(Touch* touch, Event* event);
	void onTouchEnded(Touch* touch, Event* event);
	void setTitle(const char* title);
	void setContentText(const char* text);
	void setCallBackFunc(Ref* target, SEL_CallFuncN callfun);
	bool addButton(const char* normalImage, const char* selectedImage, const char* title, int tag = 0);
	virtual void onEnter();
	virtual void onExit();
	void backgroundFinish();

private:
	Size m_visibleSize;
	Vec2 m_visibleOrigin;
	float m_textHeight;
	Size m_dialogContentSize;
	void buttonCallBack(Ref* pSender);
	Ref* m_callbackListener;
	SEL_CallFuncN m_callback;
	CC_SYNTHESIZE_RETAIN(Menu*, m__pMenu, MenuButton);
	CC_SYNTHESIZE_RETAIN(ui::Scale9Sprite*, m__s9BackGround, Sprite9BackGround);
	CC_SYNTHESIZE_RETAIN(ui::Scale9Sprite*, m__s9BackGroundShadow, Sprite9BackGroundShadow);
	CC_SYNTHESIZE_RETAIN(Label*, m__ltTitle, LabelTitle);
	CC_SYNTHESIZE_RETAIN(Label*, m__ltContentText, LabelContentText);

};

#endif#
