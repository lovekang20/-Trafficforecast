import tensorflow as tf
import numpy as np
import matplotlib
import os
import matplotlib.pyplot as plt
import random
from tensorflow.contrib import rnn

tf.set_random_seed(777)  # reproducibility

# train Parameters
batch_size=25
seq_length =24
data_dim = 10
hidden_dim = 15
output_dim = 1
learning_rate=0.01
iterations = 16950
startYear = 2015
year = 2
pre=startYear-2000
startYear-2000
repeat=0

# Open, High, Low, Volume, Close

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


filename = []
#for i in range(0,12):
#  if i >= 9:
#    filename.append("2017"+str(i+1)+".csv")
#  else:
#    filename.append("20170"+str(i+1)+".csv")

#if (repeat%21)>9:
#  year=2016
#else:
#  year=2015
#for i in range(0,2):
#  if year==2015:
#    index=((repeat)%21+i)%10
#  elif year==2016:
#    index=((repeat)%21+i-10)%11
#  filename.append(str(year)+"00"+str(index)+".csv")
print(i)

f=open("repeat.txt",'r')
element=f.read().split()
repeat=int(element[0])
learning_rate=float(element[1])
f.close()
for _ in range(0,batch_size):
  for k in range(startYear,startYear+year):
   filename.append("files/"+str(k)+"_"+map[repeat][0]+"_"+map[repeat][1]+".csv")

print(filename)

filename_queue = tf.train.string_input_producer(
    filename, shuffle=False, name='filename_queue')

reader = tf.TextLineReader()
key, value = reader.read(filename_queue)


record_defaults = [[0.],[0.], [0.], [0.], [0.],[0.],[0.],[0.],[0.],[0.]]
xy = tf.decode_csv(value, record_defaults=record_defaults)


traffic_data,traffic=tf.train.batch([xy, xy[-1:]], batch_size=batch_size)

#xy = xy[::-1]  # reverse order (chronically ordered)
#xy = MinMaxScaler(xy)
#x = xy
#y = xy[:, [-1]]  # Close as label

#print(traffic_data,traffic)
# build a dataset
dataX = []
dataY = []
#x=traffic_data[0:0 + seq_length]
#y=traffic[seq_length]
for j in range(0, batch_size - seq_length):
    _x = traffic_data[j:j + seq_length]
    _y = traffic[j + seq_length]  # Next close price
#    x=tf.stack([x,_x])
#    y=tf.stack([y,_y])
#    print(x,y)
#    x = tf.reshape(x,[i+1,seq_length])
#    x = tf.reshape(y,[i+1,1])
#    x=x.reshape(i+1,seq_length)
#    y=y.reshape(i+1,1)
#    print(_x, "->", _y)
    dataX.append(_x)
    dataY.append(_y)
X=tf.stack(dataX)
Y=tf.stack(dataY)

# reshape
# input place holders
#X = tf.placeholder(tf.float32, [None, seq_length, data_dim])
#Y = tf.placeholder(tf.float32, [None,1])
#Y = tf.reshape(Y,shape=[None,1])

# build a LSTM network
def multi_lstm_cell():
    cell = rnn.MultiRNNCell([tf.contrib.rnn.BasicLSTMCell(
      hidden_dim, state_is_tuple=True,name="lstm"+str(k)) for k in range(100)], state_is_tuple=True)
#      hidden_dim, state_is_tuple=True) for i in range(10)], state_is_tuple=True)
    return cell

multi_cells = multi_lstm_cell()
#def lstm_cell():
#    cell = tf.contrib.rnn.BasicLSTMCell(hidden_dim, state_is_tuple=True)
#    return cell

#multi_cells = rnn.MultiRNNCell([lstm_cell() for _ in range(20)], state_is_tuple=True)

def get_state_variables_origin(batch_size, cell):
    # For each layer, get the initial state and make a variable out of it
    # to enable updating its value.
    state_variables = []
    save_name=[]
    i=0
    for state_c, state_h in cell.zero_state(batch_size, tf.float32):
        cvar=tf.Variable(state_c, trainable=False,name='c'+str(i))
        hvar=tf.Variable(state_h, trainable=False,name='h'+str(i))
        state_variables.append(tf.contrib.rnn.LSTMStateTuple(
            cvar,hvar))
        save_name.append(cvar)
        save_name.append(hvar)
        ++i
        
    # Return as a tuple, so that it can be fed to dynamic_rnn as an initial state
    return tuple(state_variables),save_name

