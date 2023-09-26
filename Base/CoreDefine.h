#pragma once

// Component 클래스의 함수를 오버라이드 합니다.
// 그리고 DontUse##FunctionName() 함수의 내용을 변경합니다.
// 함수 호출 플래그를 비활성화 시키지 않게 됩니다.
#define COMPONENT_OVERRIDE(FunctionName)				\
protected: virtual void DontUse##FunctionName() {}		\
protected: virtual void FunctionName() override;