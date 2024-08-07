#pragma once

// ���� :
class Target : public ContentActor
{
public:
	// constrcuter destructer
	Target();
	~Target();

	// delete Function
	Target(const Target& _Other) = delete;
	Target(Target&& _Other) noexcept = delete;
	Target& operator=(const Target& _Other) = delete;
	Target& operator=(Target&& _Other) noexcept = delete;

	void HPMinus();

protected:
	void Start() override;
	void Update(float _Delta) override;

private:
	void DeathCheck();
	void CreateExplosionEffect();

	std::shared_ptr<class GameEngineSpriteRenderer> TargetRenderer = nullptr;
	std::shared_ptr<class GameEngineSpriteRenderer> TopperRenderer = nullptr;
	std::shared_ptr<class GameEngineCollision> TargetCollision = nullptr;

	int HP = 10;
	bool IsHit = false;
	bool IsDeath = false;
};

