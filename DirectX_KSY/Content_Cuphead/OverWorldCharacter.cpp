#include "PreCompile.h"
#include "OverWorldCharacter.h"

#include "ContentLevel.h"

OverWorldCharacter::OverWorldCharacter()
{
}

OverWorldCharacter::~OverWorldCharacter()
{
}

void OverWorldCharacter::Start()
{
	ContentActor::Start();


	// Create Sprite Folder
	if (nullptr == GameEngineSprite::Find("OverWorld_Cuphead_Idle_Right"))
	{
		GameEngineDirectory Dir;
		Dir.MoveParentToExistsChild("GameEngineResources");
		Dir.MoveChild("ContentsResources\\Texture\\OverWorld\\OverWorld_Cuphead");

		std::vector<GameEngineDirectory> Directorys = Dir.GetAllDirectory();

		for (size_t i = 0; i < Directorys.size(); i++)
		{
			GameEngineDirectory& Dir = Directorys[i];
			GameEngineSprite::CreateFolder(Dir.GetStringPath());
		}
	}

	// Create Animation
	MainRenderer = CreateComponent<GameEngineSpriteRenderer>(RenderOrder::Play);

	// Idle
	MainRenderer->CreateAnimation("OverWorld_Cuphead_Idle_Right", "OverWorld_Cuphead_Idle_Right", 0.05f);
	MainRenderer->CreateAnimation("OverWorld_Cuphead_Idle_Up", "OverWorld_Cuphead_Idle_Up", 0.05f);
	MainRenderer->CreateAnimation("OverWorld_Cuphead_Idle_Down", "OverWorld_Cuphead_Idle_Down", 0.05f);
	MainRenderer->CreateAnimation("OverWorld_Cuphead_Idle_Diagonal_Up", "OverWorld_Cuphead_Idle_Diagonal_Up", 0.05f);
	MainRenderer->CreateAnimation("OverWorld_Cuphead_Idle_Diagonal_Down", "OverWorld_Cuphead_Idle_Diagonal_Down", 0.05f);

	// Walk
	MainRenderer->CreateAnimation("OverWorld_Cuphead_Walk_Right", "OverWorld_Cuphead_Walk_Right", 0.05f);
	MainRenderer->CreateAnimation("OverWorld_Cuphead_Walk_Up", "OverWorld_Cuphead_Walk_Up", 0.05f);
	MainRenderer->CreateAnimation("OverWorld_Cuphead_Walk_Down", "OverWorld_Cuphead_Walk_Down", 0.05f);
	MainRenderer->CreateAnimation("OverWorld_Cuphead_Walk_Diagonal_Up", "OverWorld_Cuphead_Walk_Diagonal_Up", 0.05f);
	MainRenderer->CreateAnimation("OverWorld_Cuphead_Walk_Diagonal_Down", "OverWorld_Cuphead_Walk_Diagonal_Down", 0.05f);

	MainRenderer->ChangeAnimation("OverWorld_Cuphead_Idle_Right");
	MainRenderer->AutoSpriteSizeOn();
	MainRenderer->SetRenderOrder(RenderOrder::Play);

	PlayerCollision = CreateComponent<GameEngineCollision>(CollisionOrder::Player);
	PlayerCollision->SetCollisionType(ColType::AABBBOX2D);
	std::shared_ptr<GameEngineSprite> Texture = GameEngineSprite::Find("OverWorld_Cuphead_Idle_Right");
	float4 Scale = Texture->GetSpriteData(0).GetScale();
	PlayerCollision->Transform.SetLocalScale(Scale);
	PlayerCollision->Transform.SetLocalPosition({ 0, 0 });

	ChangeState(OverWorldCharacterState::Idle);
}

void OverWorldCharacter::Update(float _Delta)
{
	ContentActor::Update(_Delta);

	StateUpdate(_Delta);
	DirCheck();
	LerpCamera(_Delta);
}

void OverWorldCharacter::LevelStart(GameEngineLevel* _PrevLevel)
{
	ContentActor::LevelStart(_PrevLevel);
}

void OverWorldCharacter::LevelEnd(GameEngineLevel* _NextLevel)
{
	ContentActor::LevelEnd(_NextLevel);
}

