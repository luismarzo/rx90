#!/usr/bin/env python

import numpy
from openravepy import *

if __name__ == "__main__":
    env = Enviroment()
    env.Load('env.xml')
    env.SetViewer('qtcoin')
    viewer = env.GetViewer()
    viewer.SetBkgndColor([.8, .85, .9])  # RGB tuple
    robot = env.GetRobots()[0]
else:
    print("\nOut")


