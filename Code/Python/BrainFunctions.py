import socket
import json
import time

class Robot:

    ip = "192.168.4.1"  # The server's hostname or IP address
    port = 100  # The port used by the server
    debug = False

    def __init__(self, ip="192.168.4.1",port=100,debug=False):
        if debug: print("Robot waking up")
        self.ip = ip
        self.port = port
        self.debug = debug

    def move(self, angle, time, speed):
        return self.send_command({"cmd":"move", "angle":angle, "time":time, "speed":speed})
    
    def move_until(self,speed,distance):
        self.set_motor_speeds(speed,speed)
        while self.get_ultrasound()>distance:
            continue
        self.stop()

    def set_motor_speeds(self, left, right):
        return self.send_command({"cmd":"set_motor_speeds", "left":left, "right":right})

    def stop(self):
        return self.send_command({"cmd":"stop"})
    
    def rotate_for_time(self, wait_time, speed):
        self.set_motor_speeds(speed, -speed)
        time.sleep(wait_time)
        self.stop()

    def rotate(self, angle, speed):
        return self.send_command({"cmd":"rotate", "angle":angle, "speed":speed})

    def rotate_right(self, angle, speed):
        return self.rotate(angle, speed)

    def rotate_left(self, angle, speed):
        return self.rotate(-angle, speed)

    def wait(self, time):
        return self.send_command({"cmd":"wait", "time":time})

    def set_servo(self, angle):
        return self.send_command({"cmd":"servo", "angle":angle})

    def get_ultrasound(self):
        distance = self.send_command({"cmd":"get_ultrasound"})
        if distance is None: return 0
        else: return distance

    def is_far_enough(self, distance, minimum_distance):
        return distance > minimum_distance

    def get_yaw(self):
        return self.send_command({"cmd":"get_yaw"})

    def set_led(self, r, g, b, p):
        return self.send_command({"cmd":"led", "r":r, "g":g, "b":b, "p":p})

    def send_command(self, command, attempt=0):
        if self.debug: print(f"Send attempt: {attempt}")
        try:
            with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as connection:
                connection.connect((self.ip,self.port))
                connection.sendall(bytes(json.dumps(command),'utf-8'))
                empty_count = 0
                while True:
                    data = connection.recv(1024)
                    decoded_data = data.decode("utf-8").strip()
                    if self.debug: print(f"Read: {decoded_data}")
                    if len(decoded_data) <= 0:
                        empty_count = empty_count + 1
                        if self.debug: print(empty_count)
                        if empty_count < 5:
                            continue
                        else:
                            return False
                    lines = decoded_data.split('\n')
                    for line in lines:
                        blocks = line.split('}')
                        for block in blocks:
                            block = block.strip()
                            block += '}'
                            if self.debug: print(f"Received: {block}")
                            if block[0] != '{' or block[-1] != '}' or not "\"" in block:
                                continue
                            if self.debug: print(f"Parsing: {block}")
                            json_data = json.loads(block)
                            if("result" in json_data.keys()):
                                return json_data["result"]
                            elif("ok" in json_data.keys()):
                                return json_data["ok"]
                            else:
                                continue
        except ConnectionResetError:
            if attempt >= 3: raise
            else: self.send_command(command, attempt+1)
        finally:
            time.sleep(0.05)
