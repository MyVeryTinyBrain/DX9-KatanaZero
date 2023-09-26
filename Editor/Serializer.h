#pragma once

class IJsonConvert;

class Serializer
{
	PUBLIC static void Serialize(const wstring& path);
	PUBLIC static bool Deserialize(const wstring& path);
};

