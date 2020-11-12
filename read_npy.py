import numpy as np
np.set_printoptions(threshold=1e6)
test = np.load('./data(2).npy',encoding='latin1')
doc = open('data(2).txt','a')
print(test,file=doc)
