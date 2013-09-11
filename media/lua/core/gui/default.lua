--[[! File: lua/core/gui/default.lua

    About: Author
        q66 <quaker66@gmail.com>

    About: Copyright
        Copyright (c) 2013 OctaForge project

    About: License
        This file is licensed under MIT. See COPYING.txt for more information.

    About: Purpose
        Some default menus.
]]

local capi = require("capi")
local signal = require("core.events.signal")
local gui = require("core.gui.core")

local world = gui.get_world()

local Color = gui.Color

-- buttons

local btnv = { __properties = { "label" } }
gui.Button.__variants = { default = btnv }

local btnv_init_clone = |self, btn| do
    local lbl = gui.Label { text = btn.label }
    self:append(lbl)
    signal.connect(btn, "label_changed", |b, t| do lbl:set_text(t) end)
end

local btn_build_variant = |r, g, b| gui.Gradient {
    color = 0x0, color2 = 0x303030, clamp_h = true,
    gui.Outline {
        color = Color(r, g, b), clamp_h = true, gui.Spacer {
            pad_h = 0.01, pad_v = 0.005, init_clone = btnv_init_clone
        }
    }
}

local btn_build_variant_nobg = || gui.Filler {
    clamp_h = true, gui.Spacer {
        pad_h = 0.01, pad_v = 0.005, init_clone = btnv_init_clone
    }
}

btnv["default"     ] = btn_build_variant(255, 255, 255)
btnv["hovering"    ] = btn_build_variant(225, 225, 225)
btnv["clicked_left"] = btn_build_variant(192, 192, 192)

local mbtnv, smbtnv =
    { __properties  = { "label" } },
    { __properties  = { "label" } }
gui.Menu_Button.__variants = { default = mbtnv, submenu = smbtnv }

mbtnv["default"     ] = btn_build_variant_nobg()
mbtnv["hovering"    ] = btn_build_variant_nobg()
mbtnv["menu"        ] = btn_build_variant(192, 192, 192)
mbtnv["clicked_left"] = btn_build_variant(192, 192, 192)

smbtnv["default"     ] = btn_build_variant_nobg()
smbtnv["hovering"    ] = btn_build_variant(192, 192, 192)
smbtnv["menu"        ] = btn_build_variant(192, 192, 192)
smbtnv["clicked_left"] = btn_build_variant(192, 192, 192)

-- editors

gui.Text_Editor.__variants = {
    default = {
        gui.Color_Filler {
            color = 0x202020, clamp = true, gui.Outline { clamp = true }
        },
        __init = |ed| do
            ed:set_pad_l(0.005)
            ed:set_pad_r(0.005)
        end
    }
}
gui.Field.__variants     = gui.Text_Editor.__variants
gui.Key_Field.__variants = gui.Text_Editor.__variants

-- menus, tooltips

gui.Filler.__variants = {
    menu = {
        gui.Gradient { color = 0xFA000000, color2 = 0xFA080808, clamp = true,
            gui.Outline { color = 0xFFFFFF, clamp = true }
        }
    },
    tooltip = {
        __properties = { "label" },
        gui.Gradient {
            color = 0xFA000000, color2 = 0xFA080808, gui.Outline {
                color = 0xFFFFFF, clamp = true, gui.Spacer {
                    pad_h = 0.01, pad_v = 0.005, init_clone = |self, ttip| do
                        local lbl = gui.Label { text = ttip.label }
                        self:append(lbl)
                        signal.connect(ttip, "label_changed", |o, t| do
                            o:set_text(t) end)
                    end
                }
            }
        }
    }
}

-- checkboxes, radioboxes

local ckbox_build_variant = |r, g, b, tgl| gui.Color_Filler {
    color = 0x101010, min_w = 0.02, min_h = 0.02,
    gui.Outline {
        color = Color(r, g, b), clamp = true, tgl and gui.Spacer {
            pad_h = 0.005, pad_v = 0.005, clamp = true, gui.Color_Filler {
                clamp = true, color = 0xC0C0C0,
                gui.Outline { color = Color(r, g, b), clamp = true }
            }
        } or nil
    }
}

local rdbtn_build_variant = |r, g, b, tgl| gui.Circle {
    color = 0x101010, min_w = 0.02, min_h = 0.02,
    gui.Circle {
        style = gui.Circle.OUTLINE, color = Color(r, g, b), clamp = true,
        tgl and gui.Spacer {
            pad_h = 0.005, pad_v = 0.005, clamp = true, gui.Circle {
                clamp = true, color = 0xC0C0C0, gui.Circle {
                    style = gui.Circle.OUTLINE, color = Color(r, g, b),
                    clamp = true
                }
            }
        } or nil
    }
}

local ckboxv, rdbtnv = {}, {}

gui.Toggle.__variants = {
    checkbox = ckboxv,
    radiobutton = rdbtnv
}

ckboxv["default"         ] = ckbox_build_variant(255, 255, 255)
ckboxv["default_hovering"] = ckbox_build_variant(225, 225, 225)
ckboxv["toggled"         ] = ckbox_build_variant(192, 192, 192, true)
ckboxv["toggled_hovering"] = ckbox_build_variant(225, 225, 225, true)
rdbtnv["default"         ] = rdbtn_build_variant(255, 255, 255)
rdbtnv["default_hovering"] = rdbtn_build_variant(225, 225, 225)
rdbtnv["toggled"         ] = rdbtn_build_variant(192, 192, 192, true)
rdbtnv["toggled_hovering"] = rdbtn_build_variant(225, 225, 225, true)

