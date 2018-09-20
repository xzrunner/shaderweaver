#pragma once

#include "shaderweaver/Node.h"
#include "shaderweaver/VariableType.h"

namespace sw
{
namespace node
{

class ColorMap : public sw::Node
{
public:
	ColorMap()
		: Node("ColorMap")
	{
		InitVariables({
			{ t_col3, "col" },
			{ t_col3, "rmap" },
			{ t_col3, "gmap" },
			{ t_col3, "bmap" },
		}, {
			{ t_col3, "col_map" },
		}, {
			{ t_flt1, "s" },
			{ t_flt1, "k" },
			{ t_flt1, "r_valid" },
			{ t_flt1, "cmp_gr" },
			{ t_flt1, "cmp_br" },
			{ t_flt3, "dr" },
			{ t_flt1, "g_valid" },
			{ t_flt1, "cmp_rg" },
			{ t_flt1, "cmp_bg" },
			{ t_flt3, "dg" },
			{ t_flt1, "b_valid" },
			{ t_flt1, "cmp_rb" },
			{ t_flt1, "cmp_gb" },
			{ t_flt3, "db" },
		});
	}

	enum InputID
	{
		ID_COL = 0,
		ID_RMAP,
		ID_GMAP,
		ID_BMAP,
	};

protected:
	virtual std::string GetHeader() const override
	{
		return "#define sw_neql(a, b) (step(0.0001, abs((a) - (b))))\n";
	}

	virtual std::string GetBody() const override
	{
		return R"(
(s) = 1.2;
(k) = (col).r + (col).g + (col).b;

(r_valid) = step(0.5, sw_neql((rmap).r, 1.0) + sw_neql((rmap).g, 0.0) + sw_neql((rmap).b, 0.0));
(cmp_gr)  = step((col).g * (s), (col).r);
(cmp_br)  = step((col).b * (s), (col).r);
(dr)      = ((rmap).rgb * (k) - (col)) * (r_valid) * (cmp_gr) * (cmp_br);

(g_valid) = step(0.5, sw_neql((gmap).r, 0.0) + sw_neql((gmap).g, 1.0) + sw_neql((gmap).b, 0.0));
(cmp_rg)  = step((col).r * (s), (col).g);
(cmp_bg)  = step((col).b * (s), (col).g);
(dg)      = ((gmap).rgb * (k) - (col)) * (g_valid) * (cmp_rg) * (cmp_bg);

(b_valid) = step(0.5, sw_neql((bmap).r, 0.0) + sw_neql((bmap).g, 0.0) + sw_neql((bmap).b, 1.0));
(cmp_rb)  = step((col).r * (s), (col).b);
(cmp_gb)  = step((col).g * (s), (col).b);
(db)      = ((bmap).rgb * (k) - (col)) * (b_valid) * (cmp_rb) * (cmp_gb);

(col_map) = (dr) + (dg) + (db) + (col);
)" + 1;
	}

}; // ColorMap

}
}