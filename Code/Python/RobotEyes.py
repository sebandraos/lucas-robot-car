from ultralytics import YOLO

class Eyes:

    def __init__(self, debug=False):
        print("Opening eyes...")
        self.debug = debug
        self.model = YOLO("yolov8n.pt")  # load a pretrained model (recommended for training)

    def StartLooking(self):
        print("Looking...")
        return self.model.predict(source="http://192.168.4.1:81/stream", show=True, stream=True)

    def FindPerson(self, result):
        confidence = 0
        centre = []
        for box in result.boxes:
            if self.debug:
                print(box.cls.item())
                print(box.conf.item())
                print(box.xyxyn)
            if box.cls[0] != 0 or box.conf[0] < confidence:
                continue

            confidence = box.conf[0].item()
            corners = box.xyxyn[0]
            centre_x = (corners[0].item()+corners[2].item())/2
            centre_y = (corners[1].item()+corners[3].item())/2
            centre = [centre_x, centre_y]
        
        if self.debug:
            print("Winner:")
            print(confidence)
            print(centre)
        return centre