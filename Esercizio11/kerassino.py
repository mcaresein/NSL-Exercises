 #arget parameters of f(x) = m*x + b
m = 2 # slope
b = 1 # intersect

a = 3
b = -2
c = -3
d = 4



import numpy as np

#generate training inputs
np.random.seed(0)
x_train = np.random.uniform(-1, 1, 500)
x_valid = np.random.uniform(-1, 1, 20)
x_valid.sort()
y_target = a * x_valid**3 + b * x_valid**2 + c * x_valid + d
#y_target = m * x_valid + b # ideal (target) linear function

sigma = 1.0 # noise standard deviation, for the moment it is absent
y_train = np.random.normal(a *  x_train**3 + b * x_train**2 + c * x_train + d ) # actual measures from which we want to guess regression parameters
y_valid = np.random.normal(a *  x_valid**3 + b * x_valid**2 + c * x_valid + d)
#y_train = np.random.normal(m * x_train + b)
#y_valid = np.random.normal(m * x_valid + b)
#plot training and target dataset
import matplotlib.pyplot as plt
plt.plot(x_valid, y_target)
plt.scatter(x_valid, y_valid, color='r')
plt.grid(True); plt.savefig('scatterplot.png')
plt.clf()

#compose the NN model
import tensorflow as tf
from tensorflow import keras

from tensorflow.keras.models import Sequential
from tensorflow.keras.layers import Dense, Activation
from tensorflow.keras import backend as K,initializers
from tensorflow.keras.utils import get_custom_objects

model = tf.keras.Sequential()
model.add(Dense(10, input_shape=(1,), activation="relu",
 #         kernel_initializer=initializers.Zeros(),
 #               bias_initializer=initializers.Zeros()
))
model.add(Dense(10, activation="relu",
#          kernel_initializer=initializers.Zeros(),
#                bias_initializer=initializers.Zeros()
))
model.add(Dense(10, activation="relu",
#          kernel_initializer=initializers.Zeros(),
#                bias_initializer=initializers.Zeros()
))
model.add(Dense(10, activation="relu",
#          kernel_initializer=initializers.Zeros(),
#                bias_initializer=initializers.Zeros()
))
model.add(Dense(1, activation="relu",
#          kernel_initializer=initializers.Zeros(),
#                bias_initializer=initializers.Zeros()
))

model.compile(optimizer='sgd', loss='mse', metrics=['mse'])

#get a summary of our composed model
model.summary()

history = model.fit(x=x_train, y=y_train,batch_size=32, epochs=40,
                shuffle=False,validation_data=(x_valid, y_valid))

model.get_weights()

#evaluate model
score = model.evaluate(x_valid, y_valid, batch_size=32, verbose=1)

#print performance
print()
print('Test loss:', score[0])
print('Test accuracy:', score[1])

#evaluate model with the exact curve
score = model.evaluate(x_valid, y_target, batch_size=32, verbose=1)

#print performance
print()
print('Test loss:', score[0])
print('Test accuracy:', score[1])

#A trained model can be saved and stored to a file for later retreival. This allows you to checkpoint a model and resume training later without rebuilding and training from scratch.
#Files are saved in HDF5 format, within all weight values, model's configuration and even the optimizer's configuration.# look into training history
#Plot training & validation loss values
plt.plot(history.history['loss'])
plt.plot(history.history['val_loss'])
plt.title('Model loss')
plt.ylabel('Loss')
plt.xlabel('Epoch')
plt.legend(['Train', 'Test'], loc='best')
plt.savefig('fit.png')
plt.clf()

x_predicted = np.random.uniform(-1, 1, 20)
y_predicted = model.predict(x_predicted)
plt.scatter(x_predicted, y_predicted,color='r')
plt.plot(x_valid, y_target)
plt.grid(True)
plt.savefig('cane.png')

print(np.random.rand(1,1))
