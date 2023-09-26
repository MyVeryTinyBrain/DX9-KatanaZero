#include "base_stdafx.h"
#include "Job.h"
#include "GameObject.h"
#include "Component.h"
#include "Physics.h"
#include "SceneManager.h"
#include "Scene.h"
#include "ContactListener.h"
#include "Box2dStruct.h"
#include "Collider.h"

#define GAME_OBJECTS (GameObject::g_gameObjects)

#define SORT_BY_ORDER(VECTOR, ORDER)		\
std::sort(									\
	VECTOR.begin(),							\
	VECTOR.end(),							\
	[](Component* lhs, Component* rhs)		\
	{										\
		return lhs->ORDER < rhs->ORDER;		\
	}										\
);

Job::Job()
{
}

Job::~Job()
{
	DeleteAll();
}

void Job::Prepare()
{
	// ������ ����� ȣ�� Ÿ�� ���͸� ��� ���ϴ�.
	m_start.clear();
	m_beforeFixedStep.clear();
	m_fixedUpdate.clear();
	m_lateFixedUpdate.clear();
	m_update.clear();
	m_lateUpdate.clear();
	m_afterUpdate.clear();
	m_beforeRender.clear();
	m_render.clear();
	m_afterRender.clear();

	m_destroyGameObject.clear();
	m_destroyComponent.clear();

	// ���ӿ�����Ʈ�� ������Ʈ�� ���¿� ����
	// ��ü�� �����ϰų� �Լ� ȣ�� ���Ϳ� �����մϴ�.

	for (auto i = GAME_OBJECTS.begin(); i != GAME_OBJECTS.end();)
	{
		GameObject* gameObject = *i;

		// ������ ���ӿ�����Ʈ���
		// �ش� ���ӿ�����Ʈ�� �����մϴ�.
		// �̶� ���ӿ�����Ʈ�� ������Ʈ�� ���� �����˴ϴ�.
		// ���� ������Ʈ���� Ž������ �ʰ� ���� ���ӿ�����Ʈ�� �Ѿ�ϴ�.
		if (gameObject->IsShouldDestroy())
		{
			//SAFE_DELETE(gameObject);
			m_destroyGameObject.push_back(gameObject);
			i = GAME_OBJECTS.erase(i);
			continue;
		}

		// �� ���ӿ�����Ʈ�� ��Ȱ��ȭ ���¶�� ���� ���ӿ�����Ʈ�� �Ѿ�ϴ�.
		if (!gameObject->GetActive())
		{
			++i;
			continue;
		}

		// ���ӿ�����Ʈ�� ������Ʈ �����Դϴ�.
		auto& components = gameObject->m_components;

		// ������Ʈ ���͸� ��ȸ�մϴ�.
		for (auto j = components.begin(); j != components.end();)
		{
			Component* component = *j;

			// ������ ������Ʈ���
			// �ش� ������Ʈ�� �����մϴ�.
			// ���� �� ������Ʈ�� Ž������ �ʰ� ���� ������Ʈ�� �Ѿ�ϴ�.
			if (component->IsShouldDestroy())
			{
				//SAFE_DELETE(component);
				m_destroyComponent.push_back(component);
				j = components.erase(j);
				continue;
			}

			// �� ������Ʈ�� ��Ȱ��ȭ ���¶�� ���� ������Ʈ�� �Ѿ�ϴ�.
			if (!component->GetEnable())
			{
				++j;
				continue;
			}

			// �� �÷����� Ȱ��ȭ ���¿� ���� ���Ϳ� �����մϴ�.

			// Start() �Լ��� ȣ���ؾ� �ϴ� ����
			// ������Ʈ�� ������ ���� �������Դϴ�.
			// ���� m_start�� Ȱ��ȭ �Ǿ��ִٸ� �� ������Ʈ�� ������ ���Ķ�� ���Դϴ�.
			// ���� �� �ѹ��� ȣ���� ���̹Ƿ� �� �ڵ� ���� �ƴ� Start() �Լ��� ȣ��� ���� �ڵ� �ٿ��� m_start�� ��Ȱ��ȭ �մϴ�.
			// �ᱹ m_start�� Ȱ��ȭ��� ���Ϳ� �����մϴ�.
			if (component->m_callStart)
			{
				m_start.push_back(component);
			}

			// ���� �ٸ� �Լ� �÷��׿� ���� ���Ϳ� �����մϴ�.
			if (component->m_callBeforeFixedStep) m_beforeFixedStep.push_back(component);
			if (component->m_callFixedUpdate) m_fixedUpdate.push_back(component);
			if (component->m_callLateFixedUpdate) m_lateFixedUpdate.push_back(component);
			if (component->m_callUpdate) m_update.push_back(component);
			if (component->m_callLateUpdate) m_lateUpdate.push_back(component);
			if (component->m_callAfterUpdate) m_afterUpdate.push_back(component);
			if (component->m_callBeforeRender) m_beforeRender.push_back(component);
			if (component->m_callRender) m_render.push_back(component);
			if (component->m_callAfterRender) m_afterRender.push_back(component);

			++j;
		}

		++i;
	}

	// ���͸� ���� ������ ���� �����մϴ�.
	//SORT_BY_ORDER(m_start, m_updateExecutionOrder);
	SORT_BY_ORDER(m_beforeFixedStep, m_beforeFixedStepExecutionOrder);
	SORT_BY_ORDER(m_fixedUpdate, m_fixedUpdateExecutionOrder);
	SORT_BY_ORDER(m_lateFixedUpdate, m_lateFixedUpdateExecutionOrder);
	SORT_BY_ORDER(m_update, m_updateExecutionOrder);
	SORT_BY_ORDER(m_lateUpdate, m_lateUpdateExecutionOrder);
	SORT_BY_ORDER(m_afterUpdate, m_afterUpdateExecutionOrder);
	SORT_BY_ORDER(m_beforeRender, m_beforeRenderExecutionOrder);
	SORT_BY_ORDER(m_render, m_renderOrder);
	SORT_BY_ORDER(m_afterRender, m_afterRenderOrder);
}

