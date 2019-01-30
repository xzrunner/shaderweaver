#pragma once

#include "shaderweaver/Node.h"
#include "shaderweaver/VariableType.h"

namespace sw
{
namespace node
{

class Skin : public Node
{
public:
    Skin()
        : Node("Skin")
    {
		InitVariables({
            { t_pos4, "position", false },
            { t_pos4, "blend_indices", false },
            { t_pos4, "blend_weights", false },
		}, {
            { t_pos4, "_out" },
		}, {
            { t_mat4 | t_unif, "bone_matrix" },
		});

        // mat4 u_bone_matrix[60];
        auto type = m_internal[0].GetType();
        type.SetArray(60);
        m_internal[0].SetType(type);
    }

protected:
    virtual std::string GetHeader() const override
    {
        return R"(
int round2int(float f)
{
	return int(floor(f * 255.0 + 0.5));
}
)" + 1;
    }

    virtual std::string GetBody() const override
    {
		return R"(
#_out#  = #u_bone_matrix#[round2int(#blend_indices#.x)] * #position# * #blend_weights#.x;
#_out# += #u_bone_matrix#[round2int(#blend_indices#.y)] * #position# * #blend_weights#.y;
#_out# += #u_bone_matrix#[round2int(#blend_indices#.z)] * #position# * #blend_weights#.z;
#_out# += #u_bone_matrix#[round2int(#blend_indices#.w)] * #position# * #blend_weights#.w;
)" + 1;
    }

    RTTR_ENABLE(Node)

}; // Skin

}
}