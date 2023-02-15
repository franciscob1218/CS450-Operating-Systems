gcc ./sh.c -o "./shell.out"
echo "# Notice: sh.c built into shell.out"

gcc ./whatif.c -o "./whatif.out"
echo "# Notice: whatif.c built into whatif.out"

chmod +wrx ./shell.out
chmod +wrx ./whatif.out
echo "# Notice: Changed the permissions of both built files"

echo "# Notice: Running Shell Now!"
./shell.out