#include "MrbVec2.hpp"

//----------------------------------------------------------
namespace siv3druby {
MrbObject<Vec2>::Inner MrbVec2::fInner;

//----------------------------------------------------------
void MrbVec2::Init(mrb_state* mrb)
{
    MrbObject::Init(mrb, "Vec2");

    mrb_define_method(mrb, Cc(), "initialize", initialize, MRB_ARGS_REQ(2));
    mrb_define_method(mrb, Cc(), "x", x, MRB_ARGS_NONE());
    mrb_define_method(mrb, Cc(), "y", y, MRB_ARGS_NONE());
}

//----------------------------------------------------------
mrb_value MrbVec2::initialize(mrb_state *mrb, mrb_value self)
{
    mrb_float x, y;
    mrb_get_args(mrb, "ff", &x, &y);

    Vec2* obj = new Vec2(x, y);

    mrb_data_init(self, obj, DataType());
    return self;
}

//----------------------------------------------------------
mrb_value MrbVec2::x(mrb_state *mrb, mrb_value self)
{
    return mrb_float_value(mrb, ToCpp(self).x);
}

//----------------------------------------------------------
mrb_value MrbVec2::y(mrb_state *mrb, mrb_value self)
{
    return mrb_float_value(mrb, ToCpp(self).y);
}

}
