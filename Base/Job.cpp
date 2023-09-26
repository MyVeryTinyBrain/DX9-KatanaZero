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
	// 이전에 저장된 호출 타겟 벡터를 모두 비웁니다.
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

	// 게임오브젝트와 컴포넌트의 상태에 따라
	// 객체를 삭제하거나 함수 호출 벡터에 저장합니다.

	for (auto i = GAME_OBJECTS.begin(); i != GAME_OBJECTS.end();)
	{
		GameObject* gameObject = *i;

		// 삭제될 게임오브젝트라면
		// 해당 게임오브젝트를 삭제합니다.
		// 이때 게임오브젝트의 컴포넌트들 역시 삭제됩니다.
		// 또한 컴포넌트들을 탐색하지 않고 다음 게임오브젝트로 넘어갑니다.
		if (gameObject->IsShouldDestroy())
		{
			//SAFE_DELETE(gameObject);
			m_destroyGameObject.push_back(gameObject);
			i = GAME_OBJECTS.erase(i);
			continue;
		}

		// 이 게임오브젝트가 비활성화 상태라면 다음 게임오브젝트로 넘어갑니다.
		if (!gameObject->GetActive())
		{
			++i;
			continue;
		}

		// 게임오브젝트의 컴포넌트 벡터입니다.
		auto& components = gameObject->m_components;

		// 컴포넌트 벡터를 순회합니다.
		for (auto j = components.begin(); j != components.end();)
		{
			Component* component = *j;

			// 삭제될 컴포넌트라면
			// 해당 컴포넌트를 삭제합니다.
			// 또한 이 컴포넌트를 탐색하지 않고 다음 컴포넌트로 넘어갑니다.
			if (component->IsShouldDestroy())
			{
				//SAFE_DELETE(component);
				m_destroyComponent.push_back(component);
				j = components.erase(j);
				continue;
			}

			// 이 컴포넌트가 비활성화 상태라면 다음 컴포넌트로 넘어갑니다.
			if (!component->GetEnable())
			{
				++j;
				continue;
			}

			// 각 플래그의 활성화 상태에 따라 벡터에 저장합니다.

			// Start() 함수를 호출해야 하는 경우는
			// 컴포넌트가 생성된 다음 프레임입니다.
			// 따라서 m_start가 활성화 되어있다면 이 컴포넌트는 생성된 직후라는 뜻입니다.
			// 또한 단 한번만 호출할 것이므로 이 코드 블럭이 아닌 Start() 함수가 호출된 다음 코드 줄에서 m_start를 비활성화 합니다.
			// 결국 m_start가 활성화라면 벡터에 저장합니다.
			if (component->m_callStart)
			{
				m_start.push_back(component);
			}

			// 이하 다른 함수 플래그에 따라 벡터에 저장합니다.
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

	// 벡터를 실행 순서에 따라 정렬합니다.
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
		// 노멀 벡터의 방향은 A --> B 입니다.
		// A가 받게 될 노멀의 방향은 B --> A
		// B가 받게 될 노멀의 방향은 A --> B 입니다.

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
		// 노멀 벡터의 방향은 A --> B 입니다.
		// A가 받게 될 노멀의 방향은 B --> A
		// B가 받게 될 노멀의 방향은 A --> B 입니다.

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
	// 모든 게임오브젝트가 Destroy() 함수를 호출하도록 합니다.
	GameObject::DestroyAll();

	// 모든 게임오브젝트가 삭제 요청이 활성화 되었으므로
	// 준비 단계에서 모든 게임오브젝트가 제거될 것입니다.
	Prepare();
	DeleteGarbages();

	ContactListener* contactListener = Physics::GetInstance()->GetContactListener();
	contactListener->Clear();
}
