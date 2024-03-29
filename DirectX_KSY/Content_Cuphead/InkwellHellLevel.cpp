#include "PreCompile.h"
#include "InkwellHellLevel.h"

#include "OverWorldLevel.h"
#include "Portal.h"
#include "OverWorldCharacter.h"
#include "TitleCard_Fader.h"
#include "TitleCard_KingDice.h"

InkwellHellLevel::InkwellHellLevel()
{
}

InkwellHellLevel::~InkwellHellLevel()
{
}

void InkwellHellLevel::Start()
{
	ContentLevel::Start();

	// Sprite Load
	// Background
	if (nullptr == GameEngineSprite::Find("InkwellHell.png"))
	{
		GameEngineDirectory Dir;
		Dir.MoveParentToExistsChild("GameEngineResources");
		Dir.MoveChild("ContentsResources\\Texture\\OverWorld\\OverWorldMap\\Inkwell_Hell");

		// Background
		GameEngineTexture::Load(Dir.GetStringPath() + "\\InkwellHell.png");
		GameEngineSprite::CreateSingle("InkwellHell.png");

		// Pixel Background
		GameEngineTexture::Load(Dir.GetStringPath() + "\\InkwellHell_pixel.png");
		GameEngineSprite::CreateSingle("InkwellHell_pixel.png");

		// Background Upper
		GameEngineTexture::Load(Dir.GetStringPath() + "\\InkwellHell_Upper.png");
		GameEngineSprite::CreateSingle("InkwellHell_Upper.png");
	}

	// Frontground
	if (nullptr == GameEngineSprite::Find("world4_fg_cave_near.png"))
	{
		GameEngineDirectory Dir;
		Dir.MoveParentToExistsChild("GameEngineResources");
		Dir.MoveChild("ContentsResources\\Texture\\OverWorld\\OverWorldMap\\Inkwell_Hell\\Frontground");

		// Near
		GameEngineTexture::Load(Dir.GetStringPath() + "\\world4_fg_cave_near.png");
		GameEngineSprite::CreateSingle("world4_fg_cave_near.png");

		// Far
		GameEngineTexture::Load(Dir.GetStringPath() + "\\world4_fg_cave_far.png");
		GameEngineSprite::CreateSingle("world4_fg_cave_far.png");
	}
}


void InkwellHellLevel::LevelStart(GameEngineLevel* _PrevLevel)
{
	ContentLevel::LevelStart(_PrevLevel);

	GetMainCamera()->Transform.SetWorldPosition({ 1000, -1550 });

	// Background
	if (nullptr == CurLevelBackground)
	{
		CurLevelBackground = CreateActor<ContentBackground>();
		CurLevelBackground->BackgroundInit("InkwellHell.png");

		CurLevelPixelBackground = CreateActor<ContentBackground>();
		CurLevelPixelBackground->PixelBackgroundInit("InkwellHell_pixel.png");
		//CurLevelPixelBackground->GetRenderer()->On();

		CurLevelUpperBackground = CreateActor<ContentBackground>();
		CurLevelUpperBackground->UpperBackgroundInit("InkwellHell_Upper.png");
	}

	// Entrance To Inkwell Isle
	if (nullptr == PortalToInkwell)
	{
		PortalToInkwell = CreateActor<Portal>();
		PortalToInkwell->SetPortalScale({ 500, 500 });
		PortalToInkwell->Transform.SetLocalPosition({ 900, -1600 });
		PortalToInkwell->SetPortalValue(PortalValue::Inkwell_Isle);
		PortalToInkwell->GetRenderer()->Off();
	}

	// Entrance To Boss Stage
	if (nullptr == PortalToBossStage)
	{
		PortalToBossStage = CreateActor<Portal>();
		PortalToBossStage->Transform.SetLocalPosition({ 1600, -1027 });
		PortalToBossStage->SetPortalValue(PortalValue::BossStage);
		PortalToBossStage->GetRenderer()->Off();
	}

	// Player
	if (nullptr == OverWorldPlayer)
	{
		OverWorldPlayer = CreateActor<OverWorldCharacter>();
		OverWorldPlayer->Transform.SetLocalPosition({ 1025, -1500 });
	}
	OverWorldPlayer->SetIsCameraLerp(true);

	// Frontground
	std::shared_ptr<ContentActor> CaveFar = CreateActor<ContentActor>();
	CaveFar->SpriteInit("world4_fg_cave_far.png", { 1060, -1500 }, RenderOrder::FrontgroundObject);

	std::shared_ptr<ContentActor> CaveNear = CreateActor<ContentActor>();
	CaveNear->SpriteInit("world4_fg_cave_near.png", { 1580, -1160 }, RenderOrder::FrontgroundObject2);


	// Fade In
	float4 PlayerPos = OverWorldPlayer->Transform.GetLocalPosition();
	GetMainCamera()->Transform.SetLocalPosition(PlayerPos);
	float4 WindowScale = GameEngineCore::MainWindow.GetScale();
	float4 FadePos = { PlayerPos.X - WindowScale.ihX(), PlayerPos.Y + WindowScale.ihY() };

	FadeIn->Transform.SetLocalPosition(FadePos);

	// Fade Out
	if (nullptr == FadeOut)
	{
		FadeOut = CreateActor<FadeAnimation>();
		FadeOut->SetFade("Out");
		FadeOut->Off();
	}

	// Title Card - Fader
	if (nullptr == Fader)
	{
		Fader = CreateActor<TitleCard_Fader>();
		Fader->Off();
	}

	// Title Card - King Dice
	if (nullptr == TitleCard)
	{
		TitleCard = CreateActor<TitleCard_KingDice>();
		TitleCard->Off();
	}

	// Initialize
	NextLevel = nullptr;
}

