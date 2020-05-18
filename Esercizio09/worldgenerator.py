import sys
import numpy as np
import math as m
import matplotlib.pyplot as plt

nofcities=int(input("number of cities =  "))
#sq_side=input("square's side = ")
#circ_radius=input("circle's radius = ")
sq_output=open("squareworld", "w")
circ_output=open("circleworld", "w")

sq_output.write("{}\n".format(nofcities))
#circ_output.write(nofcities)
rng = np.random.default_rng(0)
for i in range(0,nofcities):
    x=0.0;y=0.0;
    while (x==0.0) | (y==0.0):
        x=rng.uniform()
        y=rng.uniform()
    sq_output.write("{} {}\n".format(x,y))
    theta=rng.uniform(0,2*m.pi)
    s=(1+m.sin(theta))*0.5
    c=(1+m.cos(theta))*0.5
    circ_output.write("{} {}\n".format(s,c))

circ_output.close()
sq_output.close()

x, y = np.loadtxt("squareworld", usecols=(0,1), delimiter=' ', unpack='true', skiprows=1)
plt.scatter(x,y)
plt.savefig("sq_world.png")
plt.clf()

x, y = np.loadtxt("circleworld", usecols=(0,1), delimiter=' ', unpack='true')
plt.scatter(x,y)
plt.savefig("circ_world.png")
