import BrainFunctions

robot=BrainFunctions.Robot()

speed=120
maximum_distance= 30

robot.set_motor_speeds(speed,speed)

while robot.get_ultrasound()>maximum_distance:
    continue
robot.stop()