void OverWorldCharacter::ChangeState(OverWorldCharacterState _State)
{
	if (_State != State)
	{
		switch (_State)
		{
		case OverWorldCharacterState::None:
			break;
		case OverWorldCharacterState::Idle:
			IdleStart();
			break;
		case OverWorldCharacterState::Walk:
			WalkStart();
			break;
		case OverWorldCharacterState::Win:
			WinStart();
			break;
		default:
			break;
		}
	}

	State = _State;
}

void OverWorldCharacter::StateUpdate(float _Delta)
{
	switch (State)
	{
	case OverWorldCharacterState::None:
		break;
	case OverWorldCharacterState::Idle:
		IdleUpdate(_Delta);
	case OverWorldCharacterState::Walk:
		WalkUpdate(_Delta);
	case OverWorldCharacterState::Win:
		WinUpdate(_Delta);
	default:
		break;
	}
}

void OverWorldCharacter::ChangeAnimationState(const std::string& _StateName)
{
	std::string AnimationName = "OverWorld_Cuphead_";
	AnimationName += _StateName;

	if (_StateName == "Idle" || _StateName == "Walk")
	{
		switch (Dir)
		{
		case OverWorldCharacterDir::None:
		case OverWorldCharacterDir::Left:
		case OverWorldCharacterDir::Right:
			AnimationName += "_Right";
			break;
		case OverWorldCharacterDir::Up:
			AnimationName += "_Up";
			break;
		case OverWorldCharacterDir::Down:
			AnimationName += "_Down";
			break;
		case OverWorldCharacterDir::Left_Diagonal_Up:
		case OverWorldCharacterDir::Right_Diagonal_Up:
			AnimationName += "_Diagonal_Up";
			break;
		case OverWorldCharacterDir::Left_Diagonal_Down:
		case OverWorldCharacterDir::Right_Diagonal_Down:
			AnimationName += "_Diagonal_Down";
			break;
		default:
			break;
		}
	}

	CurState = _StateName;
	MainRenderer->ChangeAnimation(AnimationName);
}

void OverWorldCharacter::DirCheck()
{
	OverWorldCharacterDir CheckDir = Dir;

	// Left
	if ((true == GameEngineInput::IsDown(VK_LEFT, this) || true == GameEngineInput::IsPress(VK_LEFT, this))
		&& (true == GameEngineInput::IsUp(VK_UP, this) || true == GameEngineInput::IsFree(VK_UP, this))
		&& (true == GameEngineInput::IsUp(VK_DOWN, this) || true == GameEngineInput::IsFree(VK_DOWN, this)))
	{
		CheckDir = OverWorldCharacterDir::Left;
		Transform.SetLocalScale({ -1.0f, 1.0f });
	}

	// Right
	if ((true == GameEngineInput::IsDown(VK_RIGHT, this) || true == GameEngineInput::IsPress(VK_RIGHT, this))
		&& (true == GameEngineInput::IsUp(VK_UP, this) || true == GameEngineInput::IsFree(VK_UP, this))
		&& (true == GameEngineInput::IsUp(VK_DOWN, this) || true == GameEngineInput::IsFree(VK_DOWN, this)))
	{
		CheckDir = OverWorldCharacterDir::Right;
		Transform.SetLocalScale({ 1.0f, 1.0f });
	}

	// Up
	if ((true == GameEngineInput::IsDown(VK_UP, this) || true == GameEngineInput::IsPress(VK_UP, this))
		&& (true == GameEngineInput::IsUp(VK_LEFT, this) || true == GameEngineInput::IsFree(VK_LEFT, this))
		&& (true == GameEngineInput::IsUp(VK_RIGHT, this) || true == GameEngineInput::IsFree(VK_RIGHT, this)))
	{
		CheckDir = OverWorldCharacterDir::Up;
	}

	// Down
	if ((true == GameEngineInput::IsDown(VK_DOWN, this) || true == GameEngineInput::IsPress(VK_DOWN, this))
		&& (true == GameEngineInput::IsUp(VK_LEFT, this) || true == GameEngineInput::IsFree(VK_LEFT, this))
		&& (true == GameEngineInput::IsUp(VK_RIGHT, this) || true == GameEngineInput::IsFree(VK_RIGHT, this)))
	{
		CheckDir = OverWorldCharacterDir::Down;
	}

	// Left Diagonal Up
	if ((true == GameEngineInput::IsDown(VK_LEFT, this) || true == GameEngineInput::IsPress(VK_LEFT, this))
		&& (true == GameEngineInput::IsDown(VK_UP, this) || true == GameEngineInput::IsPress(VK_UP, this)))
	{
		CheckDir = OverWorldCharacterDir::Left_Diagonal_Up;
		Transform.SetLocalScale({ -1.0f, 1.0f });
	}

	// Right Diagonal Up
	if ((true == GameEngineInput::IsDown(VK_RIGHT, this) || true == GameEngineInput::IsPress(VK_RIGHT, this))
		&& (true == GameEngineInput::IsDown(VK_UP, this) || true == GameEngineInput::IsPress(VK_UP, this)))
	{
		CheckDir = OverWorldCharacterDir::Right_Diagonal_Up;
		Transform.SetLocalScale({ 1.0f, 1.0f });
	}

	// Left Diagonal Down
	if ((true == GameEngineInput::IsDown(VK_LEFT, this) || true == GameEngineInput::IsPress(VK_LEFT, this))
		&& (true == GameEngineInput::IsDown(VK_DOWN, this) || true == GameEngineInput::IsPress(VK_DOWN, this)))
	{
		CheckDir = OverWorldCharacterDir::Left_Diagonal_Down;
		Transform.SetLocalScale({ -1.0f, 1.0f });
	}

	// Right Diagonal Down
	if ((true == GameEngineInput::IsDown(VK_RIGHT, this) || true == GameEngineInput::IsPress(VK_RIGHT, this))
		&& (true == GameEngineInput::IsDown(VK_DOWN, this) || true == GameEngineInput::IsPress(VK_DOWN, this)))
	{
		CheckDir = OverWorldCharacterDir::Right_Diagonal_Down;
		Transform.SetLocalScale({ 1.0f, 1.0f });
	}


	if (CheckDir != Dir)
	{
		Dir = CheckDir;
		ChangeAnimationState(CurState);
	}
}

