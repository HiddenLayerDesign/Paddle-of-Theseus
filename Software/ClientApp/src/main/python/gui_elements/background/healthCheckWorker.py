import logging
from time import sleep
from PyQt5.QtCore import QObject, pyqtSignal
from serial import SerialTimeoutException, SerialException


class HealthCheckWorker(QObject):
    finished = pyqtSignal()
    isConnectedSig = pyqtSignal(bool)

    def __init__(self, serialInterpreter):
        super().__init__()
        self.isRunning = True
        self.heartbeatIdx = 0
        self.si = serialInterpreter
        self.isConnectedBool = False
        self.allowedErrors = 5
        self.consecutiveErrors = 0
        sleep(1)

    def setRunning(self, is_running):
        running_state = "RUNNING" if is_running else "NOT RUNNING"
        logging.info(f"HealthCheck_thread is {running_state}")
        self.isRunning = is_running

    def checkHeartBeat(self):
        try:
            return_message = self.si.sendHeartbeat()
            if return_message == self.si.testResponse:
                self.isConnectedBool = True
                self.heartbeatIdx += 1
                self.consecutiveErrors = 0
            else:
                self.consecutiveErrors += 1
                logging.warning(f"HealthCheck Failed! Error {self.consecutiveErrors}/{self.allowedErrors}")
                self.si.flushSerial()
                self.isConnectedBool = True
                if self.allowedErrors <= 0:
                    self.isConnectedBool = False
                    self.setRunning(False)
        except (SerialException, SerialTimeoutException) as e:
            self.isConnectedBool = False
            self.setRunning(False)

        self.isConnectedSig.emit(self.isConnectedBool)

    def run(self):
        while True:
            if self.isRunning is True:
                sleep(2)
                self.checkHeartBeat()
            else:
                sleep(1)

        self.finished.emit()