-- windows

local window_build_titlebar = || gui.Gradient {
    color = 0xE6303030, color2 = 0xE6000000, clamp_h = true,
    gui.Spacer {
        pad_h = 0.004, pad_v = 0.004,
        init_clone = |self, win| do
            local lbl = gui.Label { text = win.title or win.obj_name }
            self:append(lbl)
            signal.connect(win, "title_changed", |w, t| do
                lbl:set_text(t or w.obj_name) end)
        end
    }
}

local window_build_regular = |mov| gui.Filler {
    clamp = true,
    gui.V_Box {
        clamp = true,
        gui.Filler { clamp_h = true,
            mov and gui.Mover { clamp_h = true,
                init_clone = |self, win| do
                    self:set_window(win)
                end,
                window_build_titlebar()
            } or window_build_titlebar(),
            gui.Spacer { pad_h = 0.009, align_h = 1,
                gui.Button {
                    variant = false, states = {
                        default = gui.Gradient {
                            color = 0x0, color2 = 0x303030, min_w = 0.015,
                            min_h = 0.015, gui.Outline { clamp = true }
                        },
                        hovering = gui.Gradient {
                            color = 0x0, color2 = 0x303030, min_w = 0.015,
                            min_h = 0.015, gui.Outline { clamp = true,
                                color = 0xE1E1E1 }
                        },
                        clicked_left = gui.Gradient {
                            color = 0x0, color2 = 0x303030, min_w = 0.015,
                            min_h = 0.015, gui.Outline { clamp = true,
                                color = 0xC0C0C0 }
                        }
                    },
                    init_clone = |self, win| do
                        signal.connect(self, "clicked", || win:hide())
                    end
                }
            }
        },
        gui.Gradient {
            color = 0xE6000000, color2 = 0xE6080808, clamp = true, gui.Spacer {
                pad_h = 0.005, pad_v = 0.005, init_clone = |self, win| do
                    win:set_container(self)
                end
            }
        },
        states = {
            default = gui.Color_Filler { min_w = 0.05, min_h = 0.07 }
        }
    },
    gui.Outline { color = 0xFFFFFF, clamp = true }
}

gui.Window.__variants = {
    borderless = {
        gui.Gradient {
            color = 0xE6000000, color2 = 0xE6080808, clamp = true,
            gui.Outline { color = 0xFFFFFF, clamp = true, gui.Spacer {
                pad_h = 0.005, pad_v = 0.005, init_clone = |self, win| do
                    win:set_container(self)
                end
            } }
        }
    },
    regular = { __properties = { "title" }, window_build_regular(false) },
    movable = { __properties = { "title" }, window_build_regular(true)  }
}

-- default windows

world:new_window("changes", gui.Window, function(win)
    win:append(gui.Color_Filler { color = 0xC0000000, min_w = 0.3,
    min_h = 0.2 }, function(r)
        r:clamp(true, true, true, true)
        win:append(gui.V_Box { padding = 0.01 }, function(box)
            box:append(gui.Label { text = "Changes" })
            box:append(gui.Label { text = "Apply changes?" })
            for i, v in ipairs(gui.changes_get()) do
                box:append(gui.Label { text = v })
            end
            box:append(gui.H_Box { padding = 0.01 }, function(hb)
                hb:append(gui.Button { label = "OK" }, function(btn)
                    signal.connect(btn, "clicked", function()
                        world:hide_window("changes")
                        gui.changes_apply()
                    end)
                end)
                hb:append(gui.Button { label = "Cancel" }, function(btn)
                    signal.connect(btn, "clicked", function()
                        world:hide_window("changes")
                        gui.changes_clear()
                    end)
                end)
            end)
        end)
    end)
end)

world:new_window("texture", gui.Window, function(win)
    win:append(gui.Color_Filler { color = 0xC0000000, min_w = 0.3,
    min_h = 0.2 }, function(r)
        r:clamp(true, true, true, true)
        win:append(gui.V_Box { padding = 0.01 }, function(box)
            box:append(gui.Label { text = "Textures" })
            box:append(gui.Grid { columns = 9, padding = 0.01 }, function(t)
                for i = 1, capi.slot_get_count() do
                    t:append(gui.Button, function(btn)
                        btn:update_state("default",
                            btn:update_state("hovering",
                                btn:update_state("clicked", gui.Slot_Viewer {
                                    index = i - 1, min_w = 0.095,
                                    min_h = 0.095 })))
                        signal.connect(btn, "clicked", function()
                            capi.slot_set(i - 1)
                        end)
                    end)
                end
            end)
            box:append(gui.Button(), function(btn)
                btn:update_state("default",
                    btn:update_state("hovering",
                        btn:update_state("clicked", gui.Color_Filler {
                            color = 0x404040, min_w = 0.2, min_h = 0.05,
                            gui.Label { text = "Close" } })))
                signal.connect(btn, "clicked", function()
                    world:hide_window("texture")
                end)
            end)
        end)
    end)
end)
