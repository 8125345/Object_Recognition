import VOC
from VOC import delete_class
import sys
import xml.etree.ElementTree as ET

from VOC import VOC
import os
import random
import shutil

annofile_dir_2007 = '/data/caffe-ssd/data/VOC0712_3-classes/VOC2007/Annotations'
annofile_dir_2012 = '/data/caffe-ssd/data/VOC0712_3-classes/VOC2012/Annotations'
annofile_dir_ship = '/data/caffe-ssd/data/ships/Annotations'

a = VOC(annofile_dir_2012)



txt_dir_2007 = '/data/caffe-ssd/data/VOC0712_3-classes/VOC2007/ImageSets/Main/trainval_new.txt'
txt_dir_2012 = '/data/caffe-ssd/data/VOC0712_3-classes/VOC2012/ImageSets/Main/trainval_new.txt'
txt_dir_ship = '/data/caffe-ssd/data/ships/ImageSets/Main/trainval.txt'
#txt_dir = '/data/caffe-ssd/data/VOC0712_3-classes/VOC2007/ImageSets/Main/test.txt'
#delete_class()

def get_txt_line(txt_dir):
    # data = []
    # for line in open(txt_dir):
    #     data.append(line)
    f = open(txt_dir,'r')
    data = f.read()
    f.close()
    return data


def get_xml_dir(annofile):
    #global count

    tree = ET.parse(annofile)
    root = tree.getroot()
    #annos = [anno for anno in root.iter()]
    #for anno in root.iter():
    annos = [anno for anno in root.iter()]

    # print(annos)
    # counter = 0
    # for i in annos:
    #     counter = counter + 1
    # print(counter)
    #for i, anno in enumerate(annos):
    if 'object' in annos:

            print(annofile)
    #         count = count + 1




annolist = a._listanno(annofile_dir_ship)
count = 0
f = open(txt_dir_ship,'w',encoding='utf-8')
for annofile_xml in annolist:

    annofile = os.path.join(annofile_dir_ship, annofile_xml)
    #get_xml_dir(annofile)
    tree = ET.parse(annofile)
    root = tree.getroot()
    if root.find('object') in root.iter():
        image_name = annofile_xml.rstrip('.xml')
        print(image_name)
        f.write(image_name+'\n')
        count = count + 1
        # data1 = get_txt_line(txt_dir)
        # print(data1)
        # if image_name in data1:
        #     print(image_name+' zai txt wenjian zhong ')

f.close()
print(count)


    # if 'object' in root.iter():
    #         print(annofile)


















