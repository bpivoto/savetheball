#include "HelloWorldScene.h"
#include "SimpleAudioEngine.h"

using namespace cocos2d;
using namespace CocosDenshion;

CCAction *mosquitoVoando1;
CCAction *mosquitoVoando2;
CCAction *pneuComAgua;

CCLabelTTF* labelPontos;
int pontuacao = 0;

CCScene* HelloWorld::scene() {
	CCScene *scene = CCScene::create();

	HelloWorld *layer = HelloWorld::create();

	scene->addChild(layer);

	return scene;
}

bool HelloWorld::init() {
	if (!CCLayer::init()) {
		return false;
	}

	//botao desliga
		CCMenuItemImage *pCloseItem = CCMenuItemImage::create(
				"CloseNormal.png",
				"CloseSelected.png",
				this,
				menu_selector(HelloWorld::menuCloseCallback) );
		pCloseItem->setPosition( ccp(CCDirector::sharedDirector()->getWinSize().width - 20, 20) );

		CCMenu* pMenu = CCMenu::create(pCloseItem, NULL);
		pMenu->setPosition( CCPointZero );
		this->addChild(pMenu, 1);

	//botao desliga

	char stopBuf[32] = { 0 };
	static unsigned int seed = time(NULL);


	CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("img.plist");


	CCArray *mosquitoVoandoArray = CCArray::create();
	mosquitoVoandoArray->addObject(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("mosq01.png"));
	mosquitoVoandoArray->addObject(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("mosq02.png"));
	mosquitoVoandoArray->addObject(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("mosq03.png"));
	CCAnimation *mosquitoVoandoAnimacao = CCAnimation::createWithSpriteFrames(mosquitoVoandoArray,0.15f);
	mosquitoVoando1 = CCRepeatForever::create(CCAnimate::create(mosquitoVoandoAnimacao));
	mosquitoVoando1->retain();
	mosquitoVoando2 = CCRepeatForever::create(CCAnimate::create(mosquitoVoandoAnimacao));
	mosquitoVoando2->retain();

	CCArray *pneuComAguaArray = CCArray::create();
	pneuComAguaArray->addObject(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("pneus01.png"));
	pneuComAguaArray->addObject(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("pneus02.png"));
	pneuComAguaArray->addObject(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("pneus03.png"));
	CCAnimation *pneuComAguaAnimacao = CCAnimation::createWithSpriteFrames(pneuComAguaArray,0.15f);
	pneuComAgua = CCRepeatForever::create(CCAnimate::create(pneuComAguaAnimacao));
	pneuComAgua->retain();

	CCSize size = CCDirector::sharedDirector()->getWinSize();

	velocidade = 200;
	pausar = true;
	pontos=0;

	CCSprite* fundo = CCSprite::create("bgc.png");
	fundo->setScale((size.width/fundo->boundingBox().size.width)*1 );
	fundo->setPosition(ccp(size.width/2,(size.height/2)*1.2 ));
	addChild(fundo);

	mosquito1 = CCSprite::createWithSpriteFrameName("mosq01.png");
	mosquito2 = CCSprite::createWithSpriteFrameName("mosq01.png");
	pneu = CCSprite::createWithSpriteFrameName("pneus01.png");

	mosquito2->setScale((0.15 * size.height) / mosquito2->boundingBox().size.height);
	mosquito1->setScale(
			(0.15 * size.height) / mosquito1->boundingBox().size.height);
	pneu->setScale((0.25 * size.height) / pneu->boundingBox().size.height);

	pneu->setPosition(ccp(size.width / 2, size.height / 2));

	srand(seed);
	seed = rand();

	mosquito2->setPosition(
			ccp((((float) seed) / RAND_MAX) * size.width / 3 + 0.1 * size.width,
					-0.1 * size.height));

	srand(seed);
	seed = rand();
	mosquito1->setPosition(
			ccp(
					(((float) seed) / RAND_MAX) * size.width / 3
							+ (size.width / 3 * 2) - 0.1 * size.width,
					1.1 * size.height));

	mosquito2->runAction(mosquitoVoando2);
	mosquito1->runAction(mosquitoVoando1);
	pneu->runAction(pneuComAgua);

	addChild(pneu);
	addChild(mosquito2);
	addChild(mosquito1);


	/////
	HelloWorld::pontos = 0;

	HelloWorld::etiquetaDePontos = CCLabelTTF::create("Pontos: 0", "Thonburi", 40);
	HelloWorld::etiquetaDeInformacao = CCLabelTTF::create("Toque na tela para jogar!!!","Thonburi", 40);

	HelloWorld::etiquetaDePontos->setPosition(ccp(HelloWorld::etiquetaDePontos->boundingBox().size.width / 2,HelloWorld::etiquetaDePontos->boundingBox().size.height));

	HelloWorld::etiquetaDeInformacao->setPosition(ccp( (size.width/2) , ((size.height/3)*1.25) - HelloWorld::etiquetaDePontos->boundingBox().size.height));

	HelloWorld::etiquetaDePontos->setColor(ccc3(0,0,0));
	HelloWorld::etiquetaDeInformacao->setColor(ccc3(0,0,0));

	addChild(HelloWorld::etiquetaDePontos);

	addChild(HelloWorld::etiquetaDeInformacao);

	//////

	setTouchEnabled(true);

	HelloWorld::vetorDirecao[0] = sqrt(2) / 2;
	HelloWorld::vetorDirecao[1] = sqrt(2) / 2;

	HelloWorld::vetorDirecao1[0] = sqrt(2) / 2 * (-1);
	HelloWorld::vetorDirecao1[1] = sqrt(2) / 2 * (-1);

	schedule(schedule_selector(HelloWorld::moveMosquito));
	schedule(schedule_selector(HelloWorld::calculaColisao));

	return true;
}

