local profile = {}
function profile:setName(name)
	self.name = name
end

function profile:getName()
	return self.name
end

local School = {}
function School:setSchoolName(name)
	self.schooName = name
end

function School:getSchoolName(name)
	return self.schooName
end

local function search(k, plist)
	for i = 1, table.getn(plist) do
		local v = plist[i][k]
		if v~= nil then
			return v
		end
	end
	return
end

function createClass(...)
	local t={}
	setmetatable(t, {__index = function(t, k) return search(k, arg) end})

	t.__index = t
	function t:new(o)
		o = o or {}
		setmetatable(o, t)
		return o
	end

	return t
end


Student = createClass(profile, School)
stu = Student:new({name = "Doris"})
print(stu:getName())

stu:setSchoolName("Amoy")
print(stu:getSchoolName())