from flask import Flask
from .routes import main
from .sockets import sock

def create_app():
    app = Flask(__name__, 
                static_url_path='',
                static_folder="../static",
                template_folder="../templates")
    app.config["SECRET_KEY"] = "secret_key"
    app.register_blueprint(main)  
    sock.init_app(app)            
    return app
