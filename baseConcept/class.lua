Account = {
	id = -1,
	balance = 0
}


function Account:new(t)
	t = t or {}
	setmetatable(t, self)
	self.__index = self
	return t
end

function Account:deposite(x)
	self.balance = self.balance + x
end
	

function Account:withdraw(x)

	print("try to withdraw "..x)
	if x > self.balance then
		print("balance not enough!")
	else
		print("withdraw success")
		self.balance = self.balance - x
	end
end

function Account:check()
	print("your account has "..self.balance)
end

s1 = Account:new()
s1:deposite(100)
s1:check()
s1:withdraw(120)
s1:withdraw(10)
s1:check() 


SpecialAccount = Account:new()
SpecialAccount:deposite(100)

function SpecialAccount:withdraw(x)
	print("try to withdraw "..x)
	if x > self.balance + self.limit then
		print("balance and limit left not enough!")
	elseif x <= self.balance then
		print("withdraw success from account ")
		self.balance = self.balance - x
	else
		print("withdraw part from limit ")
		self.limit = self.limit + self.balance - x
		self.balance = 0	
	end
	self:check()
end
si = SpecialAccount:new({limit = 1000})



function si:check()
	print("------------------------")
	print("your account has "..self.balance)
	print("your limit has "..self.limit)
	print("------------------------")
end


print("--------------Speicial account information now------------------")
si:check()
si:withdraw(100)
si:withdraw(100)

