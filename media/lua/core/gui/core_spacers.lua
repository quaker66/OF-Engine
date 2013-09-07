--[[! File: lua/core/gui/core_spacers.lua

    About: Author
        q66 <quaker66@gmail.com>

    About: Copyright
        Copyright (c) 2013 OctaForge project

    About: License
        This file is licensed under MIT. See COPYING.txt for more information.

    About: Purpose
        Spacers are widgets that have something to do with space management -
        that is, actual spacers, offsetters, fillers etc.
]]

local max = math.max
local min = math.min
local abs = math.abs

local M = require("core.gui.core")

local capi = require("capi")
local hud_get_h = capi.hud_get_h

-- widget types
local register_class = M.register_class

-- children iteration
local loop_children, loop_children_r = M.loop_children, M.loop_children_r

-- scissoring
local clip_push, clip_pop = M.clip_push, M.clip_pop

-- base widgets
local Widget = M.get_class("Widget")

-- setters
local gen_setter = M.gen_setter

-- projection
local get_projection = M.get_projection

--[[! Struct: Spacer
    A spacer will give a widget a horizontal padding (pad_h) and a vertical
    padding (pad_v). There is no other meaning to it.
]]
M.Spacer = register_class("Spacer", Widget, {
    __ctor = function(self, kwargs)
        kwargs = kwargs or {}
        self.pad_h = kwargs.pad_h or 0
        self.pad_v = kwargs.pad_v or 0

        return Widget.__ctor(self, kwargs)
    end,

    layout = function(self)
        local ph, pv = self.pad_h, self.pad_v
        local w , h  = ph, pv

        loop_children(self, function(o)
            o.x = ph
            o.y = pv
            o:layout()

            w = max(w, o.x + o.w)
            h = max(h, o.y + o.h)
        end)

        self.w = w + ph
        self.h = h + pv
    end,

    adjust_children = function(self)
        local ph, pv = self.pad_h, self.pad_v
        Widget.adjust_children(self, ph, pv, self.w - 2 * ph,
            self.h - 2 * pv)
    end,

    --[[! Function: set_pad_h ]]
    set_pad_h = gen_setter "pad_h",

    --[[! Function: set_pad_v ]]
    set_pad_v = gen_setter "pad_v"
})

--[[! Struct: Filler
    A filler will fill at least min_w space horizontally and min_h space
    vertically. If the min_w property is -1, the filler will take all
    the available space (depending on aspect ratio), if min_h is -1,
    it'll take the full height (1). It's invisible.

    Negative min_w and min_h values are in pixels.
    They can also be functions, in which case their return value is used
    (the widget is passed as an argument for the call).

    There is also the clip_children boolean property defaulting to false.
    When true, it'll clip children inside - that's useful for, say, embedded
    floating windows.
]]
M.Filler = register_class("Filler", Widget, {
    __ctor = function(self, kwargs)
        kwargs = kwargs or {}
        self.min_w = kwargs.min_w or 0
        self.min_h = kwargs.min_h or 0

        self.clip_children = kwargs.clip_children or false

        return Widget.__ctor(self, kwargs)
    end,

    layout = function(self)
        Widget.layout(self)

        local min_w = self.min_w
        local min_h = self.min_h
        if type(min_w) == "function" then min_w = min_w(self) end
        if type(min_h) == "function" then min_h = min_h(self) end

        if min_w < 0 then min_w = abs(min_w) / hud_get_h() end
        if min_h < 0 then min_h = abs(min_h) / hud_get_h() end

        local proj = get_projection()
        if min_w == -1 then min_w = proj.pw end
        if min_h == -1 then min_h = proj.ph end

        self.w = max(self.w, min_w)
        self.h = max(self.h, min_h)
    end,

    --[[! Function: target
        Makes sure the filler can take input. Makes it useful for, say, button
        surfaces (when they should be invisible).
    ]]
    target = function(self, cx, cy)
        return Widget.target(self, cx, cy) or self
    end,

    draw = function(self, sx, sy)
        if self.clip_children then
            clip_push(sx, sy, self.w, self.h)
            Widget.draw(self, sx, sy)
            clip_pop()
        else
            return Widget.draw(self, sx, sy)
        end
    end,

    --[[! Function: set_min_w ]]
    set_min_w = gen_setter "min_w",

    --[[! Function: set_min_h ]]
    set_min_h = gen_setter "min_h",

    --[[! Function: set_clip_children ]]
    set_clip_children = gen_setter "clip_children"
})

--[[! Struct: Offsetter
    Offsets a widget by offset_h and offset_v properties.
]]
M.Offsetter = register_class("Offsetter", Widget, {
    __ctor = function(self, kwargs)
        kwargs = kwargs or {}
        self.offset_h = kwargs.offset_h or 0
        self.offset_v = kwargs.offset_v or 0

        return Widget.__ctor(self, kwargs)
    end,

    layout = function(self)
        Widget.layout(self)

        local oh, ov = self.offset_h, self.offset_v

        loop_children(self, function(o)
            o.x = o.x + oh
            o.y = o.y + ov
        end)

        self.w = self.w + oh
        self.h = self.h + ov
    end,

    adjust_children = function(self)
        local oh, ov = self.offset_h, self.offset_v
        Widget.adjust_children(self, oh, ov, self.w - oh, self.h - ov)
    end,

    --[[! Function: set_offset_h ]]
    set_offset_h = gen_setter "offset_h",

    --[[! Function: set_offset_v ]]
    set_offset_v = gen_setter "offset_v"
})
