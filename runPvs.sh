#RUN in CONTAINER
#pvs-studio-analyzer credentials "PVS-Studio Free" "FREE-FREE-FREE-FREE"


#find . -name "*.c" | while read line; do sed -i '1s/^\(.*\)$/\/\/ This is a personal academic project. Dear PVS-Studio, please check it.\n\1/' "$line"; done
#find . -name "*.c" | while read line; do sed -i '2s/^\(.*\)$/\/\/ PVS-Studio Static Code Analyzer for C, C++ and C#: http:\/\/www.viva64.com\n\1/' "$line"; done


make clean -C/root/Toolchain
make clean -C/root/
pvs-studio-analyzer trace -- make -C/root/Toolchain
pvs-studio-analyzer trace -- make -C/root/
pvs-studio-analyzer analyze 
plog-converter -a '64:1,2,3;GA:1,2,3;OP:1,2,3' -t tasklist -o report.tasks PVS-Studio.log

rm strace_out
rm PVS-Studio.log
rm */PVS-Studio.log