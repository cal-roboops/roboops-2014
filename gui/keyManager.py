class keyManager():
	STATE = 0
	PRESS = 1
	HOLD = 2
	RELEASE = 3
	"""
	Call act_key_press(key) when a key is detected to have been pressed.
	Call act_key_release(key) when a key is detected to have been released.

	Have a continuous loop call act_key_hold() to act for all key bindings that have a hold function.

	Call add_binding(key, function, type) to tie a function to a key.
	Call remove_binding(key, function, type) to remove a function from a key.
	Call clear_bindings(key, type) to remove all function bindings from a key.
	Note: functions are acted in order of their binding.
	Type Legend:
		PRESS
			Manipulates pressing events.
		HOLD
			Manipulates hold events.
		RELEASE
			Manipulates release events.
	"""
	def __init__(self) :
		self.key_bindings = {}

	def add_binding(self, key, func, type) :
		if not key in self.key_bindings :
			self.key_bindings[key] = [[] for i in range(4)]
		self.key_bindings[key][type].append(func)

	def remove_binding(self, key, func, type) :
		if not key in self.key_bindings :
			return
		for i in len(self.key_bindings[key][type]) :
			if self.key_bindings[key][type][i] == i :
				del(self.key_bindings[key][type][i])

	def clear_bindings(self, key, type) :
		if not key in self.key_bindings :
			return
		self.key_bindings[key][type] = []

	def act_key_press(self, key) :
		if key in self.key_bindings and not self.key_bindings[key][self.STATE] :
			self.key_bindings[key][self.STATE] = True
			for func in self.key_bindings[key][self.PRESS] :
				func()

	def act_key_release(self, key) :
		if key in self.key_bindings and self.key_bindings[key][self.STATE] :
			self.key_bindings[key][self.STATE] = False
			for func in self.key_bindings[key][self.RELEASE] :
				func()

	def act_key_hold(self) :
		for key in self.key_bindings :
			if self.key_bindings[key][self.STATE] :
				for func in self.key_bindings[key][self.HOLD] :
					func()
