#pragma once
#include <GameEngineCore\GameEngineActor.h>

// ���� :
class ContentBackground : public ContentActor
{
public:
	// constrcuter destructer
	ContentBackground();
	~ContentBackground();

	// delete Function
	ContentBackground(const ContentBackground& _Other) = delete;
	ContentBackground(ContentBackground&& _Other) noexcept = delete;
	ContentBackground& operator=(const ContentBackground& _Other) = delete;
	ContentBackground& operator=(ContentBackground&& _Other) noexcept = delete;

	void BackgroundInit(std::string_view _SpriteName);
	void BackgroundInit(std::string_view _SpriteName, float4 _Pos);
	void PixelBackgroundInit(std::string_view _SpriteName);
	void UpperBackgroundInit(std::string_view _SpriteName);

	GameEngineColor GetColor(float4 _Pos, GameEngineColor _DefaultColor);

	float4 GetPixelTextureScale()
	{
		if (nullptr != PixelTexture)
		{
			return PixelTexture->GetScale();
		}
	}

	float4 GetTextureScale()
	{
		if (nullptr != Texture)
		{
			return Texture->GetScale();
		}
	}

	std::shared_ptr<GameEngineTexture> GetTexture()
	{
		if (nullptr != Texture)
		{
			return Texture;
		}
	}

	std::shared_ptr<GameEngineTexture> GetPixelTexture()
	{
		if (nullptr != PixelTexture)
		{
			return PixelTexture;
		}
	}

	void RenderOn()
	{
		if (nullptr != Renderer)
		{
			Renderer->On();
		}
	}

	void RenderOff()
	{
		if (nullptr != Renderer)
		{
			Renderer->Off();
		}
	}

	std::shared_ptr<GameEngineSpriteRenderer> GetRenderer()
	{
		return Renderer;
	}

	void SetBackgroundScaleRatio(float _Ratio)
	{
		if (nullptr != Texture)
		{
			Renderer->SetAutoScaleRatio(_Ratio);
		}
	}

protected:
	void Start() override;
	void Update(float _Delta) override;

	std::shared_ptr<GameEngineSpriteRenderer> Renderer = nullptr;
	std::shared_ptr<GameEngineTexture> Texture = nullptr;
	std::shared_ptr<GameEngineTexture> PixelTexture = nullptr;
	std::shared_ptr<GameEngineTexture> UpperTexture = nullptr;
private:
	std::string Background = "";
	std::string PixelBackground = "";
	std::string UpperBackground = "";
};

