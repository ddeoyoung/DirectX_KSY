#include "PreCompile.h"
#include "BaseWeapon.h"
#include "BaseCharacter.h"

#include "Target.h"
#include "BaseMonster.h"

BaseWeapon::BaseWeapon()
{
}

BaseWeapon::~BaseWeapon()
{
}

void BaseWeapon::Start()
{
	ContentActor::Start();
}

void BaseWeapon::Update(float _Delta)
{
	ContentActor::Update(_Delta);

	StateUpdate(_Delta);
}

void BaseWeapon::ChangeState(AttackState _State)
{
	if (_State != State)
	{
		switch (_State)
		{
		case AttackState::None:
			break;
		case AttackState::Spark:
			SparkStart();
			break;
		case AttackState::Attack:
			AttackStart();
			break;
		case AttackState::Death:
			DeathStart();
			break;
		default:
			break;
		}
	}

	State = _State;
}

void BaseWeapon::StateUpdate(float _Delta)
{
	switch (State)
	{
	case AttackState::None:
		break;
	case AttackState::Spark:
		return SparkUpdate(_Delta);
	case AttackState::Attack:
		return AttackUpdate(_Delta);
	case AttackState::Death:
		return DeathUpdate(_Delta);
	default:
		break;
	}
}

void BaseWeapon::ChangeAnimationState(const std::string& _StateName) {}

void BaseWeapon::AttackDirCheck()
{
	switch (AttDir)
	{
	case AttackDir::None:
		break;
	case AttackDir::Left_Up:
		Transform.SetLocalScale({ -1.0f, 1.0f });
		Transform.SetLocalRotation({ 0.0f, 0.0f, -90.0f });
		break;
	case AttackDir::Left_Diagonal_Up:
		Transform.SetLocalScale({ -1.0f, -1.0f });
		Transform.SetLocalRotation({ 0.0f, 0.0f, -45.0f });
		break;
	case AttackDir::Left_Straight:
		Transform.SetLocalScale({ -1.0f, 1.0f });
		//Transform.SetLocalRotation({ 0.0f, 0.0f, 0.0f });
		break;
	case AttackDir::Left_Diagonal_Down:
		Transform.SetLocalScale({ -1.0f, 1.0f });
		Transform.SetLocalRotation({ 0.0f, 0.0f, 225.0f });
		break;
	case AttackDir::Left_Down:
		Transform.SetLocalScale({ -1.0f, 1.0f });
		Transform.SetLocalRotation({ 0.0f, 0.0f, 270.0f });
		break;
	case AttackDir::Right_Up:
		Transform.SetLocalScale({ 1.0f, 1.0f });
		Transform.SetLocalRotation({ 0.0f, 0.0f, 90.0f });
		break;
	case AttackDir::Right_Diagonal_Up:
		Transform.SetLocalScale({ 1.0f, 1.0f });
		Transform.SetLocalRotation({ 0.0f, 0.0f, 45.0f });
		break;
	case AttackDir::Right_Straight:
		Transform.SetLocalScale({ 1.0f, 1.0f });
		//Transform.SetLocalRotation({ 0.0f, 0.0f, 0.0f });
		break;
	case AttackDir::Right_Diagonal_Down:
		Transform.SetLocalScale({ 1.0f, 1.0f });
		Transform.SetLocalRotation({ 0.0f, 0.0f, -45.0f });
		break;
	case AttackDir::Right_Down:
		Transform.SetLocalScale({ 1.0f, 1.0f });
		Transform.SetLocalRotation({ 0.0f, 0.0f, -90.0f });
		break;
	default:
		break;
	}
}

void BaseWeapon::SetAttackDirAndPos(AttackDir _AttDir, float4 _Pos, bool _IsMove)
{
	AttDir = _AttDir;
	PlayerPos = _Pos;

	float4 AttackPos = PlayerPos;

	// Run Normal
	if (false == _IsMove)
	{
		switch (AttDir)
		{
		case AttackDir::None:
			break;
		case AttackDir::Left_Up:
			AttackPos += ATTACK_LEFT_UP;
			break;
		case AttackDir::Left_Diagonal_Up:
			AttackPos += ATTACK_LEFT_DIAGONAL_UP;
			break;
		case AttackDir::Left_Straight:
			AttackPos += ATTACK_LEFT;
			break;
		case AttackDir::Left_Diagonal_Down:
			AttackPos += ATTACK_LEFT_DIAGONAL_DOWN;
			break;
		case AttackDir::Left_Down:
			AttackPos += ATTACK_LEFT_DOWN;
			break;
		case AttackDir::Right_Up:
			AttackPos += ATTACK_RIGHT_UP;
			break;
		case AttackDir::Right_Diagonal_Up:
			AttackPos += ATTACK_RIGHT_DIAGONAL_UP;
			break;
		case AttackDir::Right_Straight:
			AttackPos += ATTACK_RIGHT;
			break;
		case AttackDir::Right_Diagonal_Down:
			AttackPos += ATTACK_RIGHT_DIAGONAL_DOWN;
			break;
		case AttackDir::Right_Down:
			AttackPos += ATTACK_RIGHT_DOWN;
			break;
		default:
			break;
		}
	}
	
	// Run Shoot
	if (true == _IsMove)
	{
		switch (AttDir)
		{
		case AttackDir::Left_Diagonal_Up:
			AttackPos += RUN_ATTACK_LEFT_DIAGONAL_UP;
			break;
		case AttackDir::Left_Straight:
			AttackPos += RUN_ATTACK_LEFT;
			break;
		case AttackDir::Right_Diagonal_Up:
			AttackPos += RUN_ATTACK_RIGHT_DIAGONAL_UP;
			break;
		case AttackDir::Right_Straight:
			AttackPos += RUN_ATTACK_RIGHT;
			break;
		default:
			break;
		}
	}

	Transform.SetLocalPosition(AttackPos);
}

void BaseWeapon::HitCheck()
{
	if (nullptr == AttackCollision)
	{
		return;
	}

	// Shoot Object (Tutorial Target)
	AttackCollision->Collision(CollisionOrder::ShootObject, [=](std::vector<GameEngineCollision*>_GroupOrder)
		{
			for (size_t i = 0; i < _GroupOrder.size(); i++)
			{
				Target* Other = static_cast<Target*>(_GroupOrder[i]->GetActor());

				if (nullptr != Other)
				{
					Other->HPMinus();
				}
			}

			ChangeState(AttackState::Death);
			return;
		});

	// Monster
	AttackCollision->Collision(CollisionOrder::Monster, [=](std::vector<GameEngineCollision*> _GroupOrder)
		{
			for (size_t i = 0; i < _GroupOrder.size(); i++)
			{
				BaseMonster* Other = dynamic_cast<BaseMonster*>(_GroupOrder[i]->GetActor());
				if (nullptr != Other)
				{
					Other->HPMinus();
				}
			}

			ChangeState(AttackState::Death);
			return;
		});

	// Boss
	AttackCollision->Collision(CollisionOrder::Boss, [=](std::vector<GameEngineCollision*> _GroupOrder)
		{
			for (size_t i = 0; i < _GroupOrder.size(); i++)
			{
				BaseMonster* Other = dynamic_cast<BaseMonster*>(_GroupOrder[i]->GetActor());
				if (nullptr != Other)
				{
					Other->HPMinus();
				}
			}

			ChangeState(AttackState::Death);
			return;
		});
}