void HelloWorld::menuCloseCallback(CCObject* pSender) {


	CCDirector::sharedDirector()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	exit(0);
#endif
}

void HelloWorld::moveMosquito(float dt) {


	if (pausar == false) {
		CCSize size = CCDirector::sharedDirector()->getWinSize();
		char str[15];
		sprintf(str, "Pontos: %i", HelloWorld::pontos);
		HelloWorld::etiquetaDePontos->setString(str);
		HelloWorld::etiquetaDeInformacao->setString("");
		HelloWorld::etiquetaDePontos->setPosition(ccp(HelloWorld::etiquetaDePontos->boundingBox().size.width / 2,HelloWorld::etiquetaDePontos->boundingBox().size.height));


		HelloWorld::mosquito2->setPosition(
				ccp(
						HelloWorld::mosquito2->getPositionX()
								+ velocidade * dt * HelloWorld::vetorDirecao[0],
						HelloWorld::mosquito2->getPositionY()
								+ velocidade * dt
										* HelloWorld::vetorDirecao[1]));

		if ((HelloWorld::mosquito2->getPositionX()
				+ HelloWorld::mosquito2->boundingBox().size.width / 2 >= size.width
				&& HelloWorld::vetorDirecao[0] > 0)
				||

				(HelloWorld::mosquito2->getPositionX()
						- HelloWorld::mosquito2->boundingBox().size.width / 2 <= 0
						&& HelloWorld::vetorDirecao[0] < 0)){

			HelloWorld::vetorDirecao[0] = -HelloWorld::vetorDirecao[0];
		}
		if ((HelloWorld::mosquito2->getPositionY()
				+ HelloWorld::mosquito2->boundingBox().size.height / 2 >= size.height
				&& HelloWorld::vetorDirecao[1] > 0)
				||

				(HelloWorld::mosquito2->getPositionY()
						- HelloWorld::mosquito2->boundingBox().size.height / 2 <= 0
						&& HelloWorld::vetorDirecao[1] < 0))

			HelloWorld::vetorDirecao[1] = -HelloWorld::vetorDirecao[1];

//mosquito1

		HelloWorld::mosquito1->setPosition(
				ccp(
						HelloWorld::mosquito1->getPositionX()
								+ velocidade * dt
										* HelloWorld::vetorDirecao1[0],
						HelloWorld::mosquito1->getPositionY()
								+ velocidade * dt
										* HelloWorld::vetorDirecao1[1]));

		if ((HelloWorld::mosquito1->getPositionX()
				+ HelloWorld::mosquito1->boundingBox().size.width / 2
				>= size.width && HelloWorld::vetorDirecao1[0] > 0)
				||

				(HelloWorld::mosquito1->getPositionX()
						- HelloWorld::mosquito1->boundingBox().size.width / 2
						<= 0 && HelloWorld::vetorDirecao1[0] < 0)){

			HelloWorld::vetorDirecao1[0] = -HelloWorld::vetorDirecao1[0];


		}
		if ((HelloWorld::mosquito1->getPositionY()
				+ HelloWorld::mosquito1->boundingBox().size.height / 2
				>= size.height && HelloWorld::vetorDirecao1[1] > 0)
				||

				(HelloWorld::mosquito1->getPositionY()
						- HelloWorld::mosquito1->boundingBox().size.height / 2
						<= 0 && HelloWorld::vetorDirecao1[1] < 0))

			HelloWorld::vetorDirecao1[1] = -HelloWorld::vetorDirecao1[1];

if (vetorDirecao1[0] > 0 && mosquito1->isFlipX()==false){
		 mosquito1->setFlipX(true);
	 }

if (vetorDirecao1[0] < 0 && mosquito1->isFlipX()==true){
		 mosquito1->setFlipX(false);
	 }

if (vetorDirecao[0] > 0 && mosquito2->isFlipX()==false){
	mosquito2->setFlipX(true);
	 }

if (vetorDirecao[0] < 0 && mosquito2->isFlipX()==true){
	mosquito2->setFlipX(false);
	 }



	}else {
		if (pontos < 1)
		HelloWorld::etiquetaDeInformacao->setString("Toque na tela para jogar!!!");
		else
		HelloWorld::etiquetaDeInformacao->setString("Voce perdeu. Toque para jogar!!!");
	}
}

