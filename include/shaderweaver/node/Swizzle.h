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
	Swizzle(uint32_t channels[4])
		: Node("Swizzle")
	{
		memcpy(m_channels, channels, sizeof(m_channels));

		InitVariables({
			{ 0,  "_in" },
		}, {
			{ 0,  "_out" },
		}, {
		});

		m_dim_group = { 0, MAX_IMPORTS_COUNT };
	}

	enum ChannelType
	{
		CHANNEL_R = 0,
		CHANNEL_G,
		CHANNEL_B,
		CHANNEL_A
	};

protected:
	virtual std::string GetBody() const override
	{
		const char* COL[4] = { "r", "g", "b", "a" };

		auto dim = GetImports()[0].var.Type().dim;
		assert(GetExports()[0].var.Type().dim == dim);
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
	ChannelType m_channels[4] = { CHANNEL_R, CHANNEL_G, CHANNEL_B, CHANNEL_A };

}; // Swizzle

}
}