void InkwellHellLevel::Update(float _Delta)
{
	ContentLevel::Update(_Delta);
	CheckPortal();
	CheckBossStageEnter();
	CheckLevelChange();
}

void InkwellHellLevel::LevelEnd(GameEngineLevel* _NextLevel)
{
	ContentLevel::LevelEnd(_NextLevel);

	if (nullptr != OverWorldPlayer)
	{
		OverWorldPlayer->Death();
		OverWorldPlayer = nullptr;
	}

	if (nullptr != CurLevelBackground)
	{
		CurLevelBackground = nullptr;
		CurLevelPixelBackground = nullptr;
		CurLevelUpperBackground = nullptr;
	}

	if (nullptr != PortalToInkwell)
	{
		PortalToInkwell = nullptr;
	}

	if (nullptr != PortalToBossStage)
	{
		PortalToBossStage = nullptr;
	}

	if (nullptr != FadeOut)
	{
		FadeOut->Death();
		FadeOut = nullptr;
	}

	// Title Card - Fader
	if (nullptr != Fader)
	{
		Fader->Death();
		Fader = nullptr;
	}

	// Title Card - King Dice
	if (nullptr != TitleCard)
	{
		TitleCard->Death();
		TitleCard = nullptr;
	}
}

void InkwellHellLevel::CheckPortal()
{
	// Boss Stage
	if (true == PortalToBossStage->GetIsPortalOn())
	{
		OverWorldPlayer->SetIsCameraLerp(false);

		ShowTitleCard();
	}

	// Inkwell Island (Overworld)
	if (true == PortalToInkwell->GetIsPortalOn())
	{
		OverWorldPlayer->SetIsCameraLerp(false);
		FadeOut->On();
		float4 WindowScale = GameEngineCore::MainWindow.GetScale();
		float4 CameraPos = GetMainCamera()->Transform.GetLocalPosition();
		float4 FadePos = { CameraPos.X - WindowScale.ihX(), CameraPos.Y + WindowScale.ihY() };
		FadeOut->Transform.SetLocalPosition(FadePos);

		NextLevel = PortalToInkwell;
		NextLevel->SetPortalValue(PortalValue::Inkwell_Isle);
	}
}

void InkwellHellLevel::CheckLevelChange()
{
	if (true == FadeOut->IsCurAnimationEnd())
	{
		NextLevel->LevelChange();
	}
}

void InkwellHellLevel::ShowTitleCard()
{
	if (false == IsTitleCardOn)
	{
		IsTitleCardOn = true;

		float4 WindowScale = GameEngineCore::MainWindow.GetScale();
		float4 CameraPos = GetMainCamera()->Transform.GetLocalPosition();
		float4 FaderPos = { CameraPos.X - WindowScale.ihX(), CameraPos.Y + WindowScale.ihY() };

		Fader->On();
		Fader->Transform.SetLocalPosition(FaderPos);

		TitleCard->On();
		TitleCard->Transform.SetLocalPosition(FaderPos);
	}
}

void InkwellHellLevel::CheckBossStageEnter()
{
	if (true == IsTitleCardOn)
	{
		if (true == GameEngineInput::IsDown(VK_SPACE, this) 
			|| true == GameEngineInput::IsDown('Z', this) 
			|| true == GameEngineInput::IsDown(VK_RETURN, this))
		{
			float4 WindowScale = GameEngineCore::MainWindow.GetScale();
			float4 CameraPos = GetMainCamera()->Transform.GetLocalPosition();
			float4 FadePos = { CameraPos.X - WindowScale.ihX(), CameraPos.Y + WindowScale.ihY() };

			FadeOut->On();
			FadeOut->Transform.SetLocalPosition(FadePos);

			NextLevel = PortalToBossStage;
			NextLevel->SetPortalValue(PortalValue::BossStage);
		}
	}
}