import tensorflow as tf
import numpy as np
import matplotlib
import os

from tensorflow.contrib import rnn

tf.set_random_seed(777)  # reproducibility


import matplotlib.pyplot as plt

# train Parameters
seq_length = 25
data_dim = 10
hidden_dim = 10
output_dim = 1
learning_rate = 0.01
iterations = 1000
year=2015

map=[]

f=open("mapping2.txt",'r')
i=0
while True:
  line = f.readline()
  if not line: break
  map.append(line.split())
#  print(map[i][0]+" "+map[i][1])
  i+=1
f.close()
print(i)

f=open("repeat.txt",'r')
element=f.read().split()
repeat=int(element[0])
learning_rate=float(element[1])
f.close()

min_max=[]
i=0
f=open("min_max.txt",'r')
while True:
  line = f.readline()
  if not line: break
  min_max.append(line.split())
  i+=1
f.close()
print(i)

min_max = min_max[int(repeat/3)]
min_value = min_max[10:20]
max_value = min_max[0:10]
min=[]
max=[]

if repeat==3*i-1:
  if learning_rate > 1e-9:
    learning_rate*0.5
  elif acc<0.9:
    learning_rate*random.random()*1e7 
  year = 2015
  
if repeat%3==0:
    year=2015
elif repeat%3==1:
    year=2016
else:
    year=2017

# Open, High, Low, Volume, Close
filename="files/"+str(year)+"_"+str(map[int(repeat/3)][0])+"_"+str(map[int(repeat/3)][1])+".csv"
xy = np.loadtxt(filename,delimiter=',')
print(filename)
#xy = xy[::-1]  # reverse order (chronically ordered)

min = np.array(min_value)
max = np.array(max_value)
min=min.astype(float)
max=max.astype(float)

print(min)
print(max)

numerator = xy - min
denominator = max - min
print(xy[1])
print(numerator[1])
print(denominator[1])

xy = numerator / (denominator + 1e-7)
x = xy
y = xy[:, [-1]]  # Close as label

print(xy[1])

# build a dataset
dataX = []
dataY = []
for j in range(0, len(y) - seq_length):
    _x = x[j:j + seq_length]
    _y = y[j + seq_length]  # Next close price
#    print(_x, "->", _y)
    dataX.append(_x)
    dataY.append(_y)

# train/test split
train_size = len(dataY)
iterations = train_size
test_size = int(len(dataY)*0.9)
if year==2017:
  iterations=0
  test_size=0
trainX, testX = np.array(dataX[0:train_size]), np.array(
    dataX[test_size:len(dataX)])#dataX[train_size:len(dataX)])
trainY, testY = np.array(dataY[0:train_size]), np.array(
    dataY[test_size:len(dataY)])


# input place holders
X = tf.placeholder(tf.float32, [None, seq_length, data_dim])
Y = tf.placeholder(tf.float32, [None, 1])

# build a LSTM network
#cell = tf.contrib.rnn.BasicLSTMCell(
#    num_units=hidden_dim, state_is_tuple=True, activation=tf.tanh)


cell = rnn.MultiRNNCell([tf.contrib.rnn.BasicLSTMCell(
      hidden_dim, state_is_tuple=True,name="lstm"+str(k)) for k in range(10)], state_is_tuple=True)
outputs,_states = tf.nn.dynamic_rnn(cell, X, dtype=tf.float32 )

Y_pred = tf.contrib.layers.fully_connected(
    outputs[:, -1], output_dim, activation_fn=None)  # We use the last cell's output

# cost/loss
loss = tf.reduce_sum(tf.square(Y_pred - Y))  # sum of the squares
optimizer =tf.train.AdamOptimizer(learning_rate)
train = optimizer.minimize(loss)

saver = tf.train.Saver()

#iterations=1000

with tf.Session() as sess:
    init = tf.global_variables_initializer()
#    sess.run(init)
    saver.restore(sess,"save_/"+str(map[int(repeat/3)][0])+"_"+str(map[int(repeat/3)][1])+".ckpt")
    step_loss=0
    
    # Training step
    for k in range(int(iterations/30)):
        _, step_loss = sess.run([train, loss], feed_dict={
                                X: trainX, Y: trainY})
#        print("[step: {}] loss: {}".format(k, step_loss))

    # Test step
    test_predict = sess.run(Y_pred, feed_dict={X: testX})
    saver.save(sess,"save_/"+str(map[int(repeat/3)][0])+"_"+str(map[int(repeat/3)][1])+".ckpt")

    test_predict=test_predict* (max[data_dim-1] - min[data_dim-1] + 1e-7) + min[data_dim-1]
    testY=testY * (max[data_dim-1] - min[data_dim-1] + 1e-7) + min[data_dim-1]
    test_predict=np.array(test_predict)
    testY=np.array(testY)
    acc=abs(testY-test_predict)<testY/10
    print("acc:"+str(np.mean(acc)))
           #learning 정보 저장
    f=open("record_.txt","a")
    f.write(str(map[int(repeat/3)][0])+" "+str(map[int(repeat/3)][1])+"\n")
    f.write("loss: {}\n".format(step_loss))
    f.write("accuracy:"+str(np.mean(acc))+"\n")
    f.close()

    # Plot predictions
#if repeat%3==2:
plt.plot(testY)
plt.plot(test_predict)
plt.show()
  

f=open("repeat.txt",'w')
#f.write(str((repeat+1)%i)+" ")
#f.write(str((repeat+1)%(i*3-1))+" ")
f.write(str((repeat+1)%(i*3))+" ")
f.write(str(learning_rate))
f.close()
