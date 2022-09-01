#!/usr/bin/env python3
import matplotlib as mpl
import matplotlib.pyplot as plt
import numpy as np


respirator_rate = 12
period = 60 / respirator_rate
time = np.arange(0, period * 2, 0.01)
t = time * 2 * np.pi / period - np.pi / 2
brightness = (np.sin(t) + 1) / 2

fig, axs = plt.subplots(figsize =(36, 9))
line, = axs.plot(time, brightness, lw=1, color = "#f00")

resolution = 10
granularity = 1/resolution
x = np.arange(0, period * 2, granularity)
t = x * 2 * np.pi / period - np.pi / 2
y = (np.sin(t) + 1) / 2
colors = np.array([(0.0, 0.0, 0.0, 0.0)]*len(y))
for i in range(len(y)):
    colors[i] = (1.0, 1.0, 1.0 - y[i], y[i])
bar = axs.bar(x, y, width = granularity*0.99, color = colors, edgecolor = "#ccc")

axs.set_xlim(left=0)
axs.set_ylim(bottom=0)

plt.show()

