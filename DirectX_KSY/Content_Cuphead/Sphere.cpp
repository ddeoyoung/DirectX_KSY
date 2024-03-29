#include "PreCompile.h"
#include "Sphere.h"
#include "BaseCharacter.h"

Sphere::Sphere()
{
}

Sphere::~Sphere()
{
}


void Sphere::Start()
{
	ContentActor::Start();

	// Create Sprite Folder
	if (nullptr == GameEngineSprite::Find("Tutorial_Sphere"))
	{
		GameEngineDirectory Dir;
		Dir.MoveParentToExistsChild("GameEngineResources");
		Dir.MoveChild("ContentsResources\\Texture\\Tutorial");

		std::vector<GameEngineDirectory> Directorys = Dir.GetAllDirectory();

		for (size_t i = 0; i < Directorys.size(); i++)
		{
			GameEngineDirectory& Dir = Directorys[i];
			GameEngineSprite::CreateFolder(Dir.GetStringPath());
		}
	}

	// Create Animation
	MainRenderer = CreateComponent<GameEngineSpriteRenderer>();
	MainRenderer->CreateAnimation("Tutorial_Sphere", "Tutorial_Sphere");
	MainRenderer->CreateAnimation("Tutorial_Sphere_Pink", "Tutorial_Sphere_Pink");
	MainRenderer->ChangeAnimation("Tutorial_Sphere_Pink");
	MainRenderer->AutoSpriteSizeOn();

	// Create Collision
	ParryCollision = CreateComponent<GameEngineCollision>(CollisionOrder::ParryObject);
	std::shared_ptr<GameEngineSprite> Texture = GameEngineSprite::Find("Tutorial_Sphere_Pink");
	float4 Scale = Texture->GetSpriteData(0).GetScale();
	ParryCollision->SetCollisionType(ColType::SPHERE2D);
	ParryCollision->Transform.SetLocalScale(Scale);
	ParryCollision->Transform.SetLocalPosition({ 0, 0 });
}

void Sphere::Update(float _Delta)
{
	ContentActor::Update(_Delta);

	bool ParryCheck = BaseCharacter::MainCharacter->GetIsParry();
	bool CollisionCheck = ParryCollision->Collision(CollisionOrder::PlayerParry);
	if (true == ParryCheck && true == CollisionCheck)
	{
		MainRenderer->ChangeAnimation("Tutorial_Sphere");
		IsParry = true;
	}

	if (true == IsParry)
	{
		CollisionOffTime += _Delta;
		if (CollisionOffTime > 0.5f)
		{
			ParryCollision->Off();
		}
	}

}
