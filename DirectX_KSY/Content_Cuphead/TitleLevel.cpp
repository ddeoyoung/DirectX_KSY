#include "PreCompile.h"
#include "TitleLevel.h"

#include "TitleBackground.h"
#include "Cuphead_And_Mugman.h"
#include "TitleMenuBackground.h"

TitleLevel::TitleLevel() 
{
}

TitleLevel::~TitleLevel() 
{
}

void TitleLevel::Start()
{
	ContentLevel::Start();
}

void TitleLevel::Update(float _Delta)
{
	ContentLevel::Update(_Delta);

	if (GameEngineInput::IsPress('P', this))
	{
		Back->Off();
		CupheadAndMugman->Off();
		MenuBack->On();
	}

	if (GameEngineInput::IsPress('Z', this))
	{
		GameEngineCore::ChangeLevel("IntroLevel");
	}
}


void TitleLevel::LevelStart(GameEngineLevel* _PrevLevel)
{
	ContentLevel::LevelStart(_PrevLevel);

	Back = CreateActor<TitleBackground>();
	CupheadAndMugman = CreateActor<Cuphead_And_Mugman>();
	MenuBack = CreateActor<TitleMenuBackground>();
	MenuBack->Off();
}

void TitleLevel::LevelEnd(GameEngineLevel* _NextLevel)
{
	ContentLevel::LevelEnd(_NextLevel);
}