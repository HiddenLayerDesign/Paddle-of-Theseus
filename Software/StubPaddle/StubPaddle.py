import json
import serial
import serial.tools.list_ports as port_list
from time import sleep
from copy import deepcopy
from StubPaddleConstants import *


class StubPaddle:

    def __init__(self):
        self.non_volatile_mem = bytearray(128)
        for port in port_list.comports():
            print(port.name)
        self.serial_port = serial.Serial(port=port_list.comports()[0].name, baudrate=9600)
        self.is_running = True
        self.about_text = "Stub paddle written by Chase E. Stewart for Hidden Layer Design"
        self.color = "BLUE"
        self.config_dict = {"current_config": self.color, "all_configs": {}}
        for item in color_str_array:
            self.config_dict["all_configs"][item] = deepcopy(base_config_dict)

        print(json.dumps(self.config_dict))

        self.serial_port.write(b"\r\nPaddle>")
        print("Paddle stub initialized!")
        print(f"listening on {self.serial_port.name}")

    def mainLoop(self):
        while self.is_running:
            message = self.serial_port.read_until(expected=b"\r").decode("UTF-8").replace("\r", "")
            self.parseMessage(message)
            self.serial_port.write(b"\r\nPaddle>")

    def parseMessage(self, message):
        param = None
        command = message.split("=")[0]
        if len(message.split("=")) > 1:
            param = message.split("=")[1]
        if command == "about":
            self.handleAbout()
        elif command == "all_config":
            self.handleAllConfig()
        elif command == "enable":
            self.handleEnable(param)
        elif command == "color":
            self.handleColor(param)
        elif command == "root_note":
            self.handleRootNote(param)
        elif command == "octave":
            self.handleOctave(param)
        elif command == "mode":
            self.handleMode(param)
        elif command == "offset1":
            self.handleOffset(param, 1)
        elif command == "offset2":
            self.handleOffset(param, 2)
        elif command == "offset3":
            self.handleOffset(param, 3)
        elif command == "control":
            self.handleControl(param)
        elif command == "pitchbend":
            self.handlePitchBend(param)
        elif command == "defaults":
            self.handleDefaults()
        elif command == "memDump":
            self.handleMemDump()
        elif command == "paddlePing":
            self.handlePaddlePing()
        elif command == "exit":
            self.handleExit()
        else:
            print(f"Invalid command '{message}'")
        #print(f"config is now {json.dumps(self.config_dict)}")

    def handleAbout(self):
        print("RX 'about'")
        print(self.about_text)

    def handleAllConfig(self):
        print("RX 'all_config'")
        self.serial_port.write(json.dumps(self.config_dict).encode("UTF-8"))

    def handleEnable(self, enable):
        print("RX 'enable' = {}".format(str(enable)))
        self.config_dict["all_configs"][self.color]['enable'] = enable

    def handleColor(self, color):
        print("RX 'color' = {}".format(str(color)))
        if color in color_str_array:
            print(f"Setting color to {color}")
            self.color = color
            self.config_dict["current_config"] = self.color
        else:
            print(f"Provided color {color} is invalid")

    def handleRootNote(self, root_note):
        print("RX 'root_note' = {}".format(str(root_note)))
        self.config_dict["all_configs"][self.color]['root_note'] = root_note

    def handleOctave(self, octave):
        print("RX 'octave' = {}".format(str(octave)))
        self.config_dict["all_configs"][self.color]['octave'] = octave

    def handleMode(self, mode):
        print("RX 'mode' = {}".format(str(mode)))
        self.config_dict["all_configs"][self.color]['mode'] = mode

    def handleOffset(self, value, offset_idx):
        print("RX 'offset{}' = {}".format(offset_idx, str(value)))
        self.config_dict["all_configs"][self.color][f"offset{offset_idx}"] = int(value)

    def handleControl(self, control_channel):
        print("RX 'control' = {}".format(control_channel))
        self.config_dict["all_configs"][self.color]["control"] = int(control_channel)

    def handlePitchBend(self, pitch_bend):
        print("RX 'pitch_bend' = {}".format(pitch_bend))
        self.config_dict["all_configs"][self.color]["pitchbend"] = int(pitch_bend)

    def handleDefaults(self):
        print("RX 'defaults'")
        for item in color_str_array:
            self.config_dict["all_configs"][item] = deepcopy(base_config_dict)

    def handleMemDump(self):
        print("RX 'memDump'")
        print(self.non_volatile_mem)

    def handlePaddlePing(self):
        print("RX 'paddlePing'")
        self.serial_port.write(b"\r\npaddlePong")

    def handleExit(self):
        print("RX 'exit'")
        self.serial_port.close()
        sleep(1)
        self.serial_port.open()


if __name__ == "__main__":
    """ Run the program """
    print("MAIN: Initializing stub")
    stub = StubPaddle()
    print("MAIN: Running loop")
    stub.mainLoop()
