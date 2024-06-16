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

	// ���� ��带 ����Ű�� �ε����� �������� ��ȸ�մϴ�.
	// �� ������ ���� ������ �Ÿ��� ����� ����� �� ��带 �ε����� �������� ���� ��带 ����Ű�� �����Դϴ�.
	for (size_t i = m_currentIndex + 1; i < m_path.size(); ++i)
	{
		// �ٵ��� �ٴں��� �� �������� �Ÿ��Դϴ�.
		float distance = Vec2::Distance(GetBottom(), m_path[i].position);

		// ��ǥ ���������� �Ÿ��� ������ ũ�⺸�� ���� ���
		if (distance < m_minDistance)
		{
			// @ ������ �����ϴ� ���
			// ���� ��尡 �� ����̸鼭
			// �� �� ���� ����� ������ ������
			// �� �� ����� �ⱸ�� �����ϴ� ���
			if (m_path[i].type == StageGraphType::DOOR && 
				m_path.size() - 1 >= i + 1 && 
				m_path[i + 1].type == StageGraphType::DOOR)
			{
				// ������ �����մϴ�.

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

			// @ ������� �����ϴ� ���
			// ���� ��尡 ��� ����̸鼭
			// �� ����� ���� �����ϸ�
			// �� ��� ���� ����� ������ �ִ� ���
			if (m_path[i].type == StageGraphType::STAIR && 
				m_path.size() - 1 >= i + 1 && 
				m_path[i + 1].type == StageGraphType::STAIR)
			{
				// �������� ������ ��쿡�� ����� �ֻ�ܿ��� �ణ �Ʒ������� ��ġ�� �����մϴ�.
				// �������� ���ؼ� �ϴ� �����Դϴ�.
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

			// @ ��ܿ��� ������ ���
			// ���� ��尡 ��� ����̸鼭
			// ���� ��尡 ��� ����̸鼭
			// ����� �� ��尡 ���� ��
			// �� ��� ���� ����� ������ �ִ� ���
			if (i > 0 &&
				m_path[i - 1].type == StageGraphType::STAIR &&
				m_path[i].type == StageGraphType::STAIR &&
				m_path.size() - 1 >= i + 1 &&
				m_path[i + 1].type == StageGraphType::GROUND)
			{
				// �ö󰡴� ������ ��쿡�� ����� �ֻ�ܿ��� �̵��ϴ� ���� �������� �ణ ��ġ�� �����մϴ�.
				// �������� ���ؼ� �ϴ� �����Դϴ�.
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

			// ���� ��带 ����Ű�� �ε����� �̵���ŵ�ϴ�.
			++m_currentIndex;
		}
		else
		{
			// �� �̻� ����� ��尡 ������ ������ Ż���մϴ�.
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

	// ���� ��Ȯ�� ������ ���� ���� ���� ���� ��� ������ ������ ����մϴ�.
	Vec2 direction = m_path[nextIndex].position - m_path[currentIndex].position;

	// �Ʒ��� ���ϴ� �����̸� �÷������� �浹�� ���ϴ�.
	m_body->SetThroughPlatform(direction.y < -0.1f && m_path[nextIndex].type != StageGraphType::DOOR);

	float hd = direction.x < 0 ? (-1.0f) : (+1.0f);

	// ������ �ٲ�� ��쿡
	if (hd != m_prevHD)
	{
		// ������ �ٲߴϴ�.
		m_prevHD = hd;
		SetSpriteRenderersDirection(hd);
		OnBeginToTurn(hd);
		return;
	}

	if (m_speed > 0)
		m_body->SetHorizontalVelocity(hd * m_speed);

	// ��������Ʈ�� ������ �ٲߴϴ�.
	// �����ص� ���� ���ٸ� �����ϼ���.
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
	/* ��� ����� */

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

	// ������ ���ɴϴ�.

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
		// �� ã�� ����� ����(�̵�)�մϴ�.
		m_mutex.lock();
		{
			m_threadPath = std::move(result);
		}
		m_mutex.unlock();
	}
}