states,save_name = get_state_variables_origin(batch_size-seq_length, multi_cells)
outputs, _state = tf.nn.dynamic_rnn(multi_cells, X, initial_state=states)
Y_pred = tf.contrib.layers.fully_connected(
    outputs[:, -1], output_dim, activation_fn=None)  # We use the last cell's output

# cost/loss
loss = tf.cond(((Y[0][0]-Y[0][0]/10)<Y_pred[0][0])&((Y[0][0]+Y[0][0]/10)>Y_pred[0][0]),lambda:tf.reduce_sum(tf.square(Y_pred - Y) / 128),lambda:tf.reduce_sum(tf.square(Y_pred - Y)))  # sum of the squares
tf.summary.scalar('cost',loss)
# optimizer
optimizer = tf.train.AdamOptimizer(learning_rate)
train = optimizer.minimize(loss)

#save_name.append(optimizer)


comp_pred = ((Y-Y/10)<Y_pred)&((Y+Y/10)>Y_pred)
accuracy = tf.reduce_mean(tf.cast(comp_pred,tf.float32))

# RMSE
#targets = tf.placeholder(tf.float32, [None, 1])
#predictions = tf.placeholder(tf.float32, [None, 1])
#rmse = tf.sqrt(tf.reduce_mean(tf.square(targets - predictions)))

#saver = tf.train.Saver(save_name)
saver = tf.train.Saver()

with tf.Session() as sess:
    init = tf.global_variables_initializer()
    sess.run(init)

#    saver.restore(sess,'./ex04.ckpt')
#    saver.restore(sess,"save/"+map[repeat][0]+"_"+map[repeat][1]+".ckpt")

    coord = tf.train.Coordinator()
    threads = tf.train.start_queue_runners(sess=sess, coord=coord)

    merge = tf.summary.merge_all()
    train_writer = tf.summary.FileWriter('./summaries/',sess.graph)

    acc_sum=0
    graph_y=[]
    graph_y_pred=[]
    # Training step
    # data : 5_1-5101135#5-60833297 #9-518041
    
#    while repeat<2:
    for k in range(0,1000):
#    for k in range(0,10):
#        k+=1
        summary,y,y_pred,_, step_loss,acc = sess.run([merge,Y,Y_pred,train, loss,accuracy])#, feed_dict={X: dataX, Y: dataY})
#        y,y_pred,_, step_loss,acc = sess.run([Y,Y_pred,train, loss,accuracy])#, feed_dict={X: dataX, Y: dataY})
        acc_sum+=acc
        graph_y.append(y[0])
        graph_y_pred.append(y_pred[0])
        if (k+1)%iterations==0:
            acc_mean=acc_sum/iterations
#            print("[step: {}] loss: {}".format(k, step_loss))
#            print(y,y_pred)
#            print("accuracy:",acc_mean)
#            print("repeat:",repeat)
            train_writer.add_summary(summary,k)
            saver.save(sess,"save/"+map[repeat][0]+"_"+map[repeat][1]+".ckpt")
            #learning_rate 조절
            #if map[repeat][0]==190 and map[repeat][1]==561 and learning_rate>100:
            if learning_rate>100:
              if acc_mean>0.8:
                learning_rate=learning_rate/64
              else:
                learning_rate=learning_rate-100
            elif learning_rate>1e-15:
              if acc_mean>0.8:
                repeat+=1
                learning_rate=learning_rate/64
              else:
                learning_rate=learning_rate*random.random()*100
            else:
              learning_rate*=100
            #learning 정보 저장
            f=open("record.txt","a")
            f.write(str(map[repeat][0])+" "+str(map[repeat][1])+"\n")
            f.write("[step: {}] loss: {}\n".format(k, step_loss))
            f.write(str(y)+" "+str(y_pred)+"\n")
            f.write("accuracy:"+str(acc_sum/iterations)+"\n")
            f.close()
            acc_sum=0
#        if pre!=cur:
#            repeat+=1/year
#            pre=cur
#    print(cur)
#    print(pre)
#    print(repeat)
#    print(1/year)
    # Test step
#    test_predict = sess.run(Y_pred, feed_dict={X: testX})
#    rmse_val = sess.run(rmse, feed_dict={
#                    targets: testY, predictions: test_predict})
#    print("RMSE: {}".format(rmse_val))

    # Plot predictions
    plt.plot(graph_y)
    plt.plot(graph_y_pred,'r')
#    plt.xlabel("Time Period")
#    plt.ylabel("Time")
    plt.show()

    coord.request_stop()
    coord.join(threads)


f=open("repeat.txt",'w')
#f.write(str((repeat+1)%i)+" ")
f.write(str((repeat)%i)+" ")
f.write(str(learning_rate))
f.close()