void HelloWorld::ccTouchesBegan(CCSet *pTouches, CCEvent *pEvent) {
	static unsigned int seed = time(NULL);

	if (pausar == true){
		pausar = false;
		pontos = 0;
		CCSize size = CCDirector::sharedDirector()->getWinSize();
		srand(seed);
		seed = rand();
		mosquito2->setPosition(
				ccp((((float) seed) / RAND_MAX) * size.width / 3 + 0.1 * size.width,
						-0.1 * size.height));

		srand(seed);
		seed = rand();
		mosquito1->setPosition(
				ccp(
						(((float) seed) / RAND_MAX) * size.width / 3
								+ (size.width / 3 * 2) - 0.1 * size.width,
						1.1 * size.height));


	}
	CCPoint ponto =
			(static_cast<CCTouch*>(pTouches->anyObject()))->getLocationInView();
	CCSize size = CCDirector::sharedDirector()->getWinSize();
	ponto.y = CCDirector::sharedDirector()->getWinSize().height - ponto.y;

	srand(seed);

	if (HelloWorld::mosquito2->boundingBox().containsPoint(ponto)) {

		HelloWorld::pontos++;
		CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("tocou.mp3");
		seed = rand();
		if (seed % 4 == 0) {
			HelloWorld::vetorDirecao[0] = -HelloWorld::vetorDirecao[0];
		} else if (seed % 4 == 1) {
			HelloWorld::vetorDirecao[1] = -HelloWorld::vetorDirecao[1];
		} else {
			HelloWorld::vetorDirecao[0] = -HelloWorld::vetorDirecao[0];
			HelloWorld::vetorDirecao[1] = -HelloWorld::vetorDirecao[1];

		}
	}

	if (HelloWorld::mosquito1->boundingBox().containsPoint(ponto)) {
		HelloWorld::pontos++;
		CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("tocou.mp3");
		seed = rand();
		if (seed % 4 == 0) {
			HelloWorld::vetorDirecao1[0] = -HelloWorld::vetorDirecao1[0];
		} else if (seed % 4 == 1) {
			HelloWorld::vetorDirecao1[1] = -HelloWorld::vetorDirecao1[1];
		} else {
			HelloWorld::vetorDirecao1[0] = -HelloWorld::vetorDirecao1[0];
			HelloWorld::vetorDirecao1[1] = -HelloWorld::vetorDirecao1[1];

		}

	}

}

void HelloWorld::ccTouchesEnded(CCSet *pTouches, CCEvent *pEvent) {

}

void HelloWorld::ccTouchesMoved(CCSet *pTouches, CCEvent *pEvent) {

}

