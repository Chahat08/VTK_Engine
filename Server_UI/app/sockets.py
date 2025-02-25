from .extensions import sock
import json

clients = []  
client_ids = []  

@sock.route('/connect')
def websocket_handler(ws):
    client_id = ws.receive()  
    if not client_id:
        ws.close()
        return

    clients.append(ws)
    client_ids.append(client_id)
    
    broadcast_client_list()

    try:
        while True:
            message = ws.receive()
            if message:
                print(f"{client_id}: {message}")
                if message.startswith("valRange"):
                    range_vals = message.split(' ')
                    valRange = [float(range_vals[1]), float(range_vals[2])]
                    broadcast_value_range(valRange)
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

def broadcast_value_range(valRange):
    valRange_message = json.dumps({
        "action": "update_value_range",
        "valueRange": valRange
    })

    relay_message_to_clients(valRange_message)

def broadcast_client_list():
    """Broadcast the updated client list to all connected clients."""
    client_list_message = json.dumps({
        "action": "update_client_list",
        "clients": client_ids
    })

    relay_message_to_clients(client_list_message)
