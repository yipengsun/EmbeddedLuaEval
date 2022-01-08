#!/usr/bin/env lua

-- Configurables
MATH_OP = {'+', '-'} -- '*, /' makes the variables go to inf or nan
MAX_PARAMS = 5
KNOWN_NAMES = {'val0'}
KNOWN_EXPR = {'3.14159'}

-- Helpers
function tableLen(t)
    local count = 0
    for _ in pairs(t) do count = count + 1 end
    return count
end

function getRand(t, numOfElem)
    local result = {}
    local len = tableLen(t)

    for i = 1, math.min(tableLen(t), numOfElem) do
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

    local varsInExpr = getRand(KNOWN_NAMES, MAX_PARAMS)
    local len = tableLen(varsInExpr)
    local ops = getRand(MATH_OP, math.min(len, MAX_PARAMS - 1))

    local expr = ''
    for j = 1, tableLen(ops) do expr = expr .. varsInExpr[j] .. ops[j] end
    expr = expr .. varsInExpr[len]

    KNOWN_NAMES[#KNOWN_NAMES + 1] = 'val' .. tostring(i)
    KNOWN_EXPR[#KNOWN_EXPR + 1] = expr
end

local file = io.open(filename, 'w')
io.output(file)

for i = 1, tableLen(KNOWN_NAMES) do
    local line = KNOWN_NAMES[i] .. ': ' .. KNOWN_EXPR[i] .. '\n'
    io.write(line)
end

io.close(file)
