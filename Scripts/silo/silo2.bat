SETLOCAL 

set LOGIN=-u localadmin -p Vislab1234
SET DEMOPATH=D:\demo\chahat-run\Volume_Renderer\
set START=start /MIN utilities\psexec -s -i 1 -w %DEMOPATH% %LOGIN%

SET PATH=%PATH%;D:\demo\VTK_Build\Release

SET /a SCENE_WIDTH=2560*4
SET /a SCENE_HEIGHT=6024*2
SET INSTANCE_WIDTH=2560
SET INSTANCE_HEIGHT=6024
SET PHYSICAL_HEIGHT=2.85
SET PHYSICAL_DISTANCE=2.2
SET ANGLE_TO_ROTATE=0.0
SET WEBSOCKET_HOST_IP=ws://10.10.10.98:5000/connect

::                      DEMOPATH              SW              SH            IW                  IH          IX    IY     WX     WY            PH            PD                ANG               COL  CLID      HOST_WS_IP          GPU_ID
%START% \\SILO-2 %DEMOPATH%\VTK_Engine.exe %SCENE_WIDTH% %SCENE_HEIGHT% %INSTANCE_WIDTH% %INSTANCE_HEIGHT%  0      0      0      0     %PHYSICAL_HEIGHT% %PHYSICAL_DISTANCE% %ANGLE_TO_ROTATE%  -2   2_1   %WEBSOCKET_HOST_IP%       0
%START% \\SILO-2 %DEMOPATH%\VTK_Engine.exe %SCENE_WIDTH% %SCENE_HEIGHT% %INSTANCE_WIDTH% %INSTANCE_HEIGHT%  2056   0      2056   0     %PHYSICAL_HEIGHT% %PHYSICAL_DISTANCE% %ANGLE_TO_ROTATE%  -1   2_2   %WEBSOCKET_HOST_IP%       1
%START% \\SILO-2 %DEMOPATH%\VTK_Engine.exe %SCENE_WIDTH% %SCENE_HEIGHT% %INSTANCE_WIDTH% %INSTANCE_HEIGHT%  5120   0      5120   0     %PHYSICAL_HEIGHT% %PHYSICAL_DISTANCE% %ANGLE_TO_ROTATE%   0   2_3   %WEBSOCKET_HOST_IP%       2
%START% \\SILO-2 %DEMOPATH%\VTK_Engine.exe %SCENE_WIDTH% %SCENE_HEIGHT% %INSTANCE_WIDTH% %INSTANCE_HEIGHT%  7680   0      7680   0     %PHYSICAL_HEIGHT% %PHYSICAL_DISTANCE% %ANGLE_TO_ROTATE%   1   2_4   %WEBSOCKET_HOST_IP%       3
%START% \\SILO-2 %DEMOPATH%\VTK_Engine.exe %SCENE_WIDTH% %SCENE_HEIGHT% %INSTANCE_WIDTH% %INSTANCE_HEIGHT%  0      6024   0      6024  %PHYSICAL_HEIGHT% %PHYSICAL_DISTANCE% %ANGLE_TO_ROTATE%  -2   2_5   %WEBSOCKET_HOST_IP%       4
%START% \\SILO-2 %DEMOPATH%\VTK_Engine.exe %SCENE_WIDTH% %SCENE_HEIGHT% %INSTANCE_WIDTH% %INSTANCE_HEIGHT%  2056   6024   2056   6024  %PHYSICAL_HEIGHT% %PHYSICAL_DISTANCE% %ANGLE_TO_ROTATE%  -1   2_6   %WEBSOCKET_HOST_IP%       5
%START% \\SILO-2 %DEMOPATH%\VTK_Engine.exe %SCENE_WIDTH% %SCENE_HEIGHT% %INSTANCE_WIDTH% %INSTANCE_HEIGHT%  5120   6024   5120   6024  %PHYSICAL_HEIGHT% %PHYSICAL_DISTANCE% %ANGLE_TO_ROTATE%   0   2_7   %WEBSOCKET_HOST_IP%       6
%START% \\SILO-2 %DEMOPATH%\VTK_Engine.exe %SCENE_WIDTH% %SCENE_HEIGHT% %INSTANCE_WIDTH% %INSTANCE_HEIGHT%  7680   6024   7680   6024  %PHYSICAL_HEIGHT% %PHYSICAL_DISTANCE% %ANGLE_TO_ROTATE%   1   2_8   %WEBSOCKET_HOST_IP%       7
