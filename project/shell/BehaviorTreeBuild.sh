build_path="../../demo/BehaviorTree/build"

if [ !  -d $build_path  ]
then
    mkdir $build_path
fi

cd $build_path
cmake ..
make -j2