#!python
from __future__ import print_function, division
import atexit
import argparse
import copy
import time

from arduino_device import ArduinoDevice, ArduinoDevices, findArduinoDevicePorts


DEBUG = False
BAUDRATE = 'default'
DEVICE_MODEL_NUMBER = 2346

class ArduinoCurrentController(ArduinoDevice):

    """

    ArduinoCurrentController provides a serial interface to the
    arduino current controller.

    Dynamically generated public class methods:

    getDevInfo - returns a dictionary containing device information,
    e.g. serial number, model number.
    Usage: infoDict = dev.getDevInfo()

    setArduinoSerialNumber - sets the serial number of the device.
    Usage: dev.setSerialNumber(serial_number)

    turnOnAll - turns on all channels to the current value equal to
    the percent_capacity*0.01*current_limit
    e.g. percent_capacity = 20, current_limit = 0.7 amps,
    current_value = 0.14 amps
    Usage: dev.turnOnAll(percent_capacity)

    turnOffAll - turns off all channels. (Sets current value to 0.)
    Usage: dev.turnOffAll()

    startPwmAll - starts a continuous square wave pattern on all of
    the channels. Turns on all channels to the current value equal to
    the percent_capacity*0.01*current_limit for a duration of
    duration_on (ms), followed by a current value of 0 for
    duration_off (ms). Period (in seconds) = (duration_on +
    duration_off)/1000. Duty Cycle = duration_on/(duration_on +
    duration_off).
    Usage: dev.startPwmAll(percent_capacity,duration_on,duration_off)

    stopPwmAll - stops pwm on all channels and sets the current value
    to 0.
    Usage: dev.stopPwmAll()

    """

    def __init__(self,*args,**kwargs):
        kwargs.update({'model_number': DEVICE_MODEL_NUMBER})
        serial_number = None
        if 'debug' not in kwargs:
            kwargs.update({'debug': DEBUG})
        if 'baudrate' not in kwargs:
            kwargs.update({'baudrate': BAUDRATE})
        elif (kwargs['baudrate'] is None) or (kwargs['baudrate'].lower() == 'default'):
            kwargs.update({'baudrate': BAUDRATE})
        if 'serial_number' in kwargs:
            serial_number = kwargs.pop('serial_number')
        if ('port' not in kwargs) or (kwargs['port'] is None):
            port =  findArduinoCurrentControllerPort(baudrate=kwargs['baudrate'],
                                                     serial_number=serial_number)
            kwargs.update({'port': port})
        super(ArduinoCurrentController,self).__init__(*args,**kwargs)
        atexit.register(self._exitArduinoCurrentController)
        dev_info = self.getDevInfo()

    def _exitArduinoCurrentController(self):
        try:
            pass
        except IOError:
            pass

    def getArduinoCurrentControllerInfo(self):
        arduino_current_controller_info = self.getArduinoDeviceInfo()
        arduino_current_controller_info.update(self.getDevInfo())
        return arduino_current_controller_info


# device_names example:
# [{'port':'/dev/ttyACM0',
#   'device_name':'current_controller0'},
#  {'serial_number':3,
#   'device_name':'current_controller1'}]
class ArduinoCurrentControllers(ArduinoDevices):

    def __init__(self,*args,**kwargs):
        if ('use_ports' not in kwargs) or (kwargs['use_ports'] is None):
            kwargs['use_ports'] = findArduinoCurrentControllerPorts(*args,**kwargs)
        super(ArduinoCurrentControllers,self).__init__(*args,**kwargs)

    def appendDevice(self,*args,**kwargs):
        self.append(ArduinoCurrentController(*args,**kwargs))

    def getArduinoCurrentControllerInfo(self):
        arduino_current_controller_info = []
        for device_index in range(len(self)):
            dev = self[device_index]
            arduino_current_controller_info.append(dev.getArduinoCurrentControllerInfo())
        return arduino_current_controller_info


def findArduinoCurrentControllerPorts(baudrate=None, serial_number=None, try_ports=None, debug=DEBUG):
    arduino_device_ports = findArduinoDevicePorts(baudrate=baudrate,
                                                  model_number=DEVICE_MODEL_NUMBER,
                                                  serial_number=serial_number,
                                                  try_ports=try_ports,
                                                  debug=debug)

    if type(serial_number) is int:
        serial_number = [serial_number]

    arduino_current_controller_ports = {}
    for port in arduino_device_ports:
        try:
            dev_serial_number = arduino_device_ports[port]['serial_number']
        except KeyError:
            break
        if (serial_number is None) or (dev_serial_number in serial_number):
            arduino_current_controller_ports[port] = {'serial_number': dev_serial_number}
    return arduino_current_controller_ports

def findArduinoCurrentControllerPort(baudrate=None, serial_number=None, try_ports=None, debug=DEBUG):
    arduino_current_controller_ports = findArduinoCurrentControllerPorts(baudrate=baudrate,
                                                                         serial_number=serial_number,
                                                                         try_ports=try_ports,
                                                                         debug=debug)
    if len(arduino_current_controller_ports) == 1:
        return arduino_current_controller_ports.keys()[0]
    elif len(arduino_current_controller_ports) == 0:
        arduino_device_ports = findArduinoDevicePorts(baudrate=baudrate,
                                                      model_number=DEVICE_MODEL_NUMBER,
                                                      serial_number=serial_number,
                                                      try_ports=try_ports,
                                                      debug=debug)
        err_str = 'Could not find Arduino current controllers. Check connections and permissions.\n'
        err_str += 'Tried ports: ' + str(arduino_device_ports)
        raise RuntimeError(err_str)
    else:
        err_str = 'Found more than one Arduino current controller. Specify port or serial_number.\n'
        err_str += 'Matching ports: ' + str(arduino_current_controller_ports)
        raise RuntimeError(err_str)

def isArduinoCurrentControllerPortInfo(port_info):
    return port_info['model_number'] == DEVICE_MODEL_NUMBER

def arduinoCurrentControllerCli():
    parser = argparse.ArgumentParser(description='Arduino Current Controller')
    # parser.add_argument('-i','--info',nargs=1,type=int,default=[0],
    #                     help='current index')

    args = parser.parse_args()

    o = ArduinoCurrentController(debug=DEBUG)
    # if args.subparser_name == 'current':
    #     current = args.current[0]
    #     if args.on:
    #         o.setCurrentOn(current)
    #     elif args.off:
    #         o.setCurrentOff(current)
    #     else:
    #         print("Add --on or --off")
    # elif args.subparser_name == 'info':
    #     print(o.getArduinoCurrentControllerInfoDict())

# -----------------------------------------------------------------------------------------
if __name__ == '__main__':
    arduinoCurrentControllerCli()
