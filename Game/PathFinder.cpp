#include "stdafx.h"
#include "PathFinder.h"
#include "PlatformerBody.h"
#include "StageMap.h"
#include "GameLayer.h"

void PathFinder::Awake()
{
	m_body = GetComponent<PlatformerBody>();
	m_body->SetTripleRay(false);

	m_hasTarget = false;
	m_autoTarget = nullptr;
	m_speed = 200;
	m_doorUseTime = 0.5f;
	m_prevHD = 1;
	m_minDistance = 10;

	m_debugLine = AddComponent<LineRenderer>();
	m_debugLine->SetWidth(5);
	m_debugLine->SetColor(Color(1, 0, 0, 1));

	std::thread t([&]() { ThreadWork(); });
	m_thread.swap(t);
}

void PathFinder::FixedUpdate()
{
	if (IsPause()) return;
	if (IsUsingDoor()) return;

	// 현재 노드를 가르키는 인덱스의 다음부터 순회합니다.
	// 이 루프는 다음 노드와의 거리가 충분히 가까울 때 노드를 인덱스를 증가시켜 다음 노드를 가르키기 위함입니다.
	for (size_t i = m_currentIndex + 1; i < m_path.size(); ++i)
	{
		// 바디의 바닥부터 각 노드까지의 거리입니다.
		float distance = Vec2::Distance(GetBottom(), m_path[i].position);

		// 목표 지점까지의 거리가 일정한 크기보다 작은 경우
		if (distance < m_minDistance)
		{
			// @ 문으로 진입하는 경우
			// 다음 노드가 문 노드이면서
			// 그 문 노드와 충분히 가까이 있으며
			// 그 문 노드의 출구가 존재하는 경우
			if (m_path[i].type == StageGraphType::DOOR && 
				m_path.size() - 1 >= i + 1 && 
				m_path[i + 1].type == StageGraphType::DOOR)
			{
				// 문으로 진입합니다.

				m_readyToEnterDoor = true;
				m_exitPos = m_path[i + 1].position;
				SetBottom(m_path[i].position);
				m_currentIndex = i + 1;
				m_enterCounter = m_doorUseTime * 0.5f;
				OnBeginToEnterDoor();
				if (m_path.size() - 1 >= i + 2)
				{
					m_body->SetVelocity(Vec2::zero());
					Vec2 exitNextPos = m_path[i + 2].position;
					Vec2 exitDir = exitNextPos - m_exitPos;
					if (exitDir.x == 0) m_exitHDir = m_prevHD;
					else if (exitDir.x < 0) m_exitHDir = -1;
					else if (exitDir.x > 0) m_exitHDir = +1;
				}
				else
				{
					m_exitHDir = m_prevHD;
				}
				return;
			}

			// @ 계단으로 진입하는 경우
			// 다음 노드가 계단 노드이면서
			// 그 계단의 끝이 존재하며
			// 그 계단 노드와 충분히 가까이 있는 경우
			if (m_path[i].type == StageGraphType::STAIR && 
				m_path.size() - 1 >= i + 1 && 
				m_path[i + 1].type == StageGraphType::STAIR)
			{
				// 내려가는 방향일 경우에는 계단의 최상단에서 약간 아래쪽으로 위치를 조정합니다.
				// 안정성을 위해서 하는 동작입니다.
				Vec2 stairRel = m_path[i + 1].position - m_path[i].position;
				Vec2 stairDir = stairRel.normalized();
				if (stairDir.y < 0)
				{
					m_body->SetVelocity(Vec2::zero());
					SetBottom(m_path[i].position + stairDir * (m_body->GetCircleCollider()->GetRadius()));
					++m_currentIndex;
					return;
				}
			}

			// @ 계단에서 나가는 경우
			// 현재 노드가 계단 노드이면서
			// 다음 노드가 계단 노드이면서
			// 계단의 끝 노드가 땅일 때
			// 그 계단 노드와 충분히 가까이 있는 경우
			if (i > 0 &&
				m_path[i - 1].type == StageGraphType::STAIR &&
				m_path[i].type == StageGraphType::STAIR &&
				m_path.size() - 1 >= i + 1 &&
				m_path[i + 1].type == StageGraphType::GROUND)
			{
				// 올라가는 방향일 경우에는 계단의 최상단에서 이동하는 수평 방향으로 약간 위치를 조정합니다.
				// 안정성을 위해서 하는 동작입니다.
				Vec2 stairRel = m_path[i].position - m_path[i - 1].position;
				Vec2 stairDir = stairRel.normalized();
				if (stairDir.y > 0)
				{
					if (stairDir.y > 0 && stairDir.x < 0)
						SetBottom(m_path[i].position + Vec2::left());
					if (stairDir.y > 0 && stairDir.x > 0)
						SetBottom(m_path[i].position + Vec2::right());
					++m_currentIndex;
					return;
				}
			}

			// 현재 노드를 가르키는 인덱스를 이동시킵니다.
			++m_currentIndex;
		}
		else
		{
			// 더 이상 가까운 노드가 없으면 루프를 탈출합니다.
			break;
		}
	}

	if (m_currentIndex >= m_path.size())
	{
		m_path.clear();
		return;
	}

	int prevIndex = m_currentIndex - 1;
	int currentIndex = m_currentIndex;
	int nextIndex = m_currentIndex + 1;

	if (nextIndex >= int(m_path.size())) return;

	// 가장 정확한 방향인 이전 현재 노드와 다음 노드 사이의 방향을 사용합니다.
	Vec2 direction = m_path[nextIndex].position - m_path[currentIndex].position;

	// 아래로 향하는 방향이면 플랫폼과의 충돌을 끕니다.
	m_body->SetThroughPlatform(direction.y < -0.1f && m_path[nextIndex].type != StageGraphType::DOOR);

	float hd = direction.x < 0 ? (-1.0f) : (+1.0f);

	// 방향이 바뀌는 경우에
	if (hd != m_prevHD)
	{
		// 방향을 바꿉니다.
		m_prevHD = hd;
		SetSpriteRenderersDirection(hd);
		OnBeginToTurn(hd);
		return;
	}

	if (m_speed > 0)
		m_body->SetHorizontalVelocity(hd * m_speed);

	// 스프라이트의 방향을 바꿉니다.
	// 제거해도 문제 없다면 제거하세요.
	SetSpriteRenderersDirection(hd);
}

