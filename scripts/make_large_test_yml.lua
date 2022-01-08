#!/usr/bin/env lua

-- Configurables
mathOp = {'+', '-'} -- '*, /' make the variables go to inf or nan
maxParams = 6
knownNames = {'val0'}
knownExprs = {'3.14159'}

-- Helpers
function tabLen(t)
    local count = 0
    for _ in pairs(t) do count = count + 1 end
    return count
end

function getRand(t, numOfElem)
    local result = {}
    local len = tabLen(t)

    for i = 1, numOfElem do
        local x = math.random(1, len)
        result[#result + 1] = t[x]
    end

    return result
end

-- CLI argument
local maxSize = arg[1]
local filename = arg[2]

math.randomseed(os.time())

print(string.format('Generating a test YAML file with %d variables...',
                    maxSize + 1))
for i = 1, maxSize do
    local varName = 'val' .. tostring(i)
    local min = math.min

    local varsInExpr = getRand(knownNames, min(tabLen(knownNames), maxParams))
    local len = tabLen(varsInExpr)
    local ops = getRand(mathOp, len - 1)

    local expr = ''
    for j = 1, tabLen(ops) do expr = expr .. varsInExpr[j] .. ops[j] end
    expr = expr .. varsInExpr[len]

    knownNames[#knownNames + 1] = 'val' .. tostring(i)
    knownExprs[#knownExprs + 1] = expr
end

local file = io.open(filename, 'w')
io.output(file)

for i = 1, tabLen(knownNames) do
    io.write(knownNames[i] .. ': ' .. knownExprs[i] .. '\n')
end

io.close(file)
