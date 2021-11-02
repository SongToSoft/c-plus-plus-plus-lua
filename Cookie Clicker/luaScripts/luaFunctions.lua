function printText(text)
    print(text)
end

function increaseValues(a, b)
    return a + b
end

function compareValues(a, b)
    return a >= b
end

function writeToFile(value)
    local currentDir = io.popen("cd"):read()
    currentDir = currentDir:gsub( "\\", "/")
    currentDir = currentDir.."/luaScripts/profile.txt"
    local fh = assert(io.open(currentDir, "wb"))
    fh:write(value)
    fh:flush()
    fh:close()
end

function readFromFile()
    local currentDir = io.popen("cd"):read()
    currentDir = currentDir:gsub( "\\", "/")
    currentDir = currentDir.."/luaScripts/profile.txt"
    local fh = assert(io.open(currentDir, "rb"))
    local values = assert(fh:read(_VERSION <= "Lua 5.2" and "*a" or "a"))
    fh:close()
    return values
end