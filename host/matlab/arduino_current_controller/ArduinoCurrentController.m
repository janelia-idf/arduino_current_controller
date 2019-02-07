%
% ArduinoCurrentController - provides a serial interface to the
% arduino current controller. 
%
% Notes: 
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
%  * Motor must be enabled before sending move commands
%
% Typical Usage:  
%
%   dev = ArduinoCurrentController('com4')  % creates a device object
%   dev.open()                              % opens a serial connection to the device
%   dev.getDevInfo()                        % get device information
%   dev.getCmds()                           % get device commands
%   dev.turnOnAll(percent_capacity)         % turn on all channels
%   dev.turnOffAll()                        % turn off all channels
%   dev.startPwmAll(percent_capacity,duration_on,duration_off) % start pwm all channels
%   dev.stopPwmAll()                        % start pwm all channels
%   dev.close()                             % close serial connection
%   delete(dev)                             % deletes the device
%
%
% Public properties
% ------------------
%
%   * debug = debug flag, turns on debug messages if true. 
%
%   (Dependent)
%   * isOpen    = true is serial connection to device is open, false otherwire
%   * devInfo   = device information structure (model number, serial number)
%   * cmdIds    = structre of command identification numbers retrieved from device. 
%   * rspCodes  = structure of response codes retreived from device.
%
%
% Note, in what follows 'dev' is assumed to be an instance of the ArduinoCurrentController class.
% dev = ArduinoCurrentController(portName)
%
% Regular (public) class methods
% -----------------------------
%
%   * open - opens serial connection to device
%     Usage: dev.open()
%
%   * close - closes serial connection to device
%     Usage: dev.close()
%
%   * delete - deletes instance of device object.
%     Usage: dev.delete() or delete(dev)
%
%   * printDynamicMethods - prints the names of all dynamically generated class 
%     methods. Note, the device must be opened for this command to work.
%     Usage: dev.printDynamicMethods()
% 
% 
% Dynamically generated (public) class methods
% ---------------------------------------------
%
%  Note, the serial connection to the device must be open for these methods to exist.
%
%   * getDevInfo - returns structure containing device information, e.g. serial number, 
%     model number. 
%     Usage: infoStruct = dev.getDevInfo()
%
%   * setArduinoSerialNumber - sets the serial number of the device.    
%     Usage: dev.setSerialNumber(serialNum)
% 
%   * turnOnAll - turns on all channels to the current value equal to
%     the percent_capacity*0.01*current_limit
%     e.g. percent_capacity = 20, current_limit = 0.7 amps,
%     current_value = 0.14 amps
%     Usage: dev.turnOnAll(percent_capacity)
%
%   * turnOffAll - turns off all channels. (Sets current value to 0.)
%     Usage: dev.turnOffAll()
%
%   * startPwmAll - starts a continuous square wave pattern on all of
%     the channels. Turns on all channels to the current value equal to
%     the percent_capacity*0.01*current_limit for a duration of
%     duration_on (ms), followed by a current value of 0 for
%     duration_off (ms). Period (in seconds) = (duration_on +
%     duration_off)/1000. Duty Cycle = duration_on/(duration_on +
%     duration_off).
%     Usage: dev.startPwmAll(percent_capacity,duration_on,duration_off)
%
%   * stopPwmAll - stops pwm on all channels and sets the current value
%     to 0.
%     Usage: dev.stopPwmAll()
%



