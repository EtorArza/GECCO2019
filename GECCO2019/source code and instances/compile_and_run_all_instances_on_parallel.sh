# Edit the number of parallel executions. 
N=1

g++  -std=c++11 src/*.cpp src/*.h -o binary -O2
echo "compiled"


open_sem(){
    mkfifo pipe-$$
    exec 3<>pipe-$$
    rm pipe-$$
    local i=$1
    for((;i>0;i--)); do
        printf %s 000 >&3
    done
}
run_with_lock(){
    local x
    read -u 3 -n 3 x && ((0==x)) || exit $x
    (
     ( "$@"; )
    printf '%.3d' $? >&3
    )&
}


run_one(){
    declare -i TIME=$SECONDS
    SEED=$1    
    f=$2
    INSTANCE_NAME=$(basename "$f" .dat)
    sep="_"
    FILE="out/$INSTANCE_NAME$sep$SEED.txt"    
    echo "Working on $FILE"
    ./binary $SEED $INSTANCE_NAME < $f >> $FILE
    echo "" >> $FILE
    expr $SECONDS - $TIME >> $FILE ;
    sleep $SEED
}


open_sem $N
for f in QAPinstances/*.dat; do
for SEED in 2 3 4 5 6 7 8 9 10; do
    run_with_lock run_one $SEED $f;
done 
done





















