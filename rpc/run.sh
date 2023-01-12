for i in {1..5}
do
    xterm -fa 'Monospace' -fs 18 -hold -e ./bakery_client 127.0.0.1 &
done
wait
echo Finished