classdef ArduinoCurrentController < handle 
    
    properties
        dev = [];
        debug = false;
    end

    properties (Access=private)
        cmdIdStruct = [];
        devInfoStruct = [];
        rspCodeStruct = [];
    end

    properties (Constant, Access=private) 

        % Serial communication parameters.
        baudrate = 9600;
        databits = 8;
        stopbits = 1;
        timeout = 1.0;
        terminator = 'LF';
        resetDelay = 2.0;
        inputBufferSize = 2048;
        waitPauseDt = 0.25;
        powerOnDelay = 1.5;

        % Command ids for basic commands.
        cmdIdGetDevInfo = 0;
        cmdIdGetCmds = 1;
        cmdIdGetRspCodes = 2;

    end


    properties (Dependent)
        isOpen;
        devInfo;
        cmdIds;
        rspCodes;
    end

    methods 

        function obj = ArduinoCurrentController(port)
            % ArduinoCurrentController - class constructor.
            obj.dev = serial( ...
            port, ...
            'baudrate', obj.baudrate, ...
            'databits', obj.databits, ... 
            'stopbits', obj.stopbits, ...
            'timeout', obj.timeout, ...
            'terminator', obj.terminator,  ...
            'inputbuffersize', obj.inputBufferSize ...
            );

        end

        function open(obj)
            % Open - opens a serial connection to the device.
            if obj.isOpen == false
                fopen(obj.dev);
                pause(obj.resetDelay);
                obj.createRspCodeStruct();
                obj.createDevInfoStruct();
                obj.createCmdIdStruct();
            end
        end

        function close(obj)
            % Close - closes the connection to the device.
            if obj.isOpen == true
                fclose(obj.dev);
            end
        end

        function delete(obj)
            % Delete - deletes the object.
            if obj.isOpen
                obj.close();
            end
            delete(obj.dev);
        end

        function isOpen = get.isOpen(obj)
            % get.isOpen - returns true/false depending on whether the connection
            % to the device is open.
            status = get(obj.dev,'status');
            if strcmpi(status,'open')
                isOpen = true;
            else
                isOpen = false;
            end
        end

        function printDynamicMethods(obj)
            % printDyamicMethods - prints all dynamically generated class methods.
            cmdIdNames = fieldnames(obj.cmdIdStruct);
            fprintf('\n');
            fprintf('Arduino Current Controller Dynamic Methods\n');
            fprintf('-------------------------\n');
            for i = 1:length(cmdIdNames)
                fprintf('%s\n',cmdIdNames{i});
            end
        end

        function devInfo = get.devInfo(obj)
            % get.devInfo - returns the device information structure.
            devInfo = obj.devInfoStruct;
        end


        function cmdIds = get.cmdIds(obj)
            % get.cmdIds - returns the sturcture of command Ids.
            cmdIds = obj.cmdIdStruct;
        end

        function rspCodes = get.rspCodes(obj)
            % get.rspCodes - returns the structure of device response codes.
            rspCodes = obj.rspCodeStruct;
        end

        function varargout = subsref(obj,S)
            % subsref - overloaded subsref function to enable dynamic generation of 
            % class methods from the cmdIdStruct structure. 
            val = [];
            if obj.isDynamicMethod(S)
                val = obj.dynamicMethodFcn(S);
            else
                if nargout == 0
                    builtin('subsref',obj,S);
                else
                    val = builtin('subsref',obj,S);
                end
            end
            if ~isempty(val)
                varargout = {val};
            end
        end


        function rspStruct = sendCmd(obj,cmdId,varargin)
            % sendCmd - sends a command to device and reads the device's response.
            % The device responds to all commands with a serialized json string.
            % This string is parsed into a Matlab structure.
            %
            % Arguments:
            %  cmdId    = the Id number of the command
            %  varagin  = any additional arguments required by the command
            if obj.isOpen

                % Send command to device
                cmdStr = obj.createCmdStr(cmdId, varargin);
                if obj.debug
                    fprintf('cmdStr: '); 
                    fprintf('%c',cmdStr);
                    fprintf('\n');
                end
                fprintf(obj.dev,'%c\n',cmdStr);

                % Get response as json string and parse
                rspStrJson = fscanf(obj.dev,'%c');
                if obj.debug
                    fprintf('rspStr: '); 
                    fprintf('%c',rspStrJson);
                    fprintf('\n');
                end

                try
                    rspStruct = loadjson(rspStrJson);
                catch ME
                    causeME = MException( ... 
                        'ArduinoCurrentController:unableToPaseJSON', ... 
                        'Unable to parse device response' ...
                        );
                    ME = addCause(ME, causeME); 
                    rethrow(ME);
                end

                % Check the returned cmd Id 
                try
                    rspCmdId = rspStruct.cmd_id;
                    rspStruct = rmfield(rspStruct, 'cmd_id');
                catch ME
                    causeME = MException( ... 
                        'ArduinoCurrentController:MissingCommandId', ... 
                        'device response does not contain command Id' ...
                        );
                    ME = addCause(ME, causeME);
                    rethrow(ME);
                end

                if rspCmdId ~= cmdId
                    msg = sprintf( ...
                        'Command Id returned, %d, does not match that sent, %d', ...
                        rspCmdId, ...
                        cmdId ...
                        );
                    ME = MException('ArduinoCurrentController:cmdIDDoesNotMatch', msg);
                    throw(ME);
                end


                % Get response status
                try
                    rspStatus = rspStruct.status;
                    rspStruct = rmfield(rspStruct,'status');
                catch ME
                    causeME = MException( ... 
                        'ArduinoCurrentController:MissingStatus', ... 
                        'Device response does not contain status' ... 
                        );
                    ME = addCause(ME, causeME);
                    rethrow(ME);
                end

                % Check response status 
                if ~isempty(obj.rspCodeStruct)
                    if rspStatus ~= obj.rspCodeStruct.rsp_success
                        errMsg = 'device responded with error';
                        try
                            errMsg = sprintf('%s, %s',errMsg, rspStruct.err_msg);
                        catch ME
                            errMsg = sprintf('%s, but error message is missing', errMsg);
                        end
                        ME = MException('ArduinoCurrentController:DeviceResponseError', errMsg);
                        throw(ME);
                    end
                end

            else
                ME = MException( ... 
                    'ArduinoCurrentController:DeviceNotOpen', ... 
                    'connection must be open to send command to device' ...
                    );
                throw(ME);
            end
        end

    end

    methods (Access=private)

        function cmdStr = createCmdStr(obj, cmdId, cmdArgs) 
            % createCmdStr - create a command string for sending to the device given
            % the cmdId number and a cell array of the commands arguments.
            cmdStr = sprintf('[%d',uint16(cmdId));
            for i=1:length(cmdArgs)
                arg = cmdArgs{i};
                switch class(arg)
                    case 'double'
                        cmdStr = sprintf('%s, %f', cmdStr, arg);
                    case 'char'
                        cmdStr = sprintf('%s, %s', cmdStr, arg);
                    otherwise
                        errMsg = sprintf('unknown type, %s, in command', class(arg));
                        ME = MException('ArduinoCurrentController:UnknownType', errMsg);
                        throw(ME);
                end
            end
            cmdStr = sprintf('%s]',cmdStr);
        end

        function flag = isDynamicMethod(obj,S)
            % isDynamicMethod - used in the subsred function to determine whether
            % or not the method is dynamically generated. This is determined by
            % whether or not the name of the method given method is also the name
            % of a field in the cmdIdStruct. 
            %
            % Arguments:
            %  S = 'type' + 'subs' stucture passed to subsref function
            flag = false;
            if ~isempty(obj.cmdIdStruct)
                if S(1).type == '.' & isfield(obj.cmdIdStruct,S(1).subs)
                    flag= true;
                end
            end
        end

        function rtnVal = dynamicMethodFcn(obj,S)
            % dynamicMethodFcn - implements a the dynamically generated class methods.

            % Get command name, command args and command id number
            cmdName = S(1).subs;
            try
                cmdArgs = S(2).subs;
            catch
                cmdArgs = {};
            end
            cmdId = obj.cmdIdStruct.(cmdName);
            % Convert command arguments from structure if required
            if length(cmdArgs) == 1 && strcmp(class(cmdArgs{1}), 'struct')
                cmdArgs = obj.convertArgStructToCell(cmdArgs{1});
            end

            % Send command and get response
            rspStruct = obj.sendCmd(cmdId,cmdArgs{:});

            % If is setDrivePowerOn command pause pause to let drive wake up.
            if strcmp(cmdName,'setDrivePowerOn')
                pause(obj.powerOnDelay);
            end

            % Convert response into return value.
            rspFieldNames = fieldnames(rspStruct);
            if length(rspFieldNames) == 0
                rtnVal = [];
            elseif length(rspFieldNames) == 1;
                rtnVal = rspStruct.(rspFieldNames{1});
            else
                emptyFlag = true;
                for i = 1:length(rspFieldNames)
                    name = rspFieldNames{i};
                    value = rspStruct.(name);
                    if ~isempty(value)
                        emptyFlag = false;
                    end
                end
                % Return structure or if only fieldnames return cell array of fieldnames
                if ~emptyFlag
                    rtnVal = rspStruct;
                else
                    rtnVal = rspFieldNames;
                    for i = 1:length(rtnVal)
                        name = rtnVal{i};
                        % Convert '-' character back to human readable
                        if strcmp(name, 'x0x2D_')
                            rtnVal{i} = '-';
                        end
                        % Convert '+' character back to human readable
                        if strcmp(name, 'x0x2B_');
                            rtnVal{i} = '+';
                        end
                    end
                end
            end
        end

        function createCmdIdStruct(obj)
            % createCmdIdStruct - gets structure of command Ids from device.
            obj.cmdIdStruct = obj.sendCmd(obj.cmdIdGetCmds);
        end

        function createRspCodeStruct(obj)
            % createRspCodeStruct - gets structure of response codes from the device.
            obj.rspCodeStruct = obj.sendCmd(obj.cmdIdGetRspCodes);
        end

        function createDevInfoStruct(obj)
            % createDevInfoStruct - get device information struture from device.
            obj.devInfoStruct = obj.sendCmd(obj.cmdIdGetDevInfo);
        end

        function cmdArgs = convertArgStructToCell(obj,argStruct) 
            % Converts a command argument from a structure to a cell array. Note,
            % the structure must have fields which are either the axis names or
            % the dimension names.
            cmdArgs = {};
            argFieldNames = fieldnames(argStruct);
            if isCellEqual(obj.orderedAxisNames,argFieldNames)
                orderedNames = obj.orderedAxisNames;
            elseif isCellEqual(obj.orderedDimNames,argFieldNames)
                orderedNames = obj.orderedDimNames;
            else
                errMsg = 'unknown structure as command argument';
                ME = MException('ArduinoCurrentController:UnknownType', errMsg);
                throw(ME);
              
              end
            for i = 1:length(orderedNames)
                name = orderedNames{i};
                cmdArgs{i} = argStruct.(name);
            end
        end

    end
end

% Utility functions
% -----------------------------------------------------------------------------
function flag = isCellEqual(cellArray1, cellArray2)
    % Tests whether or not two cell arrays of strings are eqaul.  
    % Returns false if both cell array don't consist entirely of strings 
    flag = true;
    for i = 1:length(cellArray1)
        string = cellArray1{i};
        if ~isInCell(string,cellArray2)
            flag = false;
        end
    end
end


function flag = isInCell(string, cellArray)
    % Test whether or not the given string is in the given cellArray.
    flag = false;
    if strcmp(class(string), 'char')
        for i = 1:length(cellArray)
            if strcmp(class(cellArray{i}),'char') && strcmp(cellArray{i}, string)
                flag = true;
                break;
            end
        end
    end
end
