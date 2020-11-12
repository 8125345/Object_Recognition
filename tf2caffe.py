import tensorflow as tf
import numpy as np
from tensorflow.python import pywrap_tensorflow
import sys
import caffe

def tensor2d_transform(tensor):
    return tensor.transpose((1, 0))


prototxt_dir = '/home/topmoo/saver/ppo.prototxt'
model_dir = '/home/topmoo/saver/saver/'
caffe_model = '/home/topmoo/saver/ppo.caffemodel'
ckpt = tf.train.get_checkpoint_state(model_dir)
ckpt_path = ckpt.model_checkpoint_path

reader = pywrap_tensorflow.NewCheckpointReader(ckpt_path)

net = caffe.Net(prototxt_dir, caffe.TRAIN)
w0 = reader.get_tensor('pi/dense/kernel')
b0 = reader.get_tensor('pi/dense/bias')
w00 = tensor2d_transform(w0)
print(w00.shape)
print(b0.shape)
net.params['pi/dense'][0].data.flat = w00.flat
net.params['pi/dense'][1].data.flat = b0.flat


w1 = reader.get_tensor('pi/dense_1/kernel')
b1 = reader.get_tensor('pi/dense_1/bias')
w11 = tensor2d_transform(w1)
print(w11.shape)
print(b1.shape)
net.params['pi/dense_1'][0].data.flat = w11.flat
net.params['pi/dense_1'][1].data.flat = b1.flat


w2 = reader.get_tensor('pi/dense_2/kernel')
b2 = reader.get_tensor('pi/dense_2/bias')
w22 = tensor2d_transform(w2)
print(w22.shape)
print(b2.shape)
net.params['pi/dense_2'][0].data.flat = w22.flat
net.params['pi/dense_2'][1].data.flat = b2.flat


w3 = reader.get_tensor('pi/dense_3/kernel')
b3 = reader.get_tensor('pi/dense_3/bias')
w33 = tensor2d_transform(w3)
print(w33.shape)
print(b3.shape)
net.params['pi/dense_3'][0].data.flat = w33.flat
net.params['pi/dense_3'][1].data.flat = b3.flat
net.save(caffe_model)




