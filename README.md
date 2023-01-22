# handyCode

# Speech Recognition
azure-cognitiveservices-speech

# Arduino Weight Trainer (AWT)

You'll need the following packages

tensorflow

keras

numpy

jupyter notebook

matplotlib

To use:

Setup x_train and y_train in the notebook to be the examples you want the network to train on.

Run the cells sequentially and you'll see your network fit the training data.

The final cell prints out a nicely formatted text version of the trained weights that you can copy directly into your arduino sketch!

You have to disable weight randomization if you want to use your trained weights, comment out these lines in the sketch to do so:

HiddenWeights[j][i] = 2.0 * ( Rando - 0.5 ) * InitialWeightMax ;
OutputWeights[j][i] = 2.0 * ( Rando - 0.5 ) * InitialWeightMax ;

That's about it, let me know how it works!
