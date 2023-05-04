#!/usr/bin/env python
import os
import sys
import shutil
import subprocess
import traceback
from subprocess import Popen, PIPE, STDOUT

from cigarfilter import _CIGARFILTER_RESOURCES_DIR

def main_test():
    exec_path = os.path.join(_CIGARFILTER_RESOURCES_DIR,"bin","cigarfilter_test")
    args = sys.argv[1:]
    
    callProcess = subprocess.Popen([exec_path]+sys.argv[1:],stdin=PIPE, stdout=PIPE, stderr=STDOUT, close_fds=True)
    stdout, stderr = callProcess.communicate()
    if stdout:
        sys.stdout.write(stdout.decode())
    if stderr:
        sys.stderr.write(stderr.decode())


def main():
    exec_path = os.path.join(_CIGARFILTER_RESOURCES_DIR,"bin","cigarfilter")
    args = sys.argv[1:]
    
    callProcess = subprocess.Popen([exec_path]+sys.argv[1:],stdin=PIPE, stdout=PIPE, stderr=STDOUT, close_fds=True)
    stdout, stderr = callProcess.communicate()
    if stdout:
        sys.stdout.write(stdout.decode())
    if stderr:
        sys.stderr.write(stderr.decode())

if __name__ == "__main__":
    try:
        main()
    except Exception as e:
        # printing stack trace
        traceback.print_exc()
        print(e)

