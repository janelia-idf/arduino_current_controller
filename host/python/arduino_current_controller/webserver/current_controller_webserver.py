#!python
from __future__ import print_function
import os
import sys
import flask
import flask_sijax
import argparse

from arduino_device import findArduinoDevicePorts

from arduino_current_controller import ArduinoCurrentController
from arduino_current_controller import isArduinoCurrentControllerPortInfo


network_port = 5000

# Setup application w/ sijax
path = os.path.join('.', os.path.dirname(__file__), 'static/js/sijax/')
app = flask.Flask(__name__)
app.config["SIJAX_STATIC_PATH"] = path
app.config["SIJAX_JSON_URI"] = '/static/js/sijax/json2.js'
flask_sijax.Sijax(app)

class SijaxHandler(object):
    """A container class for all Sijax handlers.

    Grouping all Sijax handler functions in a class
    (or a Python module) allows them all to be registered with
    a single line of code.
    """

    # arduino_current_controller
    @staticmethod
    def turnOnAll(obj_response,percent_capacity):
        if ACC is not None:
            ACC.turnOnAll(percent_capacity)

    @staticmethod
    def turnOffAll(obj_response):
        if ACC is not None:
            ACC.turnOffAll()

    @staticmethod
    def startPwmAll(obj_response,percent_capacity,duration_on,duration_off):
        if ACC is not None:
            ACC.startPwmAll(percent_capacity,duration_on,duration_off)

    @staticmethod
    def stopPwmAll(obj_response):
        if ACC is not None:
            ACC.stopPwmAll()


@flask_sijax.route(app, "/")
def index():
    if flask.g.sijax.is_sijax_request:
        # The request looks like a valid Sijax request
        # Let's register the handlers and tell Sijax to process it
        flask.g.sijax.register_object(SijaxHandler)
        return flask.g.sijax.process_request()

    return flask.render_template('current_controller.html')


def arduinoCurrentControllerWebserver():
    global ACC
    ACC = None

    parser = argparse.ArgumentParser(description='Arduino Current Controller Webserver')
    parser.add_argument('-d', '--debug',
                        help='start the server in local-only debug mode',
                        action='store_true')
    args = parser.parse_args()

    server = 'remote'
    debug = False
    if args.debug:
        server = 'local'
        debug = True

    # Open connection to device
    arduino_device_ports = findArduinoDevicePorts()
    arduino_current_controller_port = None
    for port in arduino_device_ports:
        port_info = arduino_device_ports[port]
        if isArduinoCurrentControllerPortInfo(port_info):
            arduino_current_controller_port = port

    if arduino_current_controller_port is not None:
        ACC = ArduinoCurrentController(port=arduino_current_controller_port)

    if server == 'local':
        print(' * using debug server - localhost only')
        app.run(port=network_port,debug=debug)
    else:
        print(' * using builtin server - remote access possible')
        app.run(host='0.0.0.0',port=network_port)


# -----------------------------------------------------------------------------
if __name__ == '__main__':
    arduinoCurrentControllerWebserver()
