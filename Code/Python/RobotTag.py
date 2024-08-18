import BrainFunctions
import RobotEyes
import random
import time

speed = 255
turn_speed = 75
move_time = 0.5
wait_time = 1
minimum_distance = 10

robot = BrainFunctions.Robot()

robot.stop()

eyes = RobotEyes.Eyes()
results = eyes.StartLooking()

try:
    for result in results:
        if result is None:
            continue
        person_location = eyes.FindPerson(result)
        if len(person_location) == 2 and robot.get_ultrasound() > minimum_distance:
            x_value = person_location[0]
            if x_value > 0.475 and x_value < 0.525:
                robot.move(0, move_time, speed)
            else:
                angle = (90 * x_value) - 45
                #print(f"x: {x_value} -> {angle}")
                robot.move(angle, move_time, turn_speed)
        else:
            rotate_time = random.random() * 1
            if rotate_time < 0.1: rotate_time = 0.1
            if random.random() > 0.5:
                robot.rotate_for_time(rotate_time, turn_speed)
            else:
                robot.rotate_for_time(rotate_time, -turn_speed)
            time.sleep(0.5)
finally:
    robot.stop()