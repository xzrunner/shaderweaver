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
		InitVariables({
		}, {
			{ t_flt1, "time" },
			{ t_flt1, "sine_time" },
			{ t_flt1, "cosine_time" },
			{ t_flt1, "delta_time" },
			{ t_flt1, "smooth_delta" },
		}, {
			{ t_unif | t_flt4, TimeName() },
			{ t_unif | t_flt4, SineTimeName() },
			{ t_unif | t_flt4, CosTimeName() },
			{ t_unif | t_flt4, DeltaTimeName() },
		});
	}

	static const char* TimeName()      { return "u_time"; }
	static const char* SineTimeName()  { return "u_sine_time"; }
	static const char* CosTimeName()   { return "u_cos_time"; }
	static const char* DeltaTimeName() { return "u_delta_time"; }

protected:
	virtual std::string GetBody() const override
	{
		return cpputil::StringHelper::Format(R"(
#time#         = #%s#.y;
#sine_time#    = #u_sine_time#.w;
#cosine_time#  = #u_cos_time#.w;
#delta_time#   = #u_delta_time#.x;
#smooth_delta# = #u_delta_time#.z;
)" + 1, TimeName(), SineTimeName(), CosTimeName(), DeltaTimeName(), DeltaTimeName());
	}

	RTTR_ENABLE(Node)

}; // Time

}
}