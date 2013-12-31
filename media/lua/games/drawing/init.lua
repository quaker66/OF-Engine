--[[!<
    A test "game" for OctaForge that lets you draw on the world geometry.
    Left click to draw, right click to stop drawing, middle click to change
    colors.

    Author:
        q66 <quaker66@gmail.com>

    License:
        See COPYING.txt.
]]

local log = require("core.logger")

local gui = require("core.gui.core")
local input = require("core.engine.input")
local inputev = require("core.events.input")
local cs = require("core.engine.cubescript")
local signal = require("core.events.signal")
local svars = require("core.entities.svars")
local ents = require("core.entities.ents")
local conv = require("core.lua.conv")
local particles = require("core.engine.particles")

local splash, flare = particles.splash, particles.flare
local quadrenderer, taperenderer = particles.register_renderer_quad,
    particles.register_renderer_tape

local hextorgb = conv.hex_to_rgb

local game_manager = require("extra.game_manager")
local health = require("extra.health")

local connect = signal.connect
local Vec4 = require("core.lua.geom").Vec4
local ipairs = ipairs

local colors = {
    0xFFFFFF, 0xFF0000, 0xFFFF00, 0x00FF00, 0x00FFFF, 0x0000FF, 0xFF00FF
}

local Player = ents.Player

local SPARK, STREAK
if not SERVER then
    SPARK = quadrenderer("spark", "media/particle/spark",
        particles.flags.FLIP | particles.flags.BRIGHT)
    STREAK = taperenderer("streak", "media/particle/flare",
        particles.flags.BRIGHT)
end

--[[! Object: Game_Player
    This serves as a base for our player. It defines all the basic entry
    points for the drawing game.

    Properties:
        - new_mark - contains mark data. It's required because it has to sync
          over the server (e.g. in coop).
]]
local Game_Player = Player:clone {
    name = "Game_Player",

    __properties = {
        new_mark = svars.State_Array_Float {
            client_set = true, has_history = false
        }
    },

    next_color = function(self)
        if  self.color_id < #colors then
            self.color_id = self.color_id + 1
        else
            self.color_id = 1
        end
        self.color = colors[self.color_id]
        log.echo(("color switch: 0x%.6X"):format(self.color))
    end,

    reset_mark = function(self)
        self:set_attr("new_mark", { -1, -1, -1 })
        self.stop_batch = true
    end,

    --[[! Function: __activate
        Called on entity activation. Connects a callback to state data
        change of new_mark.
    ]]
    __activate = (not SERVER) and function(self, kwargs)
        Player.__activate(self, kwargs)
        self.marks = {}
        self.color_id = 1
        self.color    = colors[1]
        connect(self, "new_mark_changed", function(self, nm)
            if #nm == 3 then
                nm = Vec4(nm[1], nm[2], nm[3], self.color)
            else
                nm = nil
            end
            local marks = self.marks
            marks[#marks + 1] = nm
        end)
    end or nil,

    --[[! Function: __run
        Called every frame. It goes over the marks and draws everything.
    ]]
    __run = (not SERVER) and function(self, millis)
        Player.__run(self, millis)
        local last = nil
        local marks = self.marks

        for i, mark in ipairs(marks) do
            if last and mark and mark.x >= 0 and last.x >= 0 then
                local r, g, b = hextorgb(mark.w)
                flare(STREAK, mark, last, r / 255, g / 255, b / 255, 0, 1)
                flare(STREAK, last, mark, r / 255, g / 255, b / 255, 0, 1)
            end
            last = mark
        end

        local newb = #marks == 0 or not marks[#marks - 1]
        local conb = #marks  > 0 and    marks[#marks - 1]

        if conb and not self.stop_batch then
            local mark = marks[#marks - 1]
            local r, g, b = hextorgb(mark.w)
            splash(SPARK, mark, 25, 10, r / 255, g / 255, b / 255, 150, 1, 1)
        end

        if self.pressing then
            local newp = input.get_target_position()
            local topl = self:get_attr("position"):sub_new(newp)
            newp:add(topl:normalize())
            if newb or marks[#marks - 1]:dist(newp) > 5 then
                self:set_attr("new_mark", newp:to_array())
            end
        end
    end or nil
}

ents.register_class(Game_Player, {
    game_manager.player_plugin,
    health.player_plugin, {
        __activate = (not SERVER) and function(self)
            local hstatus
            gui.get_hud():append(gui.Spacer { pad_h = 0.1, pad_v = 0.1,
                align_h = 1, align_v = 1
            }, |sp| do
                hstatus = sp:append(gui.Label {
                    text = tostring(self:get_attr("health")), scale = 2.5,
                    font = "default_outline"
                })
            end)
            self.health_hud_status = hstatus
            connect(self, "health_changed", |self, v| do
                hstatus:set_text(tostring(v))
            end)
        end,
        __deactivate = (not SERVER) and function(self)
            self.health_hud_status:destroy()
        end
    }
})
ents.register_class(ents.Obstacle, { health.health_area_plugin },
    "Health_Area")

if not SERVER then
    inputev.set_event("click", function(btn, down, x, y, z, uid, cx, cy)
        local ent = ents.get(uid)
        if ent and ent.click then
            return ent:click(btn, down, x, y, z, cx, cy)
        end
        if btn == 1 then
            ents.get_player().pressing   = down
            ents.get_player().stop_batch = false
        elseif btn == 2 and down then
            ents.get_player():reset_mark()
        elseif btn == 3 and down then
            ents.get_player():next_color()
        end
    end)
else
    ents.set_player_class("Game_Player")
end