void HelloWorld::calculaColisao() {

	static CCSize size = CCDirector::sharedDirector()->getWinSize();

	static int raioMosquito =
			CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(
					"mosq01.png")->getRect().size.width
					> CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(
							"mosq01.png")->getRect().size.height

					?
					CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(
							"mosq01.png")->getRect().size.width
							* ((0.13 * size.height)
									/ CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(
											"mosq01.png")->getRect().size.height)
							* 0.5

							:
					CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(
							"mosq01.png")->getRect().size.height
							* ((0.13 * size.height)
									/ CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(
											"mosq01.png")->getRect().size.height)
							* 0.5;

// colisao dos mosquitos
	static int distanciaMaxima = (raioMosquito + raioMosquito)
			* (raioMosquito + raioMosquito);

	int dist = (HelloWorld::mosquito2->getPosition().x - mosquito1->getPosition().x)
			* (HelloWorld::mosquito2->getPosition().x - mosquito1->getPosition().x)
			+

			(HelloWorld::mosquito2->getPosition().y - mosquito1->getPosition().y)
					* (HelloWorld::mosquito2->getPosition().y
							- mosquito1->getPosition().y);

	if (dist <= distanciaMaxima) {

		static unsigned int seed = time(NULL);
		CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("chocou.mp3");

		srand(seed);
		seed = rand();
		mosquito1->setPosition(
				ccp(
						(((float) seed) / RAND_MAX) * size.width / 3
								+ (size.width / 3 * 2) - 0.1 * size.width,
						1.1 * size.height));
		srand(seed);
		seed = rand();
		mosquito2->setPosition(
				ccp(
						(((float) seed) / RAND_MAX) * size.width / 3
								+ 0.1 * size.width, -0.1 * size.height));
		velocidade = velocidade + 5;
		pontos = pontos + 10;
		srand(seed);
		seed = rand();
		if (seed % 2 == 0) {
			HelloWorld::vetorDirecao1[0] = -HelloWorld::vetorDirecao1[0];
			HelloWorld::vetorDirecao[1] = -HelloWorld::vetorDirecao[1];

		} else if (seed % 2 == 1) {
			HelloWorld::vetorDirecao1[1] = -HelloWorld::vetorDirecao1[1];
			HelloWorld::vetorDirecao[0] = -HelloWorld::vetorDirecao[0];

		} else {
			HelloWorld::vetorDirecao1[0] = -HelloWorld::vetorDirecao1[0];
			HelloWorld::vetorDirecao1[1] = -HelloWorld::vetorDirecao1[1];

			HelloWorld::vetorDirecao[0] = -HelloWorld::vetorDirecao[0];
			HelloWorld::vetorDirecao[1] = -HelloWorld::vetorDirecao[1];

		}



	}

	// colisao do pneu

			static int raioPneu =
					CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(
							"pneus01.png")->getRect().size.width
							> CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(
									"pneus01.png")->getRect().size.height

							?
							CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(
									"pneus01.png")->getRect().size.width
									* ((0.1 * size.height)
											/ CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(
													"pneus01.png")->getRect().size.height)
									* 0.5

									:
							CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(
									"pneus01.png")->getRect().size.height
									* ((0.1 * size.height)
											/ CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(
													"pneus01.png")->getRect().size.height)
									* 0.5;

			distanciaMaxima = (raioMosquito + raioPneu) * (raioMosquito + raioPneu);

			int dist1 = (HelloWorld::pneu->getPosition().x
					- mosquito1->getPosition().x)
					* (HelloWorld::pneu->getPosition().x
							- mosquito1->getPosition().x)
					+

					(HelloWorld::pneu->getPosition().y - mosquito1->getPosition().y)
							* (HelloWorld::pneu->getPosition().y
									- mosquito1->getPosition().y);

			dist = (HelloWorld::pneu->getPosition().x - mosquito2->getPosition().x)
					* (HelloWorld::pneu->getPosition().x - mosquito2->getPosition().x)
					+

					(HelloWorld::pneu->getPosition().y - mosquito2->getPosition().y)
							* (HelloWorld::pneu->getPosition().y
									- mosquito2->getPosition().y);

			if ((dist <= distanciaMaxima || dist1 <= distanciaMaxima) && pausar==false) {
				CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("boo.mp3");

				pausar = true;
				velocidade = 200;
			}


}

