import matplotlib.pyplot as plt
import numpy as np

data1 = []
with open("Python_mode.txt") as f:
    for line in f:
        data1.append(line)

data2 = []
if (data1[0] == '1'):
    name = "Out_start.txt"
else:
    name = "Out_optimal.txt"

with open(name) as f:
    for line in f:
        data2.append(([x for x in line.split()]))

num_stank= int(data2[0][0])

data2[2] =  [int(x) for x in data2[2]]
data2[4] =  [int(x) for x in data2[4]]
if (num_stank == 3):
    data2[6] =  [int(x) for x in data2[6]]

b1 = []
b2 = []
b3 = []
for i in range(int(len(data2[2])/2)):
    b1.append((data2[2][2*i],data2[2][2*i+1] ))

for i in range(int(len(data2[2])/2)):
    b2.append((data2[4][2*i],data2[4][2*i+1] ))

if (num_stank == 3):
    for i in range(int(len(data2[2])/2)):
        b3.append((data2[6][2*i],data2[6][2*i+1] ))


del1 =[]
del2 =[]
del3 =[]

 
# Declaring a figure "gnt"
fig, gnt = plt.subplots()
 
# Setting Y-axis limits
gnt.set_ylim(0, 4)
 
# Setting X-axis limits
gnt.set_xlim(0, 32)
 
plt.xticks(np.arange(1, 40, 1.0))  # изменяем шаг делений на оси X

gnt.set_xlabel('seconds since start')
gnt.set_ylabel('Processor')
 
# Setting ticks on y-axis
gnt.set_yticks([1, 2, 3])
# Labelling tickes of y-axis
gnt.set_yticklabels(['1', '2','3'])
 
# Setting graph attribute
gnt.grid(True)

gnt.broken_barh([(0,int(data2[5][0]))], (1.9, 0.2),facecolors =('y'))

if (num_stank == 3):
    gnt.broken_barh([(0,int(data2[7][0]))], (0.9, 0.2),facecolors =('y'))


gnt.broken_barh(b1, (2.9, 0.2),facecolors =('tab:red', "tab:blue", "tab:green", "tab:brown", "tab:pink"))

gnt.broken_barh(b2, (1.9, 0.2),facecolors =('tab:red', "tab:blue", "tab:green", "tab:brown", "tab:pink"))

if (num_stank == 3):
    gnt.broken_barh(b3, (0.9, 0.2),facecolors =('tab:red', "tab:blue", "tab:green", "tab:brown", "tab:pink"))

#gnt.broken_barh(del2, (1.9, 0.2),facecolors =('tab:yellow'))

if (num_stank == 3):
    gnt.broken_barh(del3, (0.9, 0.2),facecolors =('tab:yellow'))    
 
 
plt.show();