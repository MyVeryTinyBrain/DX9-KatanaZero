#pragma once

class Component;
class GameObject;

class Job
{
	PUBLIC Job();
	PUBLIC ~Job();

	PUBLIC void Prepare();
	PUBLIC void Start();
	PUBLIC void BeforeFixedStep();
	PUBLIC void PhysicsEvent();
	PUBLIC void DeleteGarbages();
	PUBLIC void FixedUpdate();
	PUBLIC void LateFixedUpdate();
	PUBLIC void Update();
	PUBLIC void LateUpdate();
	PUBLIC void AfterUpdate();
	PUBLIC void BeforeRender();
	PUBLIC void Render();
	PUBLIC void PhysicsRender();
	PUBLIC void AfterRender();

	PUBLIC void DeleteAll();

	PRIVATE std::vector<Component*> m_start;
	PRIVATE std::vector<Component*> m_beforeFixedStep;
	PRIVATE std::vector<Component*> m_fixedUpdate;
	PRIVATE std::vector<Component*> m_lateFixedUpdate;
	PRIVATE std::vector<Component*> m_update;
	PRIVATE std::vector<Component*> m_lateUpdate;
	PRIVATE std::vector<Component*> m_afterUpdate;
	PRIVATE std::vector<Component*> m_beforeRender;
	PRIVATE std::vector<Component*> m_render;
	PRIVATE std::vector<Component*> m_afterRender;

	PRIVATE std::vector<GameObject*> m_destroyGameObject;
	PRIVATE std::vector<Component*> m_destroyComponent;
};

