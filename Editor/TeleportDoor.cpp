#include "editor_stdafx.h"
#include "TeleportDoor.h"

// https://blog.naver.com/PostView.nhn?isHttpsRedirect=true&blogId=drvoss&logNo=220940541715&parentCategoryNo=&categoryNo=32&viewDate=&isShowPopularPosts=true&from=search

void TeleportDoor::Awake()
{
    EditorObject::Awake();

    m_useCenter = false;

    CreatePair();
}

void TeleportDoor::Start()
{
    EditorObject::Start();

    m_doorRenderers[0]->SetSprite(m_sprite);
    m_doorRenderers[1]->SetSprite(m_sprite);
}

void TeleportDoor::BeforeRender()
{
    m_renderer->SetSprite(nullptr);
}

void TeleportDoor::Render()
{
    EditorObject::Render();

    const Vec2& p0 = m_doorPoint[0]->GetTransform()->GetPosition();
    const Vec2& p1 = m_doorPoint[1]->GetTransform()->GetPosition();

    auto line = GraphicDevice::GetLineDevice();
    Draw2DLineParameters lineParam;
    lineParam.color = Color(1, 0, 0, 1);
    line->Draw2DLine(p0, p1, &lineParam);
}

void TeleportDoor::OnDestroy()
{
    EditorObject::OnDestroy();

    Release();
}

void TeleportDoor::OnDestroyChildDoor(Object* object)
{
    Release();
}

void TeleportDoor::Release()
{
    if (m_doorPoint[0])
    {
        m_doorPoint[0]->OnDestoryCallback -= Function<void, Object*>(this, &TeleportDoor::OnDestroyChildDoor);
        m_doorPoint[0]->GetGameObject()->Destroy();
        m_doorPoint[0] = nullptr;
    }
    if (m_doorPoint[1])
    {
        m_doorPoint[1]->OnDestoryCallback -= Function<void, Object*>(this, &TeleportDoor::OnDestroyChildDoor);
        m_doorPoint[1]->GetGameObject()->Destroy();
        m_doorPoint[1] = nullptr;
    }
    this->GetGameObject()->Destroy();
}

Json::Value TeleportDoor::ToJson() const
{
    Json::Value value = EditorObject::ToJson();

    /*float*/	value["door_0_x"] = m_doorPoint[0]->GetTransform()->GetPosition().x;
    /*float*/	value["door_0_y"] = m_doorPoint[0]->GetTransform()->GetPosition().y;
    /*float*/	value["door_1_x"] = m_doorPoint[1]->GetTransform()->GetPosition().x;
    /*float*/	value["door_1_y"] = m_doorPoint[1]->GetTransform()->GetPosition().y;

    return value;
}

TransformableRect* TeleportDoor::GetDoorPointA() const
{
    return m_doorPoint[0];
}

TransformableRect* TeleportDoor::GetDoorPointB() const
{
    return m_doorPoint[1];
}

void TeleportDoor::CreatePair()
{
    auto o0 = GameObject::CreateChild(GetGameObject());
    auto o1 = GameObject::CreateChild(GetGameObject());

    o0->GetTransform()->SetLocalPosition(Vec2::up() * 50);
    o1->GetTransform()->SetLocalPosition(Vec2::down() * 50);

    TransformableRect* point0 = o0->AddComponent<TransformableRect>();
    TransformableRect* point1 = o1->AddComponent<TransformableRect>();
    point0->OnDestoryCallback += Function<void, Object*>(this, &TeleportDoor::OnDestroyChildDoor);
    point1->OnDestoryCallback += Function<void, Object*>(this, &TeleportDoor::OnDestroyChildDoor);

    point0->m_useLeft = false;
    point0->m_useTop = false;
    point0->m_useRight = false;
    point0->m_useBottom = false;

    point1->m_useLeft = false;
    point1->m_useTop = false;
    point1->m_useRight = false;
    point1->m_useBottom = false;

    auto ren0 = o0->AddComponent<SpriteRenderer>();
    auto ren1 = o1->AddComponent<SpriteRenderer>();

    m_doorPoint[0] = point0;
    m_doorPoint[1] = point1;

    m_doorRenderers[0] = ren0;
    m_doorRenderers[1] = ren1;
}
