import pygame
import time
from constants.constants import *

from serialize import Serialize

MIN_DETECTION=.2 #to account for misc. numbers in resting state
DELAY=.5 #to avoid sending excessive info

class Controller():

	UP, DOWN, HOLD, IS_DOWN = 0, 1, 2, 3
	AXIS_FUNC, AXIS_TIME_DELAY = 0, 1
	HAT_FUNC, HAT_TIME_DELAY = 0, 1

	def __init__(self, id) :
		pygame.init()
		pygame.joystick.init()
		if not pygame.joystick.get_count() :
			self.is_active = False
			return
		self.controller = pygame.joystick.Joystick(id)
		self.controller.init()

		self.id = id

		self.num_buttons = self.controller.get_numbuttons()
		self.num_axes = self.controller.get_numaxes()
		self.num_hats = self.controller.get_numhats()

		self.button_funcs = [[[],[],[],0] for i in range(self.num_buttons)]
		self.axis_funcs = [[[], 0] for i in range(self.num_axes)]
		self.hat_funcs = [[[], 0] for i in range(self.num_hats)]

		self.is_active = True

	def bind_button_up(self, button_id, func) :
		self.button_funcs[button_id][Controller.UP].append(func)

	def bind_button_down(self, button_id, func) :
		self.button_funcs[button_id][Controller.DOWN].append(func)

	def bind_button_hold(self, button_id, func) :
		self.button_funcs[button_id][Controller.HOLD].append(func)

	def bind_axis(self, axis_id, func) :
		self.axis_funcs[axis_id][Controller.AXIS_FUNC].append(func)

	def bind_hat(self, hat_id, func) :
		self.hat_funcs[hat_id][Controller.AXIS_FUNC].append(func)

	def clear_funcs(self) :
		for i in range(self.num_buttons):
			self.button_funcs[i][Controller.UP] = []
			self.button_funcs[i][Controller.DOWN] = []
			self.button_funcs[i][Controller.HOLD] = []
			self.button_funcs[i][Controller.IS_DOWN] = 0
		for i in range(self.num_axes):
			self.axis_funcs[i][Controller.AXIS_FUNC] = []
			self.axis_funcs[i][Controller.AXIS_TIME_DELAY] = 0
		for i in range(self.num_hats):
			self.axis_funcs[i][Controller.HAT_FUNC] = []
			self.axis_funcs[i][Controller.HAT_TIME_DELAY] = 0

	def update_loop(self) :
		while(self.is_active) :
			try:
				self.update()
			except Exception as e:
				print(e)

			#time.sleep(0.001)

	def update(self) :
		for event in pygame.event.get() :
			if event.type == pygame.JOYBUTTONDOWN :
				for i in range(self.num_buttons) :
					if self.controller.get_button(i) and not self.button_funcs[i][Controller.IS_DOWN] :
						self.button_funcs[i][Controller.IS_DOWN] = 1
						for f in self.button_funcs[i][Controller.DOWN] :
							f()
					elif self.controller.get_button(i):
						for f in self.button_funcs[i][Controller.HOLD] :
							f()

			elif event.type == pygame.JOYBUTTONUP :
				for i in range(self.controller.get_numbuttons()) :
					if not self.controller.get_button(i) and self.button_funcs[i][Controller.IS_DOWN] :
						self.button_funcs[i][Controller.IS_DOWN] = 0
						for f in self.button_funcs[i][Controller.UP] :
							f()

		for i in range(self.num_axes) :
			if self.axis_funcs[i][Controller.AXIS_TIME_DELAY] < time.time():
				for f in self.axis_funcs[i][Controller.AXIS_FUNC] :
					f(self.controller.get_axis(i) if (abs(self.controller.get_axis(i)) > MIN_DETECTION) else 0)
				self.axis_funcs[i][Controller.AXIS_TIME_DELAY] = time.time() + DELAY
			
		for i in range(self.num_hats) :
			if self.hat_funcs[i][Controller.HAT_TIME_DELAY] < time.time():
				for f in self.hat_funcs[i][Controller.HAT_FUNC] :
					f(self.controller.get_hat(i) if (self.controller.get_hat(i) != (0,0)) else (0, 0))
				self.hat_funcs[i][Controller.HAT_TIME_DELAY] = time.time() + DELAY

	def shut_off(self) :
		self.is_active = False

