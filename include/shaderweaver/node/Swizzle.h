#pragma once

#include "shaderweaver/Node.h"
#include "shaderweaver/NodeHelper.h"
#include "shaderweaver/VariableType.h"

#include <cpputil/StringHelper.h>

namespace sw
{
namespace node
{

class Swizzle : public sw::Node
{
public:
	static const size_t CHANNEL_COUNT = 4;

	enum ChannelType
	{
		CHANNEL_R = 0,
		CHANNEL_G,
		CHANNEL_B,
		CHANNEL_A
	};

public:
	Swizzle(const std::array<ChannelType, CHANNEL_COUNT>& channels)
		: Node("Swizzle")
		, m_channels(channels)
	{
		InitVariables({
			{ 0,  "_in" },
		}, {
			{ 0,  "_out" },
		}, {
		});

		m_dim_group = { 0, MAX_IMPORTS_COUNT };
	}

protected:
	virtual std::string GetBody() const override
	{
		const char* COL[4] = { "r", "g", "b", "a" };

		auto dim = GetImports()[0].var.GetType().dim;
		assert(GetExports()[0].var.GetType().dim == dim);
		switch (dim)
		{
		case 4:
			return cpputil::StringHelper::Format("#_out# = vec4(#_in#.%s, #_in#.%s, #_in#.%s, #_in#.%s);\n",
				COL[m_channels[0]], COL[m_channels[1]], COL[m_channels[2]], COL[m_channels[3]]);
		case 3:
			return cpputil::StringHelper::Format("#_out# = vec3(#_in#.%s, #_in#.%s, #_in#.%s);\n",
				COL[m_channels[0]], COL[m_channels[1]], COL[m_channels[2]]);
		case 2:
			return cpputil::StringHelper::Format("#_out# = vec2(#_in#.%s, #_in#.%s);\n",
				COL[m_channels[0]], COL[m_channels[1]]);
		case 1:
			return cpputil::StringHelper::Format("#_out# = #_in#;\n", COL[m_channels[0]]);
		default:
			return "";
		}
	}

private:
	std::array<ChannelType, CHANNEL_COUNT> m_channels
		= { CHANNEL_R, CHANNEL_G, CHANNEL_B, CHANNEL_A };

	RTTR_ENABLE(Node)

}; // Swizzle

}
}