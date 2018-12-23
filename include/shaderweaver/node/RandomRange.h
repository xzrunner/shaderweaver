#pragma once

#include "shaderweaver/Node.h"
#include "shaderweaver/NodeHelper.h"

namespace sw
{
namespace node
{

class RandomRange : public sw::Node
{
public:
	RandomRange()
		: Node("RandomRange")
	{
		InitVariables({
			{ t_flt2, "seed" },
			{ 0,      "minimum" },
			{ 0,      "maximum" },
		}, {
			{ 0,      "_out" },
		}, {
			{ t_flt1, "randomno" },
		});

		m_dim_group = { ID_MIN, ID_MAX, MAX_IMPORTS_COUNT };
	}

	enum InputID
	{
		ID_SEED = 0,
		ID_MIN,
		ID_MAX
	};

protected:
	virtual std::string GetBody() const override
	{
		return R"(
#randomno# = fract(sin(dot(#seed#, vec2(12.9898, 78.233)))*43758.5453);
#_out# = mix(#minimum#, #maximum#, #randomno#);
)" + 1;
	}

	RTTR_ENABLE(Node)

}; // RandomRange

}
}