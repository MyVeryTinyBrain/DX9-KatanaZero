#pragma once

class Texture;

enum class SpriteType
{
	Single, SlicedRect, SlicedXYIndex, SlicedIndex
};

class Sprite
{
	PUBLIC Sprite(const Sprite& sprite);
	PUBLIC Sprite(wstring textureKey);
	PUBLIC Sprite(Texture* texture);
	PUBLIC Sprite(Texture* texture, const RECT& slicedRect, bool dontChangeCenter);
	PUBLIC Sprite(Texture* texture, const size_t& countX, const size_t& countY, const size_t& indexX, const size_t& indexY);
	PUBLIC Sprite(Texture* texture, const size_t& countX, const size_t& countY, const size_t& index);
	PUBLIC ~Sprite();

	PUBLIC Texture* GetTexture() const;
	PUBLIC const SpriteType& GetSpriteType() const;

	// 스프라이트의 크기를 반환합니다.
	PUBLIC Vec2 GetSize() const;

	// 스프라이트 타입이 SlicedRect 일떄 유효한 반환 함수입니다.
	PUBLIC const RECT& GetSlicedRect() const;

	// 스프라이트 타입이 SlicedRect 일떄 유효한 반환 함수입니다.
	PUBLIC bool IsDontChangedCenter() const;

	// 스프라이트 타입이 SlicedXYIndex 일떄 유효한 반환 함수입니다.
	PUBLIC const size_t& GetIndexX() const;

	// 스프라이트 타입이 SlicedXYIndex 일떄 유효한 반환 함수입니다.
	PUBLIC const size_t& GetIndexY() const;

	// 스프라이트 타입이 SlicedIndex 일떄 유효한 반환 함수입니다.
	PUBLIC const size_t& GetIndex() const;

	// 스프라이트 타입이 SlicedXYIndex, SlicedIndex 일떄 유효한 반환 함수입니다.
	PUBLIC const size_t& GetCountX() const;

	// 스프라이트 타입이 SlicedXYIndex, SlicedIndex 일떄 유효한 반환 함수입니다.
	PUBLIC const size_t& GetCountY() const;

	// 분할 사각형을 설정합니다.
	// 스프라이트 타입이 SlicedRect 일때 유효한 설정 함수입니다.
	PUBLIC void SetSlicedRect(const RECT& rect);

	// 분할 사각형 사용시에 중심점을 원래 이미지의 중심점으로 사용하는 플래그입니다.
	PUBLIC void SetDontChangeCenter(bool enable);

	// X 인덱스를 설정합니다.
	// 스프라이트 타입이 SlicedIndex 일떄 유효한 설정 함수입니다.
	PUBLIC void SetIndexX(const size_t& x);

	// Y 인덱스를 설정합니다.
	// 스프라이트 타입이 SlicedIndex 일떄 유효한 설정 함수입니다.
	PUBLIC void SetIndexY(const size_t& y);

	// 선형 인덱스를 설정합니다.
	// 스프라이트 타입이 SlicedIndex 일떄 유효한 설정 함수입니다.
	PUBLIC void SetIndex(const size_t& i);

	PRIVATE Texture* m_texture;
	PUBLIC SpriteType m_type;
	PUBLIC union
	{
		struct // SlicedRect
		{
			RECT slicedRect;
			bool dontChangeCenter;
		}USlicedRect;
		struct // SlicedXYIndex
		{
			size_t countX;
			size_t countY;
			size_t indexX;
			size_t indexY;
		}USlicedXYIndex;
		struct // SlicedIndex
		{
			size_t countX;
			size_t countY;
			size_t index;
		}USlidexIndex;
	}U;
};

