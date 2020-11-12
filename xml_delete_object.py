
import xml.etree.cElementTree as ET
import os


root_dir = '/data/caffe-ssd/data/VOC0712_3-classes/VOC2012/Annotations(wanzheng)'
new_dir = '/data/caffe-ssd/data/VOC0712_3-classes/VOC2012/new_Annotations'

CLASSES = ["car",
    "bicycle", "boat"]
#for anno_path in root_dir:
for filename in os.listdir(root_dir):
    file_path = os.path.join(root_dir,filename)
    new_path = os.path.join(new_dir,filename)


    tree = ET.parse(file_path)
    root = tree.getroot()

    for child in root.findall('object'):
        name = child.find('name').text
        if not name in CLASSES:
            root.remove(child)
    tree.write(new_path, xml_declaration=True)
    #tree.write(os.path.join(new_dir, xml_declaration=True))