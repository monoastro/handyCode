# handyCode

This repo contains all of the code required for running the hand tracking gloves. The neural network, among other things,
is written in C so everything is isolated to the system of the glove. The python stuff is for training the neural network
and not required for the runtime of the glove.

# Speech Recognition

We used the api provided by azure-cognitiveservices-speech

# Arduino Weight Trainer

You'll need the following packages
tensorflow
keras
numpy
jupyter notebook
matplotlib

To use the glove from scratch by training the network yourself:
Setup x_train and y_train in the notebook to be the examples you want the network to train on.
Run the cells sequentially and you'll see your network fit the training data.
The final cell prints out a nicely formatted text version of the trained weights that can be directly copied into the arduino sketch.

