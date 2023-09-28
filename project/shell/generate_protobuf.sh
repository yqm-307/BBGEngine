#!/bin/bash


##
## 自动生成 protobuf 文件
##


PROTO_DST_PATH="../server/protoc"   # 导出目录
PROTO_SRC_PATH="../protocol"        # protobuf源代码目录

function generate_ex()
{
    output_path=$1
    input_path=$2
    filename=$3

    echo "将 $filename 从 $input_path 生成 proto文件到 $output_path"
    protoc -I=${input_path} --cpp_out=${output_path} $filename
}

# 获取所有proto文件
function main()
{
    # 创建目录
    if [ !  -d $PROTO_DST_PATH  ]
    then
        mkdir $PROTO_DST_PATH
    fi

    ##########
    # c++ c2gs
    ##########
    generate_ex $PROTO_DST_PATH $PROTO_SRC_PATH c2gs.proto

}



main