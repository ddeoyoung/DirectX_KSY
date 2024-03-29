#pragma once
#include "BaseMonster.h"

enum class MartiniState
{
	None,
	Idle,
	Attack,
	Death,
};

// ���� :
class Martini : public BaseMonster
{
public:
	// constrcuter destructer
	Martini();
	~Martini();

	// delete Function
	Martini(const Martini& _Other) = delete;
	Martini(Martini&& _Other) noexcept = delete;
	Martini& operator=(const Martini& _Other) = delete;
	Martini& operator=(Martini&& _Other) noexcept = delete;

	void CreateDeathEffect();

protected:
	void ChangeState(MartiniState _State);
	void StateUpdate(float _Delta);
	void ChangeAnimationState(const std::string& _StateName);

	void IdleStart();
	void IdleUpdate(float _Delta);

	void AttackStart();
	void AttackUpdate(float _Delta);

	void DeathStart();
	void DeathUpdate(float _Delta);
	
protected:
	void Start() override;
	void Update(float _Delta) override;

	void CreateOliveDevil();
	void DeathCheck();

	std::string CurState = "";
	MartiniState State = MartiniState::None;

	std::shared_ptr<class GameEngineSpriteRenderer> MainRenderer = nullptr;
	std::shared_ptr<class GameEngineCollision> BossCollision = nullptr;

private:
	bool IsOliveOn = false;
	float IdleTimer = 0.0f;
	float AttackTimer = 0.0f;
};