void Job::Start()
{
	for (auto& component : m_start)
	{
		if (!component->GetGameObject()->GetActive()) continue;
		if (!component->GetEnable())continue;
		component->Start();
		component->m_callStart = false;
	}
}

void Job::BeforeFixedStep()
{
	for (auto& component : m_beforeFixedStep)
	{
		if (!component->GetGameObject()->GetActive()) continue;
		if (!component->GetEnable())continue;
		component->BeforeFixedStep();
	}
}

void Job::PhysicsEvent()
{
	ContactListener* contactListener = Physics::GetInstance()->GetContactListener();
	std::vector<CollisionResult> beginCollisions;
	std::vector<TriggerResult> beginTriggers;
	std::vector<CollisionExitResult> endCollisions;
	std::vector<TriggerResult> endTriggers;
	std::vector<CollisionResult> stayCollisions;
	std::vector<TriggerResult> stayTriggers;
	contactListener->MoveBeginCollisions(&beginCollisions);
	contactListener->MoveBeginTriggers(&beginTriggers);
	contactListener->MoveEndCollisions(&endCollisions);
	contactListener->MoveEndTriggers(&endTriggers);
	contactListener->MoveStayCollisions(&stayCollisions);
	contactListener->MoveStayTriggers(&stayTriggers);

	for (auto& beginCollision : beginCollisions)
	{
		// ��� ������ ������ A --> B �Դϴ�.
		// A�� �ް� �� ����� ������ B --> A
		// B�� �ް� �� ����� ������ A --> B �Դϴ�.

		// Normals: A --> B
		//if (!beginCollision.B->IsShouldDestroy())
			beginCollision.B->OnCollisionEnter(beginCollision);

		// Normals: B --> A
		beginCollision.localNormal *= -1;
		beginCollision.worldNormal *= -1;
		Collider* A = beginCollision.A;
		Collider* B = beginCollision.B;
		beginCollision.B = A;
		beginCollision.A = B;
		//if (!beginCollision.A->IsShouldDestroy())
			A->OnCollisionEnter(beginCollision);
	}
	for (auto& endCollision : endCollisions)
	{
		if (!endCollision.A->IsShouldDestroy())
			endCollision.A->OnCollisionExit(endCollision.B);
		if (!endCollision.B->IsShouldDestroy())
			endCollision.B->OnCollisionExit(endCollision.A);
	}
	for (auto& beginTrigger : beginTriggers)
	{
		//if (!beginTrigger.A->IsShouldDestroy())
			beginTrigger.A->OnTriggerEnter(beginTrigger.B);
		//if (!beginTrigger.B->IsShouldDestroy())
			beginTrigger.B->OnTriggerEnter(beginTrigger.A);
	}
	for (auto& endTrigger : endTriggers)
	{
		if (!endTrigger.A->IsShouldDestroy())
			endTrigger.A->OnTriggerExit(endTrigger.B);
		if (!endTrigger.B->IsShouldDestroy())
			endTrigger.B->OnTriggerExit(endTrigger.A);
	}
	for (auto& stayCollision : stayCollisions)
	{
		// ��� ������ ������ A --> B �Դϴ�.
		// A�� �ް� �� ����� ������ B --> A
		// B�� �ް� �� ����� ������ A --> B �Դϴ�.

		// Normals: A --> B
		//if (!stayCollision.B->IsShouldDestroy())
			stayCollision.B->OnCollisionStay(stayCollision);

		// Normals: B --> A
		stayCollision.localNormal *= -1;
		stayCollision.worldNormal *= -1;
		//if (!stayCollision.A->IsShouldDestroy())
			stayCollision.A->OnCollisionStay(stayCollision);
	}
	for (auto& stayTrigger : stayTriggers)
	{
		//if (!stayTrigger.A->IsShouldDestroy())
			stayTrigger.A->OnTriggerStay(stayTrigger.B);
		//if (!stayTrigger.B->IsShouldDestroy())
			stayTrigger.B->OnTriggerStay(stayTrigger.A);
	}
}