class RobotController(Controller) :

	def __init__(self, id, queue_out):
		#queue_out should be gui's queue_in, so gui can process with sliders
		Controller.__init__(self, id)

		self.queue_out = queue_out
		self.change_mode_bool = False
		self.change_mode_func = None

	def change_mode(self, func):

		def new_func():
			#print("CHANGE MODE")
			self.change_mode_bool = True
			self.change_mode_func = func

		return new_func

	def update_loop(self):
		while(self.is_active) :
			if self.change_mode_bool:
				#print("CHANGING MODE")
				self.change_mode_func()

				self.change_mode_bool = False
				self.change_mode_func = None
			else:
				self.update()

			#time.sleep(0.001)

	def set_arm_mode(self):

		self.clear_funcs()

		print("arm mode!")

		self.pre_values = {}
		self.pre_values[ARM_0] = None
		self.pre_values[ARM_1] = None
		self.pre_values[ARM_2] = None
		self.pre_values[CLAW] = None

		self.bind_axis(L_ANALOG_X, self.generate_motor_func(AXIS, L_ANALOG_X, ARM_0_SENSITIVITY, ARM_0_SENSITIVITYA, ARM_0))
		self.bind_axis(L_ANALOG_Y, self.generate_motor_func(AXIS, L_ANALOG_Y, ARM_1_SENSITIVITY, ARM_1_SENSITIVITYA, ARM_1))
		self.bind_axis(R_ANALOG_Y, self.generate_motor_func(AXIS, R_ANALOG_Y, ARM_2_SENSITIVITY, ARM_2_SENSITIVITYA, ARM_2))
		self.bind_axis(TRIGGER, self.generate_motor_func(AXIS, TRIGGER, CLAW_SENSITIVITY, CLAW_SENSITIVITY, CLAW))

	def set_car_mode(self):

		self.clear_funcs()

		print("car mode!")

		self.pre_values = {}
		self.pre_values['left'] = None
		self.pre_values['right'] = None

		self.pre_values_dict = {}
		self.pre_values_dict['swerve'] = 45
		self.pre_values_dict['pan'] = 90

		def drive_left(magnitude) :
			if(self.pre_values['left'] != magnitude) :
				self.queue_out.put(Serialize.RawMotor(AXIS, L_ANALOG_Y, LEFT_SENSITIVITY, FRONT_LEFT_WHEEL, magnitude).dump())
				self.queue_out.put(Serialize.RawMotor(AXIS, L_ANALOG_Y, LEFT_SENSITIVITY, BACK_LEFT_WHEEL, magnitude).dump())
				self.pre_values['left'] = magnitude

		def drive_right(magnitude) :
			if(self.pre_values['right'] != magnitude) :
				self.queue_out.put(Serialize.RawMotor(AXIS, R_ANALOG_Y, RIGHT_SENSITIVITY, FRONT_RIGHT_WHEEL, magnitude).dump())
				self.queue_out.put(Serialize.RawMotor(AXIS, R_ANALOG_Y, RIGHT_SENSITIVITY, BACK_RIGHT_WHEEL, magnitude).dump())
				self.pre_values['right'] = magnitude

		def swerve_shift(magnitude) :
			self.pre_values_dict['swerve'] += magnitude
			if self.pre_values_dict['swerve'] < 0:
				self.pre_values_dict['swerve'] = 0
			if self.pre_values_dict['swerve'] > 90:
				self.pre_values_dict['swerve'] = 90
			self.set_all_swerves(self.pre_values_dict['swerve'])

		def deploy_camera() :
			self.queue_out.put(Serialize.MotorHack(CAM_Y, 1).dump())

		def close_camera() :
			self.queue_out.put(Serialize.MotorHack(CAM_Y, 0).dump())

		def pan_camera(magnitude) :
			if magnitude > 0:
				self.pre_values_dict['pan'] += 10
				if self.pre_values_dict['pan'] > 170:
					self.pre_values_dict['pan'] = 170
			elif magnitude < 0:
				self.pre_values_dict['pan'] -= 10
				if self.pre_values_dict['pan'] < 10:
					self.pre_values_dict['pan'] = 10
			else:
				return
			self.queue_out.put(Serialize.MotorHack(CAM_X, self.pre_values_dict['pan']).dump())

		self.bind_axis(L_ANALOG_Y, drive_left)
		self.bind_axis(R_ANALOG_Y, drive_right)

		self.bind_axis(TRIGGER, pan_camera)

		self.bind_button_down(Y_BUTTON, deploy_camera)
		#self.bind_button_down(B_BUTTON, close_camera)

		self.bind_button_down(LEFT_BUTTON, lambda : swerve_shift(-45))
		self.bind_button_down(RIGHT_BUTTON, lambda : swerve_shift(45))

	def set_tank_mode(self):

		self.clear_funcs()

		self.set_tank_orientation()

		print("tank mode!")

		self.pre_values = {}
		self.pre_values[0] = None
		self.pre_values[1] = None

		def drive_left(magnitude) :
			if(self.pre_values[0] != magnitude) :
				self.queue_out.put(Serialize.RawMotor(AXIS, L_ANALOG_Y, LEFT_SENSITIVITY, FRONT_LEFT_WHEEL, magnitude).dump())
				self.queue_out.put(Serialize.RawMotor(AXIS, L_ANALOG_Y, LEFT_SENSITIVITY, BACK_LEFT_WHEEL, magnitude).dump())
				self.pre_values[0] = magnitude

		def drive_right(magnitude) :
			if(self.pre_values[1] != magnitude) :
				self.queue_out.put(Serialize.RawMotor(AXIS, R_ANALOG_Y, RIGHT_SENSITIVITY, FRONT_RIGHT_WHEEL, magnitude).dump())
				self.queue_out.put(Serialize.RawMotor(AXIS, R_ANALOG_Y, RIGHT_SENSITIVITY, BACK_RIGHT_WHEEL, magnitude).dump())
				self.pre_values[1] = magnitude

		self.bind_axis(L_ANALOG_Y, drive_left)
		self.bind_axis(R_ANALOG_Y, drive_right)

		self.bind_button_down(Y_BUTTON, self.change_mode(self.set_tank_mode))
		self.bind_button_down(B_BUTTON, self.change_mode(self.set_spin_mode))
		self.bind_button_down(X_BUTTON, self.change_mode(self.set_strafe_mode))

	def set_spin_mode(self):

		self.clear_funcs()

		self.set_spin_orientation()

		print("spin mode!")

		self.pre_values = {}
		self.pre_values[0] = None

		def drive_wheel(magnitude) :
			if(self.pre_values[0] != magnitude) :
				self.queue_out.put(Serialize.RawMotor(AXIS, L_ANALOG_X, LEFT_SENSITIVITY, FRONT_LEFT_WHEEL, magnitude).dump())
				self.queue_out.put(Serialize.RawMotor(AXIS, L_ANALOG_X, LEFT_SENSITIVITY, BACK_LEFT_WHEEL, magnitude).dump())
				self.queue_out.put(Serialize.RawMotor(AXIS, L_ANALOG_X, LEFT_SENSITIVITY, FRONT_RIGHT_WHEEL, magnitude).dump())
				self.queue_out.put(Serialize.RawMotor(AXIS, L_ANALOG_X, LEFT_SENSITIVITY, BACK_RIGHT_WHEEL, magnitude).dump())
				self.pre_values[0] = magnitude

		self.bind_axis(L_ANALOG_X, drive_wheel)

		self.bind_button_down(Y_BUTTON, self.change_mode(self.set_tank_mode))
		self.bind_button_down(B_BUTTON, self.change_mode(self.set_spin_mode))
		self.bind_button_down(X_BUTTON, self.change_mode(self.set_strafe_mode))

	def set_strafe_mode(self):

		self.clear_funcs()

		self.set_strafe_orientation()

		print("strafe mode!")

		self.pre_values = {}
		self.pre_values[0] = None

		def drive_wheel(magnitude) :
			if(self.pre_values[0] != magnitude) :
				self.queue_out.put(Serialize.RawMotor(AXIS, L_ANALOG_Y, LEFT_SENSITIVITY, FRONT_LEFT_WHEEL, magnitude).dump())
				self.queue_out.put(Serialize.RawMotor(AXIS, L_ANALOG_Y, LEFT_SENSITIVITY, BACK_LEFT_WHEEL, magnitude).dump())
				self.queue_out.put(Serialize.RawMotor(AXIS, L_ANALOG_Y, LEFT_SENSITIVITY, FRONT_RIGHT_WHEEL, magnitude).dump())
				self.queue_out.put(Serialize.RawMotor(AXIS, L_ANALOG_Y, LEFT_SENSITIVITY, BACK_RIGHT_WHEEL, magnitude).dump())
				self.pre_values[0] = magnitude

		self.bind_axis(L_ANALOG_Y, drive_wheel)

		self.bind_button_down(Y_BUTTON, self.change_mode(self.set_tank_mode))
		self.bind_button_down(B_BUTTON, self.change_mode(self.set_spin_mode))
		self.bind_button_down(X_BUTTON, self.change_mode(self.set_strafe_mode))

	def set_spin_orientation(self):

		self.set_all_swerves(0);

	def set_tank_orientation(self):

		self.set_all_swerves(45);

	def set_strafe_orientation(self):

		self.set_all_swerves(90);

	def set_all_swerves(self, number):

		#print("Trying to set swerves")

		#for i in range(1):
		self.queue_out.put(Serialize.MotorHack(BACK_LEFT_SWERVE, number).dump());
		self.queue_out.put(Serialize.MotorHack(BACK_RIGHT_SWERVE, number).dump());
		self.queue_out.put(Serialize.MotorHack(FRONT_LEFT_SWERVE, number).dump());
		self.queue_out.put(Serialize.MotorHack(FRONT_RIGHT_SWERVE, number).dump());
		#time.sleep(0.05)

	def set_drive_mode(self):
		"""
		Deprecated.
		"""

		self.clear_funcs()

		self.pre_values = {}
		self.pre_values[0] = None
		self.pre_values[1] = None

		self.pre_values[2] = None

		def drive_left(magnitude) :
			if(self.pre_values[0] != magnitude) :
				self.queue_out.put(Serialize.RawMotor(AXIS, L_ANALOG_Y, LEFT_SENSITIVITY, FRONT_LEFT_WHEEL, magnitude).dump())
				self.queue_out.put(Serialize.RawMotor(AXIS, L_ANALOG_Y, LEFT_SENSITIVITY, BACK_LEFT_WHEEL, magnitude).dump())
				self.pre_values[0] = magnitude

		def drive_right(magnitude) :
			if(self.pre_values[1] != magnitude) :
				self.queue_out.put(Serialize.RawMotor(AXIS, R_ANALOG_Y, RIGHT_SENSITIVITY, FRONT_RIGHT_WHEEL, magnitude).dump())
				self.queue_out.put(Serialize.RawMotor(AXIS, R_ANALOG_Y, RIGHT_SENSITIVITY, BACK_RIGHT_WHEEL, magnitude).dump())
				self.pre_values[1] = magnitude

		def drive_actuator(magnitude) :
			if(self.pre_values[2] != magnitude) :
				self.queue_out.put(Serialize.RawMotor(AXIS, TRIGGER, LEFT_SENSITIVITY, BACK_LEFT_SWERVE, magnitude).dump())
				self.pre_values[2] = magnitude

		self.bind_axis(L_ANALOG_Y, drive_left)
		self.bind_axis(R_ANALOG_Y, drive_right)
		self.bind_axis(TRIGGER, drive_actuator)

	def generate_motor_func(self, type, input_id, sensitivity_id, motor_id) :
		def func(magnitude) :
			if magnitude != self.pre_values[motor_id] :
				self.queue_out.put(Serialize.RawMotor(type, input_id, sensitivity_id, motor_id, magnitude).dump())
				self.pre_values[motor_id] = magnitude
		return func

	def generate_motor_func(self, type, input_id, sensitivity_id1, sensitivity_id2, motor_id) :
		def func(magnitude) :
			if magnitude != self.pre_values[motor_id]:
				if magnitude < 0:
					self.queue_out.put(Serialize.RawMotor(type, input_id, sensitivity_id1, motor_id, magnitude).dump())
					self.pre_values[motor_id] = magnitude
				else:
					self.queue_out.put(Serialize.RawMotor(type, input_id, sensitivity_id2, motor_id, magnitude).dump())
					self.pre_values[motor_id] = magnitude
		return func

