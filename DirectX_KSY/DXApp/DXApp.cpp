// App.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.

#pragma comment(lib, "GameEngineBase.lib")
#pragma comment(lib, "GameEnginePlatform.lib")
#pragma comment(lib, "GameEngineCore.lib")
#pragma comment(lib, "Content_Cuphead.lib")

#include <iostream>
#include <Windows.h>
#include <memory>
#include <GameEngineCore/GameEngineCore.h>
#include <Content_Cuphead/ContentsCore.h>


int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	float4x4 Rot;
	Rot.RotationYDegs(90.0f);

	GameEngineCore::EngineStart<ContentsCore>(hInstance);
}

