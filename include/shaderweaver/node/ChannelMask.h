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
	ChannelMask(uint32_t channels = 0)
		: Node("ChannelMask")
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

	void SetChannels(uint32_t channels) { m_channels = channels; }

	static const uint32_t CHANNEL_R = 0x1;
	static const uint32_t CHANNEL_G = 0x2;
	static const uint32_t CHANNEL_B = 0x4;
	static const uint32_t CHANNEL_A = 0x8;

protected:
	virtual std::string GetBody() const override
	{
		if (m_channels == 0)
		{
			int n = m_exports[0].var.GetType().dim;
			assert(n >= 1 && n <= 4);
			switch (n)
			{
			case 1:
				return "#_out# = 0;\n";
			case 2:
				return "#_out# = vec2(0, 0);\n";
			case 3:
				return "#_out# = vec3(0, 0, 0);\n";
			case 4:
				return "#_out# = vec4(0, 0, 0, 0);\n";
			default:
				return "";
			}
		}
		else if (m_channels == (CHANNEL_R & CHANNEL_G & CHANNEL_B & CHANNEL_A))
		{
			return "#_out# = #_in#;\n";
		}
		else
		{
            int dim_out = 0;
            if (m_channels & CHANNEL_R) {
                ++dim_out;
            }
            if (m_channels & CHANNEL_G) {
                ++dim_out;
            }
            if (m_channels & CHANNEL_B) {
                ++dim_out;
            }
            if (m_channels & CHANNEL_A) {
                ++dim_out;
            }

            int dim_in = m_imports[0].var.GetType().dim;
            assert(dim_in >= 1 && dim_in <= 4 && dim_in >= dim_out);
			switch (dim_out)
			{
			case 1:
				return cpputil::StringHelper::Format(
					"#_out# = %s;\n",
					(m_channels & CHANNEL_R) ? (dim_in == 1 ? "#_in#" : "#_in#.r") : "0"
				);
			case 2:
				return cpputil::StringHelper::Format(
					"#_out# = vec2(%s, %s);\n",
					(m_channels & CHANNEL_R) ? "#_in#.r" : "0",
					(m_channels & CHANNEL_G) ? "#_in#.g" : "0"
				);
			case 3:
				return cpputil::StringHelper::Format(
					"#_out# = vec3(%s, %s, %s);\n",
					(m_channels & CHANNEL_R) ? "#_in#.r" : "0",
					(m_channels & CHANNEL_G) ? "#_in#.g" : "0",
					(m_channels & CHANNEL_B) ? "#_in#.b" : "0"
				);
			case 4:
				return cpputil::StringHelper::Format(
					"#_out# = vec4(%s, %s, %s, %s);\n",
					(m_channels & CHANNEL_R) ? "#_in#.r" : "0",
					(m_channels & CHANNEL_G) ? "#_in#.g" : "0",
					(m_channels & CHANNEL_B) ? "#_in#.b" : "0",
					(m_channels & CHANNEL_A) ? "#_in#.a" : "0"
				);
			default:
				return "";
			}
		}
	}

private:
	uint32_t m_channels = CHANNEL_R | CHANNEL_G | CHANNEL_B | CHANNEL_A;

	RTTR_ENABLE(Node)

}; // ChannelMask

}
}