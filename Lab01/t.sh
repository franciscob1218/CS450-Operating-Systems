echo "# Notice: sh.c built into shell.out"
gcc ./sh.c -o "./shell.out"

echo "# Notice: whatif.c built into whatif.out"
gcc ./whatif.c -o "./whatif.out"

echo "# Notice: Changed the permissions of both built files"
chmod +wrx ./shell.out
chmod +wrx ./whatif.out

echo "# Notice: Running Shell Now!"
./shell.out