import time
import glob
import sys

from serial import Serial
from serial.tools import list_ports


class serialInterpreter:

    def __init__(self):
        """


        """
        self.serialPrompt = b"Paddle>"
        self.testMessage = b"paddlePing\r"
        self.testResponse = b"paddlePong\r\n"

        self.serial = Serial(
            baudrate=9600,
            timeout=2
        )
        self.serialConn = self.getSerialConnection()

        if not self.serialConn:
            raise RuntimeError("Failed to find a single Serial Connection")

        print("Found serial connection at {0}".format(self.serialConn))

        self.serial.open()
        self.sendSerialCommand(b"about\r")
        self.serial.close()

    def testSerialConnection(self, inPort=None):
        """

        :return: True == This serial port connects to the Paddle
        """
        if not inPort:
            return False

        self.serial.port = inPort

        try:
            self.serial.open()
        except Exception as e:
            return False

        # if you're getting the serial for the first time, read through the prompt
        string = ""
        if self.serial.read_until():
            while string != self.serialPrompt:
                string = self.serial.read_until()

        self.serial.write(self.testMessage)
        self.serial.read_until()  # throw away first newline
        if self.serial.read_until() == self.testResponse:
            self.serial.close()
            return True

        self.serial.close()
        return False

    def sendSerialCommand(self, cmd, argument=None):
        """

        :return:
        """
        if argument is not None:
            cmd_str = "{0}={1}".format(cmd, argument.upper)
        else:
            cmd_str = cmd

        self.serial.write(cmd_str)
        message = ""
        latest = b""
        while latest != self.serialPrompt:
            message += latest.decode(encoding="UTF-8")
            latest = self.serial.read_until()
        print(message)

    def getSerialConnection(self):
        """

        :return:
        """
        if sys.platform.startswith('win'):
            for comport_struct in list_ports.comports():
                if self.testSerialConnection(comport_struct.name):
                    return comport_struct.name

        elif sys.platform.startswith('linux') or sys.platform.startswith('cygwin'):
            # this excludes your current terminal "/dev/tty"
            for port_str in glob.glob('/dev/tty?*'):
                if self.testSerialConnection(port_str):
                    return port_str

        else:
            raise RuntimeError('Only Windows, Linux, and Cygwin are supported!')
        return None
