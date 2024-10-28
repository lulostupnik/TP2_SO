sudo chmod -R 777 ./*
sudo docker start SO
sudo docker exec -it SO make -C/root/Toolchain
sudo docker exec -it SO make -C/root/ > make_out.txt
sed -r "s/\x1B\[[0-9;]*[a-zA-Z]//g" make_out.txt > compile_out.txt #fixes with newline in windows and Linux
rm make_out.txt
sudo chmod 777 Image/x64BareBonesImage.qcow2
qemu-system-x86_64 -hda Image/x64BareBonesImage.qcow2 -m 512
#for audio use the command below:
#qemu-system-x86_64 -hda Image/x64BareBonesImage.qcow2 -m 512 -audiodev pa,id=speaker -machine pcspk-audiodev=speaker
sudo docker exec -it SO make clean -C/root/Toolchain
sudo docker exec -it SO make clean -C/root/
#sudo docker stop SOfe  