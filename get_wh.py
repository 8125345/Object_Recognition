import os
import cv2 as cv
import numpy as np

def Get_wh(JpgPath):
    img = cv.imread(JpgPath)
    sp =img.shape
    w = sp[1]
    h = sp[0]
    d = sp[2]


    return w,h,d




Path='/data/caffe-ssd/data/ships/JPEGImages/'
wh_txt = '/data/caffe-ssd/data/ships/wh.txt'
f_read = open('/data/caffe-ssd/data/ships/label.txt','r')  ##ke xie moshi
f_write = open('/data/caffe-ssd/data/ships/label_new.txt','w')
for line in f_read:
    lines1 = str(line)
    lines = str(line).split('  ')
    JpgPath = Path + str(lines[0])
    print(JpgPath)
    width, height, depth = Get_wh(JpgPath)
    line_new = lines1.replace('\n', ' ')
    line_new = line_new + str(width)+','+str(height)+','+str(depth)+'\n'
    f_write.write(line_new)



# img_dir = os.listdir(Path)
# for img in img_dir:
#     JpgPath = Path + img
#     width, height, depth = Get_wh(JpgPath)
#
#
# with open('/data/caffe-ssd/data/ships/label.txt','r') as f_read:   ##zhi du moshi
#     line =f_read.readlines()
#     for line_list in line:
#         line_new =line_list.replace('\n',' ')
#         line_new = line_new + str(width)+','+str(height)+','+str(depth)+'\n'
#         f_write.write(line_new)
#
# f = open(wh_txt,'w',encoding='utf-8')
# img_dir = os.listdir(Path)
# for img in img_dir:
#     if img.endswith('.jpg'):
#         JpgPath = Path + img
#         width,height,depth = Get_wh(JpgPath)
#         f.write(img + ' '+str(width)+','+str(height)+','+str(depth)+'\n')
# f.close()