def main() :
	"""
	Testing code. It works.
	"""
	pass
	
	pygame.init()
	pygame.joystick.init()
	if(pygame.joystick.get_count()) :
		joy = pygame.joystick.Joystick(0)
		joy.init()
		print("Axes: " + str(joy.get_numaxes()))
		print("Balls: " + str(joy.get_numballs()))
		print("Buttons : " + str(joy.get_numbuttons()))
		print("Hats : " + str(joy.get_numhats()))
	t = True
	while(t) :
		for event in pygame.event.get(): # User did something
			if event.type == pygame.QUIT: # If user clicked close
				done=True # Flag that we are done so we exit this loop
        
        # Possible joystick actions: JOYAXISMOTION JOYBALLMOTION JOYBUTTONDOWN JOYBUTTONUP JOYHATMOTION
			if event.type == pygame.JOYBUTTONDOWN:
				print("Joystick button pressed.")
				print([joy.get_button(i) for i in range(0, joy.get_numbuttons())])
				if(joy.get_button(2)) :
					t = False
			if event.type == pygame.JOYBUTTONUP:
				print("Joystick button released.")

			if event.type == pygame.JOYAXISMOTION:
				print([joy.get_axis(i) for i in range(0, joy.get_numaxes())])

			if event.type == pygame.JOYHATMOTION:
				print([joy.get_hat(i) for i in range(0, joy.get_numhats())])
	print("exited")
	pygame.quit
if __name__ == "__main__" :
	main()
