import paramiko
from flask import Flask, request, render_template

app = Flask(__name__)

# Raspberry Pi SSH Login Information
RPI_HOST = '172.20.10.12'
RPI_USER = 'wh'
RPI_PASS = '123456'
PTS_PATH = '/dev/pts/0' # the terminal where the program is running

@app.route('/')
def index():
    return render_template('index.html')

@app.route('/control', methods=['POST'])
def control():
    cmd = request.form['cmd']  # 比如 w、a、s、d
    try:
        ssh = paramiko.SSHClient()
        ssh.set_missing_host_key_policy(paramiko.AutoAddPolicy())
        ssh.connect(RPI_HOST, username=RPI_USER, password=RPI_PASS)
        ssh.exec_command(f'echo {cmd} > {PTS_PATH}')
        ssh.close()
        print(f"✅ 已发送：{cmd}")
        return 'OK'
    except Exception as e:
        print(f"❌ 错误：{e}")
        return 'ERROR'

if __name__ == '__main__':
    app.run(host='0.0.0.0', port=5000)
