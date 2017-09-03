#include "MrbTexture.hpp"

#include "MrbColorF.hpp"
#include "MrbEmoji.hpp"
#include "MrbPoint.hpp"
#include "MrbTextureRegion.hpp"
#include "Util.hpp"
#include "mruby/array.h"
#include "mruby/class.h"
#include "mruby/data.h"
#include "mruby/string.h"
#include "mruby/value.h"

//----------------------------------------------------------
namespace siv3druby {
namespace {
Texture& toCpp(mrb_value self)
{
    return *((Texture*)DATA_PTR(self));
}

void free(mrb_state *mrb, void *p)
{
    if (p) {
        delete ((Texture*)p);
    }
}

struct mrb_data_type data_type = { "siv3d_texture", free };

mrb_value initialize(mrb_state *mrb, mrb_value self)
{
    mrb_value src;
    mrb_int desc = static_cast<int>(TextureDesc::Unmipped);
    mrb_get_args(mrb, "o|i", &src, &desc);

    Texture* obj;

    if (mrb_string_p(src)) {
        obj = new Texture(CharacterSet::FromUTF8(mrb_string_value_ptr(mrb, src)), static_cast<TextureDesc>(desc));
    } else if (MrbEmoji::IsInstance(mrb, src)) {
        obj = new Texture(*MrbEmoji::ToCpp(mrb, src), static_cast<TextureDesc>(desc));
    } else {
        mrb_raise(mrb, E_TYPE_ERROR, "wrong argument class");
    }

    mrb_data_init(self, obj, &data_type);
    return self;
}

mrb_value draw(mrb_state *mrb, mrb_value self)
{
    mrb_float x, y;
    mrb_value color;
    int argc = mrb_get_args(mrb, "|ffo", &x, &y, &color);

    switch (argc)
    {
        case 3:
            toCpp(self).draw(x, y, Util::ToColor(mrb, color));
            break;
        case 2:
            toCpp(self).draw(x, y);
            break;
        default:
            toCpp(self).draw();
            break;
    }

    return mrb_nil_value();
}

mrb_value resize(mrb_state *mrb, mrb_value self)
{
    mrb_float x, y;
    int argc = mrb_get_args(mrb, "ff", &x, &y);

    return MrbTextureRegion::ToMrb(
        mrb,
        new TextureRegion(toCpp(self).resize(x, y))
    );
}

mrb_value scale(mrb_state *mrb, mrb_value self)
{
    mrb_float x, y;
    int argc = mrb_get_args(mrb, "ff", &x, &y);

    return MrbTextureRegion::ToMrb(
        mrb,
        new TextureRegion(toCpp(self).scale(x, y))
        );
}

}

//----------------------------------------------------------
void MrbTexture::Init(mrb_state* mrb)
{
    struct RClass *cc = mrb_define_class(mrb, "Texture", mrb->object_class);

    mrb_define_method(mrb, cc, "initialize", initialize, MRB_ARGS_ARG(1, 1));
    mrb_define_method(mrb, cc, "draw", draw, MRB_ARGS_OPT(3));
    mrb_define_method(mrb, cc, "resize", resize, MRB_ARGS_REQ(2));
    mrb_define_method(mrb, cc, "scale", scale, MRB_ARGS_REQ(2));
}

//----------------------------------------------------------
mrb_value MrbTexture::ToMrb(mrb_state *mrb, Texture *ptr)
{
    return ToMrb(mrb, mrb_class_get(mrb, "Texture"), ptr);
}

//----------------------------------------------------------
mrb_value MrbTexture::ToMrb(mrb_state *mrb, struct RClass *cc, Texture *ptr)
{
    struct RData *data = mrb_data_object_alloc(mrb, cc, ptr, &data_type);
    return mrb_obj_value(data);
}

//----------------------------------------------------------
Texture* MrbTexture::ToCpp(mrb_state *mrb, mrb_value value)
{
    return ToCpp(mrb, mrb_class_get(mrb, "Texture"), value);
}

//----------------------------------------------------------
Texture* MrbTexture::ToCpp(mrb_state *mrb, struct RClass *cc, mrb_value value)
{
    if (!mrb_obj_is_instance_of(mrb, value, cc)) {
        mrb_raise(mrb, E_TYPE_ERROR, "wrong argument class");
    }

    return static_cast<Texture*>(DATA_PTR(value));
}

}
