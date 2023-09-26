#pragma once

class MainScene : public Scene
{
	virtual void OnLoad(Scene* prevScene) override;
	virtual void OnChange(Scene* nextScene) override;
	virtual Scene* Copy() override;
};

