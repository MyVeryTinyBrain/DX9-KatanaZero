#pragma once

class Scene abstract
{
	PUBLIC Scene() {}
	PUBLIC virtual ~Scene() {}

	PUBLIC virtual void OnLoad(Scene* prevScene) = 0;
	PUBLIC virtual void OnChange(Scene* nextScene) = 0;
	PUBLIC virtual Scene* Copy() = 0;

	PUBLIC virtual void OnBeforeFixedUpdate() {};
	PUBLIC virtual void OnAfterFixedUpdate() {};
	PUBLIC virtual void OnBeforeUpdate() {};
	PUBLIC virtual void OnAfterUpdate() {};
	PUBLIC virtual void OnBeforLateUpdate() {};
	PUBLIC virtual void OnAfterLateUpdate() {};
	PUBLIC virtual void OnBeforeRender() {};
	PUBLIC virtual void OnAfterRender() {};
};

