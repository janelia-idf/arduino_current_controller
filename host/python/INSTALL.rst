arduino_current_controller installation instructions
----------------------------------------------------

* Create a new virtual environment if necessary::

    virtualenv --system-site-packages ~/virtualenvs/arduino_current_controller

* Activate a virtual environment::

    source ~/virtualenvs/arduino_current_controller/bin/activate

* Install dependencies for webpage controller::

    pip install flask --upgrade
    pip install flask-sijax --upgrade

* Clone bitbucket mercurial repositories::

    mkdir ~/mercurial
    cd ~/mercurial
    hg clone https://bitbucket.org/peterpolidoro/python_libs/
    hg clone https://bitbucket.org/peterpolidoro/arduino_current_controller/

* Install python packages::

    cd ~/mercurial/python_libs/arduino_device
    python cleanup.py
    python setup.py install
    cd ~/mercurial/arduino_current_controller/host/python
    python cleanup.py
    python setup.py install
