:: LOCAL 5 INSTANCES IN ONE ROW TO TEST ANGLE
::                      SW   SH  IW  IH  IX   IY  WX    WY  PH  PD   ANG   COL CLID  HOST_WS_IP                 GPU_ID
start "" VTK_Engine.exe 1500 300 300 300 0    0   0     0   2.5 3.0  72.0  -2   1_1   ws://localhost:5000/connect   0
start "" VTK_Engine.exe 1500 300 300 300 300  0   301   0   2.5 3.0  72.0  -1   1_2   ws://localhost:5000/connect   0
start "" VTK_Engine.exe 1500 300 300 300 600  0   602   0   2.5 3.0  72.0   0   1_3   ws://localhost:5000/connect   0
start "" VTK_Engine.exe 1500 300 300 300 900  0   903   0   2.5 3.0  72.0   1   1_4   ws://localhost:5000/connect   1
start "" VTK_Engine.exe 1500 300 300 300 1200 0   1204  0   2.5 3.0  72.0   2   1_5   ws://localhost:5000/connect   1