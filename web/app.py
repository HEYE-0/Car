import asyncio
import websockets
import serial
from flask import Flask, render_template
import threading

# 修改为你的串口号
SERIAL_PORT = '/dev/ttyUSB0'
BAUDRATE = 9600

# 初始化串口
arduino = serial.Serial(SERIAL_PORT, BAUDRATE, timeout=1)

app = Flask(__name__)

# 后台任务：WebSocket服务
async def ws_handler(websocket):
    print("🔌 WebSocket connected")
    try:
        async for message in websocket:
            print(f"⬅️ Received: {message}")
            arduino.write(message.encode())
    except websockets.ConnectionClosed:
        print("❌ WebSocket disconnected")

def start_websocket_server():
    loop = asyncio.new_event_loop()
    asyncio.set_event_loop(loop)
    start_server = websockets.serve(ws_handler, "0.0.0.0", 6789)
    loop.run_until_complete(start_server)
    loop.run_forever()

# HTTP服务
@app.route('/')
def index():
    return render_template("index.html")

if __name__ == '__main__':
    t = threading.Thread(target=start_websocket_server)
    t.start()
    print("🚀 Flask running at http://localhost:5000")
    app.run(host='0.0.0.0', port=5000)
