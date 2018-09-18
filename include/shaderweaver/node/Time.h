#pragma once

#include "shaderweaver/Node.h"
#include "shaderweaver/VariableType.h"

#include <cpputil/StringHelper.h>

#include <SM_Vector.h>

namespace sw
{
namespace node
{

class Time : public Node
{
public:
	Time()
		: Node("Time")
	{
		AddVariable(Variable(t_n_out | t_flt1, "time"));
		AddVariable(Variable(t_n_out | t_flt1, "sine_time"));
		AddVariable(Variable(t_n_out | t_flt1, "cosine_time"));
		AddVariable(Variable(t_n_out | t_flt1, "delta_time"));
		AddVariable(Variable(t_n_out | t_flt1, "smooth_delta"));

		AddVariable(Variable(t_n_mid | t_unif | t_flt4, "u_time"));
		AddVariable(Variable(t_n_mid | t_unif | t_flt4, "u_sine_time"));
		AddVariable(Variable(t_n_mid | t_unif | t_flt4, "u_cos_time"));
		AddVariable(Variable(t_n_mid | t_unif | t_flt4, "u_delta_time"));
	}

protected:
	virtual std::string GetBody() const override
	{
		return R"(
(time)         = (u_time).y;
(sine_time)    = (u_sine_time).w;
(cosine_time)  = (u_cos_time).w;
(delta_time)   = (u_delta_time).x;
(smooth_delta) = (u_delta_time).z;
)" + 1;
	}

}; // Time

}
}