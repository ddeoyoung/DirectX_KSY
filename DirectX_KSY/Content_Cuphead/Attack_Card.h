#pragma once

// ���� :
class Attack_Card : public ContentActor
{
public:
	// constrcuter destructer
	Attack_Card();
	~Attack_Card();

	// delete Function
	Attack_Card(const Attack_Card& _Other) = delete;
	Attack_Card(Attack_Card&& _Other) noexcept = delete;
	Attack_Card& operator=(const Attack_Card& _Other) = delete;
	Attack_Card& operator=(Attack_Card&& _Other) noexcept = delete;

	void SetCardAttDir(int _AttDir);
	void SetCardType(CardType _CardType);

protected:
	void Start() override;
	void Update(float _Delta) override;

	void MoveUpdate(float _Delta);
	void SetCollisionType();

	std::shared_ptr<class GameEngineSpriteRenderer> MainRenderer = nullptr;
	std::shared_ptr<class GameEngineCollision> AttackCollision = nullptr;

private:
	int Dir = 0;
	CardType CurCardType = CardType::None;
};

