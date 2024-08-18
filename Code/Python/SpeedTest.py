import BrainFunctions
import time

robot = BrainFunctions.Robot()

speed = 200

while speed > 0:
    print(speed)
    robot.set_motor_speeds(speed, -speed)
    time.sleep(5)
    speed -= 10

robot.stop()