void Job::DeleteGarbages()
{
	//PRIVATE std::vector<GameObject*> m_destroyGameObject;
	//PRIVATE std::vector<Component*> m_destroyComponent;
	for (auto& gameObject : m_destroyGameObject)
	{
		SAFE_DELETE(gameObject);
	}

	for (auto& component : m_destroyComponent)
	{
		SAFE_DELETE(component);
	}
}

void Job::FixedUpdate()
{
	if (SceneManager::GetCurrentScene()) SceneManager::GetCurrentScene()->OnBeforeFixedUpdate();
	for (auto& component : m_fixedUpdate)
	{
		if (!component->GetGameObject()->GetActive()) continue;
		if (!component->GetEnable())continue;
		component->FixedUpdate();
	}
	if (SceneManager::GetCurrentScene()) SceneManager::GetCurrentScene()->OnAfterFixedUpdate();
}

void Job::LateFixedUpdate()
{
	for (auto& component : m_lateFixedUpdate)
	{
		if (!component->GetGameObject()->GetActive()) continue;
		if (!component->GetEnable())continue;
		component->LateFixedUpdate();
	}
}

void Job::Update()
{
	if (SceneManager::GetCurrentScene()) SceneManager::GetCurrentScene()->OnBeforeUpdate();
	for (auto& component : m_update)
	{
		if (!component->GetGameObject()->GetActive()) continue;
		if (!component->GetEnable())continue;
		component->Update();
	}
	if (SceneManager::GetCurrentScene()) SceneManager::GetCurrentScene()->OnAfterUpdate();
}

void Job::LateUpdate()
{
	if (SceneManager::GetCurrentScene()) SceneManager::GetCurrentScene()->OnBeforLateUpdate();
	for (auto& component : m_lateUpdate)
	{
		if (!component->GetGameObject()->GetActive()) continue;
		if (!component->GetEnable())continue;
		component->LateUpdate();
	}
	if (SceneManager::GetCurrentScene()) SceneManager::GetCurrentScene()->OnAfterLateUpdate();
}

void Job::AfterUpdate()
{
	for (auto& component : m_afterUpdate)
	{
		if (!component->GetGameObject()->GetActive()) continue;
		if (!component->GetEnable())continue;
		component->AfterUpdate();
	}
}

void Job::BeforeRender()
{
	for (auto& component : m_beforeRender)
	{
		if (!component->GetGameObject()->GetActive()) continue;
		if (!component->GetEnable())continue;
		component->BeforeRender();
	}
}

void Job::Render()
{
	if (SceneManager::GetCurrentScene()) SceneManager::GetCurrentScene()->OnBeforeRender();
	for (auto& component : m_render)
	{
		if (!component->GetGameObject()->GetActive()) continue;
		if (!component->GetEnable())continue;
		component->Render();
	}
	if (SceneManager::GetCurrentScene()) SceneManager::GetCurrentScene()->OnAfterRender();
}

void Job::PhysicsRender()
{
	Physics::GetInstance()->Draw();
}

void Job::AfterRender()
{
	for (auto& component : m_render)
	{
		if (!component->GetGameObject()->GetActive()) continue;
		if (!component->GetEnable())continue;
		component->AfterRender();
	}
}

void Job::DeleteAll()
{
	// ��� ���ӿ�����Ʈ�� Destroy() �Լ��� ȣ���ϵ��� �մϴ�.
	GameObject::DestroyAll();

	// ��� ���ӿ�����Ʈ�� ���� ��û�� Ȱ��ȭ �Ǿ����Ƿ�
	// �غ� �ܰ迡�� ��� ���ӿ�����Ʈ�� ���ŵ� ���Դϴ�.
	Prepare();
	DeleteGarbages();

	ContactListener* contactListener = Physics::GetInstance()->GetContactListener();
	contactListener->Clear();
}
