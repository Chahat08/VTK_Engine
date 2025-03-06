from .extensions import sock
import json

clients = []  
client_ids = []  
frontend_client = None
valRange = None

@sock.route('/connect')
def websocket_handler(ws):
    global valRange
    global frontend_client
    global clients
    global client_ids

    client_id = ws.receive()  
    if not client_id:
        ws.close()
        return

    clients.append(ws)
    client_ids.append(client_id)

    if client_id.startswith("Frontend"):
        frontend_client = ws
        if valRange is not None:
            send_value_range(valRange)
    
    broadcast_client_list()

    try:
        while True:
            message = ws.receive()
            if message:
                if message.startswith("valRange"):
                    range_vals = message.split(' ')
                    valRange = [float(range_vals[1]), float(range_vals[2])]
                    send_value_range(valRange)
                else:
                    relay_message_to_clients(message)
            else:
                break
    finally:
        index = clients.index(ws)
        clients.remove(ws)
        client_ids.pop(index)
        broadcast_client_list()

def relay_message_to_clients(message):
    """Relay a message to all connected clients."""
    for client in clients:
        client.send(message)

def send_to_frontend_client(message):
    if frontend_client is not None:
        frontend_client.send(message)

def send_value_range(valRange):
    valRange_message = json.dumps({
        "action": "update_value_range",
        "valueRange": valRange
    })

    send_to_frontend_client(valRange_message)

def broadcast_client_list():
    """Broadcast the updated client list to all connected clients."""
    client_list_message = json.dumps({
        "action": "update_client_list",
        "clients": client_ids
    })

    relay_message_to_clients(client_list_message)
