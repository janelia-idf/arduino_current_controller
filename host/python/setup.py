"""
Copyright 2013 Peter Polidoro

This file is part of arduino_current_controller.

arduino_current_controller is free software: you can redistribute it
and/or modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation, either version 3
of the License, or (at your option) any later version.

arduino_current_controller is distributed in the hope that it will be
useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public
License along with arduino_current_controller.  If not, see
<http://www.gnu.org/licenses/>.  """

from distutils.core import setup


setup(
    name='arduino_current_controller',
    version='0.1.0',
    author='Peter Polidoro',
    author_email='peterpolidoro@gmail.com',
    packages=['arduino_current_controller',
              'arduino_current_controller/webserver',
              ],
    scripts=['bin/arduino_current_controller_cli.py',
             'bin/arduino_current_controller_webserver.py',
             ],
    package_data={'arduino_current_controller/webserver':['templates/*.html',
                                                          'static/*.css',
                                                          'static/images/*.gif',
                                                          'static/images/*.png',
                                                          'static/js/jquery/*.js',
                                                          'static/js/extras/*.js',
                                                          'static/js/scripts/*.js']},
    url='https://bitbucket.org/peterpolidoro/arduino_current_controller',
    license='LICENSE.LESSER.txt',
    description='Software to control the Arduino current controllers.',
    long_description=open('README.rst').read(),
)
