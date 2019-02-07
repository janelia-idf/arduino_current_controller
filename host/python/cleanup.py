#!python
from __future__ import print_function
import os,shutil

if __name__ == "__main__":
    dirlist = os.listdir('.')
    rmlist = ['build','dist','egg-info']
    for item in dirlist:
        for rm in rmlist:
            if rm in item:
                print('Removing: ' + item)
                shutil.rmtree(item)
