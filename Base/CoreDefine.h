#pragma once

// Component Ŭ������ �Լ��� �������̵� �մϴ�.
// �׸��� DontUse##FunctionName() �Լ��� ������ �����մϴ�.
// �Լ� ȣ�� �÷��׸� ��Ȱ��ȭ ��Ű�� �ʰ� �˴ϴ�.
#define COMPONENT_OVERRIDE(FunctionName)				\
protected: virtual void DontUse##FunctionName() {}		\
protected: virtual void FunctionName() override;