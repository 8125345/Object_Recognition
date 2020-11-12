import caffe
import numpy as np
prototxt_dir = '/home/topmoo/saver/ppo.prototxt'
caffe_model = '/home/topmoo/saver/ppo.caffemodel'
#caffe_model = '/home/topmoo/saver/saver/ppo.caffemodel'

net = caffe.Net(prototxt_dir,caffe_model,caffe.TEST)

#transformer = caffe.io.Transformer({'data': net.blobs['data'].data.shape})
# x = np.random.random((1, 8))
# print(x)
# y = caffe.io.arraylist_to_blobprotovector_str(x)
#
# print(y)


net.blobs['data'].data[[...]] =[3.49000000e+02, 1.13000000e+02, 1.00000000e+01,
                                0.00000000e+00, 1.80000000e+02, -5.50000000e+01,
                                1.55000000e+02, -5.00000000e+00]
#####output :12

# net.blobs['data'].data[...] = [2.90000000e+02, 1.28000000e+02, 6.85010647e+00,
#                                 -2.63831668e+00, 1.80000000e+02, -5.50000000e+01,
#                                 1.55000000e+02, -5.00000000e+00]
#####output :13


# net.blobs['data'].data[...] = [9.00000000e+01, 1.28000000e+02, -1.68869409e-01,
#                                1.78865033e-01,1.80000000e+02, - 5.50000000e+01,
#                                1.55000000e+02, - 5.00000000e+00]

#####output :14





out = net.forward()
# s = sum(out.values())
# print(sum(sum(s)))
print(out)
print(max(max(max(out.values()))))
print(np.argmax(max(max(out.values()))))




# s = sum(out.values())
# print(s)
#print(out)

#output_prob = out['prob'][0]

# fc_1_w = net.params['pi/dense_1'][1].data
# transformer = caffe.io.Transformer()
#
# transformer.set_transpose('pi/dense_1', (1, 0))
#
# print(fc_1_w)





