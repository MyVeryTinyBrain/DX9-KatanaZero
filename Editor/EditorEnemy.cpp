#include "editor_stdafx.h"
#include "EditorEnemy.h"

void EditorEnemy::Awake()
{
    EditorObject::Awake();

    m_rally = false;
}

void EditorEnemy::Render()
{
    EditorObject::Render();

    if (m_rally)
    {
        Vec2 v[3];
        v[0] = m_rallyPoint[0]->GetTransform()->GetPosition();
        v[1] = GetTransform()->GetPosition();
        v[2] = m_rallyPoint[1]->GetTransform()->GetPosition();

        auto line = GraphicDevice::GetLineDevice();
        Draw2DLineParameters lineParam;
        lineParam.color = Color(1, 0, 0, 1);
        line->Draw2DChain(v, 3, &lineParam);
     
        auto text = GraphicDevice::GetTextDevice();
        WriteTextParameters textParam;
        textParam.hAlignment = TextHAlignment::Center;
        textParam.position = v[0];
        text->WriteText(L"A", &textParam);

        textParam.position = v[2];
        text->WriteText(L"B", &textParam);
    }
}

Json::Value EditorEnemy::ToJson() const
{
    Json::Value value = EditorObject::ToJson();

    /*bool*/        value["rally"] = m_rally;
    if (m_rally)
    {
        /*float*/	value["rally_0_x"] = m_rallyPoint[0]->GetTransform()->GetPosition().x;
        /*float*/	value["rally_0_y"] = m_rallyPoint[0]->GetTransform()->GetPosition().y;
        /*float*/	value["rally_1_x"] = m_rallyPoint[1]->GetTransform()->GetPosition().x;
        /*float*/	value["rally_1_y"] = m_rallyPoint[1]->GetTransform()->GetPosition().y;
    }
    else
    {
        /*float*/	value["rally_0_x"] = GetTransform()->GetPosition().x;
        /*float*/	value["rally_0_y"] = GetTransform()->GetPosition().y;
        /*float*/	value["rally_1_x"] = GetTransform()->GetPosition().x;
        /*float*/	value["rally_1_y"] = GetTransform()->GetPosition().y;
    }

    return value;
}

void EditorEnemy::SetRally(bool enable)
{
    if (enable && !m_rally)
    {
        auto o0 = GameObject::CreateChild(GetGameObject());
        auto o1 = GameObject::CreateChild(GetGameObject());

        o0->GetTransform()->SetLocalPosition(Vec2::right() * 25);
        o1->GetTransform()->SetLocalPosition(Vec2::left() * 25);

        auto rally0 = o0->AddComponent<TransformableRect>();
        auto rally1 = o1->AddComponent<TransformableRect>();
        
        rally0->m_useLeft = false;
        rally0->m_useTop = false;
        rally0->m_useRight = false;
        rally0->m_useBottom = false;

        rally1->m_useLeft = false;
        rally1->m_useTop = false;
        rally1->m_useRight = false;
        rally1->m_useBottom = false;

        m_rallyPoint[0] = rally0;
        m_rallyPoint[1] = rally1;

        m_rally = true;
    }
    else if (!enable && m_rally)
    {
        m_rallyPoint[0]->GetGameObject()->Destroy();
        m_rallyPoint[1]->GetGameObject()->Destroy();

        m_rallyPoint[0] = nullptr;
        m_rallyPoint[1] = nullptr;

        m_rally = false;
    }
}

bool EditorEnemy::IsRally() const
{
    return m_rally;
}

TransformableRect* EditorEnemy::GetRallyPointA() const
{
    return m_rallyPoint[0];
}

TransformableRect* EditorEnemy::GetRallyPointB() const
{
    return m_rallyPoint[1];
}
