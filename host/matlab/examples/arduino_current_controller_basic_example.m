function arduino_stepper_controller_basic_example(port)
% arduino_controller_basic_example: demonstrates basic use of the
% ArduinoStepperController class to control the arduino stepper
% controller.
%
% Usage: (replace com5 with the serial port of your device)
%
% arduino_stepper_controller_basic_example('com5')
%
%  * Find serial port of Arduino board connected with a USB cable
%    Use Arduino environment to help find port or read more details
%    here: http://arduino.cc/en/Guide/HomePage
%    Windows:
%      Typically 'com3' or higher. Use 'Device Manager' and look
%      under 'Ports'.
%    Mac:
%      Typically something like '/dev/tty.usbmodem'
%    Linux:
%      Typically something like '/dev/ttyACM0'
%

    % Create the arduino stepper controller device object, open serial
    % connection and display device info.
    fprintf('Opening Arduino Stepper Controller device...\n');
    dev = ArduinoStepperController(port);
    dev.open();
    fprintf('Arduino Stepper Controller Info:');
    dev.getDevInfo()

    % Pause for a little bit for added dramma
    pause(2.0)

    % Print dynamic methods
    dev.printDynamicMethods()
    fprintf('\n');

    % Pause for a little bit for added dramma
    pause(2.0)
    
    % Set the waypoint count to 36 (36 positions per rotation)
    fprintf('Setting waypoint count to 36.\n');
    dev.setWaypointCount(36);  

    % Pause for a little bit for added dramma
    pause(2.0)
    
    % Set the waypoint travel time to 850 milliseconds (time it
    % takes to rotate to next waypoint)
    fprintf('Setting waypoint travel time to 850ms.\n');
    dev.setWaypointTravelTime(850);  

    % Pause for a little bit for added dramma
    pause(2.0)
    
    % Move to 3 waypoints pausing briefly in between
    fprintf('Moving to 3 waypoints ...\n');
    for n = 1:3
      dev.goToNextWaypoint();
      dev.wait();  
      pause(1.0)
    end
    fprintf('Finished moves.\n');
    
    % Play tone (1000Hz for 500ms)
    fprintf('Playing 1000Hz tone for 500ms.\n');
    dev.tone(1000,500);
    
    % Clean up - 
    dev.close();
    delete(dev);
    fprintf('Closed device. Goodbye!\n');
end
