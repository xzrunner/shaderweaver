#pragma once

#include "shaderweaver/Node.h"
#include "shaderweaver/NodeHelper.h"
#include "shaderweaver/VariableType.h"

#include <cpputil/StringHelper.h>

#include <assert.h>

namespace sw
{
namespace node
{

class ChannelMask : public sw::Node
{
public:
	ChannelMask(uint32_t channels)
		: Node("ChannelMask")
		, m_channels(channels)
	{
		InitVariables({
			{ 0, "input" },
		}, {
			{ 0, "channel_mask" },
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
		if (m_channels == 0)
		{
			int n = m_exports[0].var.Type().dim;
			assert(n >= 1 && n <= 4);
			switch (n)
			{
			case 1:
				return "(channel_mask) = 0;\n";
			case 2:
				return "(channel_mask) = vec2(0, 0);\n";
			case 3:
				return "(channel_mask) = vec3(0, 0, 0);\n";
			case 4:
				return "(channel_mask) = vec4(0, 0, 0, 0);\n";
			default:
				return "";
			}
		}
		else if (m_channels == (CHANNEL_R & CHANNEL_G & CHANNEL_B & CHANNEL_A))
		{
			return "(channel_mask) = (input);\n";
		}
		else
		{
			int dim_out = m_exports[0].var.Type().dim;
			assert(dim_out >= 1 && dim_out <= 4);
			int dim_in = m_imports[0].var.Type().dim;
			assert(dim_in >= 1 && dim_in <= 4 && dim_in >= dim_out);
			switch (dim_out)
			{
			case 1:
				return cpputil::StringHelper::Format(
					"(channel_mask) = %s;\n",
					(m_channels & CHANNEL_R) ? (dim_in == 1 ? "(input)" : "(input).r") : "0"
				);
			case 2:
				return cpputil::StringHelper::Format(
					"(channel_mask) = vec2(%s, %s);\n",
					(m_channels & CHANNEL_R) ? "(input).r" : "0",
					(m_channels & CHANNEL_G) ? "(input).g" : "0"
				);
			case 3:
				return cpputil::StringHelper::Format(
					"(channel_mask) = vec3(%s, %s, %s);\n",
					(m_channels & CHANNEL_R) ? "(input).r" : "0",
					(m_channels & CHANNEL_G) ? "(input).g" : "0",
					(m_channels & CHANNEL_B) ? "(input).b" : "0"
				);
			case 4:
				return cpputil::StringHelper::Format(
					"(channel_mask) = vec4(%s, %s, %s, %s);\n",
					(m_channels & CHANNEL_R) ? "(input).r" : "0",
					(m_channels & CHANNEL_G) ? "(input).g" : "0",
					(m_channels & CHANNEL_B) ? "(input).b" : "0",
					(m_channels & CHANNEL_A) ? "(input).a" : "0"
				);
			default:
				return "";
			}
		}
	}

private:
	uint32_t m_channels = CHANNEL_R | CHANNEL_G | CHANNEL_B | CHANNEL_A;

}; // ChannelMask

}
}