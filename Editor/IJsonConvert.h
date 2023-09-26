#pragma once

class IJsonConvert abstract
{
	PUBLIC virtual Json::Value ToJson() const = 0;
	PUBLIC virtual string JsonType() const = 0;
};

