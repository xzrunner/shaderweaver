#pragma once

#include "shaderweaver/Node.h"
#include "shaderweaver/VariableType.h"

#include <cpputil/StringHelper.h>

namespace sw
{
namespace node
{

class InvertColors : public sw::Node
{
public:
	InvertColors(uint32_t channels)
		: Node("InvertColors")
		, m_channels(channels)
	{
		InitVariables({
			{ 0, "_in" },
		}, {
			{ 0, "_out" },
		}, {
		});

		m_dim_group = { 0, MAX_IMPORTS_COUNT };
	}

	static const uint32_t CHANNEL_R = 0x1;
	static const uint32_t CHANNEL_G = 0x2;
	static const uint32_t CHANNEL_B = 0x4;
	static const uint32_t CHANNEL_A = 0x8;

protected:
	virtual std::string GetBody() const override
	{
		auto dim = GetImports()[0].var.Type().dim;
		switch (dim)
		{
		case 4:
			return cpputil::StringHelper::Format("#_out# = vec4(%s#_in#.r, %s#_in#.g, %s#_in#.b, %s#_in#.a);\n",
				m_channels & CHANNEL_R ? "1 - " : "", m_channels & CHANNEL_G ? "1 - " : "", m_channels & CHANNEL_B ? "1 - " : "", m_channels & CHANNEL_A ? "1 - " : "");
		case 3:
			return cpputil::StringHelper::Format("#_out# = vec3(%s#_in#.r, %s#_in#.g, %s#_in#.b);\n",
				m_channels & CHANNEL_R ? "1 - " : "", m_channels & CHANNEL_G ? "1 - " : "", m_channels & CHANNEL_B ? "1 - " : "");
		case 2:
			return cpputil::StringHelper::Format("#_out# = vec2(%s#_in#.r, %s#_in#.g);\n",
				m_channels & CHANNEL_R ? "1 - " : "", m_channels & CHANNEL_G ? "1 - " : "");
		case 1:
			return cpputil::StringHelper::Format("#_out# = vec4(%s#_in#.r);\n", m_channels & CHANNEL_R ? "1 - " : "");
		default:
			return "";
		}
	}

private:
	uint32_t m_channels;

}; // InvertColors

}
}