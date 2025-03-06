import socket
import struct
import threading
import json
import math
from .sockets import relay_message_to_clients

NUM_FLOATS = 10
BUFFER_SIZE = NUM_FLOATS * 4  # 40 bytes for 10 floats

INDEX_COL_MAP = { # indices to flex column mapping
    6: 1,  
    7: 2,  
    8: 4,  
    9: 5   
}

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
            if len(data) < BUFFER_SIZE:
                continue

            try:
                # Unpack 10 floats in little-endian format
                # (common on x86; change to ">10f" for big-endian if needed)
                float_values = struct.unpack("<10f", data)
            except struct.error:
                continue

            #  indices [6, 7, 8, 9] are flex angles
            for i in [6, 7, 8, 9]:
                radians_val = float_values[i]
                degrees_val = radians_val * (180.0 / math.pi)  # rad->deg

                col = INDEX_COL_MAP[i]

                payload = {
                    "flexAngle": degrees_val,
                    "col": col
                }
                relay_message_to_clients(json.dumps(payload))

    thread = threading.Thread(target=listen, daemon=True)
    thread.start()
