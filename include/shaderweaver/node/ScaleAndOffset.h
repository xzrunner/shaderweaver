#pragma once

#include "shaderweaver/Node.h"

namespace sw
{
namespace node
{

class ScaleAndOffset : public sw::Node
{
public:
    ScaleAndOffset()
        : Node("ScaleAndOffset")
    {
		InitVariables({
			{ 0, "_in" },
			{ 0, "scale" },
            { 0, "offset" },
		}, {
			{ 0, "_out" },
		}, {
		});

        m_dim_group = { ID_VALUE, ID_SCALE, ID_OFFSET, MAX_IMPORTS_COUNT };
    }

    enum InputID
    {
        ID_VALUE = 0,
        ID_SCALE,
        ID_OFFSET
    };

protected:
    virtual std::string GetBody() const override
    {
        return "#_out# = #_in# * #scale# + #offset#;\n";
    }

    RTTR_ENABLE(Node)

}; // ScaleAndOffset

}
}