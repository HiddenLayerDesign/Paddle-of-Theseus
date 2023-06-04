import socket


class StubPaddleUserSocket:
    def __init__(self, parent=None, host="127.0.0.1", port=65411):
        self.parent = parent
        self.HOST = host
        self.PORT = port
        self.socket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
        self.socket.bind((self.HOST, self.PORT))
        self.socket.setblocking(False)

        self.help_text = ("StubPaddle userSocket commands:\n\n"
                          "\treset: Reset config data to defaults\n"
                          "\tmemDump: Dump out current config in a memDump format mimicking Teensy-LC mem layout\n"
                          "\texit: Stop the StubPaddle program\n"
                          "\thelp: Print this message\n"
                          )

    def parseInput(self, raw_input):
        if raw_input == "":
            return
        input_command = raw_input.decode("UTF-8")

        if input_command == "reset":
            self.handleReset()
        elif input_command == "memDump":
            self.handleMemDump()
        elif input_command == "exit":
            self.handleExit()
        elif input_command == "help":
            self.printHelpText()
        else:
            print(f"Error: Invalid userSocket input \"{input_command}\"")
            self.printHelpText()

    def printHelpText(self):
        print(self.help_text)

    def handleReset(self):
        self.parent.userHandleReset()

    def handleMemDump(self):
        self.parent.userHandleMemDump()

    def handleExit(self):
        self.parent.userHandleExit()

