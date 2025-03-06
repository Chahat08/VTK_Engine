from app import create_app
import argparse

from app.udp_listener import start_udp_listener

app = create_app()

if __name__ == "__main__":
    parser = argparse.ArgumentParser(description="Run the Flask app with customizable host and port.")
    parser.add_argument("--host", type=str, default="0.0.0.0", help="Host IP address (default: 0.0.0.0)")
    parser.add_argument("--port", type=int, default=5000, help="Port number (default: 5000)")

    args = parser.parse_args()

    start_udp_listener(udp_host="0.0.0.0", udp_port=49152)
    app.run(host=args.host, port=args.port)
