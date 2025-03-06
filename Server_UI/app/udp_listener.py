# app/udp_listener.py

import socket
import struct
import threading
import json
import math
from .sockets import relay_message_to_clients

INDEX_COL_MAP = {1:0, 2:1, 4:2, 5:3}
NUM_FLOATS = 10
BUFFER_SIZE = NUM_FLOATS * 4  # 40 bytes, if each float is 4 bytes

def start_udp_listener(
    udp_host="0.0.0.0",  
    udp_port=9999
):
    def listen():
        sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
        sock.bind((udp_host, udp_port))
        
        print(f"UDP listener started on {udp_host}:{udp_port}")

        while True:
            data, addr = sock.recvfrom(BUFFER_SIZE)
            try:
                float_values = struct.unpack("<10f", data)
            except struct.error:
                continue

            # NOTE:Assuming angles 6 is same as 7 and 8 is same as 9 (for only radial cuts)
            angle1=math.degrees(float_values[7])
            angle2=math.degrees(float_values[7])
            angle4=math.degrees(float_values[8])
            angle5=math.degrees(float_values[8])


            flexAngles = [angle1, angle2, angle4, angle5]

            for i in INDEX_COL_MAP.keys(): # NOTE:Assuming angles 6 is same as 7 and 8 is same as 9

                payload = {
                    "flexAngle": flexAngles[INDEX_COL_MAP[i]],
                    "col": i
                }
                relay_message_to_clients(json.dumps(payload))

    thread = threading.Thread(target=listen, daemon=True)
    thread.start()