void PathFinder::Update()
{
	UseDoorProcess();

	if (!m_target) return;
	if (IsPause()) return;
	if (IsUsingDoor()) return;

	AutoTargetProcess();
	PathFindProcess();
	UpdatePathProcess();
}

void PathFinder::OnDestroy()
{
	while (!m_managedSpriteRenderers.empty())
	{
		RemoveAlphaManagedSpriteRenderer(m_managedSpriteRenderers[0]);
	}

	m_mutex.lock();
	m_stopThread = true;
	m_mutex.unlock();

	m_cv.notify_one();
	if (m_thread.joinable())
		m_thread.join();
}

void PathFinder::Render() {
	/* 경로 디버그 */

	//for (int i = 0; i < (int)m_path.size() - 1; ++i) {
	//	LineDevice* line = GraphicDevice::GetLineDevice();
	//	Draw2DLineParameters p;
	//	p.width = 1;
	//	p.color = Color(0, 1, 0, 1);
	//	p.inWorld = true;

	//	line->Draw2DLine(m_path[i].position, m_path[i + 1].position, &p);
	//}

	//for (int i = 0; i < m_path.size(); ++i) {
	//	LineDevice* line = GraphicDevice::GetLineDevice();
	//	Draw2DLineParameters p;
	//	p.width = 1;
	//	p.color = Color(0, 1, 0, 1);
	//	p.inWorld = true;

	//	line->Draw2DCircle(m_path[i].position, 2.f, &p);
	//}
}

void PathFinder::OnDestroySpriteRenderer(Object* object)
{
	RemoveAlphaManagedSpriteRenderer(static_cast<SpriteRenderer*>(object));
}

void PathFinder::SetTarget(Vec2 target)
{
	m_target = target;
	m_hasTarget = true;
}

void PathFinder::SetTargetWithPlatformerBody(PlatformerBody* body)
{
	SetTarget(body->GetCircleCollider()->GetTransform()->GetPosition() + Vec2::down() * (body->GetCircleCollider()->GetRadius() - 1));
}

void PathFinder::SetAutoTarget(PlatformerBody* body)
{
	m_autoTarget = body;
	m_hasTarget = false;
}

void PathFinder::SetSpeed(float speed)
{
	m_speed = speed;
}

void PathFinder::ClearPath()
{
	m_path.clear();
}

void PathFinder::Pause()
{
	m_pause = true;
}

void PathFinder::Resume()
{
	m_pause = false;
}

bool PathFinder::IsPause() const
{
	return m_pause;
}

bool PathFinder::IsUsingDoor() const
{
	return m_readyToEnterDoor || m_readyToExitDoor || (m_enterCounter > 0) || (m_exitCounter > 0);
}

float PathFinder::GetHorizontalDirection() const
{
	float hd;
	if (m_managedSpriteRenderers.size() == 0) return 0;
	hd = m_managedSpriteRenderers[0]->GetTransform()->GetScale().x;
	if (hd < 0) hd = -1;
	else hd = +1;
	return hd;
}

void PathFinder::SetHorizontalDirection(float hd)
{
	if (hd < 0) hd = -1;
	else hd = +1;
	SetSpriteRenderersDirection(hd);
	m_prevHD = hd;
}

bool PathFinder::IsLastPosition() const
{
	return m_currentIndex + 1 >= m_path.size();
}

