import BrainFunctions

speed = 120
turn_speed = 65
move_time = 0.5
rotate_time = 0.9
wait_time = 1
minimum_distance = 30

robot = BrainFunctions.Robot()

while True:
      # set the servo to 90 degrees
      robot.set_servo(90)
      # wait for 1 second
      robot.wait(wait_time)
      # check how far away the closest object is
      distance = robot.get_ultrasound()
      # if it is far enough
      if robot.is_far_enough(distance, minimum_distance):
        # move forwards a bit
        #robot.move(0, move_time, speed)
        robot.move_until(speed,minimum_distance)
      else:
        # set the servo to 0 degrees
        robot.set_servo(0)
        # wait for 1 second
        robot.wait(wait_time)
        # check how far away the closest object is
        distance = robot.get_ultrasound()
        # if it is far enough
        if robot.is_far_enough(distance, minimum_distance):
            # move forwards a bit
            #robot.rotate_right(90, turn_speed)
            robot.rotate_for_time(rotate_time,-turn_speed)
        else:
            # set the servo to 180 degrees
            robot.set_servo(180)
            # wait for 1 second
            robot.wait(wait_time)
              # check how far away the closest object is
            distance = robot.get_ultrasound()
            # if it is far enough
            if robot.is_far_enough(distance, minimum_distance):
                # move forwards a bit
                #robot.rotate_left(90, turn_speed)
                robot.rotate_for_time(rotate_time,turn_speed)
            else:
                # move backwards a bit
                robot.move(180, move_time, turn_speed)

robot.stop()