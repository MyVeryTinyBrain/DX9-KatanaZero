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

	// ��������Ʈ�� ũ�⸦ ��ȯ�մϴ�.
	PUBLIC Vec2 GetSize() const;

	// ��������Ʈ Ÿ���� SlicedRect �ϋ� ��ȿ�� ��ȯ �Լ��Դϴ�.
	PUBLIC const RECT& GetSlicedRect() const;

	// ��������Ʈ Ÿ���� SlicedRect �ϋ� ��ȿ�� ��ȯ �Լ��Դϴ�.
	PUBLIC bool IsDontChangedCenter() const;

	// ��������Ʈ Ÿ���� SlicedXYIndex �ϋ� ��ȿ�� ��ȯ �Լ��Դϴ�.
	PUBLIC const size_t& GetIndexX() const;

	// ��������Ʈ Ÿ���� SlicedXYIndex �ϋ� ��ȿ�� ��ȯ �Լ��Դϴ�.
	PUBLIC const size_t& GetIndexY() const;

	// ��������Ʈ Ÿ���� SlicedIndex �ϋ� ��ȿ�� ��ȯ �Լ��Դϴ�.
	PUBLIC const size_t& GetIndex() const;

	// ��������Ʈ Ÿ���� SlicedXYIndex, SlicedIndex �ϋ� ��ȿ�� ��ȯ �Լ��Դϴ�.
	PUBLIC const size_t& GetCountX() const;

	// ��������Ʈ Ÿ���� SlicedXYIndex, SlicedIndex �ϋ� ��ȿ�� ��ȯ �Լ��Դϴ�.
	PUBLIC const size_t& GetCountY() const;

	// ���� �簢���� �����մϴ�.
	// ��������Ʈ Ÿ���� SlicedRect �϶� ��ȿ�� ���� �Լ��Դϴ�.
	PUBLIC void SetSlicedRect(const RECT& rect);

	// ���� �簢�� ���ÿ� �߽����� ���� �̹����� �߽������� ����ϴ� �÷����Դϴ�.
	PUBLIC void SetDontChangeCenter(bool enable);

	// X �ε����� �����մϴ�.
	// ��������Ʈ Ÿ���� SlicedIndex �ϋ� ��ȿ�� ���� �Լ��Դϴ�.
	PUBLIC void SetIndexX(const size_t& x);

	// Y �ε����� �����մϴ�.
	// ��������Ʈ Ÿ���� SlicedIndex �ϋ� ��ȿ�� ���� �Լ��Դϴ�.
	PUBLIC void SetIndexY(const size_t& y);

	// ���� �ε����� �����մϴ�.
	// ��������Ʈ Ÿ���� SlicedIndex �ϋ� ��ȿ�� ���� �Լ��Դϴ�.
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

