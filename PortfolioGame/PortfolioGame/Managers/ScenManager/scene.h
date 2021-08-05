#pragma once
class Scene
{
public:
	explicit Scene();
	virtual ~Scene();

public:
	virtual int ReadyScene() = 0;
	virtual void UpdateScene() = 0;
	virtual void LateUpdateScene() = 0;
	virtual void RenderScene(HDC hDC) = 0;
	virtual void ReleaseScene() = 0;
};

