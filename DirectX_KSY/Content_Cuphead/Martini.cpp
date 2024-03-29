#include "PreCompile.h"
#include "Martini.h"
#include "Attack_Olive.h"
#include "BossExplosion.h"

Martini::Martini()
{
}

Martini::~Martini()
{
}


void Martini::Start()
{
	ContentActor::Start();

	// Create Sprite Folder
	if (nullptr == GameEngineSprite::Find("Martini_Idle"))
	{
		GameEngineDirectory Dir;
		Dir.MoveParentToExistsChild("GameEngineResources");
		Dir.MoveChild("ContentsResources\\Texture\\Stage\\TipsyTroop\\Boss\\Martini");

		std::vector<GameEngineDirectory> Directorys = Dir.GetAllDirectory();

		for (size_t i = 0; i < Directorys.size(); i++)
		{
			GameEngineDirectory& Dir = Directorys[i];
			GameEngineSprite::CreateFolder(Dir.GetStringPath());
		}
	}

	// Create Animation
	MainRenderer = CreateComponent<GameEngineSpriteRenderer>();
	MainRenderer->CreateAnimation("Martini_Idle", "Martini_Idle", 0.05f);
	MainRenderer->CreateAnimation("Martini_Attack", "Martini_Attack", 0.04f);
	MainRenderer->CreateAnimation("Martini_Death", "Martini_Death", 0.05f);
	MainRenderer->ChangeAnimation("Martini_Idle");
	MainRenderer->AutoSpriteSizeOn();
	MainRenderer->SetPivotType(PivotType::Bottom);
	Transform.SetLocalPosition({ 850 , -700 });


	// Create Collision
	BossCollision = CreateComponent<GameEngineCollision>(CollisionOrder::Boss);
	std::shared_ptr<GameEngineSprite> Texture = GameEngineSprite::Find("Martini_Idle");
	float4 Scale = Texture->GetSpriteData(0).GetScale();
	Scale -= { 80, 260 };
	BossCollision->SetCollisionType(ColType::AABBBOX2D);
	BossCollision->Transform.SetLocalScale(Scale);
	BossCollision->Transform.SetLocalPosition({ 0, Scale.hY() + 200.0f });


	ChangeState(MartiniState::Idle);
	HP = 20;
}

void Martini::Update(float _Delta)
{
	ContentActor::Update(_Delta);

	StateUpdate(_Delta);
	DeathCheck();
}

void Martini::ChangeState(MartiniState _State)
{
	if (State != _State)
	{
		switch (_State)
		{
		case MartiniState::None:
			break;
		case MartiniState::Idle:
			IdleStart();
			break;
		case MartiniState::Attack:
			AttackStart();
			break;
		case MartiniState::Death:
			DeathStart();
			break;
		default:
			break;
		}
	}

	State = _State;
}

void Martini::StateUpdate(float _Delta)
{
	switch (State)
	{
	case MartiniState::None:
		break;
	case MartiniState::Idle:
		return IdleUpdate(_Delta);
	case MartiniState::Attack:
		return AttackUpdate(_Delta);
	case MartiniState::Death:
		return DeathUpdate(_Delta);
	default:
		break;
	}
}

void Martini::ChangeAnimationState(const std::string& _StateName)
{
	std::string AnimationName = "Martini_";
	AnimationName += _StateName;

	CurState = _StateName;
	MainRenderer->ChangeAnimation(AnimationName);
}

void Martini::IdleStart()
{
	ChangeAnimationState("Idle");

	if (false == IsOliveOn)
	{
		// 첫 소환
		IdleTimer = 5.0f;
	}

	else
	{
		// 이후 소환
		IdleTimer = 8.0f;
	}
}

void Martini::IdleUpdate(float _Delta)
{
	IdleTimer -= _Delta;
	if (IdleTimer < 0.0f)
	{
		ChangeState(MartiniState::Attack);
	}
}

void Martini::AttackStart()
{
	ChangeAnimationState("Attack");
	AttackTimer = 1.0f;
}

void Martini::AttackUpdate(float _Delta)
{
	if (true == MainRenderer->IsCurAnimationEnd())
	{
		ChangeState(MartiniState::Idle);
		return;
	}

	if (AttackTimer < 0.0f )
	{
		CreateOliveDevil();
		AttackTimer = 1.0f;
	}
	AttackTimer -= _Delta;

}

void Martini::DeathStart()
{
	ChangeAnimationState("Death");
}

void Martini::DeathUpdate(float _Delta)
{

}

void Martini::CreateOliveDevil()
{
	std::shared_ptr<Attack_Olive> OliveDevil = GetLevel()->CreateActor<Attack_Olive>();
	float4 Pos = { 844.0f, -110.0f };
	OliveDevil->Transform.SetLocalPosition(Pos);
}

void Martini::DeathCheck()
{
	if (false == IsDeath)
	{
		return;
	}

	BossCollision->Off();
	ChangeState(MartiniState::Death);
}

void Martini::CreateDeathEffect()
{
	std::shared_ptr<BossExplosion> DeathEffect = GetLevel()->CreateActor<BossExplosion>();
	float4 BossPos = Transform.GetLocalPosition();
	BossPos += { 0.0f, 200.0f };
	DeathEffect->SetPos(BossPos);
}