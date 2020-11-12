
from lxml.etree import Element, SubElement, tostring
from xml.dom.minidom import parseString
import os


def make_xml(f, save_xml_path):
    # 第一层循环遍历所有的照片
    for line in f:
        lines = str(line).split(' ')
        box_num = 1
        pic_name = str(lines[0])

        node_root = Element('annotation')
        node_filename = SubElement(node_root, 'filename')
        # 图片名字
        node_filename.text = pic_name

        # 第二层循环遍历有多少个框
        #for i in range(box_num):
            #bbox = str(lines[i + 2]).split(',')
        #cls_name = str(lines[0])
        node_size = SubElement(node_root,'size')
        node_width = SubElement(node_size,'width')
        node_width.text = str(line).split('  ')[-1].split(' ')[-1].split(',')[0]
        node_height = SubElement(node_size,'height')
        node_height.text =str(line).split('  ')[-1].split(' ')[-1].split(',')[1]
        node_depth = SubElement(node_size,'depth')
        node_depth.text =str(line).split('  ')[-1].split(' ')[-1].split(',')[2]



        node_object = SubElement(node_root, 'object')
        node_name = SubElement(node_object, 'name')
            # 类别名字
        node_name.text = 'ship'
        node_truncated = SubElement(node_object,'truncated')
        node_truncated.text = '0'
        node_difficult = SubElement(node_object, 'difficult')
        node_difficult.text = '0'
        node_bndbox = SubElement(node_object, 'bndbox')
        node_xmin = SubElement(node_bndbox, 'xmin')
        node_xmin.text = str(line).split(',')[0].split(' ')[-1]
        node_ymin = SubElement(node_bndbox, 'ymin')
        node_ymin.text = str(line).split(',')[1]
        node_xmax = SubElement(node_bndbox, 'xmax')
        node_xmax.text = str(line).split(',')[2]
        node_ymax = SubElement(node_bndbox, 'ymax')
        node_ymax.text = str(line).split(',')[3].split(' ')[0]

        xml = tostring(node_root, pretty_print=True)
        dom = parseString(xml)

        xml_name = pic_name.replace(".jpg", "")
        xml_name = os.path.join(save_xml_path, xml_name + '.xml')
        with open(xml_name, 'wb') as f:
            f.write(dom.toprettyxml(indent='\t', encoding='utf-8'))


txt_path = '/data/caffe-ssd/data/ships/label_new.txt'
save_xml_path = '/data/caffe-ssd/data/ships/Annotations'
f = open(txt_path, 'r')
f = f.readlines()
make_xml(f, save_xml_path)