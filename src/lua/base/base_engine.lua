--[[!
    File: base/base_engine.lua

    About: Author
        q66 <quaker66@gmail.com>

    About: Copyright
        Copyright (c) 2011 OctaForge project

    About: License
        This file is licensed under MIT. See COPYING.txt for more information.

    About: Purpose
        This file features engine interface (quitting, engine variables, homedir etc.)

    Section: Engine interface
]]

--[[!
    Package: engine
    This module contains engine interface. Contains some core engine functions as
    well as engine variable system shared between engine and scripting.

    You can get engine varibles in scripting the same way as normal variables are
    handled, because _G environment table is overloaded to do so. You can also
    create new temporary variables in here.
]]
module("engine", package.seeall)

--- Quit the engine, showing a dialog when there are unsaved changes.
-- @class function
-- @name quit
quit = CAPI.quit

--- Quit the engine without asking for anything.
-- @class function
-- @name force_quit
force_quit = CAPI.force_quit

--- Reload graphics subsystem of the engine. 
-- @class function
-- @name resetgl
resetgl = CAPI.resetgl

--- Check for available OpenGL extension.
-- @param ext The extension to check for.
-- @return 1 if extension is available, 0 otherwise. (TODO: change to booleans)
-- @class function
-- @name glext
glext = CAPI.glext

--- Get current frame rate.
-- @return Frame rate.
-- @class function
-- @name getfps
getfps = CAPI.getfps

--- Take a screenshot.
-- @param name Screenshot will get saved as name.png.
-- @class function
-- @name screenshot
screenshot = CAPI.screenshot

--- Record an ingame video. WARNING: videos are huge!
-- Controlled by engine variables (moview, movieh,
-- moviesound, moviefps, moviesync, movieaccel,
-- movieaccelyuv, movieaccelblit)
-- @param name Movie will get recorded into name.avi.
-- @class function
-- @name movie
movie = CAPI.movie

-- Get OF home directory.
-- @return OF home directory.
-- @class function
-- @name gethomedir
gethomedir = CAPI.gethomedir

-- Get OF server log file.
-- @return OF server log file path.
-- @class function
-- @name getserverlogfile
getserverlogfile = CAPI.getserverlogfile

-- Variable types

--- Variable types. (not table)
-- @class table
-- @name vartypes
-- @field VAR_I Integer variable.
-- @field VAR_F Floating point number variable.
-- @field VAR_S String variable.
VAR_I = 0
VAR_F = 1
VAR_S = 2

-- Class for variables

--- The storage class for variables.
-- @class table
-- @name var_storage
-- @field stor The storage table.
-- @field reg The register function.
-- @field clear The clear function.
var_storage = class.new()
var_storage.stor = {}

--- Register a variable into storage.
-- @param v Variable to register (IVAR, FVAR or SVAR)
function var_storage:reg(v)
    if not v:is_a(_VAR) then
        logging.log(logging.ERROR, "Cannot register variable because wrong class was provided.")
        return nil
    end

    if self.stor[v.name] then return nil end -- do not register registered
    self.stor[v.name] = v

    self:define_getter(v.name, function(self, v) return v.curv end, v)
    self:define_setter(v.name .. "_ns", function(self, v, val) v.curv = val end, v)
    self:define_setter(v.name, function(self, v, val)
        if v:check_bounds(val) then
            v.curv = val
            CAPI.svfl(v.name, v.type, val)
        end
    end, v)
end

--- Clear the storage.
function var_storage:clear()
    self.stor = {}
end

--- The storage class instance for variables.
-- @class table
-- @name vars
vars = var_storage()

-- Variable classes

--- Default skeleton variable class.
-- @class table
-- @name _VAR
-- @field __init The constructor.
_VAR = class.new()

--- Constructor for default engine variable skeleton.
-- @param name Name of the variable.
-- @param minv Minimal value.
-- @param curv Default value.
-- @param maxv Maximal value.
-- @param ro Read only.
-- @param alias Will create an alias.
function _VAR:__init(name, minv, curv, maxv, ro, alias)
    assert(name, "Cannot register variable: name is missing.")
    self.name = name
    self.minv = minv
    self.maxv = maxv
    self.curv = curv
    self.ro = ro
    self.alias = alias
end

