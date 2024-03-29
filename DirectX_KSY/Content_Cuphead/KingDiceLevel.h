#pragma once
#include "ContentLevel.h"

enum class SpaceState
{
	None,
	SpaceStart,
	Space1,
	Space2,
	Space3,
	SpaceSafe1,
	Space4,
	Space5,
	Space6,
	SpaceSafe2,
	Space7,
	Space8,
	Space9,
	SpaceSafe3,
	SpaceStartOver,
	SpaceFinish,
};

// ���� :
class KingDiceLevel : public ContentLevel
{
public:
	// constrcuter destructer
	KingDiceLevel();
	~KingDiceLevel();

	// delete Function
	KingDiceLevel(const KingDiceLevel& _Other) = delete;
	KingDiceLevel(KingDiceLevel&& _Other) noexcept = delete;
	KingDiceLevel& operator=(const KingDiceLevel& _Other) = delete;
	KingDiceLevel& operator=(KingDiceLevel&& _Other) noexcept = delete;

	std::shared_ptr<BaseCharacter> GetCurPlayer()
	{
		if (nullptr != Player)
		{
			return Player;
		}
	}

protected:
	void Start() override;
	void Update(float _Delta) override;

	void LevelStart(GameEngineLevel* _PrevLevel) override;
	void LevelEnd(GameEngineLevel* _NextLevel) override;

	void LevelStartTextureLoad();
	void LevelStartTextureSet();

	bool CheckMarkerEnd();
	void CheckRouletteSpace();
	void ChangeToSubBossStage();

	void FightKingDiceStart();
	void FightKingDiceUpdate(float _Delta);

	void CheckStageClear();
	void CreateKnockOut();
	void CreateBossExplosion();
	void CreateFadeOut(float _Delta);

	bool IsFightKingDice = false;

private:
	std::shared_ptr<class KingDice> Boss = nullptr;
	std::shared_ptr<class BaseCharacter> Player = nullptr;
	std::shared_ptr<class Marker> Roulette_Marker = nullptr;
	std::shared_ptr<class FightText> ReadyText = nullptr;
	std::shared_ptr<class FightText> KnockOut = nullptr;
	std::shared_ptr<class FadeAnimation> FadeOut = nullptr;
	std::string LevelName = "";

	float ReadyTime = 0.0f;
	float StageClearTime = 0.0f;
	bool IsStageClear = false;
};

