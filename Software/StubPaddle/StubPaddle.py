"""
Application to mimic hardware connection to Paddle of Theseus
=============================================================

StubPaddle is a python executable distinct from the Paddle of Theseus client application
with the purpose of helping to debug and develop for the client application without having access to hardware.

This StubPaddle relies on an outside program like https://com0com.sourceforge.net/ to provide a virtual COM port-pair
to which the client application and this executable can attach.

This program receives serial commands from the clientApp and maintains a bytearray to mimic the non-volatile
memory region of the Teensy-LC hardware. It can accept UDP commands on its user socket
at default location "127.0.0.1":65411 to dump out the memory or save/load its state
"""
import json
import serial
import serial.tools.list_ports as port_list
from copy import deepcopy
from StubPaddleConstants import *
from StubPaddleUserSocket import StubPaddleUserSocket


class StubPaddle:
    def __init__(self):
        self.version_major = 0
        self.version_minor = 9
        self.version_bugfix = 9
        self.config_mode_enabled = 1
        self.non_volatile_mem = bytearray(128)  # emulating the physical EEPROM of the Paddle of Theseus
        self.serial_port = serial.Serial(port=port_list.comports()[0].name, baudrate=9600, timeout=3)
        self.is_running = True
        self.about_text = "Stub paddle written by Chase E. Stewart for Hidden Layer Design"
        self.color = "BLUE"
        self.config_dict = {"current_config": self.color, "all_configs": {}}
        for item in color_str_array:
            self.config_dict["all_configs"][item] = deepcopy(base_config_dict)

        self.updateNonVolMem()

        # see StubPaddleUserSocket.py
        print("Initializing User Socket")
        self.user_socket = StubPaddleUserSocket(parent=self, host='127.0.0.1', port=65411)

        self.serial_port.write(b"\r\nPaddle>")
        print("Paddle stub initialized!")
        print(f"listening on {self.serial_port.name}")

    def mainLoop(self):
        while self.is_running:
            try:
                message = self.serial_port.read_until(expected=b"\r").decode("UTF-8").replace("\r", "")
                self.parseMessage(message)
                self.serial_port.write(b"\r\nPaddle>")
            except BlockingIOError:
                pass
            try:
                (user_data, address) = self.user_socket.socket.recvfrom(256)
                self.user_socket.parseInput(user_data)
            except BlockingIOError:
                pass
            except Exception as e:
                print(e)

    def updateSingleConfig(self, start_offset, config):
        self.non_volatile_mem[start_offset + CT1_DELTA_ADDR] = config["offset1"]
        self.non_volatile_mem[start_offset + CT2_DELTA_ADDR] = config["offset2"]
        self.non_volatile_mem[start_offset + CT3_DELTA_ADDR] = config["offset3"]
        self.non_volatile_mem[start_offset + ROOT_NOTE_ADDR] = rootNoteDict[(config["root_note"])]
        self.non_volatile_mem[start_offset + SCALE_MOD_ADDR] = modeList.index(config["mode"])
        self.non_volatile_mem[start_offset + MODE_ENABLED_ADDR] = 1 if config["enable"] == "TRUE" else 0
        self.non_volatile_mem[start_offset + CTRL_CHAN_ADDR] = config["control"]
        self.non_volatile_mem[start_offset + OCTAVE_ADDR] = config["octave"]
        self.non_volatile_mem[start_offset + PB_CHAN_ADDR] = config["pitchbend"]

    def updateNonVolMem(self):
        self.non_volatile_mem[EEPROM_CONFIG_MODE_ADDRESS] = self.config_mode_enabled
        self.non_volatile_mem[EEPROM_VERSION_MAJOR_ADDRESS] = self.version_major
        self.non_volatile_mem[EEPROM_VERSION_MINOR_ADDRESS] = self.version_minor
        self.non_volatile_mem[EEPROM_VERSION_BUGFIX_ADDRESS] = self.version_bugfix

        self.updateSingleConfig(EEPROM_BLUE_BASE_ADDR, self.config_dict["all_configs"]["BLUE"])
        self.updateSingleConfig(EEPROM_WHITE_BASE_ADDR, self.config_dict["all_configs"]["WHITE"])
        self.updateSingleConfig(EEPROM_CYAN_BASE_ADDR, self.config_dict["all_configs"]["CYAN"])
        self.updateSingleConfig(EEPROM_PURPLE_BASE_ADDR, self.config_dict["all_configs"]["PURPLE"])
        self.updateSingleConfig(EEPROM_GREEN_BASE_ADDR, self.config_dict["all_configs"]["GREEN"])
        self.updateSingleConfig(EEPROM_RED_BASE_ADDR, self.config_dict["all_configs"]["RED"])
        self.updateSingleConfig(EEPROM_YELLOW_BASE_ADDR, self.config_dict["all_configs"]["YELLOW"])

    def parseMessage(self, message):
        if message == "":
            return

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
            self.userHandleMemDump()
        elif command == "paddlePing":
            self.handlePaddlePing()
        elif command == "exit":
            self.userHandleExit()
        else:
            print(f"Invalid command '{message}'")

        self.updateNonVolMem()

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
        self.config_dict["all_configs"][self.color]['octave'] = int(octave)

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

    def handlePaddlePing(self):
        print("RX 'paddlePing'")
        self.serial_port.write(b"\r\npaddlePong")

    """ Commands below are handlers for user socket commands """
    def userHandleReset(self):
        self.handleDefaults()

    def userHandleMemDump(self):
        print_str = "*** MemDump ***\n"
        for i in range(int(self.non_volatile_mem.__len__() / 16)):
            print_str += f"0x{(16*i):02x}:"
            for j in range(16):
                print_str += f" {(self.non_volatile_mem[(16 * i) + j]):02x}"
            print_str += f" ({mem_dump_hint[i]})\n"

        print(print_str)

    def userHandleExit(self):
        self.is_running = False


if __name__ == "__main__":
    """ Run the program """
    print("MAIN: Initializing StubPaddle")
    stub = StubPaddle()
    print("MAIN: Running StubPaddle loop")
    stub.mainLoop()
    print("MAIN: Exiting")
