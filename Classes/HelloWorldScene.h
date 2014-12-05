#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"

class HelloWorld: public cocos2d::CCLayer {
public:
	// Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
	virtual bool init();

	// there's no 'id' in cpp, so we recommand to return the exactly class pointer
	static cocos2d::CCScene* scene();

	// a selector callback
	void menuCloseCallback(CCObject* pSender);
	cocos2d::CCSprite* mosquito2;
	cocos2d::CCSprite* mosquito1;
	cocos2d::CCSprite* pneu;

	float vetorDirecao1[2];
	float vetorDirecao[2];
	bool pausar;
	int pontos;
	int velocidade;

	cocos2d::CCLabelTTF* etiquetaDePontos;
	cocos2d::CCLabelTTF* etiquetaDeInformacao;

	void moveMosquito(float dt);
	void calculaColisao();

	// void didAccelerate(cocos2d::CCAcceleration *pAccelerationValue);

	// implement the "static node()" method manually
	CREATE_FUNC (HelloWorld);

	void ccTouchesBegan(cocos2d::CCSet *pTouches, cocos2d::CCEvent *pEvent);

	void ccTouchesEnded(cocos2d::CCSet *pTouches, cocos2d::CCEvent *pEvent);

	void ccTouchesMoved(cocos2d::CCSet *pTouches, cocos2d::CCEvent *pEvent);

};

#endif // __HELLOWORLD_SCENE_H__
