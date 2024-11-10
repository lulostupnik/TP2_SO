#RUN in CONTAINER
pvs-studio-analyzer credentials "PVS-Studio Free" "FREE-FREE-FREE-FREE"


make clean -C/root/Toolchain
make clean -C/root/
pvs-studio-analyzer trace -- make -C/root/Toolchain
pvs-studio-analyzer trace -- make buddy -C/root/
pvs-studio-analyzer analyze 
plog-converter -a '64:1,2,3;GA:1,2,3;OP:1,2,3' -t tasklist -o report.tasks PVS-Studio.log
make clean -C/root/Toolchain
make clean -C/root/

rm strace_out
rm PVS-Studio.log
#rm */PVS-Studio.log
#rm Bootloader/BMFS/PVS-Studio.log
#rm Userland/shellCodeModule/PVS-Studio.log
rm .config/ -r
rm .bash_history -r