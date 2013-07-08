--[[! File: lua/core/engine/camera.lua

    About: Author
        q66 <quaker66@gmail.com>

    About: Copyright
        Copyright (c) 2013 OctaForge project

    About: License
        This file is licensed under MIT. See COPYING.txt for more information.

    About: Purpose
        Editing functions including procedural geometry generation.
]]

local capi = require("capi")

local M = {}

--[[! Variable: MATERIAL_AIR
    Represents the "air" material (no material).
]]
M.MATERIAL_AIR = 0

--[[! Variable: MATERIAL_WATER
    Represents the "water" material.
]]
M.MATERIAL_WATER = bit.lshift(1, 2)

--[[! Variable: MATERIAL_LAVA
    Represents the "lava" material.
]]
M.MATERIAL_LAVA = bit.lshift(2, 2)

--[[! Variable: MATERIAL_GLASS
    Represents the "glass" material.
]]
M.MATERIAL_GLASS = bit.lshift(3, 2)

--[[! Variable: MATERIAL_NOCLIP
    Represents the "noclip" material.
]]
M.MATERIAL_NOCLIP = bit.lshift(1, 5)

--[[! Variable: MATERIAL_CLIP
    Represents the "clip" material.
]]
M.MATERIAL_CLIP = bit.lshift(2, 5)

--[[! Variable: MATERIAL_GAMECLIP
    Represents the "gameclip" material.
]]
M.MATERIAL_GAMECLIP = bit.lshift(3, 5)

--[[! Variable: MATERIAL_DEATH
    Represents the "death" material.
]]
M.MATERIAL_DEATH = bit.lshift(1, 8)

--[[! Variable: MATERIAL_ALPHA
    Represents the "alpha" material.
]]
M.MATERIAL_ALPHA = bit.lshift(4, 8)

--[[! Variable: MATERIAL_INDEX_SHIFT ]]
M.MATERIAL_INDEX_SHIFT = 0

--[[! Variable: MATERIAL_VOLUME_SHIFT ]]
M.MATERIAL_VOLUME_SHIFT = 2

--[[! Variable: MATERIAL_CLIP_SHIFT ]]
M.MATERIAL_CLIP_SHIFT = 5

--[[! Variable: MATERIAL_FLAG_SHIFT ]]
M.MATERIAL_FLAG_SHIFT = 8

--[[! Variable: MATERIALF_INDEX ]]
M.MATERIALF_INDEX = bit.lshift(3, 0)

--[[! Variable: MATERIALF_VOLUME ]]
M.MATERIALF_VOLUME = bit.lshift(7, 2)

--[[! Variable: MATERIALF_CLIP ]]
M.MATERIALF_CLIP = bit.lshift(7, 5)

--[[! Variable: MATERIALF_FLAGS ]]
M.MATERIALF_FLAGS = bit.lshift(255, 8)

--[[! Function: add_npc
    Adds a bot into the world onto the starting position. Bots are considered
    clients. You can define their AI via their entity class. This is purely
    serverside. Takes the entity class name the bot should be. Returns the bot
    entity.
]]
M.add_npc = capi.npcadd

--[[! Function: delete_npc
    Deletes a bot. Takes the entity, returns nothing. Purely serverside.
]]
M.delete_npc = capi.npcdel

--[[! Function: new_entity
    Creates a new entity on the position where the edit cursor is aiming.
    Takes the entity class name as an argument. Also callable from cubescript
    as newent. Clientside.
]]
M.new_entity = capi.new_entity

--[[! Function: get_material
    Returns what material is on the position given by the argument.
    Materials are represented by <MATERIAL_AIR>, <MATERIAL_WATER>,
    <MATERIAL_LAVA>, <MATERIAL_GLASS>, <MATERIAL_NOCLIP> and
    <MATERIAL_CLIP>.
]]
M.get_material = function(o)
    return capi.getmat(o.x, o.y, o.z)
end

--[[! Function: erase_geometry
    Clears all the map geometry.
]]
M.erase_geometry = capi.edit_erase_geometry

--[[! Function: create_cube
    Creates a cube with the given parameters. Please note that not all
    positions are sufficient for creating cubes, you need to fit into the grid.

    For example, when you have coords 512, 512, 512 and cube of size 64, it's
    fine, because it can fit in. But if you change the coords to 1, 1, 1, only
    cube of size 1 can fit there.

    The coordinates also have to fit in the world, so mapsize. Takes the x,
    y, z coordinates and the gridsize (which is 1<<gridpower).
]]
M.create_cube = capi.edit_create_cube

--[[! Function: delete_cube
    Parameters and rules are the same as for <create_cube>, but it actually
    deletes cubes instead of creating.
]]
M.delete_cube = capi.edit_delete_cube

--[[! Function: set_cube_texture
    First 4 arguments are the same as in <create_cube>, the fifth argument is
    the face, sixth is the texture slot number. See also <set_cube_material>.

    If we're standing in the center of the map, with increasing X coordinate
    when going right and increasing Y coordinate when going forward, the
    faces go like this:

    Faces:
        0 - right side of the cube
        1 - left side of the cube
        2 - back of the cube
        3 - front of the cube
        4 - bottom of the cube
        5 - top of the cube
]]
M.set_cube_texture = capi.edit_set_cube_texture

--[[! Function: set_cube_material
    First 4 arguments are the same as in <create_cube>, the fifth argument is
    the material index, see <get_material>. See also <set_cube_texture>.
]]
M.set_cube_material = capi.edit_set_cube_material

--[[! Function: set_cube_color
    See above. The last 3 arguments are the color components (from 0 to 1).
]]
M.set_cube_color = capi.edit_set_cube_color

--[[! Function: push_cube_corner
    First 5 arguments are the same with <set_cube_texture>, the sixth argument
    is the corner index, the seventh is the direction (1 into the cube,
    -1 from the cube).

    On all faces, 0 is top-left corner, 1 is top-right, 2 is bottom-left and 3
    is bottom-right when facing them directly (that is, when we see the texture
    in the right orientation).
]]
M.push_cube_corner = capi.edit_push_cube_corner

return M