void OverWorldCharacter::LerpCamera(float _Delta)
{
	float CameraSpeed = 2.5f;
	float4 WindowScale = GameEngineCore::MainWindow.GetScale();
	float4 TextureScale = ContentLevel::CurLevel->GetCurLevelPixelBackground()->GetPixelTextureScale();

	float4 CameraPos = GetLevel()->GetMainCamera()->Transform.GetWorldPosition(); // Start
	float4 PlayerPos = Transform.GetWorldPosition(); // End
	float4 MovePos = float4::LerpClamp(CameraPos, PlayerPos, CameraSpeed * _Delta);

	if (true == IsCameraLerp)
	{
		std::string LevelName = GetLevel()->GetName();
		if (LevelName == "InkwellHellLevel")
		{
			if (MovePos.iX() > 1030 && MovePos.iX() < 2200)
			{
				GetLevel()->GetMainCamera()->Transform.SetWorldPosition(MovePos);
			}
		}

		else
		{
			GetLevel()->GetMainCamera()->Transform.SetWorldPosition(MovePos);
		}
	}
}

GameEngineColor OverWorldCharacter::GetPixelColor(float4 _Pos)
{
	float4 PlayerPos = Transform.GetLocalPosition();
	float4 CheckPos = PlayerPos + _Pos;

	GameEngineColor Color = ContentLevel::CurLevel->GetCurLevelPixelBackground()->GetColor(CheckPos, GameEngineColor::RED);

	return Color;
}

void OverWorldCharacter::PixelCheck(float _Delta)
{
	float4 PlayerPos = Transform.GetLocalPosition();
	float4 CheckPos = float4::ZERO;
	float4 MovePos = float4::ZERO;
	float MoveSpeed = 200.0f;

	if (GameEngineInput::IsPress(VK_LEFT, this))
	{
		MovePos += float4::LEFT * _Delta * MoveSpeed;
		CheckPos = { -20.0f , 0.0f };
	}

	if (GameEngineInput::IsPress(VK_RIGHT, this))
	{
		MovePos += float4::RIGHT * _Delta * MoveSpeed;
		CheckPos = { 20.0f , 0.0f };
	}

	if (GameEngineInput::IsPress(VK_UP, this))
	{
		MovePos += float4::UP * _Delta * MoveSpeed;
		CheckPos = { 0.0f, 40.0f };
	}

	if (GameEngineInput::IsPress(VK_DOWN, this))
	{
		MovePos += float4::DOWN * _Delta * MoveSpeed;
		CheckPos = { 0.0f, -40.0f };
	}

	GameEngineColor Color = GetPixelColor(CheckPos);

	if (GameEngineColor::RED != Color)
	{
		Transform.AddLocalPosition(MovePos);
	}
}