bool PathFinder::HasPath() const
{
	return m_path.size() > 0;
}

void PathFinder::AddManagedSpriteRenderer(SpriteRenderer* renderer)
{
	m_managedSpriteRenderers.push_back(renderer);
	renderer->OnDestoryCallback += Function<void, Object*>(this, &PathFinder::OnDestroySpriteRenderer);
}

void PathFinder::RemoveAlphaManagedSpriteRenderer(SpriteRenderer* renderer)
{
	auto it = std::find(m_managedSpriteRenderers.begin(), m_managedSpriteRenderers.end(), renderer);
	if (it == m_managedSpriteRenderers.end()) return;
	m_managedSpriteRenderers.erase(it);
	renderer->OnDestoryCallback -= Function<void, Object*>(this, &PathFinder::OnDestroySpriteRenderer);
}

void PathFinder::SetMinDistance(float md)
{
	m_minDistance = md;
}

float PathFinder::GetMinDistance() const
{
	return m_minDistance;
}

void PathFinder::CancelEnterDoor()
{
	m_readyToEnterDoor = false;
	m_readyToExitDoor = false;
	m_enterCounter = 0;
	m_exitCounter = 0;
	SetSpriteRenderersAlpha(1.0f);
}

void PathFinder::SetSpriteRenderersDirection(float hd)
{
	for (auto& spriteRenderer : m_managedSpriteRenderers)
	{
		Transform* transform = spriteRenderer->GetTransform();
		Vec2 scale = transform->GetScale();
		scale.x = fabsf(scale.x) * hd;
		transform->SetScale(scale);
	}
}

void PathFinder::SetSpriteRenderersAlpha(float a)
{
	for (auto& spriteRenderer : m_managedSpriteRenderers)
	{
		Color color = spriteRenderer->GetColor();
		color.a = a;
		spriteRenderer->SetColor(color);
	}
}

Vec2 PathFinder::GetBottom() const
{
	return GetTransform()->GetPosition() + Vec2::down() * m_body->GetCenterToBottom();
}

void PathFinder::SetBottom(Vec2 position) const
{
	GetTransform()->SetPosition(position + Vec2::up() * m_body->GetCenterToBottom());
}

void PathFinder::UseDoorProcess()
{
	if (m_readyToEnterDoor && m_enterCounter < 0)
	{
		SetSpriteRenderersAlpha(0);
		EnterDoor();
	}

	if (m_readyToExitDoor && m_exitCounter < 0)
	{
		m_readyToExitDoor = false;
		SetSpriteRenderersAlpha(1);
	}

	m_enterCounter -= Time::GetDeltaTime();
	m_exitCounter -= Time::GetDeltaTime();

	if (m_enterCounter > 0)
	{
		SetSpriteRenderersAlpha(m_enterCounter / m_doorUseTime * 2);
	}

	if (m_exitCounter > 0)
	{
		SetSpriteRenderersAlpha(1 - m_exitCounter / m_doorUseTime * 2);
	}
}

void PathFinder::AutoTargetProcess()
{
	if (!m_autoTarget) return;

	m_autoTargetCounter -= Time::GetDeltaTime();

	if (m_autoTargetCounter > 0) return;

	m_autoTargetCounter = 0.0f;
	SetTargetWithPlatformerBody(m_autoTarget);
}

void PathFinder::PathFindProcess()
{
	if (!m_hasTarget) return;

	m_hasTarget = false;

	std::unique_lock<std::mutex> lock(m_mutex);
	{
		m_threadTarget = m_target;
	}
	lock.unlock();

	m_cv.notify_one();
}

void PathFinder::UpdatePathProcess()
{
	if (m_threadPath.size() == 0) return;

	std::unique_lock<std::mutex> lock(m_mutex);
	{
		m_path = std::move(m_threadPath);
		m_currentIndex = 0;
	}
}

void PathFinder::EnterDoor()
{
	if (!m_readyToEnterDoor) return;

	// 문에서 나옵니다.

	m_readyToEnterDoor = false;
	m_readyToExitDoor = true;
	m_exitCounter = m_doorUseTime * 0.5f;
	SetBottom(m_exitPos);
	SetSpriteRenderersDirection(m_exitHDir);
	OnBeginToExitDoor();
}

void PathFinder::ThreadWork()
{
	while (1)
	{
		Vec2 target;
		std::unique_lock<std::mutex> lock(m_mutex);
		{
			m_cv.wait(lock);
			if (m_stopThread) break;
			if (m_threadPath.size() != 0) continue;
			target = m_threadTarget;
		}
		lock.unlock();

		std::vector<StageGraphPathNode> result;
		StageMap::PathFind(GetBottom(), m_threadTarget, &result);
		// 길 찾기 결과를 저장(이동)합니다.
		m_mutex.lock();
		{
			m_threadPath = std::move(result);
		}
		m_mutex.unlock();
	}
}