--- Integer variable class.
-- @class table
-- @name IVAR
-- @field __init The constructor.
-- @field __tostring Returns string representation.
-- @field check_bounds Returns true if value to set is in variable bounds.
IVAR = class.new(_VAR)

--- Returns string representation of the variable class.
-- @return A string representing the variable class.
function IVAR:__tostring() return "IVAR" end

--- Constructor for integer variable.
-- @param name Name of the variable.
-- @param minv Minimal value.
-- @param curv Default value.
-- @param maxv Maximal value.
-- @param ro Read only.
-- @param alias Will create an alias.
function IVAR:__init(name, minv, curv, maxv, ro, alias)
    assert(type(minv) == "number"
            and type(curv) == "number"
            and type(maxv) == "number",
       "Wrong value type provided to IVAR.")

    _VAR.__init(self, name, minv, curv, maxv, ro, alias)
    self.type = VAR_I
end

--- Checks if value to set is in variable bounds.
-- @param v Value to set.
-- @return True if the value can be set, otherwise false.
function IVAR:check_bounds(v)
    if type(v) ~= "number" then
        logging.log(logging.ERROR, "Wrong value type passed to variable.")
        return false
    end
    if self.alias then return true end
    if self.ro then
        logging.log(logging.ERROR, "Variable is read only.")
        return false
    end
    if v < self.minv or v > self.maxv then
        logging.log(logging.ERROR,
                    "Variable accepts only values of range "
                    .. self.minv .. " to " .. self.maxv)
        return false
    end
    return true
end

--- Float variable class. Inherited from IVAR, takes its check_bounds.
-- @class table
-- @name FVAR
-- @field __init The constructor.
-- @field __tostring Returns string representation.
-- @field check_bounds Returns true if value to set is in variable bounds.
FVAR = class.new(IVAR)

--- Returns string representation of the variable class.
-- @return A string representing the variable class.
function FVAR:__tostring() return "FVAR" end

--- Constructor for float variable.
-- @param name Name of the variable.
-- @param minv Minimal value.
-- @param curv Default value.
-- @param maxv Maximal value.
-- @param ro Read only.
-- @param alias Will create an alias.
function FVAR:__init(name, minv, curv, maxv, ro, alias)
    assert(type(minv) == "number"
            and type(curv) == "number"
            and type(maxv) == "number",
       "Wrong value type provided to FVAR.")

    _VAR.__init(self, name, minv, curv, maxv, ro, alias)
    self.type = VAR_F
end

--- String variable class.
-- @class table
-- @name FVAR
-- @field __init The constructor.
-- @field __tostring Returns string representation.
-- @field check_bounds Returns true if value to set is in variable bounds.
SVAR = class.new(_VAR)

--- Returns string representation of the variable class.
-- @return A string representing the variable class.
function SVAR:__tostring() return "SVAR" end

--- Constructor for string variable.
-- @param name Name of the variable.
-- @param curv Default value.
-- @param ro Read only.
-- @param alias Will create an alias.
function SVAR:__init(name, curv, ro, alias)
    assert(type(curv) == "string" or not curv, "Wrong value type provided to SVAR.")
    _VAR.__init(self, name, nil, curv, nil, ro, alias)
    self.type = VAR_S
end

--- Checks if value to set is in variable bounds.
-- @param v Value to set.
-- @return True if the value can be set, otherwise false.
function SVAR:check_bounds(v)
    if type(v) ~= "string" then
        logging.log(logging.ERROR, "Wrong value type passed to variable.")
        return false
    end
    if self.alias then return true end
    if self.read then
        logging.log(logging.ERROR, "Variable is read only.")
        return false
    end
    return true
end

-- Some wrappers mainly for C++ to simplify registering.
function ivar(name, ...) vars:reg(IVAR(name, ...)) end
function fvar(name, ...) vars:reg(FVAR(name, ...)) end
function svar(name, ...) vars:reg(SVAR(name, ...)) end

--- Reset an engine variable.
-- @param n Name of the variable.
-- @class function
-- @name reset
resetvar = CAPI.resetvar

--- Create new engine variable from Lua scripting system.
-- Useful for i.e. setting default values for GUI entries.
-- Creates variables as "aliases" and doesn't support callbacks.
-- @param name Name of the variable.
-- @param type Type of the variable. see VAR_(I,F,S) at the beginning of this file.
-- @param val Default value of the variable.
-- @class function
-- @name new
newvar = CAPI.newvar
