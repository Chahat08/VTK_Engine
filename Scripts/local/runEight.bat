:: TEST 8 INSTANCES IN TWO ROWS

::                      SW   SH  IW  IH  IX   IY  WX    WY  PH  PD   ANG   COL CLID  HOST_WS_IP                 GPU_ID
start "" VTK_Engine.exe 400 400 100 200 0    0   0     0   2.5 3.0  72.0  -2   1_1   ws://localhost:5000/connect   0
start "" VTK_Engine.exe 400 400 100 200 100  0   101   0   2.5 3.0  72.0  -1   1_2   ws://localhost:5000/connect   0
start "" VTK_Engine.exe 400 400 100 200 200  0   202   0   2.5 3.0  72.0   0   1_3   ws://localhost:5000/connect   0
start "" VTK_Engine.exe 400 400 100 200 300  0   303   0   2.5 3.0  72.0   1   1_4   ws://localhost:5000/connect   0

::                      SW   SH  IW  IH  IX   IY  WX    WY  PH  PD   ANG   COL CLID  HOST_WS_IP                 GPU_ID
start "" VTK_Engine.exe 400 400 100 200 0    200   0   201  2.5 3.0  72.0  -2  2_5   ws://localhost:5000/connect   1
start "" VTK_Engine.exe 400 400 100 200 100  200  101  201  2.5 3.0  72.0  -1  2_6   ws://localhost:5000/connect   1
start "" VTK_Engine.exe 400 400 100 200 200  200  202  201  2.5 3.0  72.0   0  2_7   ws://localhost:5000/connect   1
start "" VTK_Engine.exe 400 400 100 200 300  200  303  201  2.5 3.0  72.0   1  2_8   ws://localhost:5000/connect   1