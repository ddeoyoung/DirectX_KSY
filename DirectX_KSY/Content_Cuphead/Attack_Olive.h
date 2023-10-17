#pragma once
#include <GameEngineCore\GameEngineActor.h>

enum class OliveState
{
	None,
	Appear,
	Idle,
	Attack,
	Death,
};

// ���� :
class Attack_Olive : public ContentActor
{
public:
	// constrcuter destructer
	Attack_Olive();
	~Attack_Olive();

	// delete Function
	Attack_Olive(const Attack_Olive& _Other) = delete;
	Attack_Olive(Attack_Olive&& _Other) noexcept = delete;
	Attack_Olive& operator=(const Attack_Olive& _Other) = delete;
	Attack_Olive& operator=(Attack_Olive&& _Other) noexcept = delete;

protected:
	void ChangeState(OliveState _State);
	void StateUpdate(float _Delta);
	void ChangeAnimationState(const std::string& _StateName);

	void AppearStart();
	void AppearUpdate(float _Delta);

	void IdleStart();
	void IdleUpdate(float _Delta);

	void AttackStart();
	void AttackUpdate(float _Delta);

	void DeathStart();
	void DeathUpdate(float _Delta);

protected:
	void Start() override;
	void Update(float _Delta) override;

	std::shared_ptr<class GameEngineSpriteRenderer> MainRenderer = nullptr;
	std::shared_ptr<class GameEngineCollision> AttackCollision = nullptr;

	std::string CurState = "";
	OliveState State = OliveState::None;

private:

};

