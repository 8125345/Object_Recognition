#!/bin/bash

root_dir=/data/caffe-ssd/data/ships
root_dir_1=/data/caffe-ssd/data
#sub_dir_2=VOCdevkit
sub_dir=ImageSets/Main
bash_dir=/data/caffe-ssd/data/ships && pwd
#bash_dir="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
for dataset in trainval test
do
  dst_file=$bash_dir/$dataset.txt
  echo "dst_file $dst_file"
  if [ -f $dst_file ]
  then
    rm -f $dst_file
  fi
  #for name in VOC2007 VOC2012
  #do
  #  if [[ $dataset == "test" && $name == "VOC2012" ]]
  #  then
  #    echo "dataset ，$dataset"
  #    continue
  #    echo "dataset ，$dataset"
      #break
  #  fi
    echo "Create list for  $dataset..."
    dataset_file=$root_dir/$sub_dir/$dataset.txt
    echo "dataset_file $dataset_file"

    img_file=$bash_dir/$dataset"_img.txt"
    echo "img_file $img_file"

    cp $dataset_file $img_file
    echo "dataset_file_change $dataset_file"
    echo "img_file_change $img_file"
    

    sed -i "s/^/JPEGImages\//g" $img_file
    echo "img_file $img_file"
    sed -i "s/$/.jpg/g" $img_file
#label_file=$bash_dir/$name/$dataset/$dataset"_label.txt"
    label_file=$bash_dir/$dataset"_label.txt"
    echo "label_file: $label_file"
    cp $dataset_file $label_file

    sed -i "s/^/Annotations\//g" $label_file
    sed -i "s/$/.xml/g" $label_file

    paste -d' ' $img_file $label_file >> $dst_file

    #rm -f $label_file
    #rm -f $img_file
    
 # done

  # Generate image name and size infomation.
  if [ $dataset == "test" ]  
  then
    /data/caffe-ssd/build/tools/get_image_size $root_dir $dst_file $bash_dir/$dataset"_name_size.txt"
    #$bash_dir/../../build/tools/get_image_size $root_dir $dst_file $bash_dir/$dataset"_name_size.txt"
    echo "$root_dir_1 $dst_file $bash_dir/$dataset _name_size.txt"
  fi

  # Shuffle trainval file.
  if [ $dataset == "trainval" ]
  then
    rand_file=$dst_file.random
    cat $dst_file | perl -MList::Util=shuffle -e 'print shuffle(<STDIN>);' > $rand_file
    mv $rand_file $dst_file
  fi
done
